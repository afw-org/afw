// See the 'COPYING' file in the project root for licensing information.
import {render, screen} from "@testing-library/react";

const Test = (wrapper, TimePicker) => {

    describe("TimePicker tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <TimePicker 
                    id="TimePicker"
                    label="Time Picker"
                    description="This is a description"
                />,
                { wrapper }
            );        
           
            expect(screen.getByLabelText("Time Picker", { selector: "input" })).toBeInTheDocument();
        });
        
    });
};

export default Test;
