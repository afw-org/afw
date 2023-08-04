// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Checkbox) => {

    describe("Checkbox tests", () => {

        test("Renders properly with label", async () => {
                            
            const {queryByText} = render(
                <Checkbox       
                    label="This is a Checkbox!"
                    description="This is a Checkbox description"
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a Checkbox!/)).toBeInTheDocument();               
            expect(queryByText(/This is a Checkbox description/)).toBeInTheDocument();  
        });

        test("Checking it triggers onChange", async () => {
                            
            const onChanged = jest.fn();
            const {queryByText, queryByRole} = render(
                <Checkbox       
                    label="This is a Checkbox!"
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a Checkbox!/)).toBeInTheDocument();        
            
            queryByRole("checkbox").click();          

            expect(onChanged).toHaveBeenCalled();
        });
        
        test("Checking disabled checkbox does not trigger onChange", async () => {
                            
            const onChanged = jest.fn();
            const {queryByText, queryByRole} = render(
                <Checkbox       
                    label="This is a Checkbox!"
                    disabled
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a Checkbox!/)).toBeInTheDocument();        
                   
            queryByRole("checkbox").click();

            expect(onChanged).not.toHaveBeenCalled();
        });
        
    });
};

export default Test;
