// See the 'COPYING' file in the project root for licensing information.
import {createContext, useContext, useMemo} from "react";
import {Router as LegacyRouter} from "react-router";
import {createRootRoute, createRoute, createRouter, lazyRouteComponent, Outlet, useRouter} from "@tanstack/react-router";

import {createLegacyHistory} from "./legacyHistory";
import Loading from "../common/Loading";
import NoRoute from "../common/NoRoute";
import {createAdminRoutes} from "../Admin/routes";
import {createObjectsRoutes} from "../Objects/routes";
import {createToolsRoutes} from "../Tools/routes";
import {createDocumentationRoutes} from "../Documentation/routes";

/**
 * The admin app's TanStack Router, mid-migration from React Router 5 (see
 * designs/tanstack-router-migration.md).
 *
 * TanStack Router owns the browser history, and every page is a TanStack
 * route. The root route renders the app shell (whose main area is an
 * <Outlet/>) inside React Router 5's <Router>, driven by an adapter over
 * that same history (legacyHistory.js), for the one remaining React Router
 * 5 consumer: navigation.js, the @afw/react navigation adapter (Link,
 * Prompt, useHistory). The last migration step moves it to TanStack and
 * removes this bridge.
 */

/*
 * App builds the shell (it holds the menu, snackbar, ... state) and hands it
 * to the root route through this context.
 */
export const AppShellContext = createContext(null);

const RootLayout = () => {
    const router = useRouter();
    const shell = useContext(AppShellContext);

    /* one adapter per router: <Router> must keep the same history */
    const legacyHistory = useMemo(
        () => createLegacyHistory(router.history, router.basepath),
        [router]
    );

    return (
        <LegacyRouter history={legacyHistory}>
            { shell ?? <Outlet /> }
        </LegacyRouter>
    );
};

const rootRoute = createRootRoute({
    component: RootLayout,
});

/* top-level pages: Home (also the index) and Versions */
const page = (path, load) => createRoute({
    getParentRoute: () => rootRoute,
    path,
    component: lazyRouteComponent(load),
});

const homeLoad = () => import("../Home/Home");

/* each section module builds its own subtree */
export const routeTree = rootRoute.addChildren([
    page("/", homeLoad),
    page("Home", homeLoad),
    page("Versions", () => import("../Admin/Versions")),
    createAdminRoutes(rootRoute),
    createObjectsRoutes(rootRoute),
    createToolsRoutes(rootRoute),
    createDocumentationRoutes(rootRoute),
]);

/*
 * The app's install path from Vite's base (`afwdev build` sets PUBLIC_URL,
 * e.g. /apps/afw/admin/). Standalone builds use a relative base ("./"),
 * served from the root.
 */
export const getBasepath = () => {
    const base = import.meta.env.BASE_URL;

    return (base && base.startsWith("/")) ? (base.replace(/\/+$/, "") || "/") : "/";
};

/*
 * Search strings stay exactly as written. TanStack's default parses them as
 * key=value pairs and re-serializes them, which mangles the criteria the
 * app puts there (RQL such as ?eq(a,b)&sort(+objectId) - the "+" becomes a
 * space). Components read the raw string with useLocation().searchStr (or
 * search.raw); sections that want pairs can use URLSearchParams on it.
 */
export const parseSearch = (searchStr) => {
    const raw = searchStr.replace(/^\?/, "");
    return raw ? { raw } : {};
};

export const stringifySearch = (search) =>
    (search && search.raw) ? "?" + search.raw : "";

export const createAppRouter = (options = {}) =>
    createRouter({
        routeTree,
        basepath: getBasepath(),
        /* shown while a lazy route component loads */
        defaultPendingComponent: Loading,
        /* a path no route matches (inside a migrated section) */
        defaultNotFoundComponent: NoRoute,
        parseSearch,
        stringifySearch,
        ...options,
    });
