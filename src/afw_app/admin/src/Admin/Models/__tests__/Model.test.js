// See the 'COPYING' file in the project root for licensing information.
import {
    server, 
    rest, 
    render, 
    waitFor,
    screen, 
    mswPostCallback, 
    waitForSpinner
} from "../../test-utils";
import {Router} from "react-router-dom";
import {createMemoryHistory} from "history";
import {Models} from "../Models";

describe("Model Tests", () => {    

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

    test("Url route hashes navigate to appropriate perspective", async () => {

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

        history.push("/Admin/Models/models/test1#overview");
        await screen.findByTestId("admin-admin-models-overview");

        history.push("/Admin/Models/models/test1#spreadsheet");
        await screen.findByTestId("admin-admin-models-spreadsheet");

        history.push("/Admin/Models/models/test1#mappings");
        await screen.findByTestId("admin-admin-models-mappings");

        history.push("/Admin/Models/models/test1#source");
        await screen.findByTestId("admin-admin-models-source");
        
        history.push("/Admin/Models/models/test1#tree");
        await screen.findByTestId("admin-admin-models-tree");
    });

});
