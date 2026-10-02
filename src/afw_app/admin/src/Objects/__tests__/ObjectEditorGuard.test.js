// See the 'COPYING' file in the project root for licensing information.
import {editor as monacoEditorMock} from "monaco-editor";
import {vi} from "vitest";

import {renderRoute, act, waitFor, within, fireEvent, screen, waitForSpinner, mswPostCallback} from "../../test-utils";

/*
 * The object editor's unsaved-changes guard (TanStack Router's useBlocker,
 * see ObjectEditor.js): leaving the object with unsaved changes asks first;
 * moving within it (another view's hash, an embedded object) doesn't.
 */

const objectPath = "/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_";
const message = "This Object has unsaved changes.  Are you sure you want to leave?";

/*
 * Start a navigation without awaiting it: a navigation the guard blocks
 * never resolves router.navigate()'s promise (the app never awaits it).
 */
const navigate = (router, options) => act(() => { router.navigate(options); });

/* every component that creates a Monaco editor goes through the mock */
const getLatestMonacoEditorInstance = () => {
    const results = monacoEditorMock.create.mock.results;
    return results[results.length - 1].value;
};

/* open the object and change its source, leaving unsaved changes */
const editObjectSource = async () => {
    const {router} = renderRoute(objectPath);

    await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
    await waitForSpinner();

    await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());
    fireEvent.click(screen.getByLabelText("Edit Object"));
    await waitForSpinner();

    let toolbar, button;
    await waitFor(() => expect(toolbar = screen.getByTestId("ObjectEditorLayout-Toolbar")).toBeInTheDocument());
    await waitFor(() => expect(button = within(toolbar).getByRole("button", { name: "View object source" })).toBeInTheDocument());
    /* a view's hash: moving within the object */
    fireEvent.click(button);
    await waitFor(() => expect(router.state.location.hash).toBe("source"));

    await waitFor(() => expect(screen.getByLabelText("Save")).not.toBeEnabled());
    const monacoInstance = getLatestMonacoEditorInstance();
    const source = monacoInstance.getValue();
    const position = source.indexOf("\"allowAdd\": true");
    monacoInstance.__setValueAndFireChange(
        source.slice(0, position + 11) + "false" + source.slice(position + 16)
    );
    await waitFor(() => expect(screen.getByLabelText("Save")).toBeEnabled());

    return router;
};

describe("Object editor unsaved-changes guard", () => {

    let confirm;

    beforeEach(() => {
        mswPostCallback.mockClear();
        confirm = vi.spyOn(window, "confirm");
    });

    afterEach(() => {
        confirm.mockRestore();
    });

    test("Leaving asks first; Cancel stays on the object", async () => {
        const router = await editObjectSource();

        confirm.mockReturnValueOnce(false);
        navigate(router, { href: "/Objects" });

        await waitFor(() => expect(confirm).toHaveBeenCalledWith(message));
        expect(router.state.location.pathname).toBe(objectPath);
        expect(screen.getByLabelText("Save")).toBeEnabled();
    });

    test("Leaving asks first; OK leaves", async () => {
        const router = await editObjectSource();

        confirm.mockReturnValueOnce(true);
        navigate(router, { href: "/Objects" });

        await waitFor(() => expect(router.state.location.pathname).toBe("/Objects"));
        expect(confirm).toHaveBeenCalledWith(message);
    });

    test("Moving within the object doesn't ask", async () => {
        const router = await editObjectSource();

        navigate(router, { hash: "tree" });

        await waitFor(() => expect(router.state.location.hash).toBe("tree"));
        /* nor did the earlier move to #source */
        expect(confirm).not.toHaveBeenCalled();
    });

});
