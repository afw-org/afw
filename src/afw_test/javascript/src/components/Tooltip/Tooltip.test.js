// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Tooltip) => {

    describe("Tooltip tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <Tooltip       
                    
                />,
                { wrapper }
            );        
                      
            // \fixme validate
        });
        
    });
};

export default Test;
