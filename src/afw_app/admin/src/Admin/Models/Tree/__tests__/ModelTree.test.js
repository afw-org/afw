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
    within,
} from "../../../test-utils";
import {Router} from "react-router-dom";
import {createMemoryHistory} from "history";
import {Models} from "../../Models";

describe("ModelTree Tests", () => {    

    const test1Model = {
        modelId: "test1",
        description: "This is a test model",
        objectTypes: {
            obj1: {
                propertyTypes: {
                    prop1: {
                        dataType: "string"
                    }
                }
            }
        },
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

    test("Each level produces the correct route and breadcrumbs", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1#tree");

        /* return a model for /models/_AdaptiveModel_/test1 */
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId, adapterId} = req.body;

                if (functionId === "retrieve_objects" && adapterId === "models") { 
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

        let tree = await screen.findByRole("tree", { name: "test1" });        
        const test1 = within(tree).getByText("test1");
        
        fireEvent.click(test1);

        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1"));

        await screen.findByRole("heading", { name: "test1" });
        await screen.findByText(test1Model.description);

        /* look for "Custom Variables" at top-level */
        let custom = within(tree).getByTestId("test1.custom");
        fireEvent.click(within(custom).getByText("Custom Variables"));
        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1/custom"));

        /* look for Property Types at the top-level */        
        tree = await screen.findByRole("tree", { name: "test1" });        
        let propertyTypes = within(tree).getByTestId("test1.propertyTypes");
        fireEvent.click(within(propertyTypes).getByText("Property Types"));
        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1/propertyTypes"));

        /* look for Object Types at the top-level */
        tree = await screen.findByRole("tree", { name: "test1" });        
        const objectTypes = within(tree).getByTestId("test1.objectTypes");
        fireEvent.click(within(objectTypes).getByText("Object Types"));
        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1/objectTypes"));

        /* look for obj1 under Object Types */   
        tree = await screen.findByRole("tree", { name: "test1" });        
        const obj1 = within(tree).getByTestId("test1.objectTypes.obj1");        
        fireEvent.click(within(obj1).getByText("obj1"));
        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1/objectTypes/obj1"));      

        /* look for On Functions under obj1 */
        tree = await screen.findByRole("tree", { name: "test1" });        
        let onFunctions = within(tree).getByTestId("test1.objectTypes.obj1.onFunctions");
        fireEvent.click(within(onFunctions).getByText("On Functions"));
        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1/objectTypes/obj1/onFunctions"));

        /* look for Custom Variables under obj1 */
        tree = await screen.findByRole("tree", { name: "test1" });        
        custom = within(tree).getByTestId("test1.objectTypes.obj1.custom");
        fireEvent.click(within(custom).getByText("Custom Variables"));
        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1/objectTypes/obj1/custom"));

        /* look for Property Types under obj1 */
        tree = await screen.findByRole("tree", { name: "test1" });        
        propertyTypes = within(tree).getByTestId("test1.objectTypes.obj1.propertyTypes");
        fireEvent.click(within(propertyTypes).getByText("Property Types"));
        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1/objectTypes/obj1/propertyTypes"));

        /* look for prop1 under Property Types */
        tree = await screen.findByRole("tree", { name: "test1" });        
        const prop1 = within(tree).getByTestId("test1.objectTypes.obj1.propertyTypes.prop1");
        fireEvent.click(within(prop1).getByText("prop1"));
        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1/objectTypes/obj1/propertyTypes/prop1"));

        /* look for On Functions under prop1 */
        tree = await screen.findByRole("tree", { name: "test1" });        
        onFunctions = within(tree).getByTestId("test1.objectTypes.obj1.propertyTypes.prop1.onFunctions");
        fireEvent.click(within(onFunctions).getByText("On Functions"));
        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1/objectTypes/obj1/propertyTypes/prop1/onFunctions"));

        /* look for Custom Variables under prop1 */
        tree = await screen.findByRole("tree", { name: "test1" });        
        custom = within(tree).getByTestId("test1.objectTypes.obj1.propertyTypes.prop1.custom");
        fireEvent.click(within(custom).getByText("Custom Variables"));
        await waitFor(() => expect(history.location.pathname).toBe("/Admin/Models/models/test1/objectTypes/obj1/propertyTypes/prop1/custom"));
    });

});
