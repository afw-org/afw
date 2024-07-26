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

describe("Models Tests", () => {

    beforeEach(() => {
        mswPostCallback.mockClear();
    });

    test("Test Breadcrumbs with different routes", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models");

        render(
            <Router history={history}>
                <Models />
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();
        
        const view = await screen.findByTestId("admin-admin-models");
        let list = within(view).getByRole("list");

        within(list).getByRole("link", { name: "Admin" });
        within(list).getByRole("link", { name: "Models" });

        expect(within(list).queryByRole("link", { name: "models" })).not.toBeInTheDocument();

        /* push /Admin/Models/models onto the history route */
        history.push("/Admin/Models/models");

        within(list).getByRole("link", { name: "Admin" });
        within(list).getByRole("link", { name: "Models" });
        within(list).getByText("models");

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, uri} = req.body;
                
                if (functionId === "get_object_with_uri" && uri === "/models/_AdaptiveModel_/test") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "success",
                            result: {
                                modelId: "test",
                                description: "This is a test model",
                                _meta_: {
                                    objectId: "test",
                                    objectType: "_AdaptiveModel_",
                                    path: "/models/_AdaptiveModel_/test",
                                    reconcilable: "{}"
                                }
                            }
                        })
                    );
                }
            })
        ); 

        /* push /Admin/Models/models/test onto the history route */        
        history.push("/Admin/Models/models/test");
        
        await within(list).findByRole("link", { name: "Admin" });
        await within(list).findByRole("link", { name: "Models" });
        await within(list).findByRole("link", { name: "models" });
        await within(list).findByText("test");
                
    });

    test("Test ContextualHelp", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models");

        render(
            <Router history={history}>
                <Models />
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        const helpBtn = await screen.findByRole("button", { name: "Help" });
        expect(helpBtn).toBeEnabled();

        fireEvent.click(helpBtn);        
        const closeBtn = await screen.findByRole("button", { name: "Close Window" });
        const heading = await screen.findByRole("heading", { name: "Models Help" });

        fireEvent.click(closeBtn);

        await waitFor(() => expect(closeBtn).not.toBeInTheDocument());
        await waitFor(() => expect(heading).not.toBeInTheDocument());
    });

    test("Error message when unable to load models", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/xyz");

        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, adapterId} = req.body;

                if (functionId === "retrieve_objects" && adapterId === "xyz") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "error",
                            error: {
                                error: true,
                                message: "Error opening /afw/config/_AdaptiveServiceConf_/adapter-xyz.json"                                
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

        await screen.findByText("Error occurred while loading models.");
        await waitForSpinner();

    });

    test("Error message when unable to load model", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/xyz");

        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, uri} = req.body;
                
                if (functionId === "get_object_with_uri" && uri === "/models/_AdaptiveModel_/xyz") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "error",
                            error: {
                                error: true,
                                message: "Error opening /afw/models/_AdaptiveModel_/xyz.json errno 2"                                
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

        await screen.findByText("Error occurred while loading the model xyz");
        await waitForSpinner();

    });

});
