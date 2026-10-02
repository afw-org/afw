// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, screen, waitFor, waitForSpinner, mswPostCallback} from "../../../test-utils";

describe("Functions Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Functions renders", async () => {

        window.scrollTo = jest.fn();

        renderRoute("/Documentation/Reference/Functions");        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                        
        await waitForSpinner();
 
    });

    test("Function renders with functionId=add", async () => {

        window.scrollTo = jest.fn();

        renderRoute("/Documentation/Reference/Functions/polymorphic/add");        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitForSpinner();                    

        expect(await screen.findByTestId("admin-documentation-reference-function-add")).toBeInTheDocument();      
    });
});
