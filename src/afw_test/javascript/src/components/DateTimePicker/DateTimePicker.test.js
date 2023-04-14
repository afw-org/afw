// See the 'COPYING' file in the project root for licensing information.
import {render, screen} from "@testing-library/react";
import userEvent from "@testing-library/user-event";

const Test = (wrapper, DateTimePicker) => {

    describe("DateTimePicker tests", () => {

        test("Renders properly", async () => {

            const onChanged = jest.fn();
                            
            render(
                <DateTimePicker   
                    id="DateTimePicker"    
                    label="DateTime Picker"
                    description="This is a description"
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
           
            expect(screen.queryByLabelText("DateTime Picker")).toBeInTheDocument(); 

            const input = screen.getByLabelText("DateTime Picker");
            
            //input.focus();
            //fireEvent.change(document.activeElement, { target: { value: "08/13/1981 00:00 pm"}});          
            
            userEvent.type(input, "08/13/1981 01:00 pm");            

        });
        
    });
};

export default Test;
