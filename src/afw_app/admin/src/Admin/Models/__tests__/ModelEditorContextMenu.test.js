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

describe("ModelEditorContextMenu Tests", () => {    

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

    test("Readonly Mode displays context menu properly", async () => {

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

        within(menu).getByRole("menuitem", { name: "Edit Model" });        
        within(menu).getByRole("menuitem", { name: "Save Model" });        
        within(menu).getByRole("menuitem", { name: "Refresh Model" });        
        within(menu).getByRole("menuitem", { name: "Restart Model Adaptor" });        
        within(menu).getByRole("menuitem", { name: "Set Mapped Adaptor" });        
        within(menu).getByRole("menuitem", { name: "Simulate" });
        within(menu).getByRole("menuitem", { name: "Deploy to Service" });                

    });

    test("Select Mapped Adaptor (None)", async () => {

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

        const setMappedAdaptor = within(menu).getByRole("menuitem", { name: "Set Mapped Adaptor" });        

        fireEvent.click(setMappedAdaptor);       
        const none = screen.getByRole("menuitem", { name: "(None)" });

        fireEvent.click(none);
        await waitFor(() => expect(screen.queryByRole("menu")).not.toBeInTheDocument());

    });

    test.skip("Edit Model", async () => {

    });

    test.skip("Save Model", async () => {

    });

    test.skip("Edit Model", async () => {

    });
    
    test.skip("New", async () => {

    });

    test.skip("Rename", async () => {

    });

    test.skip("Delete", async () => {

    });

    test.skip("Restart Model Adaptor", async () => {

    });

    test.skip("Set Mapped Adaptor", async () => {

    });

    test.skip("Simulate", async () => {

    });

    test.skip("Deploy to Service", async () => {

    });

});
