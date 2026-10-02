// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, screen, waitFor, waitForSpinner, mswPostCallback} from "../test-utils";

/*
 * Schema is on TanStack Router: these render the app's real routes, so they
 * also cover the /Admin layout route, Schema's optional params, and the
 * /Admin catch-all for sections still on React Router 5.
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

    test("Admin sections still on React Router 5 render through the /Admin catch-all", async () => {

        renderRoute("/Admin/Status");

        expect(await screen.findByText("Administration")).toBeInTheDocument();

    });
});
