// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, DatePicker) => {

    describe("Native tests", () => {

        test("Renders properly", async () => {                            
            render(
                <DatePicker       
                />,
                { wrapper }
            );                   
        });
        
    });
};

export default Test;
