// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {render, fireEvent, screen} from "@testing-library/react";

const Test = (wrapper, Autocomplete) => {

    describe("Autocomplete tests", () => {

        test("Renders properly with text inside", async () => {
                            
            const options = [
                {
                    key: "1",
                    text: "You Complete Me!"
                }
            ];

            render(
                <Autocomplete       
                    label="Autocomplete Label"
                    description="Autocomplete description"
                    value={options[0]}
                    options={options}
                />,
                { wrapper }
            );        
            
            expect(screen.queryByDisplayValue(/You Complete Me!/)).toBeInTheDocument();               
        });

        test("Select an option from a list of 1", async () => {

            const options = [
                {
                    key: "1",
                    text: "You Complete Me!"
                }
            ];

            const onChanged = jest.fn();
            render(
                <Autocomplete       
                    label="Autocomplete Label"
                    description="Autocomplete description"
                    value={options[0]}
                    options={options}
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
            
            expect(screen.queryByDisplayValue(/You Complete Me!/)).toBeInTheDocument();  

            const input = screen.getByLabelText("Autocomplete Label");
            expect(input).toBeInTheDocument();

            input.focus();
            fireEvent.change(document.activeElement, { target: { value: "You Complete Me!" } });
            expect(input.value).toBe("You Complete Me!");

            expect(onChanged).not.toHaveBeenCalled();
        });

        test("Select an option from a list of 2", async () => {
            const options = [
                {
                    key: "1",
                    text: "This is option 1!"
                },
                {
                    key: "2",
                    text: "This is option 2!"
                }
            ];

            const onChanged = jest.fn();
            render(
                <Autocomplete       
                    label="Autocomplete Label"
                    description="Autocomplete description"
                    value={options[0]}
                    options={options}
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
            
            expect(screen.queryByDisplayValue(/This is option 1!/)).toBeInTheDocument();              

            const input = screen.getByLabelText("Autocomplete Label");
            expect(input).toBeInTheDocument();

            input.focus();
            fireEvent.change(document.activeElement, { target: { value: "This is option 2!" } });

            expect(input.value).toBe("This is option 2!");

            input.focus();
            input.blur();

            expect(onChanged).toHaveBeenCalledWith(options[1]);
        });

        test("Select an option from a list of 3", async () => {
            const options = [
                {
                    key: "1",
                    text: "This is option 1!"
                },
                {
                    key: "2",
                    text: "This is option 2!"
                },
                {
                    key: "3",
                    text: "This is option 3!"
                }
            ];

            const onChanged = jest.fn();
            render(
                <Autocomplete       
                    label="Autocomplete Label"
                    description="Autocomplete description"
                    value={options[0]}
                    options={options}
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
            
            expect(screen.queryByDisplayValue(/This is option 1!/)).toBeInTheDocument();              

            const input = screen.getByLabelText("Autocomplete Label");
            expect(input).toBeInTheDocument();

            input.focus();
            fireEvent.change(document.activeElement, { target: { value: "This is option 2!" } });
            expect(input.value).toBe("This is option 2!");

            input.focus();
            input.blur();

            expect(onChanged).toHaveBeenCalledWith(options[1]);
        });

        test("Select each of the options, one after another", async () => {
            const options = [
                {
                    key: "1",
                    text: "This is option 1!"
                },
                {
                    key: "2",
                    text: "This is option 2!"
                },
                {
                    key: "3",
                    text: "This is option 3!"
                },
                {
                    key: "4",
                    text: "This is option 4!"
                }
            ];

            const onChanged = jest.fn();
            render(
                <Autocomplete       
                    label="Autocomplete Label"
                    description="Autocomplete description"
                    value={options[0]}
                    options={options}
                    onChanged={onChanged}
                />,
                { wrapper }
            );        
            
            expect(screen.queryByDisplayValue(/This is option 1!/)).toBeInTheDocument();              

            let input = screen.getByLabelText("Autocomplete Label");
            expect(input).toBeInTheDocument();

            input.focus();
            fireEvent.change(document.activeElement, { target: { value: "This is option 2!" } });
            expect(input.value).toBe("This is option 2!");
            
            input.focus();
            input.blur();

            expect(onChanged).toHaveBeenLastCalledWith(options[1]);            

            input = screen.getByLabelText("Autocomplete Label");
            input.focus();
            fireEvent.change(document.activeElement, { target: { value: "This is option 3!" } });
            expect(input.value).toBe("This is option 3!");

            input.focus();
            input.blur();

            expect(onChanged).toHaveBeenLastCalledWith(options[2]);

            input = screen.getByLabelText("Autocomplete Label");
            input.focus();
            fireEvent.change(document.activeElement, { target: { value: "This is option 4!" } });
            expect(input.value).toBe("This is option 4!");

            input.focus();
            input.blur();

            expect(onChanged).toHaveBeenLastCalledWith(options[3]);
            
            input = screen.getByLabelText("Autocomplete Label");
            input.focus();
            fireEvent.change(document.activeElement, { target: { value: "This is option 2!" } });
            expect(input.value).toBe("This is option 2!");

            input.focus();
            input.blur();

            expect(onChanged).toHaveBeenLastCalledWith(options[1]);
            
            expect(onChanged).toHaveBeenCalledTimes(4);
        });

        test("Create a new option, not in the list", async () => {
            const options = [
                {
                    key: "1",
                    text: "This is option 1!"
                },
                {
                    key: "2",
                    text: "This is option 2!"
                }
            ];

            const onChanged = jest.fn();
            const onCreateOption = jest.fn();

            render(
                <Autocomplete       
                    label="Autocomplete Label"
                    description="Autocomplete description"
                    value={options[0]}
                    options={options}
                    onChanged={onChanged}
                    onCreateOption={onCreateOption}
                />,
                { wrapper }
            );        
            
            expect(screen.queryByDisplayValue(/This is option 1!/)).toBeInTheDocument();              

            const input = screen.getByLabelText("Autocomplete Label");
            expect(input).toBeInTheDocument();

            input.focus();
            fireEvent.change(document.activeElement, { target: { value: "This is new!" } });
            expect(input.value).toBe("This is new!");

            input.focus();
            input.blur();

            expect(onChanged).not.toHaveBeenCalled();
            expect(onCreateOption).toHaveBeenCalledWith("This is new!");
        });

        test("Select an option from a list of 3, using selection by keyboard navigation", async () => {
            const options = [
                {
                    key: "1",
                    text: "This is option 1!"
                },
                {
                    key: "2",
                    text: "This is option 2!"
                },
                {
                    key: "3",
                    text: "This is option 3!"
                }
            ];

            const onChanged = jest.fn();
            render(
                <Autocomplete       
                    label="Autocomplete Label"
                    description="Autocomplete description"
                    value={options[0]}
                    options={options}
                    onChanged={onChanged}                    
                />,
                { wrapper }
            );        
            
            expect(screen.queryByDisplayValue(/This is option 1!/)).toBeInTheDocument();      
            
            const input = screen.getByLabelText("Autocomplete Label");
            expect(input).toBeInTheDocument();
            input.focus();

            fireEvent.keyDown(document.activeElement, { key: "ArrowDown" });
            fireEvent.keyDown(document.activeElement, { key: "ArrowDown" });
            fireEvent.keyDown(document.activeElement, { key: "Enter" });

            input.focus();
            input.blur();

            expect(onChanged).toHaveBeenCalledWith(options[1]);
        });
        
    });
};

export default Test;
