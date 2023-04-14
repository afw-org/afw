// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent, screen} from "@testing-library/react";

const Test = (wrapper, Dialog) => {

    describe("Dialog tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Dialog   
                    isBlocking={true}
                    open={true}                                     
                    title="Dialog Title"
                    maxWidth="lg"
                    contains={
                        <span>This is inside a large Dialog!</span>                                                        
                    }
                    footer={
                        <span>This is inside the Dialog footer!</span>
                    }
                />,
                { wrapper }
            );        
            
            expect(queryByText(/Dialog Title/)).toBeInTheDocument();          
            expect(queryByText(/This is inside a large Dialog!/)).toBeInTheDocument();                      
            expect(queryByText(/This is inside the Dialog footer!/)).toBeInTheDocument();          
        });
       
        test("Renders when closed, content not visible", () => {
            const {queryByText} = render(
                <Dialog   
                    open={false}                                     
                    title="Dialog Title"                    
                    contains={
                        <span>This is inside a Dialog!</span>                                                        
                    }
                    footer={
                        <span>This is inside the Dialog footer!</span>
                    }
                />,
                { wrapper }
            );      

            expect(queryByText(/This is inside a Dialog!/)).not.toBeInTheDocument();                      
        });

        test("Renders subText", () => {
            const {queryByText} = render(
                <Dialog   
                    open={true}                                     
                    title="Dialog Title"    
                    subText="This is some subText"                
                    contains={
                        <span>This is inside a Dialog!</span>                                                        
                    }
                    footer={
                        <span>This is inside the Dialog footer!</span>
                    }
                />,
                { wrapper }
            );      

            expect(queryByText(/This is some subText/)).toBeInTheDocument();   
        });

        test("Renders when closed, content not visible", () => {
            const {queryByText} = render(
                <Dialog   
                    open={false}                                     
                    title="Dialog Title"                    
                    contains={
                        <span>This is inside a Dialog!</span>                                                        
                    }
                    footer={
                        <span>This is inside the Dialog footer!</span>
                    }
                />,
                { wrapper }
            );    

            expect(queryByText(/This is inside a Dialog!/)).not.toBeInTheDocument();       
        });

        test("Renders close button", () => {
            const {queryByRole} = render(
                <Dialog   
                    open={true}                                     
                    title="Dialog Title"      
                    showClose={true}             
                    contains={
                        <span>This is inside a Dialog!</span>                                                        
                    }
                    footer={
                        <span>This is inside the Dialog footer!</span>
                    }
                />,
                { wrapper }
            );    

            expect(queryByRole("button")).toBeTruthy();    
        });

        test("Renders without close button", () => {
            const {queryByRole} = render(
                <Dialog   
                    open={true}                                     
                    title="Dialog Title"      
                    showClose={false}             
                    contains={
                        <span>This is inside a Dialog!</span>                                                        
                    }
                    footer={
                        <span>This is inside the Dialog footer!</span>
                    }
                />,
                { wrapper }
            );    

            expect(queryByRole("button")).not.toBeTruthy();    
        });

        test("Close button closes Dialog", async () => {
            const onDismiss = jest.fn();

            const {queryByRole, queryByText} = render(
                <Dialog   
                    open={true}                                     
                    title="Dialog Title"      
                    showClose={true}       
                    onDismiss={onDismiss}      
                    contains={
                        <span>This is inside a Dialog!</span>                                                        
                    }
                    footer={
                        <span>This is inside the Dialog footer!</span>
                    }
                />,
                { wrapper }
            );    

            expect(queryByText(/This is inside a Dialog!/)).toBeInTheDocument();       
            expect(queryByRole("button")).toBeInTheDocument();
            
            fireEvent.click(screen.queryByRole("button"));
        
            expect(onDismiss).toHaveBeenCalled();
        });


        test("Renders blocking dialog (doesn't dismiss when escape is pressed)", () => {
            const onDismiss = jest.fn();
            const {queryByText} = render(                
                <Dialog   
                    open={true}          
                    isBlocking={true}                   
                    onDismiss={onDismiss}        
                    title="Dialog Title"                                   
                    contains={
                        <span>This is inside a Dialog!</span>                                                        
                    }
                    footer={
                        <span>This is inside the Dialog footer!</span>
                    }
                />,
                { wrapper }
            );   

            expect(queryByText(/This is inside a Dialog!/)).toBeInTheDocument();    
                
            /* fire ESC key */
            fireEvent.keyDown(queryByText(/This is inside a Dialog!/), { key: "Escape", code: "Escape", keyCode: 27, charCode: 27 });

            expect(onDismiss).not.toHaveBeenCalled();
        });

        test("Renders non-blocking dialog (does not dismiss when escape pressed)", () => {

            const onDismiss = jest.fn();
            const {queryByText} = render(                
                <Dialog   
                    open={true}          
                    isBlocking={false}                   
                    onDismiss={onDismiss}        
                    title="Dialog Title"                                   
                    contains={
                        <span>This is inside a Dialog!</span>                                                        
                    }
                    footer={
                        <span>This is inside the Dialog footer!</span>
                    }
                />,
                { wrapper }
            );   

            expect(queryByText(/This is inside a Dialog!/)).toBeInTheDocument();    
                
            /* fire ESC key */
            fireEvent.keyDown(queryByText(/This is inside a Dialog!/), { key: "Escape", code: "Escape", keyCode: 27, charCode: 27 });

            expect(onDismiss).toHaveBeenCalled();            
        });

        test("Renders fullScreen", () => {

            render(                
                <Dialog   
                    open          
                    fullScreen   
                    title="Dialog Title"                                   
                    contains={
                        <span>This is inside a Dialog!</span>                                                        
                    }
                    footer={
                        <span>This is inside the Dialog footer!</span>
                    }
                />,
                { wrapper }
            );  
            
            /* no way from within jest to actually know if it's fullScreen or not */

        });
    });
};

export default Test;
