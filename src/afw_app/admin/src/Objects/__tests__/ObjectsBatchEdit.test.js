// See the 'COPYING' file in the project root for licensing information.
import {createMemoryHistory} from "history";
import {Router} from "react-router-dom";


import {render, waitFor, fireEvent, screen, waitForSpinner, mswPostCallback} from "../../test-utils";

import Objects from "../Objects";


describe("ObjectsBatchEdit Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("ObjectsBatchEdit renders", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("retrieve_objects"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("adaptorId", "files"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("objectType", "_AdaptiveObjectType_"));  
        
        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Batch Edit Object(s)")).toBeInTheDocument());          
        await waitFor(() => expect(screen.getByText("_AdaptiveFunctionGenerate_")).toBeInTheDocument());        

        const row = screen.getByText("_AdaptiveFunctionGenerate_").closest("tr");
        fireEvent.click(row);
        fireEvent.click(screen.getByLabelText("Batch Edit Object(s)"));

        await waitFor(() => expect(screen.getByLabelText("Apply Changes")).toBeInTheDocument());

    });
});
