#!/usr/bin/python

import json
import serial
import time

from collections import OrderedDict as odict

ACK = 0x01

class PacketException(Exception):
    pass
class ResponseException(Exception):
    def __init__(self, expected, got):
        self.expected, self.got = expected, got
        txt = 'Unexpected response type: {}, wanted: {}'
        Exception.__init__(self, txt.format(hex(got), hex(expected)))

def hexify(s):
    if type(s[0]) is str:
        return [hex(ord(c)) for c in s]
    else:
        return [hex(c) for c in s]

def unpack(s):
    return sum([c*(2**(u*8)) for (u, c) in enumerate(reversed(s))])
def pack(i, n):
    retval = []
    for u in range(n):
        retval.insert(0, (i / (2 ** (u*8))) % 256)
    return retval

Cmd_dict = {}

def load_cmds(filename=None):
    global Cmd_dict
    if filename is None:
        filename = 'cmd_def.json'
    with open(filename) as f:
        Cmd_dict = {int(k): v for k,v in json.load(f)['commands'].items()}

class Packet:
    _reformat = {'int1': lambda x: (x & 0x7f) - 0x80 \
                                   if x > 0x7f else x,
                 'int2': lambda x: (x & 0x7fff) - 0x8000 \
                                   if x > 0x7fff else x,
                 'int3': lambda x: (x & 0x7fffff) - 0x800000 \
                                   if x > 0x7fffff else x,
                 'int4': lambda x: (x & 0x7fffffff) - 0x80000000 \
                                   if x >  0x7fffffff else x,
                 'uint': int,
                 'float': float,
                 'hex': hex,
                 'bytes': lambda xs: xs,
                 # TODO expand this to represent all bits
                 'flags': lambda fs: bin(fs)[2:]
                }
    def __init__(self, intlist, cmds=None):
        if cmds is None:
            if len(Cmd_dict) == 0:
                load_cmds()
            cmds = Cmd_dict

        if intlist[0] not in cmds:
            raise PacketException('command byte not defined')

        self.raw = intlist
        cmd = cmds[intlist[0]]
        self.fields = odict()
        self.label = cmd['label']
        i = 1
        for field in cmd.get('payload', []):
            flen = field[0]
            ftype = field[2]
            if ftype == 'int':
                parser = self._reformat['int' + str(flen)]
            else:
                parser = self._reformat[ftype]

            if field[2] == 'bytes':
                raw = intlist[i : i + flen]
            else:
                raw = unpack(intlist[i : i + flen])
            i += flen
            self.fields[field[1]] = parser(raw)

        if 'print' in cmd:
            self.fmtline = cmd['print']
        else:
            self.fmtline = ('{label}: '
                            + ''.join(['{{{}}} ({})'.format(v,k) 
                                       for k,v in self.fields.items()]))

    def __str__(self):
        return self.fmtline.format(label=self.label, **self.fields)
    
    def __getitem__(self, key):
        return self.fields[key]

