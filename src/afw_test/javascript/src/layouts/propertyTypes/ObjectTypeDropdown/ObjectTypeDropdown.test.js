// See the 'COPYING' file in the project root for licensing information.
import {waitFor, fireEvent, render, screen} from "@testing-library/react";
import {withProfiler} from "@afw/test";

import {ObjectTypeDropdown} from "@afw/react";

const Test = (wrapper) => {

    test("No props", async () => {

        const ObjectTypeDropdownWithProfiler = withProfiler(ObjectTypeDropdown);
        render(
            <ObjectTypeDropdownWithProfiler id="ObjectTypeDropdown" />,
            { wrapper }
        );

        await waitFor(() => {
            expect(screen.getByRole("combobox")).not.toBeDisabled();
        });

    });   
    
    test("All props", async () => {

        const ObjectTypeDropdownWithProfiler = withProfiler(ObjectTypeDropdown);
        render(
            <ObjectTypeDropdownWithProfiler 
                id="ObjectTypeDropdown"
                label="Select Object Type"
                description="Select an Object Type from the list."
                adapterId="afw"
                value="_AdaptiveObjectType_"
                required
            />,
            { wrapper }
        );

        
        await screen.findByLabelText("Select Object Type");          
        await waitFor(() => {
            expect(screen.getByRole("combobox", { name: "Select Object Type"})).not.toBeDisabled();
        });


        await waitFor(() => expect(screen.getByLabelText("Select Object Type")).toBeInTheDocument());        
        await waitFor(() => expect(screen.getByText("Select an Object Type from the list.")).toBeInTheDocument());

    });    

    test("Select an Object Type", async () => {
        
        const ObjectTypeDropdownWithProfiler = withProfiler(ObjectTypeDropdown);
        const onChanged = jest.fn();

        render(
            <ObjectTypeDropdownWithProfiler 
                id="ObjectTypeDropdown"
                label="Select Object Type"
                description="Select an Object Type from the list."
                adapterId="afw"         
                value="_AdaptiveObjectType_"
                onChanged={onChanged}       
            />,
            { wrapper }
        );

        await screen.findByLabelText("Select Object Type");                               
                
        let options;
        await waitFor(async () => {
            const combobox = screen.getByRole("combobox");

            fireEvent.mouseDown(combobox);
            fireEvent.click(combobox);

            options = screen.queryAllByRole("option");            
            expect(options).not.toHaveLength(0);
        });

        /* click the second option */
        options[1].click();                        
               
        await waitFor(() => expect(onChanged).toHaveBeenCalled());                

    });
};

export default Test;
