// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render} from "@testing-library/react";

const Test = (wrapper, Breadcrumb) => {

    describe("Breadcrumb tests", () => {

        test("Renders properly", async () => {
                            
            const {queryByText} = render(
                <Breadcrumb       
                    items={[
                        {
                            key: "1",
                            text: "This is a Breadcrumb!",
                            link: "/1",
                        },
                        {
                            key: "2",
                            text: "Item 2",
                            link: "/2",
                        }
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a Breadcrumb!/)).toBeInTheDocument();               
        });
        
    });
};

export default Test;
