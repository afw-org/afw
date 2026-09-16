// See the 'COPYING' file in the project root for licensing information.
import {
    server,
    http,
    HttpResponse,
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

        const moreOptionsBtn = await screen.findByRole("button", { name: "More Options" });        
        
        fireEvent.click(moreOptionsBtn);
        const menu = await screen.findByRole("menu");

        within(menu).getByRole("menuitem", { name: "Edit Model" });        
        within(menu).getByRole("menuitem", { name: "Save Model" });        
        within(menu).getByRole("menuitem", { name: "Refresh Model" });        
        within(menu).getByRole("menuitem", { name: "Restart Model Adapter" });        
        within(menu).getByRole("menuitem", { name: "Set Mapped Adapter" });        
        within(menu).getByRole("menuitem", { name: "Simulate" });
        within(menu).getByRole("menuitem", { name: "Deploy to Service" });                

    });

    test("Select Mapped Adapter (None)", async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models/test1");

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

        const setMappedAdapter = within(menu).getByRole("menuitem", { name: "Set Mapped Adapter" });        

        fireEvent.click(setMappedAdapter);       
        const none = screen.getByRole("menuitem", { name: "(None)" });

        fireEvent.click(none);
        await waitFor(() => expect(screen.queryByRole("menu")).not.toBeInTheDocument());

    });

    /* eslint-disable jest/no-disabled-tests, jest/expect-expect -- acknowledged, tracked TODOs for the remaining context menu actions, not yet implemented */

    test.skip("Edit Model", async () => {

    });

    test.skip("Save Model", async () => {

    });

    test.skip("Refresh Model", async () => {

    });

    test.skip("New", async () => {

    });

    test.skip("Rename", async () => {

    });

    test.skip("Delete", async () => {

    });

    test.skip("Restart Model Adapter", async () => {

    });

    test.skip("Set Mapped Adapter", async () => {

    });

    test.skip("Simulate", async () => {

    });

    test.skip("Deploy to Service", async () => {

    });

    /* eslint-enable jest/no-disabled-tests, jest/expect-expect */

});
