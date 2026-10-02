// See the 'COPYING' file in the project root for licensing information.
import {createRoute, lazyRouteComponent} from "@tanstack/react-router";

/**
 * createToolsRoutes(parentRoute)
 *
 * The /Tools section's TanStack routes (see
 * designs/tanstack-router-migration.md): a layout route (Tools.js's
 * container, breadcrumbs and help around an <Outlet/>), the tools menu as
 * its index, and one route per tool. Components load lazily.
 */
export const createToolsRoutes = (parentRoute) => {

    const toolsRoute = createRoute({
        getParentRoute: () => parentRoute,
        path: "Tools",
        component: lazyRouteComponent(() => import("./Tools")),
    });

    const toolsHomeRoute = createRoute({
        getParentRoute: () => toolsRoute,
        path: "/",
        component: lazyRouteComponent(() => import("./Tools"), "ToolsHome"),
    });

    const fiddleRoute = createRoute({
        getParentRoute: () => toolsRoute,
        path: "Fiddle",
        component: lazyRouteComponent(() => import("./Fiddle/Fiddle")),
    });

    const layoutsRoute = createRoute({
        getParentRoute: () => toolsRoute,
        path: "Layouts",
        component: lazyRouteComponent(() => import("./Layouts/Layouts")),
    });

    const requestsRoute = createRoute({
        getParentRoute: () => toolsRoute,
        path: "Requests",
        component: lazyRouteComponent(() => import("./Requests/Requests")),
    });

    return toolsRoute.addChildren([
        toolsHomeRoute,
        fiddleRoute,
        layoutsRoute,
        requestsRoute,
    ]);
};

export default createToolsRoutes;
