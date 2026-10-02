// See the 'COPYING' file in the project root for licensing information.
import {createRoute, lazyRouteComponent} from "@tanstack/react-router";

/**
 * createObjectsRoutes(parentRoute)
 *
 * The /Objects section's TanStack route (see
 * designs/tanstack-router-migration.md): one drill-down route - adapter >
 * object type > object - whose components read the optional params. What
 * follows the objectId (the splat) is an embedded object's path inside it.
 */
export const createObjectsRoutes = (parentRoute) =>
    createRoute({
        getParentRoute: () => parentRoute,
        path: "Objects/{-$adapterId}/{-$objectTypeId}/{-$objectId}/$",
        component: lazyRouteComponent(() => import("./Objects")),
    });

export default createObjectsRoutes;
