// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Snackbar) => {

    describe("Snackbar tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <Snackbar       
                    
                />,
                { wrapper }
            );        
                     
            // \fixme validate
        });
        
    });
};

export default Test;
