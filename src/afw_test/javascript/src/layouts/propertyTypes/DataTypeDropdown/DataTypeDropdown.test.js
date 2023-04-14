// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {waitFor, fireEvent, render, screen, act} from "@testing-library/react";
import {withProfiler} from "@afw/test";

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
        const button = await screen.findByRole("button");
        
        await waitFor(() => {
            fireEvent.mouseDown(button);

            expect(screen.queryAllByRole("option")).not.toHaveLength(0);
        });

        // \fixme I don't think this act() is necessary anymore
        await act(async () => {                                      
            const options = screen.getAllByRole("option");
            options[1].click();                                             

            // wait options to disappear
            await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));
        });

        await waitFor(() => expect(onChanged).toHaveBeenCalled());    
        
    });
    
};

export default Test;
