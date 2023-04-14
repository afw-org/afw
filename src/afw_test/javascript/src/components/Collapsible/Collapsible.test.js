// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent, screen} from "@testing-library/react";

const Test = (wrapper, Collapsible) => {

    describe("Collapsible tests", () => {

        test("Renders properly with text hidden", async () => {
                            
            render(
                <Collapsible   
                    contains={
                        <span>This is inside a Collapsible!</span>
                    }
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is inside a Collapsible!/)).not.toBeInTheDocument();            
        });

        test("Click to expand contents", async () => {

            render(                    
                <Collapsible   
                    contains={
                        <span>This is inside a Collapsible!</span>
                    }
                />,
                { wrapper }
            );

            fireEvent.click(screen.queryByRole("button"));
            
            expect(screen.getByText(/This is inside a Collapsible!/)).toBeInTheDocument();
        });

        test("Click to expand contents, then close again", async () => {

            render(                    
                <Collapsible   
                    contains={
                        <span>This is inside a Collapsible!</span>
                    }
                />,
                { wrapper }
            );

            fireEvent.click(screen.queryByRole("button"));
            
            expect(screen.queryByText(/This is inside a Collapsible!/)).toBeInTheDocument();

            fireEvent.click(screen.queryByRole("button"));

            expect(screen.queryByText(/This is inside a Collapsible!/)).not.toBeInTheDocument();
        });

        test("Renders properly in controlled mode (closed)", async () => {
                            
            render(
                <Collapsible   
                    isCollapsed={true}
                    contains={
                        <span>This is inside a Collapsible!</span>
                    }
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is inside a Collapsible!/)).not.toBeInTheDocument();            
        });
        
        test("Renders properly in controlled mode (open)", async () => {
                            
            render(
                <Collapsible   
                    isCollapsed={false}
                    contains={
                        <span>This is inside a Collapsible!</span>
                    }
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is inside a Collapsible!/)).toBeInTheDocument();            
        });

        test("Renders properly with header items", async () => {
                            
            render(
                <Collapsible   
                    headerItems={
                        <span>This is a header!</span>
                    }
                    contains={
                        <span>This is inside a Collapsible!</span>
                    }
                />,
                { wrapper }
            );        
            
            expect(screen.queryByText(/This is a header!/)).toBeInTheDocument();            
        });
    });
};

export default Test;
