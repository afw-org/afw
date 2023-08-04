// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Picker) => {

    describe("Picker tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Picker       
                    label="Picker"
                    description="This is a picker!"
                    items={[
                        {
                            key: "1",
                            label: "This is inside a Picker!"
                        },
                        {
                            key: "2",
                            label: "This is also inside a Picker!"
                        }
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Picker!/)).toBeTruthy();               
        });
        
    });
};

export default Test;
