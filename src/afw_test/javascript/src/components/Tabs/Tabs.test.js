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

        /* tabs built inline, as callers do: a new array on every render */
        const makeTabs = (texts, suffix = "") => texts.map(text => ({
            text,
            contains: <div>{"This is inside " + text + suffix}</div>
        }));

        test("A new tabs array with the same tabs keeps the selected tab", async () => {

            const {queryByLabelText, queryByText, rerender} = render(
                <Tabs tabs={makeTabs(["Tab 1", "Tab 2"])} />,
                { wrapper }
            );

            fireEvent.click(queryByLabelText("Tab 2"));
            rerender(<Tabs tabs={makeTabs(["Tab 1", "Tab 2"], " (edited)")} />);

            expect(queryByText("This is inside Tab 2 (edited)")).toBeInTheDocument();
        });

        test("Different tabs show the first one", async () => {

            const {queryByLabelText, queryByText, rerender} = render(
                <Tabs tabs={makeTabs(["Tab 1", "Tab 2"])} />,
                { wrapper }
            );

            fireEvent.click(queryByLabelText("Tab 2"));
            rerender(<Tabs tabs={makeTabs(["Tab A", "Tab B"])} />);

            expect(queryByText("This is inside Tab A")).toBeInTheDocument();
        });

        test("A new selectedTab selects that tab", async () => {

            const {queryByText, rerender} = render(
                <Tabs tabs={makeTabs(["Tab 1", "Tab 2"])} selectedTab={0} />,
                { wrapper }
            );

            rerender(<Tabs tabs={makeTabs(["Tab 1", "Tab 2"])} selectedTab={1} />);

            expect(queryByText("This is inside Tab 2")).toBeInTheDocument();
        });

        test("A selectedTab past the last tab shows the first", async () => {

            const {queryByText} = render(
                <Tabs tabs={makeTabs(["Tab 1", "Tab 2"])} selectedTab={5} />,
                { wrapper }
            );

            expect(queryByText("This is inside Tab 1")).toBeInTheDocument();
        });
    });
};

export default Test;
