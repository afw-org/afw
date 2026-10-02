// See the 'COPYING' file in the project root for licensing information.
import {
    renderRoute,
    server,
    http,
    HttpResponse,
    waitFor,
    screen,
    mswPostCallback,
    waitForSpinner,
    act
} from "../../test-utils";

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

        /* return a model for /models/_AdaptiveModel_/test */
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

        const {router} = renderRoute("/Admin/Models/models/test1");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        act(() => { router.navigate({ href: "/Admin/Models/models/test1#overview" }); });
        await screen.findByTestId("admin-admin-models-overview");

        act(() => { router.navigate({ href: "/Admin/Models/models/test1#spreadsheet" }); });
        await screen.findByTestId("admin-admin-models-spreadsheet");

        act(() => { router.navigate({ href: "/Admin/Models/models/test1#mappings" }); });
        await screen.findByTestId("admin-admin-models-mappings");

        act(() => { router.navigate({ href: "/Admin/Models/models/test1#source" }); });
        await screen.findByTestId("admin-admin-models-source");
        
        act(() => { router.navigate({ href: "/Admin/Models/models/test1#tree" }); });
        await screen.findByTestId("admin-admin-models-tree");
    });

});
