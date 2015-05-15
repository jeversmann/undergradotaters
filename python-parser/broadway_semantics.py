from __future__ import print_function, division, absolute_import, unicode_literals
from parser_base import BroadwaySemantics
import inflect
import itertools
p = inflect.engine()


def flatten_lists(ls):
    result = []
    for l in ls:
        if isinstance(l, list):
            result.extend(flatten_lists(l))
        else:
            result.append(l)
    return result

class Semantics(BroadwaySemantics):
    def __init__(self, *args, **kwargs):
        super(Semantics, self).__init__(*args, **kwargs)
        self.procedures = {}
        self.properties = {}
        self.global_structure = []
        self.global_analyses = []
        self.headers = []

    def start(self, ast):
        print(ast)
        annotations = ast['annotations']
        names = ['property', 'procedure', 'global_structure', 'global_analysis', 'header']
        node = { p.plural(name): [n[name] for n in annotations if name in n]
                 for name in names }

        return node

    def header(self, ast):
        return { 'header': '\n'.join(ast['code']) }

    def property(self, ast):
        definition = ast['def_']
        node = { 
            'name': ast['name'],
            'direction': definition['direction'],
            'initial': definition['initial'],
            'set_type': definition['set_type'],
            'weak': ast['weak'],
            'lattice': definition['values_']
        }
        return { 'property': node }

    def weak_property(self, ast):
        ast['weak'] = True
        return ast

    def property_value(self, ast):
        name = ast['name']
        children = ast['sublist']
        root = { 'name': name, 'parent': None }
        if children:
            children = flatten_lists(children)
            for child in children:
                if not child['parent']:
                    child['parent'] = name
            children.append(root)
            return children
        else:
            return [root]

    def set_type(self, ast):
        return [v for k, v in ast.items() if v][0]

    def procedure(self, ast):
        return { 'procedure': ast }

    def global_(self, ast):
        return { (k if k != 'analysis_rule_annotation' else 'global_analysis') : v 
                 for k, v in ast }

    def global_pointer(self, ast):
        return { 'global_structure': ast }

    def analysis_rule_annotation(self, ast):
        return { 'analysis_rule_annotation': ast }

    def forward(self, ast):
        return 'forward'

    def backward(self, ast):
        return 'backward'
