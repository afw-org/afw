// See the 'COPYING' file in the project root for licensing information.
import {
    server,
    http,
    HttpResponse,
    render,
    waitFor,
    screen,
    mswPostCallback,
    waitForSpinner,
    fireEvent,
} from "../../../test-utils";
import {Router} from "react-router-dom";
import {createMemoryHistory} from "history";
import {Models} from "../../Models";

describe("ModelSpreadsheet Tests", () => {

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
            },
            /* obj2 deliberately has no propertyTypes - a minimal object type with none defined yet */
            obj2: {}
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

    test("View spreadsheet", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1#spreadsheet");

        /* return a model for /models/_AdaptiveModel_/test1 */
        server.use(
            http.post("/afw", async ({request}) => {
                const body = await request.clone().json();
                const {function: functionId, adapterId} = body;

                if (functionId === "retrieve_objects" && adapterId === "models") {
                    mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});

                    return HttpResponse.json({
                        status: "success",
                        result: [ test1Model ],
                    });
                }
            })
        );

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            http.post("/afw", async ({request}) => {
                const body = await request.clone().json();
                const {function: functionId, uri} = body;

                if (functionId === "get_object_with_uri" && uri === "/models/_AdaptiveModel_/test1") {
                    mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});

                    return HttpResponse.json({
                        status: "success",
                        result: test1Model,
                    });
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

        await screen.findByTestId("admin-admin-models-spreadsheet");
    });

    test("View spreadsheet (editable)", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1#spreadsheet");

        /* return a model for /models/_AdaptiveModel_/test1 */
        server.use(
            http.post("/afw", async ({request}) => {
                const body = await request.clone().json();
                const {function: functionId, adapterId} = body;

                if (functionId === "retrieve_objects" && adapterId === "models") {
                    mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});

                    return HttpResponse.json({
                        status: "success",
                        result: [ test1Model ],
                    });
                }
            })
        );

        /* return a model for /models/_AdaptiveModel_/test */
        server.use(
            http.post("/afw", async ({request}) => {
                const body = await request.clone().json();
                const {function: functionId, uri} = body;

                if (functionId === "get_object_with_uri" && uri === "/models/_AdaptiveModel_/test1") {
                    mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});

                    return HttpResponse.json({
                        status: "success",
                        result: test1Model,
                    });
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

        await screen.findByTestId("admin-admin-models-spreadsheet");
    });

});
