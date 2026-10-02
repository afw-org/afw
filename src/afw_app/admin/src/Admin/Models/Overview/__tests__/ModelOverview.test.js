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
    fireEvent
} from "../../../test-utils";

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
    }); 

    test("Readonly Overview", async () => {

        renderRoute("/Admin/Models/models/test1#overview");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        await screen.findByRole("heading", { name: test1Model.modelId });
        await screen.findByText(test1Model.description);

        await screen.findByRole("table", { name: "Object Types" });

        expect(await screen.findByRole("link", { name: "obj1" }))
            .toHaveAttribute("href", "/Admin/Models/models/test1/objectTypes/obj1#overview");
    });

    /* editing shows the tables whose links append the view hash */
    const editModel = async (path) => {
        renderRoute(path);

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        fireEvent.click(await screen.findByRole("button", { name: "Edit Model" }));
        await waitFor(() => expect(screen.queryByRole("button", { name: "Edit Model" })).not.toBeInTheDocument());
    };

    test("Editable overview links its object types, hash after the path", async () => {

        await editModel("/Admin/Models/models/test1#overview");
        fireEvent.click(await screen.findByRole("tab", { name: /Object Types/ }));

        expect(await screen.findByRole("link", { name: "obj1" }))
            .toHaveAttribute("href", "/Admin/Models/models/test1/objectTypes/obj1#overview");
    });

    test("Editable object type overview links its property types, hash after the path", async () => {

        await editModel("/Admin/Models/models/test1/objectTypes/obj1#overview");
        fireEvent.click(await screen.findByRole("tab", { name: /Properties/ }));

        expect(await screen.findByRole("link", { name: "prop1" }))
            .toHaveAttribute("href", "/Admin/Models/models/test1/objectTypes/obj1/propertyTypes/prop1#overview");
    });

});
