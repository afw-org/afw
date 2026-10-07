#!/usr/bin/env python3

##
# @file parse.py
# @ingroup afwdev_build
# @brief Parse harvested W3C EBNF (syntax.ebnf) into a small tuple tree.
#
# The grammar uses a subset of W3C EBNF (https://www.w3.org/TR/xml/#sec-notation):
# sequences, `|`, `?`, `*`, `+`, `A - B`, quoted strings, `[...]` character
# classes and `#xN` characters. Nodes are tuples:
#
#   ('name', 'Expression')      ('str', "'if'")        ('cls', '[0-9]')
#   ('hex', '#x0A')             ('comment', 'text')
#   ('seq', [items])            ('choice', [alts])
#   ('opt', node)  ('star', node)  ('plus', node)  ('diff', a, b)
#
# A comment inside a rule is kept as an item so it can label the diagram.
# Comments between rules are not part of any rule; doc_comments() returns
# the comment block in front of each rule.
#

import re

_TOKEN = re.compile(r"""
  (?P<ws>\s+)
 |(?P<comment>/\*.*?\*/)
 |(?P<defn>::=)
 |(?P<name>[A-Za-z_][A-Za-z0-9_]*)
 |(?P<str>'[^']*'|"[^"]*")
 |(?P<cls>\[\^?(?:[^\]\\]|\\.)*\]|\[\^?\]\])
 |(?P<hex>\#x[0-9A-Fa-f]+)
 |(?P<op>[()|?*+\-])
""", re.S | re.X)

_POSTFIX = {'?': 'opt', '*': 'star', '+': 'plus'}

_PRODUCTION = re.compile(r'^([A-Za-z_][A-Za-z0-9_]*)\s*::=')


class EbnfSyntaxError(Exception):
    """A grammar the parser cannot read. The message names the line."""


def _line_of(text, pos):
    return text.count('\n', 0, pos) + 1


def tokenize(text):
    """Return (kind, value, line) tokens. Whitespace is dropped."""
    pos, tokens = 0, []
    while pos < len(text):
        m = _TOKEN.match(text, pos)
        if not m:
            raise EbnfSyntaxError('line {}: unexpected {!r}'.format(
                _line_of(text, pos), text[pos:pos + 20]))
        kind = m.lastgroup
        if kind == 'comment':
            tokens.append(('comment', m.group()[2:-2].strip(), _line_of(text, pos)))
        elif kind != 'ws':
            tokens.append((kind, m.group(), _line_of(text, pos)))
        pos = m.end()
    return tokens


class _Parser:

    def __init__(self, tokens):
        self.tokens = tokens
        self.i = 0

    def peek(self, k=0):
        j = self.i + k
        return self.tokens[j] if j < len(self.tokens) else (None, None, None)

    def take(self):
        self.i += 1
        return self.tokens[self.i - 1]

    def error(self, what):
        kind, value, line = self.peek()
        if kind is None:
            return EbnfSyntaxError('end of grammar: ' + what)
        return EbnfSyntaxError('line {}: {} (found {!r})'.format(line, what, value))

    def at_rule_start(self, k=0):
        return self.peek(k)[0] == 'name' and self.peek(k + 1)[0] == 'defn'

    def grammar(self):
        rules = {}
        while self.peek()[0]:
            if self.peek()[0] == 'comment':
                self.take()
                continue
            if not self.at_rule_start():
                raise self.error('expected a production (Name ::= ...)')
            name = self.take()[1]
            self.take()
            if name in rules:
                raise EbnfSyntaxError('line {}: {} is defined twice'.format(
                    self.peek(-2)[2], name))
            rules[name] = self.choice()
        return rules

    def choice(self):
        alts = [self.difference()]
        while self.peek()[:2] == ('op', '|'):
            self.take()
            alts.append(self.difference())
        return alts[0] if len(alts) == 1 else ('choice', alts)

    def difference(self):
        a = self.sequence()
        if self.peek()[:2] == ('op', '-'):
            self.take()
            return ('diff', a, self.sequence())
        return a

    def sequence(self):
        items = []
        while True:
            kind, value, _ = self.peek()
            if kind is None or self.at_rule_start():
                break
            if kind == 'op' and value in '|)-':
                break
            if kind == 'comment':
                # Comments just before the next rule belong to that rule.
                j = 0
                while self.peek(j)[0] == 'comment':
                    j += 1
                if self.peek(j)[0] is None or self.at_rule_start(j):
                    break
            items.append(self.postfix())
        return items[0] if len(items) == 1 else ('seq', items)

    def postfix(self):
        node = self.primary()
        while self.peek()[0] == 'op' and self.peek()[1] in _POSTFIX:
            node = (_POSTFIX[self.take()[1]], node)
        return node

    def primary(self):
        kind, value, _ = self.peek()
        if kind == 'op' and value == '(':
            self.take()
            node = self.choice()
            if self.peek()[:2] != ('op', ')'):
                raise self.error("expected ')'")
            self.take()
            return node
        if kind in ('name', 'str', 'cls', 'hex', 'comment'):
            self.take()
            return (kind, value)
        raise self.error('expected a name, string, character class or (')


