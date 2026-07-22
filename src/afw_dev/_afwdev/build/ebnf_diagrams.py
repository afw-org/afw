#!/usr/bin/env python3

##
# @file ebnf_diagrams.py
# @ingroup afwdev_build
# @brief Build railroad diagrams from harvested syntax.ebnf (rr.war).
#
# Pipeline:
#   1) afwdev generate harvests C /*ebnf>>> … <<<ebnf*/ → generated/ebnf/syntax.ebnf
#   2) This module runs java -jar rr.war (Railroad Diagram Generator) to PNG zip
#   3) Zip is extracted under build/docs/…/reference/language/ebnf/syntax/
#   4) index.html is post-processed so the full grammar page matches handbook
#      slate light/dark chrome when opened from Docs → Language → Syntax EBNF
#
# Manual use of the harvested .ebnf (bottlecaps web UI) remains documented in
# the harvest header comments in compile sources / generated syntax.ebnf.
#

import os
import re
import shutil
import zipfile
from _afwdev.common import msg, nfc

# Relative path from …/reference/language/ebnf/syntax/index.html → docs root
# (where afw.css / afw.js are written by the handbook docs build).
_DOCS_ROOT_REL = '../../../../../../'

# Injected after RR's own <style> so we can override body/tabs/links without
# rewriting the large generated sheet. Marker lets re-runs stay idempotent.
_THEME_BEGIN = '<!-- AFW-RR-THEME-BEGIN -->'
_THEME_END = '<!-- AFW-RR-THEME-END -->'

