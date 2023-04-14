// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Chart) => {

    describe("Chart tests", () => {

        test("Renders properly with text inside", async () => {
                            
            render(
                <Chart    
                    data={[
                        {
                            x: "a",
                            y: "1",
                        },
                        {
                            key: "b",
                            text: "2"
                        },
                        {
                            key: "c",
                            text: "3"
                        }
                    ]}
                />,
                { wrapper }
            );        
                       
            // \fixme not sure how to validate this yet
        });
        
    });
};

export default Test;
