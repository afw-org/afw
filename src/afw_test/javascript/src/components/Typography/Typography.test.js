// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Typography) => {

    describe("Typography tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Typography       
                    text="This is inside a Typography!"
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Typography!/)).toBeTruthy();               
        });
        
        [ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" ].forEach(size => {
            test("Renders text size " + size, async () => {
                            
                const {queryByText} = render(
                    <Typography       
                        size={size}
                        text="This is inside a Typography!"
                    />,
                    { wrapper }
                );        
                
                expect(queryByText(/This is inside a Typography!/)).toBeTruthy();               
            });
        });
        
        test("Renders text with required emphasis", async () => {
                            
            const {queryByText} = render(
                <Typography       
                    text="This is inside a Typography!"
                    required={true}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Typography!/)).toBeTruthy();               
        });
    });
};

export default Test;
