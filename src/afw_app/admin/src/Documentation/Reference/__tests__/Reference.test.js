// See the 'COPYING' file in the project root for licensing information.
import {render, screen, waitForSpinner, mswPostCallback} from "../../../test-utils";
import Reference from "../Reference";

describe("Reference Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Reference renders", async () => {

        window.scrollTo = jest.fn();

        render( <Reference /> );        
  
        await waitForSpinner();                     

        expect(await screen.findByTestId("admin-documentation-reference")).toBeInTheDocument();
    });
});
