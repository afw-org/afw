// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent} from "@testing-library/react";

const Test = (wrapper, Modal) => {

    describe("Modal tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Modal                    
                    open
                    contains={
                        <span>This is inside a Modal!</span>                   
                    }
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Modal!/)).toBeInTheDocument();               
        });

        test("Renders non-blocking Modal (dismisses when escape pressed)", async () => {

            const onDismiss = jest.fn();
                            
            const {queryByText} = render(
                <Modal                    
                    open  
                    onDismiss={onDismiss}
                    contains={
                        <span>This is inside a Modal!</span>                   
                    }
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Modal!/)).toBeInTheDocument();   
            
            /* fire ESC key */
            fireEvent.keyDown(queryByText(/This is inside a Modal!/), { key: "Escape", code: "Escape", keyCode: 27, charCode: 27 });

            expect(onDismiss).toHaveBeenCalled();                        
        });

        test("Renders blocking Modal (does not dismiss when escape pressed)", async () => {

            const onDismiss = jest.fn();
                            
            const {queryByText} = render(
                <Modal                    
                    open
                    isBlocking
                    onDismiss={onDismiss}
                    contains={
                        <span>This is inside a Modal!</span>                   
                    }
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Modal!/)).toBeInTheDocument();   
            
            /* fire ESC key */
            fireEvent.keyDown(queryByText(/This is inside a Modal!/), { key: "Escape", code: "Escape", keyCode: 27, charCode: 27 });

            expect(onDismiss).not.toHaveBeenCalled();                        
        });
        
    });
};

export default Test;