_AFW_RR_THEME_HTML = r'''<!-- AFW-RR-THEME-BEGIN -->
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="''' + _DOCS_ROOT_REL + r'''afw.css">
<style type="text/css" id="afw-rr-theme">
  /* Align Railroad Diagram Generator HTML with AFW handbook slate chrome */
  html {
    color-scheme: light dark;
  }
  body {
    font-family: ui-sans-serif, system-ui, -apple-system, "Segoe UI", Roboto,
      "Helvetica Neue", Arial, sans-serif !important;
    font-size: 14px !important;
    color: #334155 !important;           /* slate-700 */
    background: #f8fafc !important;      /* slate-50 */
    margin: 0 !important;
    padding: 0 0 2.5rem 0 !important;
  }
  a:link, a:visited {
    color: #0369a1 !important;           /* sky-700 */
  }
  a:hover {
    color: #0ea5e9 !important;           /* sky-500 */
  }
  a.button, #tabs li a {
    border: 1px solid #cbd5e1 !important;
    background: #f1f5f9 !important;
    color: #334155 !important;
    border-radius: 0.25rem;
    font-weight: 600 !important;
  }
  a.button:hover, #tabs li a:hover {
    color: #0f172a !important;
    background: #e2e8f0 !important;
    border-color: #94a3b8 !important;
  }
  #tabs {
    margin-top: 0 !important;
    padding: 0.5rem 1rem !important;
    border-bottom: 1px solid #e2e8f0 !important;
    background: #ffffff;
  }
  #tabs li a.active {
    color: #0369a1 !important;
    background: #ffffff !important;
    border-color: #e2e8f0 !important;
    border-bottom-color: #ffffff !important;
  }
  #text {
    border-color: #e2e8f0 !important;
    background: #ffffff !important;
    color: #0f172a !important;
  }
  .ebnf a:hover, .grammar a:hover {
    color: #0ea5e9 !important;
  }
  .ebnf code, .grammar code, textarea, pre {
    font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas,
      "Liberation Mono", "Courier New", monospace !important;
  }
  #divs div.ebnf, .ebnf code {
    background: #f1f5f9;
    color: #1e293b;
  }
  .signature, a:link.signature, a:visited.signature {
    color: #64748b !important;
  }
  .afw-rr-banner {
    position: sticky;
    top: 0;
    z-index: 40;
    width: 100%;
    backdrop-filter: blur(8px);
    background: rgba(255, 255, 255, 0.95);
    border-bottom: 1px solid #e2e8f0;
    padding: 0.75rem 1.25rem;
    box-sizing: border-box;
    margin-bottom: 1rem;
  }
  .afw-rr-banner-inner {
    max-width: 90rem;
    margin: 0 auto;
    display: flex;
    flex-wrap: wrap;
    align-items: baseline;
    gap: 0.5rem 1rem;
  }
  .afw-rr-banner .afw-rr-title {
    font-weight: 700;
    font-size: 1.05rem;
    color: #0f172a;
  }
  .afw-rr-banner .afw-rr-sub {
    font-size: 0.875rem;
    color: #64748b;
  }
  .afw-rr-banner nav {
    margin-left: auto;
    font-size: 0.875rem;
    font-weight: 600;
  }
  .afw-rr-banner nav a {
    color: #0369a1 !important;
    text-decoration: none;
  }
  .afw-rr-banner nav a:hover {
    color: #0ea5e9 !important;
  }
  .afw-rr-main {
    max-width: 90rem;
    margin: 0 auto;
    padding: 0 1.25rem;
  }
  .afw-rr-main p[style*="font-weight:bold"],
  .afw-rr-main p[style*="font-weight: bold"] {
    color: #0f172a !important;
    margin-top: 1.25rem;
  }
  .afw-rr-main img {
    max-width: 100%;
    height: auto;
    /* Light: keep rr -color:#DCDCDC gray-on-light PNG as generated */
    background: transparent;
    border-radius: 0.25rem;
  }
  hr {
    border: none;
    border-top: 1px solid #e2e8f0;
    margin: 1.5rem 0;
  }

  @media (prefers-color-scheme: dark) {
    body {
      color: #94a3b8 !important;         /* slate-400 */
      background: #0f172a !important;    /* slate-900 */
    }
    a:link, a:visited {
      color: #38bdf8 !important;         /* sky-400 */
    }
    a:hover {
      color: #7dd3fc !important;
    }
    a.button, #tabs li a {
      border-color: #334155 !important;
      background: #1e293b !important;
      color: #e2e8f0 !important;
    }
    a.button:hover, #tabs li a:hover {
      color: #f1f5f9 !important;
      background: #334155 !important;
      border-color: #64748b !important;
    }
    #tabs {
      border-bottom-color: #334155 !important;
      background: #0f172a;
    }
    #tabs li a.active {
      color: #38bdf8 !important;
      background: #0f172a !important;
      border-color: #334155 !important;
      border-bottom-color: #0f172a !important;
    }
    #text {
      border-color: #334155 !important;
      background: #1e293b !important;
      color: #e2e8f0 !important;
    }
    #divs div.ebnf, .ebnf code {
      background: #1e293b;
      color: #e2e8f0;
    }
    .signature, a:link.signature, a:visited.signature {
      color: #64748b !important;
    }
    .afw-rr-banner {
      background: rgba(15, 23, 42, 0.92);
      border-bottom-color: rgba(248, 250, 252, 0.08);
    }
    .afw-rr-banner .afw-rr-title {
      color: #f1f5f9;
    }
    .afw-rr-banner .afw-rr-sub {
      color: #94a3b8;
    }
    .afw-rr-banner nav a {
      color: #38bdf8 !important;
    }
    .afw-rr-main p[style*="font-weight:bold"],
    .afw-rr-main p[style*="font-weight: bold"] {
      color: #f1f5f9 !important;
    }
    /*
     * Same trick as handbook image.html (class="dark:invert"): RR PNGs are
     * dark strokes on light fills (-color:#DCDCDC). Invert in dark mode so
     * diagrams blend with slate-900 pages (statements.html uses Tailwind
     * dark:invert on the same PNGs).
     */
    .afw-rr-main img {
      background: transparent;
      filter: invert(1);
    }
    hr {
      border-top-color: #334155;
    }
  }
</style>
<!-- AFW-RR-THEME-END -->
'''

_AFW_RR_BANNER = r'''
<div class="afw-rr-banner">
  <div class="afw-rr-banner-inner">
    <span class="afw-rr-title">Adaptive Framework</span>
    <span class="afw-rr-sub">Syntax EBNF (railroad diagrams)</span>
    <nav>
      <a href="../../index.html">Language</a>
      &nbsp;·&nbsp;
      <a href="''' + _DOCS_ROOT_REL + r'''index.html">Docs home</a>
    </nav>
  </div>
</div>
<div class="afw-rr-main">
'''