##
# @brief Parse W3C EBNF text.
# @param text The grammar.
# @return dict of production name to node, in grammar order.
#
def parse(text):
    return _Parser(tokenize(text)).grammar()


##
# @brief The comment block in front of each production.
#
# Only comments in the same harvested block count: afwdev generate starts each
# block with a "From <file>" comment. Wrapped lines join into paragraphs, an
# empty comment line ends a paragraph, and an indented line keeps its own line.
#
# @param text The grammar.
# @return dict of production name to doc text (paragraphs separated by "\n\n").
#
def doc_comments(text):
    docs, block = {}, []
    for line in text.split('\n'):
        m = _PRODUCTION.match(line)
        if m:
            paragraphs, current = [], []
            for raw in block + ['']:
                if not raw.strip():
                    if current:
                        paragraphs.append(current)
                        current = []
                elif raw.startswith('  ') and current:
                    current.append('\n' + raw.strip())
                else:
                    current.append(raw.strip())
            if paragraphs:
                docs[m.group(1)] = '\n\n'.join(
                    ' '.join(p).replace(' \n', '\n') for p in paragraphs)
            block = []
        elif line.startswith('/*'):
            raw = line[2:line.rfind('*/')].rstrip()
            raw = raw[1:] if raw.startswith(' ') else raw
            if raw.startswith('From '):
                block = []
            else:
                block.append(raw)
    return docs


##
# @brief Where each production was harvested from ("From <file>" comments).
# @param text The grammar.
# @return dict of production name to source file name.
#
def source_files(text):
    origin, current = {}, ''
    for line in text.split('\n'):
        m = re.match(r'/\* From (\S+)', line)
        if m:
            current = m.group(1)
            continue
        m = _PRODUCTION.match(line)
        if m:
            origin[m.group(1)] = current
    return origin


##
# @brief The source text of each production, as written.
# @param text The grammar.
# @return dict of production name to its text (from "Name ::=" to the next
#         comment line or production).
#
def production_text(text):
    lines, out, i = text.split('\n'), {}, 0
    while i < len(lines):
        m = _PRODUCTION.match(lines[i])
        if not m:
            i += 1
            continue
        body, j = [lines[i]], i + 1
        while j < len(lines) and not (_PRODUCTION.match(lines[j]) or lines[j].startswith('/*')):
            body.append(lines[j])
            j += 1
        while body and not body[-1].strip():
            body.pop()
        out[m.group(1)] = '\n'.join(body)
        i = j
    return out


##
# @brief Call f on node and every node below it.
#
def walk(node, f):
    f(node)
    kind = node[0]
    if kind in ('seq', 'choice'):
        for child in node[1]:
            walk(child, f)
    elif kind in ('opt', 'star', 'plus'):
        walk(node[1], f)
    elif kind == 'diff':
        walk(node[1], f)
        walk(node[2], f)
    elif kind == 'loop':
        walk(node[1], f)
        if node[2] is not None:
            walk(node[2], f)


##
# @brief Names each production refers to.
# @return dict of production name to the set of names it uses.
#
def references(rules):
    out = {}
    for name, node in rules.items():
        used = set()
        walk(node, lambda n: used.add(n[1]) if n[0] == 'name' else None)
        out[name] = used
    return out
