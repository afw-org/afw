// See the 'COPYING' file in the project root for licensing information.
import {
    server, 
    rest, 
    render, 
    fireEvent,
    waitFor,     
    waitForElementToBeRemoved,
    within, 
    screen, 
    mswPostCallback, 
    waitForSpinner
} from "../../test-utils";
import {Router} from "react-router-dom";
import {createMemoryHistory} from "history";
import Models from "../Models";

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

        const history = createMemoryHistory();
        history.push("/Admin/Models");

        /* respond to the retrieve_objects with no models */
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, adaptorId} = req.body;

                if (functionId === "retrieve_objects" && adaptorId === "models") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "success",
                            result: []
                        })
                    );
                }
            })
        );

        render(
            <Router history={history}>
                <Models />
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        await screen.findByRole("table");
        await screen.findByText("No models found.");
    });

    test("One model listed in table", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models");

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, adaptorId} = req.body;

                if (functionId === "retrieve_objects" && adaptorId === "models") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "success",
                            result: [ test1Model ]
                        })
                    );
                }
            })
        );

        render(
            <Router history={history}>
                <Models />
            </Router>
        );

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

        const history = createMemoryHistory();
        history.push("/Admin/Models/models");

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, adaptorId} = req.body;

                if (functionId === "retrieve_objects" && adaptorId === "models") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "success",
                            result: [ test1Model ]
                        })
                    );
                }
            })
        );

        render(
            <Router history={history}>
                <Models />
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        const table = await screen.findByRole("table");
        await within(table).findByRole("link", { name: "test1" });
        await within(table).findByText("This is a test model");

        let deleteBtn = await screen.findByRole("button", { name: "Delete Model" });
        expect(deleteBtn).not.toBeEnabled();

        /* locate the checkbox to select this row */
        const cell = screen.getByRole("cell", { name: /select table row/i });        
        const checkbox = within(cell).getByRole("checkbox");        

        checkbox.click();

        deleteBtn = await screen.findByRole("button", { name: "Delete Model" });
        await waitFor(() => expect(deleteBtn).toBeEnabled());

        fireEvent.click(deleteBtn);
        
        const dialog = await screen.findByRole("dialog", { name: "Delete Model" });
        const cancelBtn = await within(dialog).findByRole("button", { name: "Cancel" });

        fireEvent.click(cancelBtn);

        await waitForElementToBeRemoved(() => screen.queryByRole("dialog", { name: "Delete Model" }));
    });

    test("Delete a model", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models");

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, adaptorId} = req.body;

                if (functionId === "retrieve_objects" && adaptorId === "models") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "success",
                            result: [ test1Model ]
                        })
                    );
                }
            })
        );

        render(
            <Router history={history}>
                <Models />
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        const table = await screen.findByRole("table");
        await within(table).findByRole("link", { name: "test1" });
        await within(table).findByText("This is a test model");

        let deleteBtn = await screen.findByRole("button", { name: "Delete Model" });
        expect(deleteBtn).not.toBeEnabled();

        /* locate the checkbox to select this row */
        const cell = screen.getByRole("cell", { name: /select table row/i });        
        const checkbox = within(cell).getByRole("checkbox");        

        checkbox.click();

        deleteBtn = await screen.findByRole("button", { name: "Delete Model" });
        await waitFor(() => expect(deleteBtn).toBeEnabled());

        fireEvent.click(deleteBtn);
        
        const dialog = await screen.findByRole("dialog", { name: "Delete Model" });
        const confirmDeleteBtn = await within(dialog).findByRole("button", { name: "Delete" });

        fireEvent.click(confirmDeleteBtn);

        await waitForElementToBeRemoved(() => screen.queryByRole("dialog", { name: "Delete Model" }));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("delete_object_with_uri"));

    });

    test.skip("Delete multiple models", async () => {

    });

    test("Import Model, cancel", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models");

        render(
            <Router history={history}>
                <Models />
            </Router>
        );

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

});
