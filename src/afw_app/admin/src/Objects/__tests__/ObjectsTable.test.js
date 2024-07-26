// See the 'COPYING' file in the project root for licensing information.
import {createMemoryHistory} from "history";
import {Router} from "react-router-dom";

import {act, render, waitFor, within, fireEvent, screen, waitForSpinner, mswPostCallback} from "../../test-utils";

import objectTypes from "@afw/test/build/cjs/__mocks__/retrieve_objects/files/_AdaptiveObjectType_.json";
import Objects from "../Objects";


describe("ObjectsTable Tests", () => { 

    // \fixme There is a timing issue here that needs investigation.  Sometimes selecting adapterId doesn't work
    test.skip("Retrieve _AdaptiveObjectType_ objects", async () => {
        const history = createMemoryHistory();
        history.push("/Objects");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitForSpinner();        
               
        await waitFor(() => expect(screen.getByLabelText("Adapter")).toHaveTextContent("afw"));
        await waitFor(() => expect(screen.getByLabelText("Object Type")).toBeInTheDocument());       
        
        fireEvent.mouseDown(screen.getByRole("button", { name: /adapter /i }));

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled()); 
        await waitForSpinner();

        mswPostCallback.mockClear();
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        act(() => {
            const filesOpt = screen.getByRole("option", { name: "files" });                 
            // select 'files'        
            filesOpt.click();
        });
        
        // wait options to disappear
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));       
        await waitFor(() => expect(screen.getByLabelText("Adapter")).toHaveTextContent("files"));

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("retrieve_objects"));         
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("adapterId", "files"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("objectType", "_AdaptiveObjectType_"));
        await waitForSpinner();

        const combobox = screen.getByRole("combobox");

        const textbox = screen.getByRole("textbox", { name: "Object Type" });        
        const dropBtn = within(combobox).getByRole("button", { name: "Open" });
        
        await waitFor(() => expect(textbox).not.toBeDisabled());                
        await waitFor(() => expect(dropBtn).not.toBeDisabled());                
        
        fireEvent.click(dropBtn);        
        
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        const typeOpt = screen.getByRole("option", { name: "_AdaptiveObjectType_" });        
        // select '_AdaptiveObjectType_'
        fireEvent.mouseDown(typeOpt);
        typeOpt.click();
        // wait options to disappear
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));         
        
        mswPostCallback.mockClear();
        fireEvent.click(screen.getByLabelText("Retrieve Objects"));

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("retrieve_objects_to_response"));        
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("adapterId", "files"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("objectType", "_AdaptiveObjectType_"));

        await waitForSpinner();

        // make sure each objectType is returned in the table    
        await waitFor(() => expect(screen.getByRole("table")).toBeInTheDocument());
        const table = within(screen.getByRole("table"));
        for (const o of objectTypes.result) {
            const {objectType, description} = o;
            
            expect(table.getByText(objectType)).toBeInTheDocument();

            if (description.indexOf("/") >= 0)
                continue;
                
            // find the row
            const row = within(table.getByText(objectType).closest("tr"));
            expect(row.getByText(description)).toBeInTheDocument();
        }

    });

    test("View objects", async () => {
        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("retrieve_objects_to_response"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("adapterId", "files"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("objectType", "_AdaptiveObjectType_"));    

        await waitForSpinner();        
        expect(await screen.findByRole("table")).toBeInTheDocument();

    });

});
