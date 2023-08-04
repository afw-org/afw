// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Skeleton) => {

    describe("Skeleton tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <Skeleton       
                    
                />,
                { wrapper }
            );        
                    
            // \fixme validate
        });
        
    });
};

export default Test;
