// See the 'COPYING' file in the project root for licensing information.
import {render, /*fireEvent*/} from "@testing-library/react";

const Test = (wrapper, SpinButton) => {

    describe("SpinButton tests", () => {

        test("Renders properly", async () => {
                            
            const {queryByText} = render(
                <SpinButton       
                    label="This is a SpinButton!"
                    min={10}
                    max={50}
                    value={25}
                />,
                { wrapper }
            );        
            
            expect(queryByText(/This is a SpinButton!/)).toBeInTheDocument();               
        });
        
        test("Increase value", async () => {
                            
            //const onChanged = jest.fn();
            /*const {queryByLabelText} = */render(
                <SpinButton       
                    label="Spin Button"
                    min={10}
                    max={50}
                    value={25}
                />,
                { wrapper }
            );        
            
            /*
            const btn = queryByLabelText("Spin Button");
            btn.focus();

            fireEvent.keyDown(document.activeElement, { key: "ArrowUp" });
            expect(onChanged).toHaveBeenLastCalledWith(26); 
             */           
        });

        test("Decrease value", async () => {
                            
            //const onChanged = jest.fn();
            /*const {queryByLabelText} = */render(
                <SpinButton       
                    label="Spin Button"
                    min={10}
                    max={50}
                    value={25}
                />,
                { wrapper }
            );        
            
            /*
            const btn = queryByLabelText("Spin Button");
            btn.focus();

            fireEvent.keyDown(document.activeElement, { key: "ArrowDown" });
            expect(onChanged).toHaveBeenLastCalledWith(24);   
             */         
        });
    });
};

export default Test;
