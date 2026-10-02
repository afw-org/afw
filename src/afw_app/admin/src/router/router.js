// See the 'COPYING' file in the project root for licensing information.
import {createContext, useContext, useMemo} from "react";
import {Router as LegacyRouter} from "react-router";
import {createRootRoute, createRoute, createRouter, Outlet, useRouter} from "@tanstack/react-router";

import {createLegacyHistory} from "./legacyHistory";
import AppRoutes from "../App/AppRoutes";
import Loading from "../common/Loading";
import {createAdminRoutes} from "../Admin/routes";
import {createObjectsRoutes} from "../Objects/routes";
import {createToolsRoutes} from "../Tools/routes";

/**
 * The admin app's TanStack Router, mid-migration from React Router 5 (see
 * designs/tanstack-router-migration.md).
 *
 * TanStack Router owns the browser history. The root route renders the app
 * shell inside React Router 5's <Router>, driven by an adapter over that
 * same history (legacyHistory.js), so every React Router 5 hook, <Link>
 * and <Prompt> - in the shell, in sections not yet migrated, and in
 * navigation.js - keeps working. The shell renders <Outlet/> for its main
 * area; the catch-all route below sends everything not yet migrated to the
 * React Router 5 routes in AppRoutes. Migrated sections become TanStack
 * routes, matched before the catch-all.
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

/* everything not migrated yet: React Router 5's routes */
const legacyRoute = createRoute({
    getParentRoute: () => rootRoute,
    path: "$",
    component: AppRoutes,
});

/* migrated sections (each module builds its own subtree), then the catch-all */
export const routeTree = rootRoute.addChildren([
    createAdminRoutes(rootRoute),
    createObjectsRoutes(rootRoute),
    createToolsRoutes(rootRoute),
    legacyRoute,
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
        parseSearch,
        stringifySearch,
        ...options,
    });
