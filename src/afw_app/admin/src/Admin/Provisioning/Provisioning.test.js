// See the 'COPYING' file in the project root for licensing information.
import {MemoryRouter} from "react-router-dom";
import {render, screen, waitFor, mswPostCallback, waitForSpinner} from "../test-utils";
import Provisioning from "./Provisioning";


describe("Provisioning Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Provisioning renders", async () => {
 
        render(
            <MemoryRouter initialEntries={[ "/Admin/Provisioning" ]}>
                <Provisioning /> 
            </MemoryRouter>
        );  

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                        
        await waitForSpinner();
        
        expect(await screen.findByTestId("admin-admin-provisioning")).toBeInTheDocument();            
    });
});
