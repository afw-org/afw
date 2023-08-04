// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {waitFor, fireEvent, render, screen, act} from "@testing-library/react";
import {renderHook} from "@testing-library/react-hooks";
import {withProfiler} from "@afw/test";

import {PropertyTypeDropdown, useModel} from "@afw/react";
import {AfwObject} from "@afw/client";

/* eslint-disable react-hooks/rules-of-hooks */

const Test = (wrapper) => {

    const { result } = renderHook(() => useModel(), { wrapper });
    const model = result.current;    

    test("No props", async () => {

        const PropertyTypeDropdownWithProfiler = withProfiler(PropertyTypeDropdown);
        const {container} = render(
            <PropertyTypeDropdownWithProfiler id="PropertyTypeDropdown" />,
            { wrapper }
        );

        expect(container.firstChild).not.toBeNull();     

    });   
    
    test("All props", async () => {

        const PropertyTypeDropdownWithProfiler = withProfiler(PropertyTypeDropdown);
        const {container} = render(
            <PropertyTypeDropdownWithProfiler 
                id="PropertyTypeDropdown"
                label="Select Property Type"
                description="Select a Property Type from the list."                
                value="description"
                required
            />,
            { wrapper }
        );

        expect(container.firstChild).not.toBeNull();

        await waitFor(() => expect(screen.getByLabelText("Select Property Type")).toBeInTheDocument());        
        await waitFor(() => expect(screen.getByText("Select a Property Type from the list.")).toBeInTheDocument());

    });    

    test("Select an Property Type", async () => {

        const PropertyTypeDropdownWithProfiler = withProfiler(PropertyTypeDropdown);
        const objectTypeObject = new AfwObject({
            model,
            objectType: "_AdaptiveObjectType_",
            objectId: "MyObjectType",
            object: {
                propertyTypes: {
                    description: {
                        dataType: "string"
                    },
                    prop1: {}
                }
            }
        });

        await objectTypeObject.initialize();

        const timeout = (ms) => {
            return new Promise(resolve => setTimeout(resolve, ms));
        };
        
        const onChanged = jest.fn();

        render(
            <PropertyTypeDropdownWithProfiler 
                id="PropertyTypeDropdown"
                label="Select Property Type"
                description="Select a Property Type from the list."
                objectTypeObject={objectTypeObject}         
                value="description"
                onChanged={onChanged}       
            />,
            { wrapper }
        );

        await screen.findByLabelText("Select Property Type");        
        
        const combobox = screen.getByRole("combobox");

        await act(async () => {
            fireEvent.mouseDown(combobox);
            fireEvent.click(combobox);

            // \fixme hack:  wait for icons to load via dynamic import()
            await timeout(1000);

            const options = screen.getAllByRole("option");
            options[1].click();                              
        });
               
        await waitFor(() => expect(onChanged).toHaveBeenCalled());                

    });
};

export default Test;
