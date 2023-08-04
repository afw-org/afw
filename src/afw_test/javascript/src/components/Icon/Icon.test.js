// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {act, render} from "@testing-library/react";

const Test = async (wrapper, Icon) => {

    describe("Icon tests", () => {

        test("Renders 'edit' icon properly ", async () => {

            await act( async() => {
                render(
                    <Icon iconName="edit" />,
                    { wrapper }
                );                          
            });  

            // \fixme validate
        });
        
    });
};

export default Test;
