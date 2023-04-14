// See the 'COPYING' file in the project root for licensing information.
import {createMemoryHistory} from "history";
import {Router} from "react-router-dom";

import {render, waitFor, fireEvent, screen, waitForSpinner, mswPostCallback} from "../../test-utils";

import Objects from "../Objects";

describe("Objects Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Objects renders", async () => {

        render( <Objects /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitForSpinner();                    

        await waitFor(() => expect(screen.getByLabelText("Adaptor")).toHaveTextContent("afw"));        
        await waitFor(() => expect(screen.getByLabelText("Object Type")).not.toBeDisabled());
        await waitFor(() => expect(screen.getByLabelText("Retrieve")).not.toBeDisabled());

    });

    test("Objects page shows adaptors and object types", async () => {
        const history = createMemoryHistory();
        history.push("/Objects");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitForSpinner();
        
        await waitFor(() => expect(screen.getByLabelText("Adaptor")).toHaveTextContent("afw"));
        await waitFor(() => expect(screen.getByLabelText("Object Type")).toBeInTheDocument());               
        await waitFor(() => expect(screen.getByLabelText("Retrieve")).not.toBeDisabled());
        await waitFor(() => expect(screen.getByLabelText("Object Type")).not.toBeDisabled());


        fireEvent.mouseDown(screen.getByRole("button", { name: /adaptor /i }));

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled()); 
        await waitForSpinner();

        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        const filesOpt = screen.getByRole("option", { name: "files" });        
        // select 'files'
        filesOpt.click();
        // wait options to disappear
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));    

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled()); 
        await waitForSpinner();
        
        const combobox = screen.getByRole("combobox", { name: /object type/i });
        await waitFor(() => expect(combobox).not.toBeDisabled());
        
        fireEvent.mouseDown(combobox);
        fireEvent.click(combobox);

        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        const typeOpt = screen.getByRole("option", { name: "_AdaptiveObjectType_" });        
        // select '_AdaptiveObjectType_'
        typeOpt.click();
        // wait options to disappear
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0)); 

        expect(await screen.findByLabelText("Object Type")).toHaveValue("_AdaptiveObjectType_");
        //expect(await screen.findByTitle(/The adaptive object type of all adaptive object type objects./)).toBeInTheDocument();

    });    
    
});
