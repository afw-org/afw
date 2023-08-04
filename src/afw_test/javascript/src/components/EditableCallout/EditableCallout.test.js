// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, EditableCallout) => {

    describe("EditableCallout tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <EditableCallout       
                    open={true}
                    value="This is inside a EditableCallout!\n\nThis is some more text."
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a EditableCallout!/)).toBeTruthy();               
        });
        
    });
};

export default Test;