def _theme_rr_index_html(index_path):
    """Post-process Railroad Generator index.html for handbook-adjacent look."""
    if not os.path.isfile(index_path):
        return
    with nfc.open(index_path, 'r') as fd:
        html = fd.read()

    # Idempotent: strip a previous injection
    if _THEME_BEGIN in html:
        html = re.sub(
            re.escape(_THEME_BEGIN) + r'.*?' + re.escape(_THEME_END),
            '',
            html,
            count=1,
            flags=re.S,
        )
    if 'class="afw-rr-banner"' in html:
        html = re.sub(
            r'\s*<div class="afw-rr-banner">.*?</div>\s*<div class="afw-rr-main">\s*',
            '\n   <body>\n',
            html,
            count=1,
            flags=re.S,
        )
        html = html.replace('</div>\n   </body>', '</body>')
        html = html.replace('</div>\n</body>', '</body>')

    if '</head>' in html:
        html = html.replace('</head>', _AFW_RR_THEME_HTML + '\n   </head>', 1)
    else:
        msg.warn('    RR index has no </head>; theme CSS not injected: ' + index_path)
        return

    # Wrap body content in banner + main for max-width / sticky header
    body_m = re.search(r'<body[^>]*>', html, flags=re.I)
    if body_m:
        insert_at = body_m.end()
        html = html[:insert_at] + _AFW_RR_BANNER + html[insert_at:]
        # Close .afw-rr-main before </body>
        html = re.sub(
            r'</body>',
            '</div><!-- /.afw-rr-main -->\n   </body>',
            html,
            count=1,
            flags=re.I,
        )

    # Title
    html = re.sub(
        r'<title>[^<]*</title>',
        '<title>Adaptive Framework — Syntax EBNF</title>',
        html,
        count=1,
        flags=re.I,
    )

    with nfc.open(index_path, 'w') as fd:
        fd.write(html)
    msg.info('    Applied AFW theme to ' + index_path)


##
# @brief Builds EBNF diagrams.
# @param options The options dictionary.
#
def build(options):

    msg.highlighted_info('Building EBNF diagrams')

    # for now, building ebnf diagrams is only supported for afw srcdir
    options['srcdir_path'] = options['afw_package_dir_path'] + 'src/afw/'

    syntax_ebnf = options['srcdir_path'] + '/generated/ebnf/syntax.ebnf'
    syntax_output_dir_html = (
        options['build_directory_docs']
        + 'afw/html/reference/language/ebnf/syntax/'
    )
    syntax_output_dir_xml = (
        options['build_directory_docs']
        + 'afw/xml/reference/language/ebnf/syntax/'
    )

    if not os.path.exists(syntax_ebnf):
        msg.error('syntax EBNF file not found: {}'.format(syntax_ebnf))
        return

    need_rr = True
    # Skip rr.war when outputs are newer than harvested EBNF (diagrams unchanged).
    if os.path.exists(syntax_output_dir_html):
        syntax_ebnf_mtime = os.path.getmtime(syntax_ebnf)
        syntax_output_dir_mtime = os.path.getmtime(syntax_output_dir_html)
        if syntax_ebnf_mtime < syntax_output_dir_mtime:
            msg.info('    EBNF diagrams are up to date')
            need_rr = False

    if need_rr and os.path.exists(syntax_output_dir_xml):
        syntax_ebnf_mtime = os.path.getmtime(syntax_ebnf)
        syntax_output_dir_mtime = os.path.getmtime(syntax_output_dir_xml)
        if syntax_ebnf_mtime < syntax_output_dir_mtime:
            msg.info('    EBNF diagrams are up to date')
            need_rr = False

    if need_rr:
        # Railroad Diagram Generator (same engine as https://bottlecaps.de/rr/ui).
        # -color matches handbook-friendly gray used historically in the web UI.
        os.system(
            'java -jar /usr/local/bin/rr.war -png -color:#DCDCDC '
            '-out:/tmp/syntax.zip ' + syntax_ebnf
        )

        if not os.path.isfile('/tmp/syntax.zip') or not zipfile.is_zipfile(
            '/tmp/syntax.zip'
        ):
            msg.error_exit('Railroad generator failed to create zip file')
            return

        with zipfile.ZipFile('/tmp/syntax.zip', 'r') as zip_ref:
            msg.info('    Deleting old diagrams')
            shutil.rmtree(syntax_output_dir_html, ignore_errors=True)
            shutil.rmtree(syntax_output_dir_xml, ignore_errors=True)

            msg.info('    Extracting EBNF diagrams from zip archive')
            zip_ref.extractall(syntax_output_dir_html)
            zip_ref.extractall(syntax_output_dir_xml)

            msg.info(
                '    EBNF diagrams extracted to {}'.format(syntax_output_dir_html)
            )
            msg.info(
                '    EBNF diagrams extracted to {}'.format(syntax_output_dir_xml)
            )

    # Always skin HTML (and XML copy if present) so theme updates apply without
    # forcing a full rr.war re-run when only theming code changed.
    for out_dir in (syntax_output_dir_html, syntax_output_dir_xml):
        _theme_rr_index_html(os.path.join(out_dir, 'index.html'))
