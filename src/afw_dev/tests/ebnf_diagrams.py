#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
EBNF railroad diagrams (_afwdev.build.ebnf): parse, rr.war-style rewrites,
comments as captions and labels, SVG output, and the whole syntax.ebnf.
"""

import os
import shutil
import tempfile

from _afwdev.build import ebnf
from _afwdev.build.ebnf import normalize, parse, render

_SYNTAX_EBNF = os.path.normpath(os.path.join(
    os.path.dirname(os.path.abspath(__file__)),
    '..', '..', 'afw', 'generated', 'ebnf', 'syntax.ebnf'))


def _case(name, description, passed, error=None):
    return {
        'test': name,
        'description': description,
        'passed': bool(passed),
        'skip': False,
        'error': None if passed else error,
    }


def _norm(text, name):
    return normalize.normalize(parse.parse(text)[name])


S = lambda v: ('str', "'" + v + "'")
N = lambda v: ('name', v)


def run():
    tests = []

    # --- parse -------------------------------------------------------------
    got = parse.parse("A ::= 'a' B? | [0-9]+ - C* #x0A\nB ::= 'b'")
    want = {
        'A': ('choice', [
            ('seq', [S('a'), ('opt', N('B'))]),
            ('diff', ('plus', ('cls', '[0-9]')),
             ('seq', [('star', N('C')), ('hex', '#x0A')]))]),
        'B': S('b'),
    }
    tests.append(_case('parse-constructs',
                       'sequence, choice, ? + *, difference, class, #x',
                       got == want, repr(got)))

    try:
        parse.parse("A ::= 'a'\nB ::= ( 'b'\nC ::= 'c'")
        tests.append(_case('parse-error-line', 'unbalanced ( is an error',
                           False, 'no error raised'))
    except parse.EbnfSyntaxError as e:
        tests.append(_case('parse-error-line', 'parse errors name the line',
                           'line 3' in str(e), str(e)))

    # --- normalize: rr.war's default rewrites ------------------------------
    got = _norm("T ::= 'throw' | 'throw' E ( ( 'data' E | 'id' E )+ )?", 'T')
    want = ('seq', [S('throw'), ('opt', ('loop', N('E'),
                                         ('choice', [S('data'), S('id')])))])
    tests.append(_case('left-right-factor-throw',
                       "'throw' | 'throw' E ... factors to one 'throw' and a loop",
                       got == want, repr(got)))

    got = _norm("D ::= A B? | C B? | 'x'", 'D')
    want = ('choice', [('seq', [('choice', [N('A'), N('C')]), ('opt', N('B'))]),
                       S('x')])
    tests.append(_case('right-factor-group',
                       'only alternatives sharing an ending are right-factored',
                       got == want, repr(got)))

    got = _norm("P ::= '(' ( '...'? E ( ',' '...'? E )* )? ')'", 'P')
    want = ('seq', [S('('), ('opt', ('loop', ('seq', [('opt', S('...')), N('E')]),
                                     S(','))), S(')')])
    tests.append(_case('loop-multi-item', 'X ( sep X )* with X of two items',
                       got == want, repr(got)))

    got = _norm("O ::= ( X+ )?", 'O')
    tests.append(_case('opt-plus-is-star', '( X+ )? becomes X*',
                       got == ('star', N('X')), repr(got)))

    # --- comments ----------------------------------------------------------
    text = """
/* From a.c */
/* Header of a.c, not about any rule. */

/* From b.c */
/* What R does.                        */
/* Wrapped onto a second line.         */
/*                                     */
/*   indented line                     */

R ::= 'r'
    /* label */
    X
/* About Q. */

Q ::= 'q' /* ws: explicit */ 'z'
"""
    docs = parse.doc_comments(text)
    tests.append(_case('doc-comments',
                       'docs join wrapped lines, keep paragraphs, reset per harvest block',
                       docs == {'R': 'What R does. Wrapped onto a second line.\n\nindented line',
                                'Q': 'About Q.'}, repr(docs)))
    rules = parse.parse(text)
    tests.append(_case('inline-comment-kept',
                       'a comment inside a rule is an item; one before a rule is not',
                       rules == {'R': ('seq', [S('r'), ('comment', 'label'), N('X')]),
                                 'Q': ('seq', [S('q'), ('comment', 'ws: explicit'), S('z')])},
                       repr(rules)))

    r = render.Renderer(normalize.normalize_rules(rules))
    svg_r, svg_q = r.svg('R', doc=docs['R']), r.svg('Q')
    tests.append(_case('label-drawn', 'the comment is drawn as a label',
                       'class="label-text"' in svg_r and '>label</text>' in svg_r,
                       svg_r[:300]))
    tests.append(_case('directive-not-drawn', 'ws: directives are not drawn',
                       'ws: explicit' not in svg_q and 'class="label-text"' not in svg_q,
                       svg_q[:300]))
    tests.append(_case('svg-title-desc', 'SVG carries <title> and <desc>',
                       '<title>R</title><desc>What R does.' in svg_r, svg_r[:300]))

    # --- render ------------------------------------------------------------
    r = render.Renderer(normalize.normalize_rules(
        parse.parse("A ::= B C\nB ::= 'b'\nC ::= 'c' D\nD ::= [x]")),
        href=lambda n: '../index.html#' + n)
    svg = r.svg('A')
    tests.append(_case('inline-single-literal',
                       'a production that is one literal is drawn inline',
                       r.inline == {'B': 'b'} and '>b</text>' in svg
                       and 'index.html#B' not in svg, svg[:300]))
    tests.append(_case('nonterminal-link', 'production names link with the href prefix',
                       'xlink:href="../index.html#C"' in svg, svg[:300]))
    tests.append(_case('both-palettes', 'standalone SVG carries both palettes',
                       'prefers-color-scheme: dark' in svg, svg[-400:]))
    tests.append(_case('inline-svg-no-style', 'css=None leaves out the stylesheet',
                       '<style>' not in r.svg('A', css=None), ''))

    # --- the real grammar --------------------------------------------------
    with open(_SYNTAX_EBNF, encoding='utf-8') as fd:
        syntax = fd.read()
    out_dir = tempfile.mkdtemp(prefix='afwdev-ebnf-')
    try:
        count, warnings = ebnf.build_diagrams(syntax, out_dir, 'test')
        files = os.listdir(os.path.join(out_dir, 'diagram'))
        tests.append(_case('syntax-ebnf-renders',
                           'every production of syntax.ebnf gets a diagram',
                           count > 150 and len(files) == count
                           and os.path.exists(os.path.join(out_dir, 'index.html')),
                           '{} diagrams, {} files'.format(count, len(files))))
        tests.append(_case('syntax-ebnf-defined',
                           'syntax.ebnf uses no undefined names',
                           not warnings, '; '.join(warnings)))
    finally:
        shutil.rmtree(out_dir, ignore_errors=True)

    return {
        'description': 'EBNF railroad diagrams',
        'tests': tests,
    }


if __name__ == '__main__':
    # Allow `python3 ebnf_diagrams.py` from a source checkout.
    for t in run()['tests']:
        print('PASS' if t['passed'] else 'FAIL', t['test'], t['error'] or '')
