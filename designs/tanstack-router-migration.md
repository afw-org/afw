# Admin app: React Router 5 → TanStack Router

**Status (2026-10-02):** on `feat/tanstack-router`: steps 1-4 (bridge + shell, `/Admin` + Schema, Objects, Admin/Models) and Tools committed; the Services group (Services, Adapters, Logs, AuthHandlers) in review. Lessons are under *Patterns*; known issues under *Open questions*.

**Scope:** `src/afw_app/admin` only. Since #451 the component libraries (`@afw/react`, `@afw/react-material-ui`) import no router: they go through the navigation contract (`useNavigation()` → `Link`, `useNavigate`, `NavigationBlocker`), and the app adapts its router in one file, `admin/src/navigation.js`.

## Why TanStack Router, not React Router 7

- **Direction.** React Router keeps changing its model — 5 → 6 was a rewrite, 7 merged Remix and pushes a framework mode, 8 requires React ≥ 19.2.7. TanStack Router stays a library and composes with the rest of TanStack.
- **Room to grow.** TanStack Query may be useful later; Router and Query are designed to work together (route loaders can prefetch through a QueryClient). Query is **out of scope** here — it overlaps with our own model cache (`useGetObject` / `useRetrieveObjects`, `AfwModel` invalidation) and deserves its own pad.
- **Guards.** Both need a central route tree once `<Prompt>` goes away (React Router 6/7's `useBlocker` only works with a data router). TanStack's `useBlocker` works with plain code-defined routes and also covers `beforeunload`.
- **Cost of its main feature.** TanStack's headline is typed routes, params and search; the admin app is JavaScript, so we mostly don't get that unless the app moves to TypeScript. Accepted.

## Verified against `@tanstack/react-router` 1.170.41 (`@tanstack/history` 1.162.4)

Exports used: `createRootRoute`, `createRoute`, `createRouter`, `RouterProvider`, `Outlet`, `Link`, `useNavigate`, `useLocation`, `useParams`, `useMatch`, `useMatchRoute`, `useRouterState`, `useBlocker`, `Block`, `createBrowserHistory`, `createMemoryHistory`. React peer `>=18 || >=19` (we are on 19.3).

- `createRouter({ basepath })` covers `BrowserRouter basename={import.meta.env.BASE_URL}`.
- Navigation and links take a `hash` option; `useLocation().hash` reads it (`#tree`, `#overview`, … perspectives).
- `useBlocker({ shouldBlockFn, enableBeforeUnload, disabled, withResolver })`.

## Mapping

| React Router 5 | TanStack Router |
|---|---|
| `<BrowserRouter basename getUserConfirmation>` | `createRouter({ routeTree, basepath, history })` + `<RouterProvider>` |
| nested `<Switch>`/`<Route path>` in each section | one code-defined route tree; a section's layout renders `<Outlet/>` |
| `useHistory().push(to)` / `.replace` | `useNavigate()({ to, hash, replace })` |
| `useLocation()` | `useLocation()` (`pathname`, `search`, `hash`) |
| `useRouteMatch(path)`, `matchPath(path, opts)` | `useParams`, `useMatch` / `useMatchRoute`; route params instead of re-parsing paths |
| `<Prompt when message>` | `useBlocker({ shouldBlockFn: () => when && !window.confirm(message), enableBeforeUnload: () => when })` |
| tests: `<Router history={createMemoryHistory()}>` + `history.push` | `createRouter({ routeTree, history: createMemoryHistory({ initialEntries }) })` + `router.navigate` |
| `navigation.js` adapter (RR5) | same three members over TanStack (`Link`, `useNavigate`, `useBlocker`) |

**Code-based routes, not file-based.** File-based routing needs the Vite plugin's generated route tree; code-based keeps each section owning its route definitions (a section module exports its routes, the root assembles them) and needs no codegen.

## The bridge: migrate one section at a time

Each router normally owns the browser history, so two routers at once would drift. The bridge avoids that:

- TanStack Router owns the real history (`createBrowserHistory`).
- A catch-all route (`/$`) renders the not-yet-migrated app inside React Router 5's `<Router history={rr5History}>`, where `rr5History` is a small adapter presenting `@tanstack/history`'s `RouterHistory` in `history` v4 shape:
  - `location` ← `{ pathname, search, hash, state, key }`; `listen(fn)` ← `subscribe` (it passes `{ location, action: { type } }` with `PUSH` / `REPLACE` / `BACK` / `FORWARD` / `GO`; RR5 expects `PUSH` / `REPLACE` / `POP`, so map the last three to `POP`); `push`/`replace` (string or location object) ← `push`/`replace`; `go`/`goBack`/`goForward` ← `go`/`back`/`forward`; `createHref` ← `createHref`; `block(prompt)` ← `block({ blockerFn })`.
  - **Base path:** TanStack's history sees full paths (`/apps/afw/admin/…`); RR5's routes expect them without the base. The adapter strips the base on read and adds it on push.
- All navigation from either side goes through the one TanStack history, so both routers see the same location.
- A section migrates by moving its routes into the TanStack tree (matched before the catch-all) and replacing its RR5 hooks. Legacy sections keep working meanwhile; `navigation.js` keeps adapting RR5 until the end.

The adapter is the riskiest piece — prove it in the first slice (link clicks, back/forward, a `<Prompt>` through `block`, the base path) before converting sections.

## Patterns

- **Layout route + per-level catch-all.** A section whose parent wraps every page (Admin.js: config loading, `ConfigContext`, `Container`, `RouteBasePathContext`) becomes a TanStack layout route rendering that wrapper around `<Outlet/>`. Its children are the migrated pages plus a `$` catch-all that renders the parent's remaining React Router 5 `<Route>`s - here `AdminLayout` / `AdminLegacyRoutes` in Admin.js, wired in `Admin/routes.js`. The root catch-all stays for top-level sections.
- **Drill-down views: one route with optional params.** Schema (adapter > object type > property) passed data loaded at each level to the next through RR5 `render` props, which `<Outlet/>` can't do. One route, `Schema/{-$adapterId}/{-$objectTypeId}/{-$propertyName}`, with each component reading `useParams({ strict: false })` and rendering its list or its child, keeps that data flow and turns each `<Switch>` into a conditional.
- **Lazy components.** Route components load with `lazyRouteComponent(() => import(...), "ExportName")`, keeping section chunks; `defaultPendingComponent: Loading` replaces the old `Suspense` fallback.
- **Links need no change.** `@afw/react`'s `Link` goes through `navigation.js` (still RR5), whose pushes reach TanStack through the bridge.
- **Tests.** `renderRoute(path)` in `src/test-utils.js` renders the real route tree on a memory history (`router.navigate()` moves it), so tests of migrated sections also cover the layout route and params.

From step 3 (Objects):

- **Search strings stay raw.** TanStack's default search handling parses `?a=b` pairs and re-serializes them, mangling the criteria AFW puts there (RQL: `?eq(a,b)&sort(+objectId)` - the `+` became a space, `=` got appended). `router.js` sets `parseSearch`/`stringifySearch` that keep the string as `{ raw }`; read it with `useLocation().searchStr` (includes the `?`). The history itself was never rewritten, so RR5 code reading it through the bridge was unaffected.
- **Params and pathname arrive decoded**, without the basepath (TanStack applies `basepath` as a rewrite). Drop the old `decodeURIComponent(match.params.x)`; `encodeURIComponent` each segment when building an href; compare against decoded values (the Objects guard does).
- **`location.hash` has no leading `#`** (RR5's did). Navigate with `navigate({ hash })` rather than rebuilding `pathname + "#" + x` from a decoded pathname.
- **Drill-down with a tail:** `Objects/{-$adapterId}/{-$objectTypeId}/{-$objectId}/$` - the splat (`_splat`, decoded) is the embedded object path. An encoded `/` inside it decodes to a real one; fine for property names.
- **Guards:** `<Prompt when message>` becomes `useBlocker({ disabled: !dirty, shouldBlockFn: ({next}) => !stayingInside(next.pathname) && !window.confirm(message) })`, called at the top of the component (before early returns). It sits on the same TanStack history as RR5 `<Prompt>`s going through the bridge.
- **Navigation is async.** A TanStack navigation commits after the click's `act()` returns, so a test whose last step navigates must wait on the result (`await waitFor(() => expect(router.state.location.hash).toBe("source"))`) or React warns about updates outside `act()`. And a navigation a guard **blocks never resolves `router.navigate()`'s promise** - tests start it inside `act()` without awaiting it, then wait on what they expect (see `Objects/__tests__/ObjectEditorGuard.test.js`).

From step 4 (Admin/Models):

- **Grammar-heavy sections keep their patterns.** The model editor's URL grammar (17 exact patterns in three views, ~15 more in its context menu, positional parsing in `Models`) stays as written: one route `Admin/Models/{-$adapterId}/{-$modelId}/$`, and `router/matchPath.js` - React Router 5's `matchPath` semantics (`{ path, exact }`, arrays, `:params`, case- and trailing-slash-insensitive) - applied to TanStack's decoded pathname. `ModelEditor`'s `<Switch>` became the first matching pattern list (`modelPaths`, `objectTypePaths`, `propertyTypePaths`).
- **`useLocationHash()`** (`router/hooks.js`) returns the hash in React Router 5's form (`"#tree"` or `""`) for code that appends it to links. Import `router/matchPath` and `router/hooks` directly, not `router/index` (that pulls in the route tree).
- **Tests reading `history.location`** after render switch to `router.state.location` (from `renderRoute`); hashes there have no `#`. With the local `history` gone, a leftover `history.location` silently reads `window.history` (no `.location`).

From Tools and the Services group:

- **Layout + index:** a section whose page is a wrapper around its tools (`Tools.js`) is a layout route with an index child (`ToolsHome`) and one child per tool.
- **List/detail pages** (`Services`, `Adapters`, `Logs`, `AuthHandlers`): one route with an optional id, `Services/{-$serviceId}`; the detail `render` callback becomes a local `renderX()` function, and `match.url + "/" + id` links become the literal base path plus `encodeURIComponent(id)`.
- **Drop RR5 route props.** `render={(props) => <X {...props} />}` passed `match`/`location`/`history` along; nothing used them, so the spreads go (check the target for `props.match|location|history` first).

## Inventory (admin `src/`, excluding tests unless noted)

| Section | Files using RR | `<Route>` | RR hooks | `matchPath` | `<Prompt>` | Test files (memory history) |
|---|---|---|---|---|---|---|
| App (shell, AppRoutes) | 5 | 8 | 6 | 0 | 0 | 1 (0) |
| Home | 1 | 0 | 1 | 0 | 0 | 1 (1) |
| Admin (top level) | 2 | 14 | 2 | 0 | 0 | 2 (0) |
| Admin/Models | 8 | 8 | 12 | 16 | 1 | 13 (13) |
| Admin/Services | 4 | 9 | 3 | 0 | 0 | 1 (0) |
| Admin/Schema | 3 | 6 | 1 | 0 | 0 | 1 (0) |
| Admin/RequestHandlers | 1 | 2 | 0 | 0 | 0 | 1 (0) |
| Objects | 4 | 4 | 9 | 0 | 1 | 5 (5) |
| Tools (Fiddle, Requests) | 2 | 4 | 1 | 0 | 1 | 2 (0) |
| Documentation | 6 | 22 | 5 | 0 | 0 | 3 (0) |
| common | 1 | 0 | 1 | 1 | 0 | 0 |

## Order (each step a PR, tests green)

1. **Bridge + shell.** Add `@tanstack/react-router`; root route + app shell (AppBar, AppNav, Snackbar) in TanStack; catch-all → RR5 via the adapter; `navigation.js` unchanged; `test-utils.js` gains a TanStack-based harness that still accepts the tests' existing `history` setup through the bridge. Prove links, back/forward, a guard, base path.
2. **A small section** — Admin/Schema or Admin/RequestHandlers — to set the per-section pattern (route module, `<Outlet/>`, hooks, tests).
3. **Objects** — first guard (`<Prompt>` → `useBlocker`), hash-free, 5 memory-history tests.
4. **Admin/Models** — the heaviest: 16 `matchPath`, hash perspectives, a guard, 13 memory-history tests.
5. **Services, Admin top level, Tools (Fiddle guard), Documentation (22 routes), Home, common.**
6. **Remove the bridge**: `navigation.js` → TanStack members; drop `react-router` and `react-router-dom`; delete the adapter. (`history` is **not** a declared dependency — tests import `createMemoryHistory` from it only through `react-router-dom`; by this step no test should import it.)

## Open questions / risks

- **Adapter fidelity.** RR5 also reads `history.action` and `history.length`; check every member RR5's `<Router>`, `<Prompt>` and hooks touch, and that a blocked navigation notifies neither side.
- **Guard UX.** Today `<Prompt>` goes through `BrowserRouter`'s `getUserConfirmation` (`window.confirm`). `useBlocker` with `withResolver` would allow an in-app dialog later; keep `window.confirm` for parity first.
- **Tests.** 19 test files build `Router` + `createMemoryHistory` from the `history` package and call `history.push` (some after render, wrapped in `act()` since #454). The harness should let unmigrated tests keep that shape until their section moves.
- **Query** stays out of scope (see above).
- **Known issue (pre-existing, kept as is):** the model overview tables put the hash into `uriComponents` (`[..., objectType, hash]`), so with a view hash set the link becomes `.../objectTypes/<type>/%23tree` - the `#` encoded into the path. Fix separately (append the hash to the URL instead).
