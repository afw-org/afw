// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent} from "@testing-library/react";

const Test = (wrapper, Slider) => {

    describe("Slider tests", () => {

        test("Renders properly", async () => {
                            
            const {queryByText} = render(
                <Slider       
                    label="This is a Slider!"
                    min={10}
                    max={100}
                    step={10}
                    value={50}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a Slider!/)).toBeInTheDocument();               
        });
        
        test("Slide to a value", async () => {

            const onChanged = jest.fn();
            const {queryByLabelText} = render(
                <Slider      
                    label="Slider"
                    min={10}
                    max={100}
                    step={10}
                    value={50}
                    onChanged={onChanged}
                />,
                { wrapper }
            );   
            
            const slider = queryByLabelText("Slider");
            slider.focus();

            fireEvent.keyDown(document.activeElement, { key: "ArrowUp" });
            //expect(onChanged).toHaveBeenLastCalledWith(60);

        });

        test("Slide passed max value", async () => {

            const onChanged = jest.fn();
            const {queryByLabelText} = render(
                <Slider      
                    label="Slider"
                    min={10}
                    max={100}
                    step={10}
                    value={100}
                    onChanged={onChanged}
                />,
                { wrapper }
            );   
            
            const slider = queryByLabelText("Slider");
            slider.focus();

            fireEvent.keyDown(document.activeElement, { key: "ArrowUp" });
            //expect(onChanged).toHaveBeenLastCalledWith(100);
        });

        test("Slide passed min value", async () => {

            const onChanged = jest.fn();
            const {queryByLabelText} = render(
                <Slider      
                    label="Slider"
                    min={10}
                    max={100}
                    step={10}
                    value={10}
                    onChanged={onChanged}
                />,
                { wrapper }
            );   
            
            const slider = queryByLabelText("Slider");
            slider.focus();

            fireEvent.keyDown(document.activeElement, { key: "ArrowDown" });
            //expect(onChanged).toHaveBeenLastCalledWith(10);
        });

        test("Slide with disabled slider", async () => {

            const onChanged = jest.fn();
            const {queryByLabelText} = render(
                <Slider      
                    label="Slider"
                    min={10}
                    max={100}
                    step={10}
                    value={100}
                    onChanged={onChanged}
                    disabled
                />,
                { wrapper }
            );   
            
            const slider = queryByLabelText("Slider");
            slider.focus();

            fireEvent.keyDown(document.activeElement, { key: "ArrowUp" });
            //expect(onChanged).not.toHaveBeenCalled();            
        });
    });
};

export default Test;
