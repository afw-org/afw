// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, CodeEditor) => {

    describe("CodeEditor tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <CodeEditor       
                    source="This is inside a CodeEditor!\n\nThis is some more text."
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a CodeEditor!/)).toBeTruthy();               
        });
        
    });
};

export default Test;
