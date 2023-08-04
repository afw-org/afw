// See the 'COPYING' file in the project root for licensing information.
import {MemoryRouter} from "react-router-dom";
import {render, screen, waitFor, waitForSpinner, mswPostCallback} from "../test-utils";
import RequestHandlers from "./RequestHandlers";

describe("RequestHandlers Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("RequestHandlers renders", async () => {

        render(
            <MemoryRouter initialEntries={[ "/Admin/RequestHandlers" ]}>
                <RequestHandlers /> 
            </MemoryRouter>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                        
        await waitForSpinner();

        expect(await screen.findByTestId("admin-admin-requestHandlers")).toBeInTheDocument();             
    });
});
