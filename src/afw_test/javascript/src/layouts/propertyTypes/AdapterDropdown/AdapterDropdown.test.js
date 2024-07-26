// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {waitFor, fireEvent, render, screen, act} from "@testing-library/react";
import {withProfiler} from "@afw/test";

import {AdapterDropdown} from "@afw/react";

const Test = (wrapper) => {

    test("No props", async () => {

        const AdapterDropdownWithProfiler = withProfiler(AdapterDropdown);
        render(
            <AdapterDropdownWithProfiler id="AdapterDropdown" />,
            { wrapper }
        );

    });   
    
    test("All props", async () => {

        const AdapterDropdownWithProfiler = withProfiler(AdapterDropdown);
        render(
            <AdapterDropdownWithProfiler 
                id="AdapterDropdown"
                label="Select Adapter"
                description="Select an Adapter from the list."
                value="afw"                
            />,
            { wrapper }
        );

        await waitFor(() => expect(screen.getByLabelText("Select Adapter")).toBeInTheDocument());        
        await waitFor(() => expect(screen.getByText("Select an Adapter from the list.")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Select Adapter")).toHaveTextContent("afw"));

    });    

    test("Select an adapterId", async () => {
        
        const AdapterDropdownWithProfiler = withProfiler(AdapterDropdown);
        const onChanged = jest.fn();

        render(
            <AdapterDropdownWithProfiler 
                id="AdapterDropdown"
                label="Select Adapter"
                description="Select an Adapter from the list."
                value="afw"         
                onChanged={onChanged}       
            />,
            { wrapper }
        );

        await waitFor(() => expect(screen.getByLabelText("Select Adapter")).toBeInTheDocument());   
        await waitFor(() => expect(screen.getByText("Select an Adapter from the list.")).toBeInTheDocument());   
        await waitFor(() => expect(screen.getByLabelText("Select Adapter")).toHaveTextContent("afw"));  

        await act(async () => {            
            fireEvent.mouseDown(screen.getByRole("button"));

            // wait for render with options            
            await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));

            const options = screen.getAllByRole("option");            
            options[3].click();                    
            
            // wait options to disappear
            await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));
        });
               
        await waitFor(() => expect(onChanged).toHaveBeenCalled());                

    });
};


export default Test;
