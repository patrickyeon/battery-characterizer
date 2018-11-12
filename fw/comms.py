#!/usr/bin/python

import fcntl
import json
import os
import re
import serial
import termios
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

msg_dict = {}
rev_msg = {}

def load_msgs(filename=None):
    global msg_dict
    global rev_msg
    if filename is None:
        filename = 'msg_def.json'
    with open(filename) as f:
        idx = 0
        for msg in json.load(f)['messages']:
            if 'index' in msg:
                if msg['index'] < idx:
                    raise Exception('message index out of order')
                idx = msg['index']
            msg_dict[idx] = msg
            rev_msg[msg['label']] = idx
            idx += 1

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
    def __init__(self, intlist, msgs=None):
        if msgs is None:
            if len(msg_dict) == 0:
                load_msgs()
            msgs = msg_dict

        if intlist[0] not in msgs:
            raise PacketException('message byte not defined')

        self.raw = intlist
        msg = msgs[intlist[0]]
        self.fields = odict()
        self.label = msg['label']
        i = 1
        for field in msg.get('payload', []):
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

        if 'print' in msg:
            self.fmtline = msg['print']
        else:
            self.fmtline = ('{label}: '
                            + ' '.join(['{{{}}} ({})'.format(k,k)
                                        for k in self.fields]))

    def __str__(self):
        return self.fmtline.format(label=self.label, **self.fields)
    
    def __getitem__(self, key):
        return self.fields[key]

class MessageException(Exception):
    pass

def bytes_for(label, **values):
    if len(msg_dict) == 0:
        load_msgs()
    idx = rev_msg[label]
    cmd = msg_dict[idx]
    pkt = [idx]
    for (n, field, typ) in cmd.get('payload', []):
        # assume if the user didn't name a value it's optional or no-op
        val = values.get(field, 0)
        if typ == 'int' and val < 0:
            topbit = 0x80 << ((n - 1) * 8)
            val = (val + topbit) | topbit
        if typ in ('flags', 'int', 'uint', 'hex'):
            bs = [0] * n
            for i in range(n):
                bs[n - i - 1] = (val >> (i * 8)) & 0xff
            pkt.extend(bs)
        elif typ == 'bytes':
            pkt.extend(val)
        else:
            raise MessageException('don\'t know what to do with {}'.format(typ))
    return pkt # _send method will pack it to length