class bc:
    def __init__(self, port='/dev/ttybc', echo=False, cmds=None):
        if cmds == None:
            global Cmd_dict
            if len(Cmd_dict) == 0:
                load_cmds()
            cmds = Cmd_dict
        self.cmds = cmds
        self.ser = serial.Serial(port, 19200, timeout=1, write_timeout=1)
        self.echo = echo

    def _send(self, bstream):
        pre = [chr(0xa5)]
        post = [chr(0x00)]
        payload = [chr(c) for c in bstream]
        payload = payload + ([chr(0x00)] * (9 - len(payload)))
        # TODO checksum
        self.ser.write(''.join(pre + payload + post))

    def _read(self, nbytes):
        return self.ser.read(nbytes)

    def _read_pkt(self):
        retval = '\x00'
        while retval[0] != chr(0xa5):
            retval = self._read(1)
            if len(retval) == 0:
                raise PacketException('no response')
        retval = self._read(9)
        if len(retval) != 9:
            raise PacketException('no/incomplete response')
        # TODO checksum
        try:
            pkt = Packet([ord(c) for c in retval[:-1]])
        except PacketException as e:
            if self.echo:
                print e
                #print 'unknown command: ' + str(hexify(retval))
            raise e
        if self.echo:
            print pkt
        return pkt

    def expect(self, pkt_type):
        resp = self._read_pkt()
        if resp.raw[0] != pkt_type:
            raise ResponseException(pkt_type, resp.raw[0])
        return resp

    def ping(self):
        self._send([0x02])
        self.expect(0x03)
        return True

    def get_time(self):
        self._send([0x05])
        ret = self.expect(0x06)
        return ret['sec'] + ret['ms'] * 0.001

    def set_time(self, sec, ms=0):
        self._send([0x06] + pack(sec, 4) + pack(ms, 2))
        ret = self.expect(0x06)
        return ret['sec'] + ret['ms'] * 0.001

    def tag(self, tag):
        tag = tag[:4]
        if type(tag) is str:
            tag = [ord(c) for c in tag]
        self._send([0x04] + tag)
        resp = self.expect(0x04)
        return ret['user']

    def dlog(self):
        self._send([0x07])
        resp = self.expect(ACK)
        # FIXME need a proper log return type, I guess
        return resp.fields

    def wipelog(self):
        self._send([0x08])
        self.expect(ACK)
        return True

    def read_flash(self, addr, nbytes):
        self._send([0x0a] + pack(addr, 4) + [nbytes & 0xff])
        # FIXME raw bytes return type
        resp = self._read_pkt()
        if resp[0] == 0x00:
            print 'nak'
        else:
            print hexify(resp[1:nbytes + 1])

    def peek_flash(self, addr, nbytes):
        self._send([0x09] + pack(addr, 4) + [nbytes & 0xff])
        resp = self._read_pkt()
        # FIXME raw bytes return type
        if resp[0] == 0x00:
            print 'nak'
        else:
            print hexify(resp[1:nbytes + 1])

    def adc_read(self):
        adcs = []
        self._send([0x0e])
        self.expect(0x0e)
        for chan in range(8):
            self._send([0x0b, chan])
            try:
                resp = self.expect(0x0b)
                adcs.append(resp['value'])
            except ResponseException:
                adcs.append(-1)
        return odict(zip('ic_a ic_b id_a id_b vb_1 vb_0 vb_3 vb_2'.split(),
                         adcs))

    def flush_read(self):
        nflushed = 0
        while True:
            try:
                self._read_pkt()
                nflushed += 1
            except PacketException as e:
                if e.message == 'no response':
                    return nflushed

    def read_temp(self):
        temps = []
        for addr in (0x48, 0x49, 0x4c, 0x4d):
            self._send([0x0c, addr])
            try:
                resp = self.expect(0x0c)
                temps.append(resp['value'] + resp['frac'] * 1./256)
            except ResponseException:
                temps.append(-99)
        return temps

    def cenden(self, cendenA, cendenB, cenA=False, denA=False,
               cenB=False, denB=False):
        # cendenN = True --> cenN is lower cell, denN is higher cell
        req = [0x0d, 0x0, 0x0]
        if (cendenA):
            req[1] = req[1] | 0x1
        if (cendenB):
            req[1] = req[1] | 0x2
        for i, t in enumerate((cenA, denA, cenB, denB)):
            if t:
                req[2] = req[2] | (2 ** i)
        self._send(req)
        self.expect(ACK)

    def set_i(self, chan, val):
        #channels: 0 = IDA, 1 = IDB, 2 = ICA, 3 = ICB
        req = [0x0f, chan] + pack(val, 2)
        self._send(req)
        self.expect(ACK)

    def ticktock(self):
        self._send([0x12, 0x01])
        self.expect(ACK)
        time.sleep(0.003)
        self._send([0x12, 0x02])
        resp = self.expect(0x12)
        print '300:', unpack(resp['value'][:4]) / 10.

        time.sleep(0.008)
        self._send([0x12, 0x02])
        resp = self.expect(0x12)
        print '1100:', unpack(resp['value'][:4]) / 10.

        time.sleep(0.1)
        self._send([0x12, 0x02])
        resp = self.expect(0x12)
        print '101100:', unpack(resp['value'][:4]) / 10.

        self._send([0x12, 0x01])
        self.expect(ACK)
        time.sleep(0.003)
        self._send([0x12, 0x02])
        resp = self.expect(0x12)
        print '300:', unpack(resp['value'][:4]) / 10.

    def ticktock2(self):
        self._send([0x12, 0x01])
        self.expect(ACK)
        tstart = time.time()
        for i in range(5):
            self._send([0x12, 0x02])
            resp = self.expect(0x12)
            print '', unpack(resp['value'][:4]) / 10.
        print 'wallclock: ', (time.time() - tstart)

