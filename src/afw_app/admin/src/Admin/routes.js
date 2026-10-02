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

    /*
     * one route for the model editor too: adapter > model, then a path the
     * editor and its context menu interpret (objectTypes/..., custom/...)
     */
    const modelsRoute = createRoute({
        getParentRoute: () => adminRoute,
        path: "Models/{-$adapterId}/{-$modelId}/$",
        component: lazyRouteComponent(() => import("./Models/Models")),
    });

    /* list/detail sections: the list, or one item by its optional id */
    const listDetailRoute = (path, load) => createRoute({
        getParentRoute: () => adminRoute,
        path,
        component: lazyRouteComponent(load),
    });

    const servicesRoute = listDetailRoute("Services/{-$serviceId}", () => import("./Services/Services"));
    const adaptersRoute = listDetailRoute("Adapters/{-$adapterId}", () => import("./Services/Adapters/Adapters"));
    const logsRoute = listDetailRoute("Logs/{-$logId}", () => import("./Services/Logs/Logs"));
    const authHandlersRoute = listDetailRoute("AuthHandlers/{-$authorizationHandlerId}",
        () => import("./Services/AuthorizationHandlers/AuthorizationHandlers"));

    const requestHandlersRoute = listDetailRoute("RequestHandlers/{-$requestHandlerId}",
        () => import("./RequestHandlers/RequestHandlers"));

    /* sections with no routes of their own; /Admin itself shows Status */
    const page = (path, load) => createRoute({
        getParentRoute: () => adminRoute,
        path,
        component: lazyRouteComponent(load),
    });

    const statusLoad = () => import("./Status");

    return adminRoute.addChildren([
        schemaRoute,
        modelsRoute,
        servicesRoute,
        adaptersRoute,
        logsRoute,
        authHandlersRoute,
        requestHandlersRoute,
        page("/", statusLoad),
        page("Status", statusLoad),
        page("Server", () => import("./Server/Server")),
        page("Application", () => import("./Application/Application")),
        page("Extensions", () => import("./Extensions/Extensions")),
        page("Provisioning", () => import("./Provisioning/Provisioning")),
        page("Authorization", () => import("./Authorization/Authorization")),
    ]);
};

export default createAdminRoutes;
