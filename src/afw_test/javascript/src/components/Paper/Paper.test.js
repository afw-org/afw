// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Paper) => {

    describe("Paper tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Paper            
                    contains={
                        <span>This is inside a Paper!</span>                   
                    }
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a Paper!/)).toBeInTheDocument();               
        });
        
    });
};

export default Test;
