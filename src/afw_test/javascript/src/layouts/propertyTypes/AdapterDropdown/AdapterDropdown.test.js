// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {waitFor, render, screen} from "@testing-library/react";
import {withProfiler, userEvent} from "@afw/test";

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

        // userEvent (not raw fireEvent/.click()) is what reliably opens/closes
        // a MUI Select in tests
        await userEvent.click(screen.getByRole("combobox"));

        // wait for render with options
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));

        const options = screen.getAllByRole("option");
        await userEvent.click(options[3]);

        // wait options to disappear
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));
               
        await waitFor(() => expect(onChanged).toHaveBeenCalled());                

    });
};


export default Test;
