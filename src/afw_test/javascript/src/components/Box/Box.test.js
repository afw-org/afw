// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Box) => {

    describe("Box tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Box   
                    contains={[
                        {
                            contains: <span>This is inside a Box!</span>
                        },
                        {
                            contains: <span>This is also inside a Box!</span>
                        }                            
                    ]}
                />,
                { wrapper }                
            );        
            
            expect(queryByText(/This is inside a Box!/)).toBeInTheDocument();            
            expect(queryByText(/This is also inside a Box!/)).toBeInTheDocument();            
            expect(queryByText(/This is not inside the Box!/)).not.toBeInTheDocument();
        });
        
    });
};

export default Test;
