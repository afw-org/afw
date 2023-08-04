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

describe("ModelDeploy Tests", () => {    

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

    test("Step 1 Cancel", async () => {

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

        const moreOptionsBtn = await screen.findByRole("button", { name: "More Options" });        
        
        fireEvent.click(moreOptionsBtn);
        const menu = await screen.findByRole("menu");
        
        const deployBtn = within(menu).getByRole("menuitem", { name: "Deploy to Service" });                
        fireEvent.click(deployBtn);
        
        const dialog = await screen.findByRole("dialog", { name: "Deploy Model" });
        within(dialog).getByRole("heading", { name: "Deploy Model" });

        /* adaptorId should be filled out with "test" */
        expect(within(dialog).getByRole("textbox", { name: "Adaptor" })).toHaveValue("test1");
        expect(within(dialog).getByRole("textbox", { name: "Description" })).toHaveValue("This is a test model");

        const cancelBtn = within(dialog).getByRole("button", { name: "Cancel" });
        fireEvent.click(cancelBtn);

        await waitFor(() => expect(screen.queryByRole("dialog")).not.toBeInTheDocument());
    });    

    test("Step 3 Deploy", async () => {

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

        const moreOptionsBtn = await screen.findByRole("button", { name: "More Options" });        
        
        fireEvent.click(moreOptionsBtn);
        const menu = await screen.findByRole("menu");
        
        const deployServiceBtn = within(menu).getByRole("menuitem", { name: "Deploy to Service" });                
        fireEvent.click(deployServiceBtn);
        
        const dialog = await screen.findByRole("dialog", { name: "Deploy Model" });
        within(dialog).getByRole("heading", { name: "Deploy Model" });

        /* adaptorId should be filled out with "test" */
        expect(within(dialog).getByRole("textbox", { name: "Adaptor" })).toHaveValue("test1");
        expect(within(dialog).getByRole("textbox", { name: "Description" })).toHaveValue("This is a test model");

        let nextBtn = within(dialog).getByRole("button", { name: "Next" });
        fireEvent.click(nextBtn);

        await screen.findByRole("button", { name: /journal adaptor/i });

        let deployBtn = within(dialog).getByRole("button", { name: "Deploy" });
        expect(deployBtn).toBeDisabled();

        nextBtn = within(dialog).getByRole("button", { name: "Next" });
        fireEvent.click(nextBtn);

        deployBtn = within(dialog).getByRole("button", { name: "Deploy" });
        expect(deployBtn).not.toBeDisabled();

        mswPostCallback.mockClear();
        fireEvent.click(deployBtn);

        await waitFor(() => expect(screen.queryByRole("dialog")).not.toBeInTheDocument());
        
        /* verify we called the add object */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("add_object_with_uri"));
        
    });    

});
