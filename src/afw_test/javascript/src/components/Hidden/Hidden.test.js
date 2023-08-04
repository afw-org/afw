// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Hidden) => {

    describe("Hidden tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <Hidden       
                    
                />,
                { wrapper }
            );        
                      
            // \fixme validate
        });
        
    });
};

export default Test;
