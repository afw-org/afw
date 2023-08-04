// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Divider) => {

    describe("Divider tests", () => {
        
        test("Renders properly", async () => {                            
            render(
                <Divider />,
                { wrapper }
            );                               
        });
        
    });
};

export default Test;
