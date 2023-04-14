// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, DiffEditor) => {

    describe("DiffEditor tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <DiffEditor       
                    original="This is inside a DiffEditor!\n\nThis is some more text."
                    modified="This is new text!"
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a DiffEditor!/)).toBeTruthy();               
        });
        
    });
};

export default Test;
