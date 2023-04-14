// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, TimezonePicker) => {

    describe("TimezonePicker tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <TimezonePicker />,
                { wrapper }
            );        
                      
            // \fixme validate
        });
        
    });
};

export default Test;
