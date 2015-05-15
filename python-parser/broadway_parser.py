from __future__ import print_function, division, absolute_import, unicode_literals
from parser_base import BroadwayParser
from broadway_semantics import Semantics
from grako.buffering import Buffer
import sys, json

def main(filename, trace):
    parser = BroadwayParser(parseinfo=True, semantics=Semantics())
    with open(filename) as f:
        buffer = Buffer(f.read(), filename=filename, trace=True)
        ast = parser.parse(buffer, 'start', trace=trace)
        print(json.dumps(ast, indent=2))
        print(ast)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        thisprog = path.basename(sys.argv[0])
        print(thisprog)
        print('Usage:')
        print('\t', thisprog, 'FILENAME.g [--trace]')
        sys.exit(1)

    main(sys.argv[1], '--trace' in sys.argv)
