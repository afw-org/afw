// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent} from "@testing-library/react";

const Test = (wrapper, Toggle) => {

    describe("Toggle tests", () => {

        test("Renders properly", async () => {
                            
            const {queryByText} = render(
                <Toggle       
                    label="This is a Toggle!"
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a Toggle!/)).toBeInTheDocument();               
        });
        
        test("Toggles properly", async () => {
                            
            const onChanged = jest.fn();
            const {queryByRole} = render(
                <Toggle       
                    label="This is a Toggle!"
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
            
            fireEvent.click(queryByRole("checkbox"));

            expect(onChanged).toHaveBeenCalled();
        });
    });
};

export default Test;
