// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Tree) => {

    describe("Tree tests", () => {

        test("Renders properly with text inside", async () => {
                            
            render(
                <Tree    
                    children={[
                        {
                            key: "1",
                            label: "node 1"
                        },
                        {
                            key: "2",
                            label: "node 2",
                            children: [
                                {
                                    key: "2a",
                                    label: "node 2a"
                                },
                                {
                                    key: "2b",
                                    label: "node 2b"
                                }
                            ]
                        },
                        {
                            key: "3",
                            label: "node 3"
                        }
                    ]}
                    
                />,
                { wrapper }
            );        

            // \fixme validate
                      
        });
        
    });
};

export default Test;
