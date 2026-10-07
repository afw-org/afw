#!/usr/bin/env python3

##
# @file __init__.py
# @ingroup afwdev_build
# @brief Railroad diagrams from harvested W3C EBNF, in pure Python.
#
# parse.py reads syntax.ebnf, normalize.py applies rr.war's default rewrites,
# render.py draws SVG in the handbook style (on vendored railroad.py), and
# page.py writes the "Syntax EBNF" page.
#

import os

from _afwdev.common import nfc

from . import normalize, page, parse, render

__all__ = ['build_diagrams', 'parse', 'normalize', 'render', 'page']


##
# @brief Write diagram/<Name>.svg for every production, and index.html.
#
# Standalone SVGs carry both palettes and link to the production on
# ../index.html. A production that is a single literal is drawn inline where
# it is used and gets no diagram (rr.war does the same).
#
# @param ebnf_text The harvested grammar (syntax.ebnf).
# @param out_dir   Output directory; created if missing.
# @param version   Version for the page header, or None.
# @return (number of diagrams, list of warning strings).
# @throws parse.EbnfSyntaxError when the grammar cannot be read.
#
def build_diagrams(ebnf_text, out_dir, version=None):
    parsed = parse.parse(ebnf_text)
    rules = normalize.normalize_rules(parsed)
    docs = parse.doc_comments(ebnf_text)

    warnings = []
    uses = parse.references(parsed)
    for name, used in uses.items():
        for missing in sorted(used - set(parsed)):
            warnings.append('{} uses {}, which has no production'.format(name, missing))

    standalone = render.Renderer(rules, href=lambda n: '../index.html#' + n)
    names = [n for n in rules if n not in standalone.inline]

    diagram_dir = os.path.join(out_dir, 'diagram')
    os.makedirs(diagram_dir, exist_ok=True)
    for name in names:
        with nfc.open(os.path.join(diagram_dir, name + '.svg'), 'w') as fd:
            fd.write(standalone.svg(name, doc=docs.get(name)))

    referenced = {}
    for name, used in uses.items():
        for u in used:
            if u != name:
                referenced.setdefault(u, []).append(name)
    referenced = {k: sorted(v, key=str.lower) for k, v in referenced.items()}

    html = page.build_page(
        render.Renderer(rules), names, docs, parse.production_text(ebnf_text),
        parse.source_files(ebnf_text), referenced, version)
    with nfc.open(os.path.join(out_dir, 'index.html'), 'w') as fd:
        fd.write(html)

    return len(names), warnings
