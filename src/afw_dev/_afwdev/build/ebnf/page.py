#!/usr/bin/env python3

##
# @file page.py
# @ingroup afwdev_build
# @brief The "Syntax EBNF" page: every production with its diagram.
#
# Diagrams are inline SVG, so rule names link to their production. The page
# carries the diagram stylesheet once; the inline SVGs carry none. Colors are
# the handbook's (Tailwind slate / sky) and follow prefers-color-scheme.
#

from html import escape
import re

from . import render

ISSUE_URL = 'https://github.com/afw-org/afw/issues/'

# Relative path from .../reference/language/ebnf/syntax/index.html to the
# docs root and to the language reference index.
_DOCS_ROOT = '../../../../../../'
_LANGUAGE = '../../index.html'

_PAGE_CSS = """
:root {
  --bg: #ffffff; --fg: #334155; --heading: #0f172a; --muted: #64748b;
  --line: rgba(15, 23, 42, 0.1); --accent: #0ea5e9; --link: #0369a1;
  --code-bg: #f8fafc; --code-fg: #334155; --chip: #f1f5f9;
  color-scheme: light;
}
@media (prefers-color-scheme: dark) {
  :root {
    --bg: #0f172a; --fg: #94a3b8; --heading: #e2e8f0; --muted: #64748b;
    --line: rgba(248, 250, 252, 0.08); --accent: #38bdf8; --link: #7dd3fc;
    --code-bg: #1e293b; --code-fg: #cbd5e1; --chip: #1e293b;
    color-scheme: dark;
  }
}
* { box-sizing: border-box; }
html { scroll-padding-top: 5rem; }
body { margin: 0; background: var(--bg); color: var(--fg);
  font: 16px/1.6 "Inter var", Inter, ui-sans-serif, system-ui, -apple-system, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased; }
a { color: var(--link); text-decoration: none; }
a:hover { text-decoration: underline; }
code, pre, .mono { font-family: ui-monospace, SFMono-Regular, Menlo, Consolas, "Liberation Mono", monospace; }
.bar { position: sticky; top: 0; z-index: 10; background: var(--bg); border-bottom: 1px solid var(--line); }
.bar-inner { max-width: 90rem; margin: 0 auto; padding: 1rem 2rem; display: flex; flex-wrap: wrap; align-items: center; gap: .25rem 1rem; }
.brand { font-weight: 700; font-size: 1.125rem; color: var(--heading); }
.version { font-size: .875rem; color: var(--muted); }
.crumbs { margin-left: auto; font-size: .875rem; font-weight: 600; display: flex; gap: .5rem; }
.crumbs span { color: var(--muted); }
main { max-width: 90rem; margin: 0 auto; padding: 2rem; }
h1 { margin: 0; font-size: 1.875rem; font-weight: 800; letter-spacing: -0.02em; color: var(--heading); }
.intro { max-width: 48rem; font-size: 1.125rem; margin: .75rem 0 0; }
.legend { display: flex; flex-wrap: wrap; gap: .5rem 1.25rem; margin: 1rem 0 0; padding: 0; list-style: none; font-size: .875rem; }
.legend li { display: flex; align-items: center; gap: .5rem; }
.chip { display: inline-block; padding: .05rem .6rem; font-size: .8rem; border: 1px solid; }
.chip.terminal { border-radius: 999px; font-family: ui-monospace, SFMono-Regular, Menlo, monospace; color: #0369a1; background: #f0f9ff; border-color: #7dd3fc; }
.chip.nonterminal { border-radius: 4px; color: #334155; background: #ffffff; border-color: #cbd5e1; }
.chip.regexp { font-family: ui-monospace, SFMono-Regular, Menlo, monospace; color: #475569; background: #f8fafc; border-color: #cbd5e1;
  clip-path: polygon(8px 0, calc(100% - 8px) 0, 100% 50%, calc(100% - 8px) 100%, 8px 100%, 0 50%); padding: .05rem .9rem; }
.chip.label { border: 0; padding: 0; font-style: italic; color: #64748b; }
@media (prefers-color-scheme: dark) {
  .chip.terminal { color: #7dd3fc; background: #13283f; border-color: #0284c7; }
  .chip.nonterminal { color: #e2e8f0; background: #1e293b; border-color: #334155; }
  .chip.regexp { color: #94a3b8; background: #172033; border-color: #334155; }
  .chip.label { color: #94a3b8; }
}
.az { margin: 2rem 0 0; padding: 1rem 0; border-top: 1px solid var(--line); border-bottom: 1px solid var(--line);
  columns: 14rem; column-gap: 1.5rem; font-size: .875rem; }
.az a { display: block; break-inside: avoid; overflow-wrap: anywhere; }
.prod { margin-top: 3rem; }
.prod h2 { margin: 0; font-size: 1.05rem; font-weight: 600; color: var(--heading); }
.prod h2 a { color: inherit; }
.prod .src { font-size: .8rem; color: var(--muted); }
.doc { max-width: 48rem; margin: .5rem 0 0; }
.fig { margin-top: .75rem; overflow-x: auto; }
.fig svg { display: block; max-width: none; height: auto; }
pre.ebnf { margin: .75rem 0 0; padding: .75rem 1rem; background: var(--code-bg); color: var(--code-fg);
  border-radius: .5rem; overflow-x: auto; font-size: .8rem; line-height: 1.6; }
.refs { margin: .6rem 0 0; font-size: .875rem; }
.refs b { color: var(--heading); font-weight: 600; }
@media (max-width: 640px) {
  .bar-inner, main { padding-left: 1rem; padding-right: 1rem; }
  .crumbs { margin-left: 0; }
}
"""

