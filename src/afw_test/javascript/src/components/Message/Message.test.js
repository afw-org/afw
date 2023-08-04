// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent, screen} from "@testing-library/react";

const Test = (wrapper, Message) => {

    describe("Message tests", () => {

        test("Renders properly with text inside", async () => {
                            
            render(
                <Message          
                    message="This is inside a Message!"                    
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is inside a Message!/)).toBeInTheDocument();               
        });

        test("Renders error message properly with text inside", async () => {
                            
            render(
                <Message        
                    status="error"  
                    message="This is inside a Message!"                    
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is inside a Message!/)).toBeInTheDocument();               
        });
        
        test("Renders info message properly with text inside", async () => {
                            
            render(
                <Message        
                    status="info"  
                    message="This is inside a Message!"                    
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is inside a Message!/)).toBeInTheDocument();               
        });

        test("Renders warning message properly with text inside", async () => {
                            
            render(
                <Message        
                    status="warning"  
                    message="This is inside a Message!"                    
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is inside a Message!/)).toBeInTheDocument();               
        });

        test("Renders multiline message properly", async () => {
                            
            render(
                <Message        
                    isMultiline={true}
                    message="This is inside a Message!\nThis is another line\nMore text!"                    
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is another line/)).toBeInTheDocument();               
        });

        test("Renders message content (contains) properly with text inside", async () => {
                            
            render(
                <Message         
                    contains={<div>This is inside a Message!</div>}
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is inside a Message!/)).toBeInTheDocument();               
        });

        test("Renders message that can be dismissed", async () => {
                            
            const onDismiss = jest.fn();
            render(
                <Message        
                    canDismiss={true}
                    onDismiss={onDismiss}
                    message="This is inside a Message!"                    
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is inside a Message!/)).toBeInTheDocument();          
            
            fireEvent.click(screen.getByText("close"));

            expect(onDismiss).toHaveBeenCalled();

        });
    });
};

export default Test;
