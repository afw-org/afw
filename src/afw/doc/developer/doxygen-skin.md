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
3. **API/group prose** → `src/afw/include/afw_doxygen.h`, interface XML,
   hand headers — separate from the skin.

## Build and preview

```bash
./afwdev build --docs -j
# After CSS or Doxygen comment changes that must re-run doxygen:
./afwdev build --docs --clean -j
```

- Output: `build/docs/doxygen/html/` (e.g. `modules.html`, group pages).  
- Install (if used): `./afwdev build --docs --install -j` → typically
  `…/docs/doxygen/html/` under the web root.  
- After editing only the CSS file, you can **copy** it over an existing tree
  for a quick check:
  `cp src/afw/doc/doxygen-extra.css build/docs/doxygen/html/doxygen-extra.css`
  (and the install path if nginx serves from there), then **hard-refresh**
  the browser.  
- Handbook reference look (for comparison): docs path
  `afw_dev/html/reference/afwdev/index.html` under the same docs root.

## What broke the stock look (and how we fixed it)

| Symptom | Cause | Fix pattern |
|---------|--------|-------------|
| Menus still blue/gradient | `.sm-dox` rules in `tabs.css` use `tab_*.png` | `background-image: none !important` on `.sm-dox` / links / dropdowns; solid slate backgrounds |
| White strip under menu | `div.header { background-image: url(nav_h.png); …}` | Style **`div.header`**, not only `.header`; force image off |
| Group pages half-themed | Member tables / `.memtitle` / `.groupheader` still default | Override `.mdesc*`, `.memItem*`, `.memtitle`, `h2.groupheader`, field tables, `#powerTip` |
| Module names show `@ingroup …` | One-line `@defgroup name Title @ingroup parent` | Multi-line `@defgroup` + `@ingroup` in `afw_doxygen.h` |

## Guidance for changes

- Prefer **small, targeted** CSS additions when something still looks stock.  
- Re-check at least: `modules.html`, one group (e.g. compile), one call-macro
  detail, light and dark OS preference.  
- Do **not** replace all of `doxygen.css` via `HTML_STYLESHEET` (fragile across
  Doxygen upgrades); stay on `HTML_EXTRA_STYLESHEET`.  
- A full rewrite/simplification of `doxygen-extra.css` is optional cleanup only;
  the length is mostly necessary overrides, not accidental bloat.

## Discarding the skin

1. Delete `src/afw/doc/doxygen-extra.css`.  
2. Clear `HTML_EXTRA_STYLESHEET` in `Doxyfile`.  
3. Optionally restore `HTML_COLORSTYLE_HUE/SAT/GAMMA` defaults.  
4. `./afwdev build --docs --clean -j`.

## Related

- @ref afw_dev_overview — rebuild docs commands  
- Cursor rule `afw-interfaces-doxygen` (always-on for agents)  
- Group essays / module map: `src/afw/include/afw_doxygen.h`  
