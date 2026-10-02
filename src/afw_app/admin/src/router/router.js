// See the 'COPYING' file in the project root for licensing information.
import {createContext, useContext, useMemo} from "react";
import {Router as LegacyRouter} from "react-router";
import {createRootRoute, createRoute, createRouter, Outlet, useRouter} from "@tanstack/react-router";

import {createLegacyHistory} from "./legacyHistory";
import AppRoutes from "../App/AppRoutes";

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

export const routeTree = rootRoute.addChildren([
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

export const createAppRouter = (options = {}) =>
    createRouter({
        routeTree,
        basepath: getBasepath(),
        ...options,
    });
