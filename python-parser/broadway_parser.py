from __future__ import print_function, division, absolute_import, unicode_literals
from parser_base import BroadwayParser
from broadway_semantics import Semantics
from grako.buffering import Buffer
import argparse
import sys
import os
import json
import subprocess

def main(options):
    parser = BroadwayParser(parseinfo=True, semantics=Semantics())

    buffer = ''
    if options.preprocess:
        buffer = subprocess.check_output(['clang', '-E', '-P', '-x', 'c', options.input_file])
    else:
        with open(options.input_file) as input:
            buffer = input.read()

    ast = parser.parse(buffer, 'start', trace=options.trace)

    output_file = options.output_file
    if not output_file:
        output_file = os.path.splitext(os.path.basename(options.input_file))[0] + '.json'
    with open(output_file, 'w') as out:
        text = json.dumps(ast, indent=2)
        out.write(text)
        print(text)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('input_file')
    parser.add_argument('--trace', action='store_true')
    parser.add_argument('-o', '--output', dest='output_file')
    parser.add_argument('-bc', '--bytecode', dest='bytecode_file')
    parser.add_argument('--no-preprocess', action='store_false', dest='preprocess')
    main(parser.parse_args())

