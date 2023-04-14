// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Dropdown) => {

    describe("Dropdown tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Dropdown    
                    label="Dropdown"
                    description="This is a dropdown"   
                    selectedKeys={[ "1" ]}
                    options={[
                        {
                            key: "1",
                            text: "This is inside a Dropdown!"
                        },
                        {
                            key: "2",
                            text: "This is also inside a Dropdown!"
                        }
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Dropdown!/)).toBeTruthy();               
        });
        
    });
};

export default Test;
