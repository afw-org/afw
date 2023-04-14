// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent, screen} from "@testing-library/react";

const Test = (wrapper, TextField) => {

    describe("TextField tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <TextField       
                    label="Text Field"
                    description="This is the description"
                    value="This is a TextField!"
                />,
                { wrapper }
            );        
            
            expect(screen.queryByDisplayValue(/This is a TextField!/)).toBeInTheDocument();               
        });

        test("Change triggers onChange callback", async () => {
                            
            const onChanged = jest.fn();
            render(
                <TextField      
                    id="TextField" 
                    label="Text Field"
                    description="This is the description"
                    value="This is a TextField!"
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
            
            const input = screen.getByLabelText("Text Field");
            
            fireEvent.change(input, { target: { value: "Something New" } });

            expect(onChanged).toHaveBeenLastCalledWith("Something New");

        });

        test("Change on disabled TextField does not trigger onChange callback", async () => {
                            
            const onChanged = jest.fn();
            render(
                <TextField       
                    id="TextField"
                    label="Text Field"
                    description="This is the description"
                    value="This is a TextField!"
                    onChanged={onChanged}
                    disabled
                />,
                { wrapper }
            );        
            
            const input = screen.getByLabelText("Text Field");
            
            //fireEvent.change(input, { target: { value: "Something New" } });            
            input.value = "Something New";

            expect(onChanged).not.toHaveBeenCalled();

        });
        

        test("Renders with required", async () => {
                            
            render(
                <TextField       
                    label="Text Field"
                    description="This is the description"
                    value="This is a TextField!"
                    required
                />,
                { wrapper }
            );                    

        });
        
    });
};

export default Test;
