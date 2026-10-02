// See the 'COPYING' file in the project root for licensing information.
import {createRoute, lazyRouteComponent} from "@tanstack/react-router";

/**
 * createAdminRoutes(parentRoute)
 *
 * The /Admin section's TanStack routes (see
 * designs/tanstack-router-migration.md). The /Admin layout route renders
 * what every admin page shares (Admin.js's AdminLayout) around an
 * <Outlet/>; its children are the migrated sections, plus a catch-all
 * for the admin sections still on React Router 5.
 *
 * Components load lazily, keeping the admin code in its own chunk.
 */
export const createAdminRoutes = (parentRoute) => {

    const adminRoute = createRoute({
        getParentRoute: () => parentRoute,
        path: "Admin",
        component: lazyRouteComponent(() => import("./Admin"), "AdminLayout"),
    });

    /* one route for the whole drill-down: adapter > object type > property */
    const schemaRoute = createRoute({
        getParentRoute: () => adminRoute,
        path: "Schema/{-$adapterId}/{-$objectTypeId}/{-$propertyName}",
        component: lazyRouteComponent(() => import("./Schema/Schema")),
    });

    /* admin sections not migrated yet: React Router 5's routes */
    const adminLegacyRoute = createRoute({
        getParentRoute: () => adminRoute,
        path: "$",
        component: lazyRouteComponent(() => import("./Admin"), "AdminLegacyRoutes"),
    });

    return adminRoute.addChildren([
        schemaRoute,
        adminLegacyRoute,
    ]);
};

export default createAdminRoutes;
