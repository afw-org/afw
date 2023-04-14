// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {waitFor, render, screen, fireEvent} from "@testing-library/react";

import {HybridEditor} from "@afw/react";

/**
 * Tests for dataType=object  
 */
const Test = (wrapper) => {

    test("Renders with no props", async () => {
        render(
            <HybridEditor 
            />,
            { wrapper }
        );
    });

    describe("Test with template values", () => {

        test("Renders with a label, description and value", async () => {
            
            render(                
                <HybridEditor 
                    id="HybridEditor"
                    label="Hybrid Editor"
                    value="${var}"
                    description="This is a hybrid editor."                
                />,                
                { wrapper }
            );
    
            await waitFor(() => expect(screen.getByLabelText("Hybrid Editor")).toBeInTheDocument());
        });

        test("Enter a value directly in the TextField", async () => {

            const onChanged = jest.fn();
            render(                
                <HybridEditor 
                    id="HybridEditor"
                    label="Hybrid Editor"                    
                    description="This is a hybrid editor."       
                    onChanged={onChanged}         
                />,                
                { wrapper }
            );
    
            await waitFor(() => expect(screen.getByLabelText("Hybrid Editor")).toBeInTheDocument());

            const input = screen.getByLabelText("Hybrid Editor");
            fireEvent.change(input, { target: { value: "${var}"}});

            await waitFor(() => expect(onChanged).toHaveBeenLastCalledWith("${var}"));

        });
    });



};

export default Test;