class bc:
    def __init__(self, port='/dev/ttybc', echo=False, msgs=None):
        if msgs == None:
            global msg_dict
            if len(msg_dict) == 0:
                load_msgs()
            msgs = msg_dict
        self.msgs = msgs
        self.ser = serial.Serial(port, 19200, timeout=1, write_timeout=1)
        fcntl.ioctl(self.ser.fileno(), termios.TIOCEXCL)
        self.echo = echo

    def __del__(self):
        self.ser.close()

    def _send(self, str_or_bstream, **values):
        if type(str_or_bstream) == str:
            bstream = bytes_for(str_or_bstream, **values)
        else:
            bstream = str_or_bstream
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
        retval = self._read(14)
        if len(retval) != 14:
            raise PacketException('no/incomplete response')
        # TODO checksum
        try:
            pkt = Packet([ord(c) for c in retval[:-1]])
        except PacketException as e:
            if self.echo:
                print e
                #print 'unknown message: ' + str(hexify(retval))
            raise e
        if self.echo:
            print pkt
        return pkt

    def expect(self, pkt_type):
        if type(pkt_type) == str:
            pkt_type = rev_msg[pkt_type]
        resp = self._read_pkt()
        if resp.raw[0] != pkt_type:
            raise ResponseException(pkt_type, resp.raw[0])
        return resp

    def rx_for(self, expected, str_or_bstream, **values):
        self._send(str_or_bstream, **values)
        return self.expect(expected)

    def ping(self):
        self.rx_for('pong', 'ping')
        return True

    def get_time(self):
        ret = self.rx_for('time_set', 'time_get')
        return ret['sec'] + ret['ms'] * 0.001

    def set_time(self, sec, ms=0):
        ret = self.rx_for('time_set', 'time_set', sec=sec, ms=ms)
        return ret['sec'] + ret['ms'] * 0.001

    def tag(self, tag):
        tag = tag[:4]
        if type(tag) is str:
            tag = [ord(c) for c in tag]
        resp = self.rx_for('tagged', 'create_tag', user=tag)
        return resp['logline']

    def dlog(self):
        resp = self.rx_for('logline', 'dequeue_log')
        return resp.fields

    def wipelog(self):
        self.rx_for('wipe_log', 'wipe_log')
        return True

    def read_flash(self, addr, nbytes):
        resp = self.rx_for('flash_data',
                           'flash_read', address=addr, count=nbytes)
        return resp['data']

    def peek_flash(self, addr, nbytes):
        resp = self.rx_for('flash_data',
                           'flash_peek', address=addr, count=nbytes)
        return resp['data']

    def read_adc(self, chan):
        self.rx_for('adc_scan', 'adc_scan')
        return self.rx_for('adc_read', 'adc_read', channel=chan).fields

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
            try:
                resp = self.rx_for('temp_read', 'temp_read', address=addr)
                temps.append(resp['value'] + resp['frac'] * 1./256)
            except ResponseException:
                temps.append(-99)
        return temps

    def cenden(self, truthstr):
        # truthstr is t/f for (cendenA, cendenB, cenA, denA, cenB, denB)
        #  right-padded to fill with 'f's. eg 'ttfft' for only cenB on.
        # cendenN = True --> cenN is lower cell, denN is higher cell
        thruthstr += 'ffffff'[len(truthstr):]
        dirmask, enmask = 0, 0
        if (truthstr[0] == 't'):
            dirmask |= 0x1
        if (truthstr[1] == 't'):
            dirmask |= 0x1
        for i, t in enumerate(truthstr[2:]):
            if t == 't':
                enmask |= 2 ** i
        self.rx_for('cenden_set',
                    'cenden_set', bitmask_dir=dirmask, bitmask_en=enmask)
        return True

    def set_i(self, chan, val):
        #channels: 0 = IDA, 1 = IDB, 2 = ICA, 3 = ICB
        self.rx_for('current_set', 'current_set', channel=chan, ma=val)
        return True

    def get_hwid(self):
        return self.rx_for('hwid_set', 'hwid_get')['hwid']

class TestFoo(bc):
    def read_all_adcs(self):
        adcs = []
        self.rx_for('adc_scan', 'adc_scan')
        for chan in range(8):
            try:
                resp = self.rx_for('adc_read', 'adc_read', channel=chan)
                adcs.append(resp['value'])
            except ResponseException:
                adcs.append(-1)
        return odict(zip('ic_a ic_b id_a id_b vb_1 vb_0 vb_3 vb_2'.split(),
                         adcs))
    def test_logger(self, period=100, sleep=5):
        print self.get_time()
        self.rx_for('log_period_set', 'log_period_set', period=period)
        self.rx_for('log_en_dis', 'log_en_dis', enable=1)
        time.sleep(sleep)
        try:
            while True:
                print self.rx_for('logline', 'dequeue_log')
        except ResponseException:
            pass
        self.rx_for('log_en_dis', 'log_en_dis', enable=0)
        print self.get_time()

class Characterizer(bc):
    def __init__(self, hwid, cell_serials, tty=None):
        assert(len(cell_serials) == 4)
        if tty is None:
            tty = self.search_hwid(hwid)
        self.cells = cell_serials
        bc.__init__(self, tty)

    @classmethod
    def search_hwid(cls, hwid):
        # assumes that all battery characterizers enumerate as /dev/ttybcN
        candidates = ['/dev/' + dev for dev in os.listdir('/dev/')
                      if re.match('ttybc[0-9]*$', dev) is not None]
        for c in candidates:
            try:
                dev = bc(c)
                if dev.get_hwid() == hwid:
                    del dev
                    return c
            except serial.SerialException:
                pass
        raise Exception('could not find hwid {}'.format(hwid))
