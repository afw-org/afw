// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Stepper) => {

    describe("Stepper tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <Stepper       
                    
                />,
                { wrapper }
            );        
                    
            // \fixme validate
        });
        
    });
};

export default Test;
