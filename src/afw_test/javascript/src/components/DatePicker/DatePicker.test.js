// See the 'COPYING' file in the project root for licensing information.
import {render, screen} from "@testing-library/react";

const Test = (wrapper, DatePicker) => {

    describe("DatePicker tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <DatePicker       
                    id="DatePicker"
                    label="Date Picker"
                    description="This is a description"
                />,
                { wrapper }
            );        
           
            expect(screen.queryByLabelText("Date Picker")).toBeInTheDocument(); 
        });
        
    });
};

export default Test;
