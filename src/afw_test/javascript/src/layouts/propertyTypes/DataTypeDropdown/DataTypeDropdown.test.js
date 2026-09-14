// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {waitFor, render, screen} from "@testing-library/react";
import {withProfiler, userEvent} from "@afw/test";

import {DataTypeDropdown} from "@afw/react";

const Test = (wrapper) => {

    test("No props", async () => {
        
        const DataTypeDropdownWithProfiler = withProfiler(DataTypeDropdown);
        const {container} = render(
            <DataTypeDropdownWithProfiler id="DataTypeDropdown" />,
            { wrapper }
        );

        expect(container.firstChild).not.toBeNull();

    });   
    
    test("All props", async () => {

        const DataTypeDropdownWithProfiler = withProfiler(DataTypeDropdown);
        const {container} = render(
            <DataTypeDropdownWithProfiler 
                id="DataTypeDropdown"
                label="Select Datatype"
                description="Select a datatype from the list."
                value="string"
            />,
            { wrapper }
        );
        
        expect(container.firstChild).not.toBeNull();
        
        await waitFor(() => expect(screen.getByLabelText("Select Datatype")).toHaveTextContent("string"));
        await waitFor(() => expect(screen.getByText("Select a datatype from the list.")).toBeInTheDocument());        
        
            
    });    

    test("Select a dataType", async () => {
        
        const DataTypeDropdownWithProfiler = withProfiler(DataTypeDropdown);
        const onChanged = jest.fn();

        const {container} = render(
            <DataTypeDropdownWithProfiler 
                id="DataTypeDropdown"
                label="Select Datatype"
                description="Select a dataType from the list."
                value="string"    
                onChanged={onChanged}       
            />,
            { wrapper }
        );
        
        await screen.findByLabelText("Select Datatype");        
        const button = await screen.findByRole("combobox");
        
        // userEvent (not a raw fireEvent.mouseDown) is what reliably opens a
        // MUI Select in tests
        await userEvent.click(button);
        await waitFor(() => {
            expect(screen.queryAllByRole("option")).not.toHaveLength(0);
        });

        const options = screen.getAllByRole("option");
        await userEvent.click(options[1]);

        // wait options to disappear
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));

        await waitFor(() => expect(onChanged).toHaveBeenCalled());    
        
    });
    
};

export default Test;
