// See the 'COPYING' file in the project root for licensing information.
import {render, screen, waitFor, waitForSpinner, mswPostCallback} from "../../../test-utils";
import Functions, {Function} from "../Functions";

describe("Functions Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Functions renders", async () => {

        window.scrollTo = jest.fn();

        render( <Functions /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                        
        await waitForSpinner();
 
    });

    test("Function renders with functionId=add", async () => {

        window.scrollTo = jest.fn();

        render( <Function functionId="add" /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitForSpinner();                    

        expect(await screen.findByTestId("admin-documentation-reference-function-add")).toBeInTheDocument();      
    });
});
