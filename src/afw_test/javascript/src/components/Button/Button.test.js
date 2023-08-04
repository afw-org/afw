// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent} from "@testing-library/react";

const Test = (wrapper, Button) => {

    describe("Button tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Button       
                    label="This is inside a Button!"
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Button!/)).toBeInTheDocument();               
        });

        test("Renders properly with an icon", async () => {
                            
            const {queryByText} = render(
                <Button       
                    label="This is inside a Button!"
                    icon="edit"
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Button!/)).toBeInTheDocument();               
        });
       
        test("Button responds to click", async () => {

            const onClick = jest.fn();
            const {queryByText, queryByRole} = render(
                <Button       
                    label="This is inside a Button!"
                    onClick={onClick}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Button!/)).toBeInTheDocument();  

            fireEvent.click(queryByRole("button"));
            
            expect(onClick).toHaveBeenCalled();
        });

        test("Disabled Button should not respond to click", async () => {

            const onClick = jest.fn();
            const {queryByText, queryByRole} = render(
                <Button       
                    label="This is inside a Button!"
                    disabled
                    onClick={onClick}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Button!/)).toBeInTheDocument();  

            fireEvent.click(queryByRole("button"));
            
            expect(onClick).not.toHaveBeenCalled();
        });

        test("Renders properly with menu items", async () => {
                            
            const {queryByRole} = render(
                <Button       
                    type="icon"
                    items={[
                        {
                            key: "item1",
                            label: "Item 1"
                        },
                        {
                            key: "item2",
                            label: "Item 2"
                        },
                    ]}
                />,
                { wrapper }
            );        
                                      
            fireEvent.click(queryByRole("button"));

            expect(queryByRole("menu")).toBeInTheDocument();
        });

        test("Button with menu items can be selected", async () => {
                            
            const onClick = jest.fn();
            const {queryByRole, queryByText} = render(
                <Button       
                    type="icon"
                    items={[
                        {
                            key: "item1",
                            label: "Item 1"
                        },
                        {
                            key: "item2",
                            label: "Item 2",
                            onClick
                        },
                    ]}
                />,
                { wrapper }
            );        
                                      
            fireEvent.click(queryByRole("button"));

            expect(queryByRole("menu")).toBeInTheDocument();
            expect(queryByText("Item 1")).toBeInTheDocument();
            expect(queryByText("Item 2")).toBeInTheDocument();

            fireEvent.click(queryByText("Item 2"));

            expect(onClick).toHaveBeenCalled();
        });

        test("Renders properly with a tooltip", async () => {

            render(
                <Button       
                    label="Click Me!"
                    tooltip="This is a tooltip!"
                />,
                { wrapper }
            );  

            // not sure how to validate this yet
        });
    });
};

export default Test;
