#!/usr/bin/python

import json
from datetime import datetime

version = '0.0.1'

pre = '''#ifndef GENERATED_MSGS_H
#define GENERATED_MSGS_H

// generated with codegen version {}
// using msgdefs version {}
// {}

'''

post = '#endif // GENERATED_MSGS_H'

def cstyle(msg):
    pattern = '    CMD_{}{}'
    if 'index' in msg:
        idx = ' = {}'.format(msg['index'])
    else:
        idx = ''
    return pattern.format(msg['label'].upper(), idx)

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('msgdefs', help='json file describing the messages')
    parser.add_argument('-o', '--outfile', help='target file to generate into')
    args = parser.parse_args()

    with open(args.msgdefs) as f:
        msg_dict = json.load(f)
    out_txt = (pre.format(version, msg_dict['version'],
                          datetime.now().strftime('%F %T'))
               + 'typedef enum msg_type_e {\n'
               + ',\n'.join([cstyle(msg) for msg in msg_dict['messages']])
               + '\n} msg_type_e;\n\n'
               + post)
    if args.outfile:
        with open(args.outfile, 'w') as f:
            f.write(out_txt)
    else:
        print out_txt
