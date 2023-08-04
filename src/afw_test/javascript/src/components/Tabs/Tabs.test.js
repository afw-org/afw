// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent} from "@testing-library/react";

const Test = (wrapper, Tabs) => {

    describe("Tabs tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <Tabs                           
                    tabs={[
                        {
                            text: "Tab 1",
                            contains: <div>This is inside tab 1</div>
                        },
                        {
                            text: "Tab 2",
                            contains: <div>This is inside tab 2</div>
                        }
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is inside tab 1/)).toBeInTheDocument();               
        });
        
        test("Click tab to change view", async () => {
                            
            const {queryByLabelText, queryByText} = render(
                <Tabs                           
                    tabs={[
                        {
                            text: "Tab 1",
                            contains: <div>This is inside tab 1</div>
                        },
                        {
                            text: "Tab 2",
                            contains: <div>This is inside tab 2</div>
                        }
                    ]}
                />,
                { wrapper }
            );        

            fireEvent.click(queryByLabelText("Tab 2"));
            
            expect(queryByText(/This is inside tab 2/)).toBeInTheDocument();               
        });
    });
};

export default Test;
