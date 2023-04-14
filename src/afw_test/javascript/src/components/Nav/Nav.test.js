// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent} from "@testing-library/react";

const Test = (wrapper, Nav) => {

    describe("Nav renders", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Nav       
                    items={[
                        {
                            label: "This is a Nav item!"
                        },
                        {
                            label: "This is also a Nav item!"
                        }
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a Nav item!/)).toBeInTheDocument();               
            expect(queryByText(/This is also a Nav item!/)).toBeInTheDocument();               
        });        
    });

    describe("Nav with nested items renders", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Nav       
                    items={[
                        {
                            label: "item 1"
                        },
                        {
                            label: "item 2",
                            isExpanded: true,
                            items: [
                                {
                                    label: "item 3"
                                }
                            ]
                        }
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/item 1/)).toBeInTheDocument();               
            expect(queryByText(/item 2/)).toBeInTheDocument();    
            expect(queryByText(/item 3/)).toBeInTheDocument();   
        });        
    });

    test("Nav onClick gets called", async () => {
                     
        const onClick = jest.fn();
        const {queryByLabelText} = render(
            <Nav       
                onClick={onClick}
                items={[
                    {
                        label: "This is a Nav item!"
                    },
                    {
                        label: "This is also a Nav item!"
                    }
                ]}
            />,
            { wrapper }
        );        
        
        fireEvent.click(queryByLabelText("This is a Nav item!"));               
        
        expect(onClick).toHaveBeenCalled();
    });        

    test("Nav item onClick gets called", async () => {
                     
        const onClick = jest.fn();
        const {queryByLabelText} = render(
            <Nav       
                items={[
                    {
                        label: "This is a Nav item!",
                        onClick
                    },
                    {
                        label: "This is also a Nav item!"
                    }
                ]}
            />,
            { wrapper }
        );        
        
        fireEvent.click(queryByLabelText("This is a Nav item!"));               
        
        expect(onClick).toHaveBeenCalled();
    }); 
   
};

export default Test;
