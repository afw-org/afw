// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {waitFor, fireEvent, render, screen, act} from "@testing-library/react";
import {withProfiler} from "@afw/test";

import {AdaptorDropdown} from "@afw/react";

const Test = (wrapper) => {

    test("No props", async () => {

        const AdaptorDropdownWithProfiler = withProfiler(AdaptorDropdown);
        render(
            <AdaptorDropdownWithProfiler id="AdaptorDropdown" />,
            { wrapper }
        );

    });   
    
    test("All props", async () => {

        const AdaptorDropdownWithProfiler = withProfiler(AdaptorDropdown);
        render(
            <AdaptorDropdownWithProfiler 
                id="AdaptorDropdown"
                label="Select Adaptor"
                description="Select an Adaptor from the list."
                value="afw"                
            />,
            { wrapper }
        );

        await waitFor(() => expect(screen.getByLabelText("Select Adaptor")).toBeInTheDocument());        
        await waitFor(() => expect(screen.getByText("Select an Adaptor from the list.")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Select Adaptor")).toHaveTextContent("afw"));

    });    

    test("Select an adaptorId", async () => {
        
        const AdaptorDropdownWithProfiler = withProfiler(AdaptorDropdown);
        const onChanged = jest.fn();

        render(
            <AdaptorDropdownWithProfiler 
                id="AdaptorDropdown"
                label="Select Adaptor"
                description="Select an Adaptor from the list."
                value="afw"         
                onChanged={onChanged}       
            />,
            { wrapper }
        );

        await waitFor(() => expect(screen.getByLabelText("Select Adaptor")).toBeInTheDocument());   
        await waitFor(() => expect(screen.getByText("Select an Adaptor from the list.")).toBeInTheDocument());   
        await waitFor(() => expect(screen.getByLabelText("Select Adaptor")).toHaveTextContent("afw"));  

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
