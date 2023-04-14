// See the 'COPYING' file in the project root for licensing information.
import {
    server, 
    rest, 
    render, 
    waitFor,
    screen, 
    mswPostCallback, 
    waitForSpinner,
    fireEvent,
    userEvent,
    within,
} from "../../../test-utils";
import {Router} from "react-router-dom";
import {createMemoryHistory} from "history";
import {Models} from "../../Models";

describe("ModelTreeContextMenu Tests", () => {    

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

    test("ContextMenu at model level", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1#tree");

        /* return a model for /models/_AdaptiveModel_/test1 */
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

        await waitForSpinner();
        await waitFor(() => expect(screen.queryByRole("button", { name: "Edit Model" })).not.toBeInTheDocument());

        const tree = await screen.findByRole("tree", { name: "test1" });        
        const test1 = within(tree).getByText("test1");
        
        /* right-click context menu */        
        userEvent.click(test1, { ctrlKey: true });
        userEvent.click(test1, { button: 2 });

        const menu = await screen.findByRole("menu");
        within(menu).getByRole("menuitem", { name: "New Object Type" });
        within(menu).getByRole("menuitem", { name: "New Property Type" });
        within(menu).getByRole("menuitem", { name: "New Custom Variable" });
    });

    test("Model create object type, cancel", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1#tree");

        /* return a model for /models/_AdaptiveModel_/test1 */
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

        await waitForSpinner();
        await waitFor(() => expect(screen.queryByRole("button", { name: "Edit Model" })).not.toBeInTheDocument());

        const tree = await screen.findByRole("tree", { name: "test1" });        
        const test1 = within(tree).getByText("test1");
        
        /* right-click context menu */        
        userEvent.click(test1, { ctrlKey: true });
        userEvent.click(test1, { button: 2 });

        const menu = await screen.findByRole("menu");
        const newObjType = within(menu).getByRole("menuitem", { name: "New Object Type" });

        fireEvent.click(newObjType);

        const dialog = await screen.findByRole("dialog", { name: "New Object Type" });        
        const cancelBtn = within(dialog).getByRole("button", { name: "Cancel" });

        fireEvent.click(cancelBtn);
        await waitFor(() => expect(screen.queryByRole("dialog", { name: "New Object Type" })).not.toBeInTheDocument());
        await waitFor(() => expect(screen.queryByRole("menu")).not.toBeInTheDocument());

    });

    test("Model create property type, cancel", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1#tree");

        /* return a model for /models/_AdaptiveModel_/test1 */
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

        await waitForSpinner();
        await waitFor(() => expect(screen.queryByRole("button", { name: "Edit Model" })).not.toBeInTheDocument());

        const tree = await screen.findByRole("tree", { name: "test1" });        
        const test1 = within(tree).getByText("test1");
        
        /* right-click context menu */        
        userEvent.click(test1, { ctrlKey: true });
        userEvent.click(test1, { button: 2 });

        const menu = await screen.findByRole("menu");
        const newObjType = within(menu).getByRole("menuitem", { name: "New Property Type" });

        fireEvent.click(newObjType);

        const dialog = await screen.findByRole("dialog", { name: "New Property Type" });        
        const cancelBtn = within(dialog).getByRole("button", { name: "Cancel" });

        fireEvent.click(cancelBtn);
        await waitFor(() => expect(screen.queryByRole("dialog", { name: "New Property Type" })).not.toBeInTheDocument());
        await waitFor(() => expect(screen.queryByRole("menu")).not.toBeInTheDocument());

    });

    test("Model create custom variable, cancel", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1#tree");

        /* return a model for /models/_AdaptiveModel_/test1 */
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

        await waitForSpinner();
        await waitFor(() => expect(screen.queryByRole("button", { name: "Edit Model" })).not.toBeInTheDocument());

        const tree = await screen.findByRole("tree", { name: "test1" });        
        const test1 = within(tree).getByText("test1");
        
        /* right-click context menu */        
        userEvent.click(test1, { ctrlKey: true });
        userEvent.click(test1, { button: 2 });

        const menu = await screen.findByRole("menu");
        const newObjType = within(menu).getByRole("menuitem", { name: "New Custom Variable" });

        fireEvent.click(newObjType);

        const dialog = await screen.findByRole("dialog", { name: "New Custom Variable" });        
        const cancelBtn = within(dialog).getByRole("button", { name: "Cancel" });

        fireEvent.click(cancelBtn);
        await waitFor(() => expect(screen.queryByRole("dialog", { name: "New Custom Variable" })).not.toBeInTheDocument());
        await waitFor(() => expect(screen.queryByRole("menu")).not.toBeInTheDocument());

    });

});
