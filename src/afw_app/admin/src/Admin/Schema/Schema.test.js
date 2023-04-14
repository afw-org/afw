// See the 'COPYING' file in the project root for licensing information.
import {MemoryRouter} from "react-router-dom";
import {render, screen, waitFor, waitForSpinner, mswPostCallback} from "../test-utils";
import Schema from "./Schema";

describe("Schema Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Schema renders", async () => {

        render(
            <MemoryRouter initialEntries={[ "/Admin/Schema" ]}>
                <Schema /> 
            </MemoryRouter>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                        
        await waitForSpinner();

        expect(await screen.findByTestId("admin-admin-schema")).toBeInTheDocument();
    
    });
});
