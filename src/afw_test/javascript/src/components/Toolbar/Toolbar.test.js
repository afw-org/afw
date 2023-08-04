// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Toolbar) => {

    describe("Toolbar tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Toolbar          
                    contains={[
                        <span>This is inside a Toolbar!</span>,
                        <span>This is also inside a Toolbar!</span>                   
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Toolbar!/)).toBeInTheDocument();      
            expect(queryByText(/This is also inside a Toolbar!/)).toBeInTheDocument();
        });
        
    });
};

export default Test;
