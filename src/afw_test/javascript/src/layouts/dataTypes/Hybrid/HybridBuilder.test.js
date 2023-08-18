// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {waitFor, render, screen, fireEvent} from "@testing-library/react";

import {HybridBuilder, OperationalContext} from "@afw/react";

/**
 * Tests for dataType=object  
 */
const Test = (wrapper) => {

    test("Renders with no props", async () => {
        render(
            <HybridBuilder
            />,
            { wrapper }
        );
    });    

    test("Renders with no props (editable)", async () => {

        render(
            <OperationalContext.Provider value={{ editable: false }}>
                <HybridBuilder
                />
            </OperationalContext.Provider>,
            { wrapper }
        );
    });

    describe("Testing Templates", () => {

        test("Renders properly with a value", async () => {
            render(
                <HybridBuilder
                    hybrid="${var}"
                />,
                { wrapper }
            );
        });

        test("Renders properly with a value (editable)", async () => {
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <HybridBuilder
                        hybrid="${var}"
                    />
                </OperationalContext.Provider>,
                { wrapper }
            );

            await waitFor(() => expect(screen.getByDisplayValue("${var}")).toBeInTheDocument());
        });

        test("Renders properly with a label and value (editable)", async () => {
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <HybridBuilder
                        label="The Template"
                        hybrid="${var}"
                    />
                </OperationalContext.Provider>,
                { wrapper }
            );

            await waitFor(() => expect(screen.getByLabelText("The Template").value).toBe("${var}"));
        });

        test("Changing the template value calls onChanged", async () => {
            
            const onChanged = jest.fn();

            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <HybridBuilder
                        label="The Template"
                        hybrid="${var}"
                        onChanged={onChanged}
                    />
                </OperationalContext.Provider>,
                { wrapper }
            );

            await waitFor(() => expect(screen.getByLabelText("The Template")).toBeInTheDocument());

            const textarea = screen.getByLabelText("The Template");
            fireEvent.change(textarea, { target: { value: "${newVar}" } });

            await waitFor(() => expect(onChanged).toHaveBeenLastCalledWith("${newVar}"));

        });
    });

};

export default Test;
