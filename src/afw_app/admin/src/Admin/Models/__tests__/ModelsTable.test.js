// See the 'COPYING' file in the project root for licensing information.
import {
    renderRoute,
    server,
    http,
    HttpResponse,
    fireEvent,
    waitFor,
    waitForElementToBeRemoved,
    within,
    screen,
    mswPostCallback,
    waitForSpinner
} from "../../test-utils";

describe("ModelsTable Tests", () => {

    const test1Model = {
        modelId: "test1",
        description: "This is a test model",
        _meta_: {
            objectId: "test1",
            objectType: "_AdaptiveModel_",
            path: "/models/_AdaptiveModel_/test1",
            reconcilable: "{}"
        }
    };

    beforeEach(() => {
        mswPostCallback.mockClear();
    });
   
    test("No models found", async () => {

        /* respond to the retrieve_objects with no models */
        server.use(
            http.post("/afw", async ({request}) => {
                const body = await request.clone().json();
                const {function: functionId, adapterId} = body;

                if (functionId === "retrieve_objects" && adapterId === "models") {
                    mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});

                    return HttpResponse.json({
                        status: "success",
                        result: []
                    });
                }
            })
        );

        renderRoute("/Admin/Models");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        await screen.findByRole("table");
        await screen.findByText("No models found.");
    });

    test("One model listed in table", async () => {

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            http.post("/afw", async ({request}) => {
                const body = await request.clone().json();
                const {function: functionId, adapterId} = body;

                if (functionId === "retrieve_objects" && adapterId === "models") {
                    mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});

                    return HttpResponse.json({
                        status: "success",
                        result: [ test1Model ]
                    });
                }
            })
        );

        renderRoute("/Admin/Models/models");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        const table = await screen.findByRole("table");
        await within(table).findByRole("link", { name: "test1" });
        await within(table).findByText("This is a test model");

        await screen.findByRole("button", { name: "New Model" });
        await screen.findByRole("button", { name: "Delete Model" });
        await screen.findByRole("button", { name: "Import Model" });
    });

    test("Delete a model, cancel", async () => {

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            http.post("/afw", async ({request}) => {
                const body = await request.clone().json();
                const {function: functionId, adapterId} = body;

                if (functionId === "retrieve_objects" && adapterId === "models") {
                    mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});

                    return HttpResponse.json({
                        status: "success",
                        result: [ test1Model ]
                    });
                }
            })
        );

        renderRoute("/Admin/Models/models");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        const table = await screen.findByRole("table");
        await within(table).findByRole("link", { name: "test1" });
        await within(table).findByText("This is a test model");

        let deleteBtn = await screen.findByRole("button", { name: "Delete Model" });
        expect(deleteBtn).not.toBeEnabled();

        /* locate the checkbox to select this row */
        const cell = await screen.findByRole("cell", { name: /select table row/i });        
        const checkbox = within(cell).getByRole("checkbox");        

        fireEvent.click(checkbox);

        deleteBtn = await screen.findByRole("button", { name: "Delete Model" });
        await waitFor(() => expect(deleteBtn).toBeEnabled());

        fireEvent.click(deleteBtn);
        
        const dialog = await screen.findByRole("dialog", { name: "Delete Model" });
        const cancelBtn = await within(dialog).findByRole("button", { name: "Cancel" });

        fireEvent.click(cancelBtn);

        await waitForElementToBeRemoved(() => screen.queryByRole("dialog", { name: "Delete Model" }));
    });

    test("Delete a model", async () => {

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            http.post("/afw", async ({request}) => {
                const body = await request.clone().json();
                const {function: functionId, adapterId} = body;

                if (functionId === "retrieve_objects" && adapterId === "models") {
                    mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});

                    return HttpResponse.json({
                        status: "success",
                        result: [ test1Model ]
                    });
                }
            })
        );

        renderRoute("/Admin/Models/models");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        const table = await screen.findByRole("table");
        await within(table).findByRole("link", { name: "test1" });
        await within(table).findByText("This is a test model");

        let deleteBtn = await screen.findByRole("button", { name: "Delete Model" });
        expect(deleteBtn).not.toBeEnabled();

        /* locate the checkbox to select this row */
        const cell = await screen.findByRole("cell", { name: /select table row/i });        
        const checkbox = within(cell).getByRole("checkbox");        

        fireEvent.click(checkbox);

        deleteBtn = await screen.findByRole("button", { name: "Delete Model" });
        await waitFor(() => expect(deleteBtn).toBeEnabled());

        fireEvent.click(deleteBtn);
        
        const dialog = await screen.findByRole("dialog", { name: "Delete Model" });
        const confirmDeleteBtn = await within(dialog).findByRole("button", { name: "Delete" });

        fireEvent.click(confirmDeleteBtn);

        await waitForElementToBeRemoved(() => screen.queryByRole("dialog", { name: "Delete Model" }));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("delete_object_with_uri"));

    });

    /* eslint-disable jest/no-disabled-tests, jest/expect-expect -- acknowledged, tracked TODOs, not yet implemented */
    test.skip("Delete multiple models", async () => {

    });

    test("Import Model, cancel", async () => {

        renderRoute("/Admin/Models/models");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        await screen.findByRole("table");
        const importModelBtn = await screen.findByRole("button", { name: "Import Model" });

        expect(importModelBtn).toBeEnabled();
        fireEvent.click(importModelBtn);

        const dialog = await screen.findByRole("dialog", { name: "Import Model" });
        const cancelBtn = await within(dialog).findByRole("button", { name: "Cancel" });

        expect(cancelBtn).toBeEnabled();
        fireEvent.click(cancelBtn);

        await waitForElementToBeRemoved(() => screen.queryByRole("dialog", { name: "Import Model" }));

    });

    test.skip("Import Model", async () => {

    });
    /* eslint-enable jest/no-disabled-tests, jest/expect-expect */

});
