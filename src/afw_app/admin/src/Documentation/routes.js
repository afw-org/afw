// See the 'COPYING' file in the project root for licensing information.
import {createRoute, lazyRouteComponent} from "@tanstack/react-router";

/**
 * createDocumentationRoutes(parentRoute)
 *
 * The /Documentation section's TanStack routes (see
 * designs/tanstack-router-migration.md):
 *
 *   /Documentation             layout (Documentation.js), index page
 *     Reference                layout (Reference.js: loads functions, data
 *                              types and object types; breadcrumbs), index
 *       DataTypes/{-$dataTypeId}
 *       Schema/{-$adapterId}/{-$objectType}/{-$propertyName}
 *       Components/{-$category}/{-$component}
 *       Functions/{-$category}/{-$functionId}
 *
 * Each reference area is one route whose component picks its view from the
 * optional params. Components load lazily.
 */
export const createDocumentationRoutes = (parentRoute) => {

    const documentationRoute = createRoute({
        getParentRoute: () => parentRoute,
        path: "Documentation",
        component: lazyRouteComponent(() => import("./Documentation")),
    });

    const documentationHomeRoute = createRoute({
        getParentRoute: () => documentationRoute,
        path: "/",
        component: lazyRouteComponent(() => import("./Documentation"), "DocumentationHome"),
    });

    const referenceRoute = createRoute({
        getParentRoute: () => documentationRoute,
        path: "Reference",
        component: lazyRouteComponent(() => import("./Reference/Reference")),
    });

    const referenceChild = (path, load, name) => createRoute({
        getParentRoute: () => referenceRoute,
        path,
        component: lazyRouteComponent(load, name),
    });

    return documentationRoute.addChildren([
        documentationHomeRoute,
        referenceRoute.addChildren([
            referenceChild("/", () => import("./Reference/Reference"), "ReferenceHome"),
            referenceChild("DataTypes/{-$dataTypeId}", () => import("./Reference/Reference"), "ReferenceDataTypes"),
            referenceChild("Schema/{-$adapterId}/{-$objectType}/{-$propertyName}", () => import("./Reference/ObjectTypes"), "ObjectTypes"),
            referenceChild("Components/{-$category}/{-$component}", () => import("./Reference/Components"), "Components"),
            referenceChild("Functions/{-$category}/{-$functionId}", () => import("./Reference/Functions"), "Functions"),
        ]),
    ]);
};

export default createDocumentationRoutes;
