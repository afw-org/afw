// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, List) => {

    describe("List tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <List                       
                    contains={[
                        <span>This is inside a List!</span>,
                        <span>This is also inside a List!</span>
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a List!/)).toBeInTheDocument();          
            expect(queryByText(/This is also inside a List!/)).toBeInTheDocument();          
        });
        
    });
};

export default Test;
