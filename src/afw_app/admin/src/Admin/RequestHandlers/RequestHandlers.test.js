// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, screen, waitFor, waitForSpinner, mswPostCallback} from "../test-utils";

describe("RequestHandlers Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("RequestHandlers renders", async () => {

        renderRoute("/Admin/RequestHandlers");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                        
        await waitForSpinner();

        expect(await screen.findByTestId("admin-admin-requestHandlers")).toBeInTheDocument();             
    });
});
