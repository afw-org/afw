// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, screen, waitForSpinner} from "../test-utils";

/*
 * The /Documentation routes (see routes.js): the layout's index, and
 * Reference areas whose component picks a view from optional params.
 */
describe("Documentation routes", () => {

    test("/Documentation shows the index", async () => {
        renderRoute("/Documentation");

        expect(await screen.findByText(/provides Reference material/)).toBeInTheDocument();
    });

    test("Components with no category lists the categories", async () => {
        renderRoute("/Documentation/Reference/Components");
        await waitForSpinner();

        expect(await screen.findByText("Component Categories")).toBeInTheDocument();
    });

    test("Schema with no adapter shows the overview", async () => {
        renderRoute("/Documentation/Reference/Schema");
        await waitForSpinner();

        expect(await screen.findByText(/Adapters provide Object Type and Property Type definitions/)).toBeInTheDocument();
    });

    test("Schema with an adapter shows its object types", async () => {
        renderRoute("/Documentation/Reference/Schema/files");
        await waitForSpinner();

        expect(await screen.findByText("_AdaptiveManifest_")).toBeInTheDocument();
        expect(screen.queryByText(/Adapters provide Object Type and Property Type definitions/)).not.toBeInTheDocument();
    });

    test("A data type id shows that data type", async () => {
        renderRoute("/Documentation/Reference/DataTypes/string");
        await waitForSpinner();

        expect((await screen.findAllByText("string")).length).toBeGreaterThan(0);
    });

});
