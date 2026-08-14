Doxygen HTML skin {#afw_dev_doxygen_skin}
=================

@brief Maintainer notes for the experimental AFW Doxygen light/dark stylesheet.

## What this is

AFW’s handbook HTML (afwdev docs) uses a custom Tailwind dark layout. Doxygen
generates a separate tree under `build/docs/doxygen/html/` with its own
structure and default CSS.

To make Doxygen **feel related** (slate dark / clean light) without rewriting
Doxygen’s generator, we add:

| Piece | Path |
|-------|------|
| Extra stylesheet | `src/afw/doc/doxygen-extra.css` |
| Doxyfile hook | `HTML_EXTRA_STYLESHEET = src/afw/doc/doxygen-extra.css` |

This is **not** the same chrome as the handbook. Full theme sharing is not
supported by Doxygen; an extra sheet is the durable approach.

## Where to edit

1. **Colors / layout overrides** → `src/afw/doc/doxygen-extra.css`  
   (long file header = primary maintainer guide; read it first.)  
2. **Enable/disable skin** → package-root `Doxyfile` (`HTML_EXTRA_STYLESHEET`,
   optional `HTML_COLORSTYLE_*`).  
3. **Nav tabs / page layout** → package-root `DoxygenLayout.xml`
   (`LAYOUT_FILE` in `Doxyfile`). C-focused: **Docs** user tab
   (`../../index.html` → handbook home) + Modules + Related Pages + Data
   Structures; hierarchy / namespaces / examples tabs off.
4. **API/group prose** → `src/afw/include/afw_doxygen.h`, interface XML,
   hand headers — separate from the skin.

## Build and preview

```bash
./afwdev build --docs -j
# After Doxygen *content* (comments, Doxyfile) must re-run doxygen:
./afwdev build --docs --clean -j
```

**Important:** Doxygen is only invoked when `build/docs/doxygen/` is **missing**
(skip for speed). A normal `./afwdev build --docs` / `--fulldev` therefore may
**not** re-copy generated HTML. The docs builder **always refreshes**
`doxygen-extra.css` from `src/afw/doc/doxygen-extra.css` into
`build/docs/doxygen/html/` when that html dir exists; with `--install` it is
then copied to the web root (`--web-root`, default `/usr/share/nginx/html`).

- Output: `build/docs/doxygen/html/` (e.g. `modules.html`, group pages).  
- Served (devcontainer): often `/usr/share/nginx/html/docs/doxygen/html/…`
  via `./afwdev build --docs --install -j` (or fulldev).  
- Quick CSS-only check without full rebuild:
  ```bash
  cp src/afw/doc/doxygen-extra.css build/docs/doxygen/html/doxygen-extra.css
  cp src/afw/doc/doxygen-extra.css /usr/share/nginx/html/docs/doxygen/html/doxygen-extra.css
  ```
  Docs builds also rewrite HTML to `doxygen-extra.css?v=<mtime>` so browsers
  do not keep a stale skin after a 304. After a manual `cp`, either run
  `./afwdev build --docs -j` (refresh + bust) or hard-refresh with
  DevTools → Network → **Disable cache**.  
- **`afw.css` is the handbook sheet**, not Doxygen. Doxygen pages load
  `doxygen.css` + `doxygen-extra.css` only. A 304 on `afw.css` means the
  browser reused a cached handbook stylesheet; it does not apply to Doxygen.  
- Handbook reference look (for comparison): docs path
  `afw_dev/html/reference/afwdev/index.html` under the same docs root.
- **Project version:** `afwdev generate` (package `root_generate`) sets
  `Doxyfile` `PROJECT_NUMBER` from `afw-package.json` `version` (same bump
  that refreshes `*_version_info.h`). Commit the updated `Doxyfile` with
  other generate output. HTML still needs **`--docs --clean`** to rebuild
  Doxygen for a new number (no auto-clean on version change).

## What broke the stock look (and how we fixed it)

| Symptom | Cause | Fix pattern |
|---------|--------|-------------|
| Menus still blue/gradient | `.sm-dox` rules in `tabs.css` use `tab_*.png` | `background-image: none !important` on `.sm-dox` / links / dropdowns; solid slate backgrounds |
| White strip under menu | `div.header { background-image: url(nav_h.png); …}` | Style **`div.header`**, not only `.header`; force image off |
| Group pages half-themed | Member tables / `.memtitle` / `.groupheader` still default | Override `.mdesc*`, `.memItem*`, `.memtitle`, `h2.groupheader`, field tables, `#powerTip` |
| Module names show `@ingroup …` | One-line `@defgroup name Title @ingroup parent` | Multi-line `@defgroup` + `@ingroup` in `afw_doxygen.h` |
| Source left gutter white / lime-yellow | Stock `span.lineno` uses `#E8E8E8` + `border-right: 2px solid #0F0` | Theme `span.lineno` / `a`; border/background only on `div.fragment`, not every `div.line` |
| File list white squares | `span.icondoc` uses `doc.png` (white paper) | `background-image: none`; CSS-drawn `.icondoc` / folder icons |
| Hash / diagram jump lands under sticky bar | Sticky `#top` covers the target | `html { scroll-padding-top: var(--afw-dox-sticky-h) }` and `scroll-margin-top` on `:target` / anchors; retune `--afw-dox-sticky-h` if title+nav height changes |
| Header shorter / nav unlike handbook | Doxygen default chrome vs handbook `py-4` + `text-sm font-semibold` | Sticky `#top`; taller `#titlearea` padding; `.sm-dox` `text-sm` / `font-weight: 600` / roomier line-height |

## Guidance for changes

- Prefer **small, targeted** CSS additions when something still looks stock.  
- Re-check at least: `modules.html`, one group (e.g. compile), one call-macro
  detail, one `*_source.html` page, light and dark OS preference; click a
  member / diagram link and confirm the target is not under the sticky bar.  
- Do **not** replace all of `doxygen.css` via `HTML_STYLESHEET` (fragile across
  Doxygen upgrades); stay on `HTML_EXTRA_STYLESHEET`.  
- A full rewrite/simplification of `doxygen-extra.css` is optional cleanup only;
  the length is mostly necessary overrides, not accidental bloat.

## Discarding the skin

1. Delete `src/afw/doc/doxygen-extra.css`.  
2. Clear `HTML_EXTRA_STYLESHEET` in `Doxyfile`.  
3. Optionally restore `HTML_COLORSTYLE_HUE/SAT/GAMMA` defaults.  
4. `./afwdev build --docs --clean -j`.

## C-focused Doxyfile knobs (not only the skin)

| Setting | Intent |
|---------|--------|
| `OPTIMIZE_OUTPUT_FOR_C = YES` | “Data Structures” wording, C-friendly index |
| `TYPEDEF_HIDES_STRUCT = YES` | Prefer public `afw_*_t` over `*_s` tags |
| `SHOW_NAMESPACES = NO` | Hide useless namespace chrome |
| `LAYOUT_FILE = DoxygenLayout.xml` | Modules / Related Pages / Data Structures nav |
| `CLASS_GRAPH` / `COLLABORATION_GRAPH` / `GRAPHICAL_HIERARCHY = NO` | Avoid C++-style graphs (also `HAVE_DOT = NO` today) |
| `PROJECT_BRIEF` | One-line subtitle under the project name |
| `PROJECT_NUMBER` | From `afw-package.json` via `afwdev generate` |
| `FILE_PATTERNS` | `*.c` / `*.h` / `*.md` (not the whole repo noise) |

## Types: typedef vs struct (C API)

AFW names public types as **`afw_*_t`** via typedefs (often in
`afw_common_opaques.h` / generated `afw_interface_opaques.h`). The struct tag
is usually `afw_*_s`, with the full definition in another header or left
incomplete on purpose.

Doxyfile **`TYPEDEF_HIDES_STRUCT = YES`** so Data Structures and detail pages
prefer the **typedef name** people use in code, not the `*_s` tag.

Call macros (`afw_<iface>_<method>`) are the method-like API for those
types (`@relates` instance `*_t` in generated headers); see interface groups
and @ref afw_dev_interfaces.

## Related

- @ref afw_dev_overview — rebuild docs commands  
- Cursor rule `afw-interfaces-doxygen` (always-on for agents)  
- Group essays / module map: `src/afw/include/afw_doxygen.h`  
