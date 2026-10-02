// See the 'COPYING' file in the project root for licensing information.
import {editor as monacoEditorMock} from "monaco-editor";
import {vi} from "vitest";

import {
    renderRoute,
    server,
    http,
    HttpResponse,
    act,
    waitFor,
    screen,
    mswPostCallback,
    waitForSpinner,
    fireEvent
} from "../../test-utils";

/*
 * The model editor's unsaved-changes guard (TanStack Router's useBlocker,
 * see ModelEditor.js): leaving the adapter's models with unsaved changes
 * asks first; moving within them (another view, another part of a model)
 * doesn't.
 */

const message = "This Model has unsaved changes.  Are you sure you want to leave?";

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

/* every component that creates a Monaco editor goes through the mock */
const getLatestMonacoEditorInstance = () => {
    const results = monacoEditorMock.create.mock.results;
    return results[results.length - 1].value;
};

/* start a navigation without awaiting it: a blocked one never resolves */
const navigate = (router, options) => act(() => { router.navigate(options); });

/* open the model's source, edit it, leaving unsaved changes */
const editModelSource = async () => {
    server.use(
        http.post("/afw", async ({request}) => {
            const body = await request.clone().json();
            const {function: functionId, adapterId, uri} = body;

            if (functionId === "retrieve_objects" && adapterId === "models") {
                mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});
                return HttpResponse.json({ status: "success", result: [ test1Model ] });
            }

            if (functionId === "get_object_with_uri" && uri === "/models/_AdaptiveModel_/test1") {
                mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});
                return HttpResponse.json({ status: "success", result: test1Model });
            }
        })
    );

    const {router} = renderRoute("/Admin/Models/models/test1#source");

    await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
    await waitForSpinner();

    fireEvent.click(await screen.findByRole("button", { name: "Edit Model" }));
    await waitForSpinner();
    await screen.findByTestId("admin-admin-models-source");

    /* type through the Monaco mock; wait out CodeEditor's 100ms debounce */
    await act(async () => {
        getLatestMonacoEditorInstance().__setValueAndFireChange(
            JSON.stringify({ ...test1Model, description: "Changed" }, null, 4)
        );
        await new Promise(resolve => setTimeout(resolve, 150));
    });
    await waitFor(() => expect(screen.getByRole("button", { name: "Save" })).toBeEnabled());

    return router;
};

describe("Model editor unsaved-changes guard", () => {

    let confirm;

    beforeEach(() => {
        mswPostCallback.mockClear();
        confirm = vi.spyOn(window, "confirm");
    });

    afterEach(() => {
        confirm.mockRestore();
    });

    test("Leaving asks first; Cancel stays", async () => {
        const router = await editModelSource();

        confirm.mockReturnValueOnce(false);
        navigate(router, { href: "/Admin/Status" });

        await waitFor(() => expect(confirm).toHaveBeenCalledWith(message));
        expect(router.state.location.pathname).toBe("/Admin/Models/models/test1");
        expect(screen.getByRole("button", { name: "Save" })).toBeEnabled();
    });

    test("Leaving asks first; OK leaves", async () => {
        const router = await editModelSource();

        confirm.mockReturnValueOnce(true);
        navigate(router, { href: "/Admin/Status" });

        await waitFor(() => expect(router.state.location.pathname).toBe("/Admin/Status"));
        expect(confirm).toHaveBeenCalledWith(message);
    });

    test("Moving within the models doesn't ask", async () => {
        const router = await editModelSource();

        navigate(router, { href: "/Admin/Models/models/test1/objectTypes/obj1#source" });

        await waitFor(() => expect(router.state.location.pathname).toBe("/Admin/Models/models/test1/objectTypes/obj1"));
        expect(confirm).not.toHaveBeenCalled();
    });

});
