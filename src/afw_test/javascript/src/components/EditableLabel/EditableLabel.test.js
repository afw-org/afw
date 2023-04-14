// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, EditableLabel) => {

    describe("EditableLabel tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <EditableLabel       
                    value="This is inside a EditableLabel!"
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside a EditableLabel!/)).toBeTruthy();               
        });
        
    });
};

export default Test;
