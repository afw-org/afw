// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, TimeDuration) => {

    describe("TimeDuration tests", () => {

        test("Renders properly", async () => {
                            
            const {queryByText} = render(
                <TimeDuration       
                    label="This is a TimeDuration!"
                    description="This is a description"
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a TimeDuration!/)).toBeInTheDocument();               
            expect(queryByText(/This is a description!/)).toBeInTheDocument();     
        });
        
    });
};

export default Test;
