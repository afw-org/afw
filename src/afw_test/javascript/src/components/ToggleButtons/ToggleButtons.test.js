// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent} from "@testing-library/react";

const Test = (wrapper, ToggleButtons) => {

    describe("ToggleButtons tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <ToggleButtons       
                    options={[
                        {
                            label: "button1",
                            value: "button1"
                        },
                        {
                            label: "button2",
                            value: "button2"
                        }
                    ]}
                />,
                { wrapper }
            );             
            
            // \fixme validate
        });

        test("Toggles properly in exclusive mode", async () => {
                            
            const onChanged = jest.fn();
            const {queryByLabelText} = render(
                <ToggleButtons       
                    options={[
                        {
                            label: "button1",
                            value: "button1"
                        },
                        {
                            label: "button2",
                            value: "button2"
                        }
                    ]}
                    onChanged={onChanged}
                    exclusive
                />,
                { wrapper }
            );        
                       
            fireEvent.click(queryByLabelText("button1"));   
            
            expect(onChanged).toHaveBeenLastCalledWith("button1");
        });

        test("Toggles properly in non-exclusive mode", async () => {
                            
            const onChanged = jest.fn();
            const {queryByLabelText} = render(
                <ToggleButtons       
                    options={[
                        {
                            label: "button1",
                            value: "button1"
                        },
                        {
                            label: "button2",
                            value: "button2"
                        }
                    ]}
                    onChanged={onChanged}
                    exclusive={false}
                    value={[ "button1" ]}
                />,
                { wrapper }
            );        
            

            fireEvent.click(queryByLabelText("button2"));   
            expect(onChanged).toHaveBeenLastCalledWith([ "button1", "button2" ]);
            
        });
        
    });
};

export default Test;
