// See the 'COPYING' file in the project root for licensing information.
import {
    server, 
    rest, 
    render, 
    waitFor,
    screen, 
    mswPostCallback, 
    waitForSpinner,
} from "../../../test-utils";
import {Router} from "react-router-dom";
import {createMemoryHistory} from "history";
import {Models} from "../../Models";

describe("ModelOverview Tests", () => {    

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

    test("Readonly Overview", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1#overview");

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

        await screen.findByRole("heading", { name: test1Model.modelId });
        await screen.findByText(test1Model.description);

        await screen.findByRole("table", { name: "Object Types" });
    });

});
