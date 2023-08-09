// See the 'COPYING' file in the project root for licensing information.
import {fireEvent, render, screen, waitFor} from "@testing-library/react";
import {renderHook} from "@testing-library/react-hooks";
import {withProfiler} from "@afw/test";

import {ValueLayout, useModel} from "@afw/react";
import {AfwObject, AfwProperty, AfwValue} from "@afw/client";
import {OperationalContext, AdaptiveLayoutContext} from "@afw/react";

import objectTypeObject from "../../__mocks__/objectTypeObject.json";

/* eslint-disable react-hooks/rules-of-hooks */

const Test = (wrapper) => {
    const { result } = renderHook(() => useModel(), { wrapper });    
    const model = result.current;        
    
    describe("General Tests", () => {    
        
        test("Renders minimal", async () => {            
            const ValueLayoutWithProfiler = withProfiler(ValueLayout);
            render(                
                <ValueLayoutWithProfiler />,                
                { wrapper }
            );                              
        });

        test.skip("Renders minimal object", async () => {
            const ValueLayoutWithProfiler = withProfiler(ValueLayout);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {},
                object: {}
            });

            await object.initialize();
                   
            render(
                <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                    <ValueLayoutWithProfiler />
                </AdaptiveLayoutContext.Provider>,
                { wrapper }
            );      
        });

        test("Renders minimal property", async () => {
            const ValueLayoutWithProfiler = withProfiler(ValueLayout);
            const property = new AfwProperty({                 
                model,
                propertyType: {}                
            });
                   
            render(
                <AdaptiveLayoutContext.Provider value={{ data: { property } }}>
                    <ValueLayoutWithProfiler />
                </AdaptiveLayoutContext.Provider>,
                { wrapper }
            ); 
        });

        test("Renders minimal value", async () => {
            const ValueLayoutWithProfiler = withProfiler(ValueLayout);
            const value = new AfwValue({                 
                model,
                propertyType: {}                
            });
                   
            render(
                <AdaptiveLayoutContext.Provider value={{ data: { value } }}>
                    <ValueLayoutWithProfiler />
                </AdaptiveLayoutContext.Provider>,
                { wrapper }
            ); 
        });

    });

    describe("Tests without Adaptive data", () => {
                
        describe("Not editable", () => {

            Object.entries(objectTypeObject.propertyTypes).forEach(([dataType, propertyType]) => {
        
                if (dataType === "object")
                    return;
    
                const {testValues} = propertyType;
    
                test("dataType=" + dataType, async () => {
                    const ValueLayoutWithProfiler = withProfiler(ValueLayout);
    
                    render(                
                        <ValueLayoutWithProfiler valueMeta={propertyType} value={testValues[0]} />,
                        { wrapper }
                    ); 
                });
            });       
        });

        describe("Editable", () => {
            
            const skipDataTypes = [
                "object", "array", "time", "date", "dateTime"
            ];

            Object.entries(objectTypeObject.propertyTypes).forEach(([dataType, propertyType]) => {
        
                // \fixme skip these for now
                if (skipDataTypes.includes(dataType))
                    return;
    
                const {testValues, expectedTestValues, label, brief} = propertyType;
    
                test("dataType=" + dataType, async () => {
    
                    const ValueLayoutWithProfiler = withProfiler(ValueLayout);
                    render(    
                        <OperationalContext.Provider value={{ editable: true }}>
                            <ValueLayoutWithProfiler valueMeta={propertyType} value={testValues[0]} />
                        </OperationalContext.Provider>,
                        { wrapper }
                    ); 

                    await waitFor(() => expect(screen.getByDisplayValue(expectedTestValues[0])).toBeInTheDocument());                    

                });                
    
                test("dataType=" + dataType + " contains labels, values and briefs", async () => {
    
                    const ValueLayoutWithProfiler = withProfiler(ValueLayout);
                    render(    
                        <OperationalContext.Provider value={{ editable: true }}>
                            <ValueLayoutWithProfiler id={dataType} valueMeta={propertyType} value={testValues[0]} />
                        </OperationalContext.Provider>,
                        { wrapper }
                    ); 

                    if (testValues.length > 1) {                        
                        await waitFor(() => expect(screen.getByDisplayValue(expectedTestValues[0])).toBeInTheDocument());
                        await waitFor(() => expect(screen.getByText(brief)).toBeInTheDocument());
                        await waitFor(() => expect(screen.getByLabelText(label).value).toBe(expectedTestValues[0]));
                    }

                });
                
                test("dataType=" + dataType + " calls onChanged", async () => {

                    const ValueLayoutWithProfiler = withProfiler(ValueLayout);
                    const onChanged = jest.fn();
    
                    render(    
                        <OperationalContext.Provider value={{ editable: true }}>
                            <ValueLayoutWithProfiler 
                                id={dataType} 
                                valueMeta={propertyType} 
                                value={testValues[0]} 
                                onChanged={onChanged}
                            />
                        </OperationalContext.Provider>,
                        { wrapper }
                    ); 

                    if (testValues.length > 1) {                                                
                        await waitFor(() => expect(screen.getByDisplayValue(expectedTestValues[0])).toBeInTheDocument());
                        await waitFor(() => expect(screen.getByText(brief)).toBeInTheDocument());
                        await waitFor(() => expect(screen.getByLabelText(label).value).toBe(expectedTestValues[0]));

                        const input = screen.getByLabelText(label);
                        if (input.type === "checkbox")
                            fireEvent.click(input);
                        else
                            fireEvent.change(input, { target: { value: expectedTestValues[1] } });

                        await waitFor(() => expect(onChanged).toHaveBeenLastCalledWith( testValues[1] ));
                    }

                });
                
            });       
        });

    });
};

export default Test;
