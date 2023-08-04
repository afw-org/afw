// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, ListEditor) => {

    describe("ListEditor tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <ListEditor       
                    items={[
                        {
                            text: "This is inside a ListEditor!"
                        },
                        {
                            text: "This is also inside a ListEditor!"
                        }
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a ListEditor!/)).toBeTruthy();               
        });
        
    });
};

export default Test;
