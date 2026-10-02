// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, screen, waitFor, waitForSpinner, mswPostCallback, server, http, HttpResponse} from "../test-utils";

import filesObjectTypes from "@afw/test/build/cjs/__mocks__/retrieve_objects/files/_AdaptiveObjectType_.json";

/*
 * Schema is on TanStack Router: these render the app's real routes, so they
 * also cover the /Admin layout route, Schema's optional params, the /Admin
 * index, and the not-found page.
 */
describe("Schema Tests", () => {

    beforeEach(() => {
        mswPostCallback.mockClear();
    });

    test("Schema renders", async () => {

        renderRoute("/Admin/Schema");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        expect(await screen.findByTestId("admin-admin-schema")).toBeInTheDocument();

    });

    test("An adapter shows its Object Types", async () => {

        renderRoute("/Admin/Schema/files");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        expect(await screen.findByText(/Object Types found/)).toBeInTheDocument();
        /* the adapterId param reaches the breadcrumbs */
        expect(screen.getByText("files")).toBeInTheDocument();

    });

    /* the adapter's _AdaptiveObjectType_ object type says what it allows */
    test("An adapter that allows managing Object Types offers New", async () => {

        renderRoute("/Admin/Schema/files");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        expect(await screen.findByRole("button", { name: "New" })).toBeEnabled();
        expect(screen.queryByText("Object Types cannot be managed directly through this adapter.")).not.toBeInTheDocument();

    });

    test("An adapter that does not allow managing Object Types says so", async () => {

        /* files' object types, with _AdaptiveObjectType_ allowing nothing */
        server.use(
            http.post("/afw", async ({request}) => {
                const body = await request.clone().json();
                if (body.function === "retrieve_objects" && body.adapterId === "files" && body.objectType === "_AdaptiveObjectType_") {
                    mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});

                    return HttpResponse.json({
                        ...filesObjectTypes,
                        result: filesObjectTypes.result.map(o => (o.objectType === "_AdaptiveObjectType_") ?
                            { ...o, allowAdd: false, allowChange: false, allowDelete: false } : o
                        ),
                    });
                }
            })
        );

        renderRoute("/Admin/Schema/files");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        expect(await screen.findByText("Object Types cannot be managed directly through this adapter.")).toBeInTheDocument();
        expect(screen.queryByRole("button", { name: "New" })).not.toBeInTheDocument();

    });

    test("An Object Type shows its tabs", async () => {

        renderRoute("/Admin/Schema/files/_AdaptiveManifest_");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        expect(await screen.findByRole("tab", { name: "General" })).toBeInTheDocument();
        expect(screen.getByRole("tab", { name: "Properties" })).toBeInTheDocument();
        /* the objectTypeId param reaches the breadcrumbs (and the editor header) */
        expect(screen.getAllByText("_AdaptiveManifest_").length).toBeGreaterThan(0);

    });

    test("A Property Type replaces the Object Type's tabs", async () => {

        renderRoute("/Admin/Schema/files/_AdaptiveManifest_/extensionId");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        /* the propertyName param reaches the breadcrumbs */
        expect(await screen.findByText("extensionId")).toBeInTheDocument();
        expect(screen.queryByRole("tab", { name: "General" })).not.toBeInTheDocument();

    });

    test("/Admin shows the Status page", async () => {

        renderRoute("/Admin");

        expect(await screen.findByText("Administration")).toBeInTheDocument();

    });

    test("An unknown /Admin path shows the Invalid Route page", async () => {

        renderRoute("/Admin/NoSuchPage");

        expect(await screen.findByText("Invalid Route")).toBeInTheDocument();

    });
});
