// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent} from "@testing-library/react";

const Test = (wrapper, ChoiceGroup) => {

    describe("ChoiceGroup tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const {queryByText} = render(
                <ChoiceGroup       
                    label="This is a ChoiceGroup!"
                    description="This is a description"
                    options={[
                        {
                            key: 1,
                            text: "Choice 1"
                        },
                        {
                            key: 2,
                            text: "Choice 2"
                        }
                    ]}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a ChoiceGroup!/)).toBeInTheDocument();               
            expect(queryByText(/This is a description/)).toBeInTheDocument();               
        });
        
        test("Select each option from a ChoiceGroup", async () => {
                            
            const onChanged = jest.fn();
            const {queryByLabelText} = render(
                <ChoiceGroup       
                    label="This is a ChoiceGroup!"
                    description="This is a description"
                    options={[
                        {
                            key: "1",
                            text: "Choice 1"
                        },
                        {
                            key: "2",
                            text: "Choice 2"
                        },
                        {
                            key: "3",
                            text: "Choice 3"
                        }
                    ]}
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
                         
            fireEvent.click(queryByLabelText("Choice 1"));
            expect(onChanged).toHaveBeenLastCalledWith({ key: "1" });

            fireEvent.click(queryByLabelText("Choice 2"));
            expect(onChanged).toHaveBeenLastCalledWith({ key: "2" });

            fireEvent.click(queryByLabelText("Choice 3"));
            expect(onChanged).toHaveBeenLastCalledWith({ key: "3" });
        });

        test("Select a disabled option (doesn't call onChanged)", async () => {
                            
            const onChanged = jest.fn();
            const {queryByLabelText} = render(
                <ChoiceGroup       
                    label="This is a ChoiceGroup!"
                    description="This is a description"
                    options={[
                        {
                            key: "1",
                            text: "Choice 1"
                        },
                        {
                            key: "2",
                            text: "Choice 2",
                            disabled: true
                        },
                        {
                            key: "3",
                            text: "Choice 3"
                        }
                    ]}
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
                         
            fireEvent.click(queryByLabelText("Choice 1"));
            expect(onChanged).toHaveBeenLastCalledWith({ key: "1" });

            queryByLabelText("Choice 2").click();            
            expect(onChanged).not.toHaveBeenLastCalledWith({ key: "2" });

            fireEvent.click(queryByLabelText("Choice 3"));
            expect(onChanged).toHaveBeenLastCalledWith({ key: "3" });
        });
    });
};

export default Test;
