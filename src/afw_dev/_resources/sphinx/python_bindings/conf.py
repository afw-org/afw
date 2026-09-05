##
# @file conf.py
# @ingroup afwdev_build_docs
# @brief Sphinx configuration for the generated Python bindings reference.
#
# Copied by afwdev's docs build (docs.py) into a scratch build directory
# alongside a copy of src/afw/generated/python_bindings/, then run via
# sphinx-apidoc + sphinx-build. Not run in place, so paths are relative to
# this file's own directory at build time, not this source location.
#

import os
import sys

# The generated binding modules, merged with the real afw client package
# (session.py/request.py/exceptions.py), are copied under a package
# directory named afw/ next to this conf.py so their filenames (e.g.
# json.py, random.py) can't shadow same-named modules in the Python
# standard library.
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

project = 'Adaptive Framework Python Bindings'
copyright = '2026, Adaptive Framework Contributors'
author = 'Adaptive Framework Contributors'

extensions = [
    'sphinx.ext.autodoc',
    'sphinx.ext.napoleon',
]

napoleon_google_docstring = True
napoleon_numpy_docstring = False

# 'basic' is Sphinx's minimal built-in theme (least extra chrome/markup).
# Its own head/sidebar/footer are discarded entirely at build time -
# docs.py's _wrap_sphinx_html() extracts just the <div role="main"> content
# and re-wraps it in the handbook's own page shell (same header, sidenav,
# afw.css/afw.js) so these pages read as part of the site, not a bolted-on
# Sphinx site. See sphinx_theme_overrides.css for the matching color pass.
html_theme = 'basic'
html_show_sourcelink = False
html_copy_source = False
html_show_sphinx = False
