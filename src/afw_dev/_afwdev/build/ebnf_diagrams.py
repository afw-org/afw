#!/usr/bin/env python3

##
# @file ebnf_diagrams.py
# @ingroup afwdev_build
# @brief Build railroad diagrams from harvested syntax.ebnf.
#
# Pipeline:
#   1) afwdev generate harvests C /*ebnf>>> … <<<ebnf*/ → generated/ebnf/syntax.ebnf
#   2) _afwdev.build.ebnf parses it, applies rr.war-style rewrites, and draws
#      SVG in the handbook style: diagram/<Name>.svg plus index.html (the
#      "Syntax EBNF" page) under build/docs/…/reference/language/ebnf/syntax/
#   3) Handbook pages embed diagram/<Name>.svg via generated-src
#
# The whole grammar renders in well under a second, so it is regenerated on
# every docs build.
#

import os
import shutil

from _afwdev.build import ebnf
from _afwdev.common import msg, nfc


##
# @brief Builds EBNF diagrams.
# @param options The options dictionary.
#
def build(options):

    msg.highlighted_info('Building EBNF diagrams')

    # for now, building ebnf diagrams is only supported for afw srcdir
    options['srcdir_path'] = options['afw_package_dir_path'] + 'src/afw/'

    syntax_ebnf = options['srcdir_path'] + 'generated/ebnf/syntax.ebnf'
    # `afwdev ebnf` runs without the build setup that sets this.
    build_directory_docs = (options.get('build_directory_docs')
                            or options['afw_package_dir_path'] + 'build/docs/')
    syntax_output_dir = (
        build_directory_docs + 'afw/html/reference/language/ebnf/syntax/'
    )

    if not os.path.exists(syntax_ebnf):
        msg.error('syntax EBNF file not found: {}'.format(syntax_ebnf))
        return

    with nfc.open(syntax_ebnf, 'r') as fd:
        ebnf_text = fd.read()

    version = (options.get('srcdir_info') or {}).get('version')

    # Start clean so diagrams of removed productions do not linger.
    shutil.rmtree(syntax_output_dir, ignore_errors=True)
    try:
        count, warnings = ebnf.build_diagrams(ebnf_text, syntax_output_dir, version)
    except ebnf.parse.EbnfSyntaxError as e:
        msg.error_exit('{}: {}'.format(syntax_ebnf, e))
        return

    for warning in warnings:
        msg.warn('    ' + warning)
    msg.info('    {} EBNF diagrams written to {}'.format(count, syntax_output_dir))
