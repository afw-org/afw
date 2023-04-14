// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Link) => {

    describe("Link tests", () => {

        test("Renders properly with a link inside", async () => {
                            
            const {queryByText, queryByRole} = render(
                <Link       
                    text="This is a Link!"
                    url="http://www.google.com"
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a Link!/)).toBeInTheDocument();     
            expect(queryByRole("link")).toBeInTheDocument();
        });
        
    });
};

export default Test;
