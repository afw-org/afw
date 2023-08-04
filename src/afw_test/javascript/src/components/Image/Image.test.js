// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Image) => {

    describe("Image tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <Image />,
                { wrapper }
            );       
            
            // \fixme validate
        });
        
    });
};

export default Test;
