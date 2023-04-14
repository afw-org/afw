// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Responsive) => {

    describe("Responsive tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Responsive       
                    rows={[
                        {
                            columns: [
                                {
                                    contains:
                                        <span>This is inside a Responsive layout!</span>
                                }
                            ]
                        }                                            
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Responsive layout!/)).toBeInTheDocument();               
        });
        
    });
};

export default Test;
