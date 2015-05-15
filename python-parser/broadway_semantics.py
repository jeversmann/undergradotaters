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

def aggregate_elements(names, annotations):
    return { p.plural(name): flatten_lists([n[name] for n in annotations if name in n])
             for name in names }

def condense_operator(ast):
    return ast if ast.get('operator') else ast['lhs']

class Semantics(BroadwaySemantics):
    def __init__(self, *args, **kwargs):
        super(Semantics, self).__init__(*args, **kwargs)
        self.procedures = {}
        self.properties = {}
        self.global_structure = []
        self.global_analyses = []
        self.headers = []

    def start(self, ast):
        annotations = ast['annotations']
        names = ['property', 'procedure', 'global_structure', 'global_analysis', 'header']
        return aggregate_elements(names, annotations)

    def header(self, ast):
        return { 'header': '\n'.join(ast['code']) }

    def procedure(self, ast):
        print(ast)
        statements = ast['statements']
        names = ['entry_pointer', 'exit_pointer', 'modify', 'access', 'report', 'action', 'analysis']
        statements = aggregate_elements(names, statements)
        return {
            'procedure': {
                'name': ast['name'],
                'arguments': ast['arguments'],
                'statements': statements
            }
        }

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

    def pointer_structure(self, ast):
        keys = ['name', 'target', 'io', 'new', 'delete', 'members']
        node = { key: ast.get(key) for key in keys }
        if node['new']:
            node['target']['new'] = True
            node['new'] = None
        return node

    def variable(self, ast):
        if ast['io']:
            ast = { 'io': True, 'name': ast['name'] }
        return ast

    def delete(self, ast):
        ast['delete'] = True
        return ast

    def access(self, ast):
        return { 'access': ast['names'] }

    def modify(self, ast):
        return { 'modify': ast['names'] }

    def report_annotation(self, ast):
        return { 'report': ast }

    def callsite(self, ast):
        return 'callsite'

    def context(self, ast):
        return 'context'

    def inline(self, ast):
        ast['inline'] = True
        return ast

    def pointer_exit(self, ast):
        pointers = [{'condition': None, 'pointers': ast['pointers']}] if ast['pointers'] else []
        if ast['cond_pointers']:
            pointers.extend([{'condition': c['condition'], 'pointers': c['structures']} for c in ast['cond_pointers']])
        return { 'exit_pointer': pointers }

    def analysis_rule_annotation(self, ast):
        rules = [{'condition':None, 'effects': ast['effects']}] if ast['effects'] else []
        if ast['rules']:
            rules.extend([{'condition': c['condition'], 'effects': c['effects']} for c in ast['rules']])
        return {
            'analysis': {
                'name': ast['name'],
                'rules': rules
            }
        }

    def default(self, ast):
        ast['condition'] = None
        return ast

    def condition(self, ast):
        return condense_operator(ast)

    def bitwise_expression(self, ast):
        return condense_operator(ast)

    def sum_expression(self, ast):
        return condense_operator(ast)

    def mult_expression(self, ast):
        return condense_operator(ast)

    def factor(self, ast):
        return condense_operator(ast)

    def number(self, ast):
        try:
            return int(ast)
        except ValueError:
            return float(ast)

    def identifier_operation(self, ast):
        ast['operator'] = 'triple-identifier'
        return ast