_TEXT_TOKEN = re.compile(r"""
   (?P<str>'[^']*'|"[^"]*")
  |(?P<comment>/\*.*?\*/)
  |(?P<cls>\[\^?(?:[^\]\\]|\\.)*\])
  |(?P<name>[A-Za-z_][A-Za-z0-9_]*)
  |(?P<other>.)
""", re.S | re.X)


def _link_ebnf(text, linkable):
    """EBNF source as HTML with production names linked."""
    out = []
    for m in _TEXT_TOKEN.finditer(text):
        if m.lastgroup == 'name' and m.group() in linkable:
            out.append('<a href="#{0}">{0}</a>'.format(m.group()))
        else:
            out.append(escape(m.group()))
    return ''.join(out)


def _doc_html(doc):
    """Doc comment paragraphs; #NN links to the GitHub issue."""
    paragraphs = []
    for p in doc.split('\n\n'):
        html = escape(p).replace('\n', '<br>')
        html = re.sub(r'#(\d+)\b', r'<a href="' + ISSUE_URL + r'\1">#\1</a>', html)
        paragraphs.append('<p class="doc">' + html + '</p>')
    return '\n'.join(paragraphs)


##
# @brief Build the page.
# @param renderer   render.Renderer for the grammar (links '#Name').
# @param names      Productions to show, in order.
# @param docs       name -> doc comment (parse.doc_comments()).
# @param texts      name -> EBNF source text (parse.production_text()).
# @param sources    name -> harvested-from file (parse.source_files()).
# @param referenced name -> sorted names that use it.
# @param version    Version string for the header, or None.
# @return The HTML document.
#
def build_page(renderer, names, docs, texts, sources, referenced, version=None):
    linkable = set(names)
    sections = []
    for name in names:
        refs = referenced.get(name) or []
        ref_html = ', '.join('<a href="#{0}">{0}</a>'.format(r) for r in refs)
        sections.append("""
<section class="prod" id="{name}">
  <h2><a href="#{name}">{name}</a> <span class="src">· {src}</span></h2>
  {doc}
  <div class="fig">{svg}</div>
  <pre class="ebnf">{ebnf}</pre>
  {refs}
</section>""".format(
            name=name,
            src=escape(sources.get(name, '')),
            doc=_doc_html(docs[name]) if docs.get(name) else '',
            svg=renderer.svg(name, css=None, doc=docs.get(name)),
            ebnf=_link_ebnf(texts.get(name, ''), linkable),
            refs=('<p class="refs"><b>Referenced by</b> ' + ref_html + '</p>') if refs else ''))

    az = '\n'.join('<a href="#{0}">{0}</a>'.format(n) for n in sorted(names, key=str.lower))
    return """<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Adaptive Framework — Syntax EBNF</title>
<style>{page_css}</style>
<style>{diagram_css}</style>
</head>
<body>
<header class="bar"><div class="bar-inner">
  <span class="brand">Adaptive Framework</span>
  {version}
  <nav class="crumbs" aria-label="Breadcrumb">
    <a href="{docs_root}index.html">Docs</a><span>/</span>
    <a href="{language}">Language</a><span>/</span>
    <span>Syntax EBNF</span>
  </nav>
</div></header>
<main>
  <h1>Syntax EBNF</h1>
  <p class="intro">Railroad diagrams for every production of the Adaptive
  syntax, generated from the W3C EBNF in the compiler's C sources. The C parser
  is authoritative where it and this grammar disagree. Select a rule name to go
  to its diagram.</p>
  <ul class="legend">
    <li><span class="chip terminal">if</span> literal token</li>
    <li><span class="chip nonterminal">Expression</span> production</li>
    <li><span class="chip regexp">[0-9]</span> character class</li>
    <li><span class="chip label">label</span> note from the grammar</li>
  </ul>
  <nav class="az" aria-label="Productions">
{az}
  </nav>
{sections}
</main>
</body>
</html>
""".format(
        page_css=_PAGE_CSS,
        diagram_css=render.CSS,
        version=('<span class="version">' + escape(version) + '</span>') if version else '',
        docs_root=_DOCS_ROOT,
        language=_LANGUAGE,
        az=az,
        sections='\n'.join(sections))
