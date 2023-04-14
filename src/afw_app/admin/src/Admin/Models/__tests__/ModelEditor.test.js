// See the 'COPYING' file in the project root for licensing information.
import {
    server, 
    rest, 
    render, 
    waitFor, 
    within, 
    screen, 
    fireEvent, 
    mswPostCallback, 
    waitForSpinner
} from "../../test-utils";
import {Router} from "react-router-dom";
import {createMemoryHistory} from "history";
import {Models} from "../Models";

describe("ModelEditor Tests", () => {    

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


    test("Displays a model with buttons and menus", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1");

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
                            result: [ test1Model ],
                        })
                    );
                }
            })
        );

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, uri} = req.body;

                if (functionId === "get_object_with_uri" && uri === "/models/_AdaptiveModel_/test1") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "success",
                            result: test1Model,
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

        await screen.findByRole("button", { name: "Show Overview" });
        await screen.findByRole("button", { name: "Show Tree View" });
        await screen.findByRole("button", { name: "Show Spreadsheet" });
        await screen.findByRole("button", { name: "Show Data Mappings" });
        await screen.findByRole("button", { name: "Show Source" });

        const moreOptionsBtn = await screen.findByRole("button", { name: "More Options" });
        await screen.findByRole("button", { name: "Edit Model" });

        const view = await screen.findByTestId("admin-admin-models");
        await within(view).findByRole("heading", { name: "test1" });

        fireEvent.click(moreOptionsBtn);
        const menu = await screen.findByRole("menu");

        within(menu).getByRole("menuitem", { name: "Edit Model" });
        within(menu).getByRole("menuitem", { name: "Save Model" });
        within(menu).getByRole("menuitem", { name: "Refresh Model" });
        within(menu).getByRole("menuitem", { name: "Restart Model Adaptor" });
        within(menu).getByRole("menuitem", { name: "Set Mapped Adaptor" });
        within(menu).getByRole("menuitem", { name: "Simulate" });
        within(menu).getByRole("menuitem", { name: "Deploy to Service" });

    });

    test("Displays no such model", async () => {
        
        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1");

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, uri} = req.body;

                if (functionId === "get_object_with_uri" && uri === "/models/_AdaptiveModel_/test1") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "error",
                            error: {
                                error: true,
                                errorCode: 9,
                                errorCodeId: "not_found",
                                message: "Error opening /afw/models/_AdaptiveModel_/test1"
                            }
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

        await screen.findByText("Error occurred while loading the model test1");

        await waitForSpinner();
    });

    test("Edit Mode displays additional buttons", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1");

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
                            result: [ test1Model ],
                        })
                    );
                }
            })
        );

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, uri} = req.body;

                if (functionId === "get_object_with_uri" && uri === "/models/_AdaptiveModel_/test1") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "success",
                            result: test1Model,
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

        const editBtn = await screen.findByRole("button", { name: "Edit Model" });
        fireEvent.click(editBtn);

        const saveBtn = await screen.findByRole("button", { name: "Save" });
        const cancelBtn = await screen.findByRole("button", { name: "Cancel" });

        expect(screen.queryByRole("button", { name: "Edit Model" })).not.toBeInTheDocument();

        expect(saveBtn).not.toBeEnabled();
        expect(cancelBtn).toBeEnabled();

        /* now cancel */
        fireEvent.click(cancelBtn);

        await screen.findByRole("button", { name: "Edit Model" });
        expect(screen.queryByRole("button", { name: "Save" })).not.toBeInTheDocument();
        expect(screen.queryByRole("button", { name: "Cancel" })).not.toBeInTheDocument();

    });

    test("Clicking through perspectives updates the route", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1");

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
                            result: [ test1Model ],
                        })
                    );
                }
            })
        );

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, uri} = req.body;

                if (functionId === "get_object_with_uri" && uri === "/models/_AdaptiveModel_/test1") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "success",
                            result: test1Model,
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

        const overviewBtn = await screen.findByRole("button", { name: "Show Overview" });
        fireEvent.click(overviewBtn);
        await waitFor(() => expect(history.location.hash).toBe("#overview"));        

        const spreadsheetBtn = await screen.findByRole("button", { name: "Show Spreadsheet" });
        fireEvent.click(spreadsheetBtn);
        await waitFor(() => expect(history.location.hash).toBe("#spreadsheet"));        

        const mappingsBtn = await screen.findByRole("button", { name: "Show Data Mappings" });
        fireEvent.click(mappingsBtn);
        await waitFor(() => expect(history.location.hash).toBe("#mappings"));    

        const sourceBtn = await screen.findByRole("button", { name: "Show Source" });
        fireEvent.click(sourceBtn);
        await waitFor(() => expect(history.location.hash).toBe("#source"));    
    });

});
