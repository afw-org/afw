// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, screen, waitForSpinner, mswPostCallback} from "../../../test-utils";

describe("Reference Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Reference renders", async () => {

        window.scrollTo = jest.fn();

        renderRoute("/Documentation/Reference");        
  
        await waitForSpinner();                     

        expect(await screen.findByTestId("admin-documentation-reference")).toBeInTheDocument();
    });
});
