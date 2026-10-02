// See the 'COPYING' file in the project root for licensing information.
import {editor as monacoEditorMock} from "monaco-editor";
import {vi} from "vitest";

import {renderRoute, act, waitFor, fireEvent, screen, waitForSpinner, mswPostCallback} from "../../test-utils";

/*
 * Fiddle's unsaved-changes guard (TanStack Router's useBlocker, see
 * Fiddle.js): with unsaved source, leaving Fiddle asks first.
 */

const message = "You have unsaved changes.  Are you sure you want to leave?";

/* every component that creates a Monaco editor goes through the mock */
const getLatestMonacoEditorInstance = () => {
    const results = monacoEditorMock.create.mock.results;
    return results[results.length - 1].value;
};

/* start a navigation without awaiting it: a blocked one never resolves */
const navigate = (router, options) => act(() => { router.navigate(options); });

/* open a source window and type into it, leaving unsaved changes */
const editFiddle = async () => {
    const {router} = renderRoute("/Tools/Fiddle");

    await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
    await waitForSpinner();

    await waitFor(() => expect(screen.getByLabelText("New Source Window")).toBeInTheDocument());
    fireEvent.click(screen.getByLabelText("New Source Window"));
    await waitFor(() => expect(screen.getByText("Untitled-1")).toBeInTheDocument());

    getLatestMonacoEditorInstance().__setValueAndFireChange("1 + 1");
    await waitFor(() => expect(screen.getByText("Untitled-1 *")).toBeInTheDocument());

    return router;
};

describe("Fiddle unsaved-changes guard", () => {

    let confirm;

    beforeEach(() => {
        mswPostCallback.mockClear();
        confirm = vi.spyOn(window, "confirm");
    });

    afterEach(() => {
        confirm.mockRestore();
    });

    test("Leaving asks first; Cancel stays", async () => {
        const router = await editFiddle();

        confirm.mockReturnValueOnce(false);
        navigate(router, { href: "/Tools" });

        await waitFor(() => expect(confirm).toHaveBeenCalledWith(message));
        expect(router.state.location.pathname).toBe("/Tools/Fiddle");
        expect(screen.getByText("Untitled-1 *")).toBeInTheDocument();
    });

    test("Leaving asks first; OK leaves", async () => {
        const router = await editFiddle();

        confirm.mockReturnValueOnce(true);
        navigate(router, { href: "/Tools" });

        await waitFor(() => expect(router.state.location.pathname).toBe("/Tools"));
        expect(confirm).toHaveBeenCalledWith(message);
    });

});
