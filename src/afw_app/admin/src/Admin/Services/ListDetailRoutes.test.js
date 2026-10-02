// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, screen, waitForSpinner} from "../test-utils";

/*
 * The Adapters, Logs and Authorization Handlers pages each have one TanStack
 * route with an optional id (see ../routes.js): the list, or one item.
 */
describe("Admin list/detail routes", () => {

    test("Adapters lists each adapter with a link to it", async () => {
        renderRoute("/Admin/Adapters");
        await waitForSpinner();

        expect(await screen.findByText(/To create a new Adapter/)).toBeInTheDocument();
        expect(await screen.findByRole("link", { name: "files" })).toHaveAttribute("href", "/Admin/Adapters/files");
    });

    test("An adapter's id opens its details", async () => {
        renderRoute("/Admin/Adapters/files");
        await waitForSpinner();

        expect(await screen.findByText("Properties")).toBeInTheDocument();
        expect(screen.queryByText(/To create a new Adapter/)).not.toBeInTheDocument();
    });

    test("Logs renders its list", async () => {
        renderRoute("/Admin/Logs");
        await waitForSpinner();

        expect(await screen.findByText(/To create a new Log/)).toBeInTheDocument();
    });

    test("Authorization Handlers renders its list", async () => {
        renderRoute("/Admin/AuthHandlers");
        await waitForSpinner();

        expect(await screen.findByText(/To create a new/)).toBeInTheDocument();
    });

});
