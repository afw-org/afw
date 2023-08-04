// See the 'COPYING' file in the project root for licensing information.
import {render} from "@testing-library/react";

const Test = (wrapper, Spinner) => {

    describe("Spinner tests", () => {

        test("Renders properly", async () => {
                            
            render(
                <Spinner       
                    label="This is spinning"
                />,
                { wrapper }
            );        
                      
            // \fixme expect(screen.queryByLabelText("This is spinning")).toBeInTheDocument();
        });

        test("Renders fullScreen", async () => {
            render(
                <Spinner       
                    label="This is spinning"
                    fullScreen
                />,
                { wrapper }
            );        
                      
            // \fixme expect(screen.queryByLabelText("This is spinning")).toBeInTheDocument();
        });
        
    });
};

export default Test;
