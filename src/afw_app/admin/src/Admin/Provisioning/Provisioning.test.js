// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, screen, waitFor, mswPostCallback, waitForSpinner} from "../test-utils";


describe("Provisioning Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Provisioning renders", async () => {
 
        renderRoute("/Admin/Provisioning");  

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                        
        await waitForSpinner();
        
        expect(await screen.findByTestId("admin-admin-provisioning")).toBeInTheDocument();            
    });
});
