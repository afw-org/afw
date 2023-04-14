// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, DropdownEditor) => {

    describe("DropdownEditor tests", () => {

        test("Renders properly with text inside", async () => {
                            
            render(
                <DropdownEditor
                    label="Dropdown Editor"
                    description="This is a dropdown editor"       
                    options={[]}
                />,
                { wrapper }
            );        
                       
            // \fixme validate
        });
        
    });
};

export default Test;
