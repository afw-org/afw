// See the 'COPYING' file in the project root for licensing information.
import {waitFor, render, screen, fireEvent, act} from "@testing-library/react";

import {PropertyResponsive} from "@afw/react";
import {AfwProperty} from "@afw/client";
import {OperationalContext} from "@afw/react";
import {withProfiler, trace} from "@afw/test";

import objectTypeObject from "../../../__mocks__/objectTypeObject";
import { mswPostCallback } from "@afw/test";

/**
 * Tests for dataType=object  
 */
const TestObject = (wrapper) => {

    test("Renders properly with a property name and value", async () => {

        const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
        const property = new AfwProperty({ 
            propertyName: "myprop", 
            propertyType: {
                dataType: "object"
            } 
        });
                
        const {container} = render(                
            <PropertyResponsiveWithProfiler      
                data-testid="PropertyResponsive.object" 
                property={property}
            />,
            { wrapper }
        );           
        
        await waitFor(() => expect(container.firstChild).not.toBeNull());        
    });

    test("Renders properly (editable) with a property name and value", async () => {

        const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
        const property = new AfwProperty({ 
            propertyName: "myprop", 
            propertyType: {
                dataType: "object"
            } 
        });
                    
        const {container} = render(
            <OperationalContext.Provider value={{ editable: true }}>
                <PropertyResponsiveWithProfiler   
                    data-testid="PropertyResponsive.object"    
                    property={property}
                />
            </OperationalContext.Provider>,
            { wrapper }
        );  

        await waitFor(() => expect(container.firstChild).not.toBeNull());
    });
};

/*
 * Tests for dataType=arrat
 */
const TestList = (wrapper) => {    

    test("Renders properly with a property name and value", async () => {

        const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
        const property = new AfwProperty({ 
            propertyName: "myprop",             
            propertyType: {
                dataType: "array"
            } 
        });
                        
        const {container} = render(                
            <PropertyResponsiveWithProfiler       
                property={property}
                data-testid="PropertyResponsive.array"
            />,
            { wrapper }
        );            

        await waitFor(() => expect(container.firstChild).not.toBeNull());        
    });

    test("Renders properly (editable) with a property name and value", async () => {

        const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
        const property = new AfwProperty({ 
            propertyName: "myprop", 
            propertyType: {
                dataType: "array"
            },
            value: [ "This is a list entry" ] 
        });
                        
        const {container} = render(
            <OperationalContext.Provider value={{ editable: true }}>
                <PropertyResponsiveWithProfiler       
                    property={property}
                    data-testid="PropertyResponsive.list"
                />
            </OperationalContext.Provider>,
            { wrapper }
        );     

        await waitFor(() => expect(container.firstChild).not.toBeNull());   
    });

    // Run through every dataType/propertyType
    Object.entries(objectTypeObject.propertyTypes).forEach(([dataType, propertyType]) => {

        // \fixme skip these for now
        if (dataType === "array" || dataType === "object" || dataType === "time" || dataType === "date" || dataType === "dateTime")
            return;

        const {testValues, testEditable, expectedTestValues, expectedReadonlyTestValues} = propertyType;

        describe("Renders lists of dataType=" + dataType, () => {

            test("Renders empty list", async () => {

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
                const property = new AfwProperty({ 
                    propertyName: "myprop", 
                    propertyType: {
                        dataType: "array",
                        dataTypeParameter: dataType
                    },
                });
                                
                const {container} = render(                    
                    <PropertyResponsiveWithProfiler     
                        data-testid={"PropertyResponsive.list." + dataType}  
                        property={property}
                    />,
                    { wrapper }
                );    

                await waitFor(() => expect(container.firstChild).not.toBeNull()); 
            });

            test("Renders array of one", async () => {

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
                const property = new AfwProperty({ 
                    propertyName: "myprop", 
                    propertyType: {
                        dataType: "array",
                        dataTypeParameter: dataType
                    },
                    value: [ testValues[0] ]
                });
                                
                render(
                    <PropertyResponsiveWithProfiler       
                        property={property}
                        data-testid={"PropertyResponsive.array." + dataType}
                    />,
                    { wrapper }
                );    
    
                if (expectedReadonlyTestValues)
                    expect(screen.getByText(expectedReadonlyTestValues[0])).toBeInTheDocument();
                else
                    expect(screen.getByText(expectedTestValues[0])).toBeInTheDocument();
            });

            test("Renders list of two", async () => {

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
                const property = new AfwProperty({ 
                    propertyName: "myprop", 
                    propertyType: {
                        dataType: "array",
                        dataTypeParameter: dataType
                    },
                    value: [ testValues[0], testValues[1] ]
                });
                                
                render(                    
                    <PropertyResponsiveWithProfiler       
                        property={property}
                        data-testid={"PropertyResponsive.list." + dataType}
                    />,
                    { wrapper }
                );    
    
                if (expectedReadonlyTestValues) {                    
                    expect(screen.getByText(expectedReadonlyTestValues[0])).toBeInTheDocument();
                    if (expectedReadonlyTestValues.length > 1)
                        expect(screen.getByText(expectedReadonlyTestValues[1])).toBeInTheDocument();
                } else {
                    expect(screen.getByText(expectedTestValues[0])).toBeInTheDocument();
                    if (expectedTestValues.length > 1)
                        expect(screen.getByText(expectedTestValues[1])).toBeInTheDocument();
                } 
            });

            test("Renders empty list, add one", async () => {

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
                const property = new AfwProperty({ 
                    propertyName: "myprop",                     
                    propertyType: {
                        dataType: "array",
                        dataTypeParameter: dataType,
                        label: "My Prop",
                    },
                });
    
                const onChanged = jest.fn();
                property.addEventListener("onChanged", onChanged);
                property.addEventListener("onChildChanged", onChanged);
                                
                render(    
                    <OperationalContext.Provider value={{ editable: true }}>
                        <PropertyResponsiveWithProfiler       
                            property={property}
                        />
                    </OperationalContext.Provider>,
                    { wrapper }
                );    

                /* \fixme list of nulls doesn't work right */
                if (dataType === "null")
                    return;
    
                // click the add button
                fireEvent.click(screen.getByText("add"));
                        
                // look for the labeled input            
                const input = await screen.findByLabelText("My Prop");
    
                if (input.type === "checkbox") {
                    // click the Checkbox to true
                    fireEvent.click(input);                  
                } else if (testEditable !== false)
                    fireEvent.change(input, { target: { value: expectedTestValues[0] }});
                
                // click the ok button
                fireEvent.click(screen.getByText("Ok"));
                    
                expect(onChanged).toHaveBeenLastCalledWith(expect.objectContaining({ eventId: "onChildChanged", value: testValues[0] }));

                if (dataType === "hybrid")
                    await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());   
                
                if (expectedReadonlyTestValues)
                    await waitFor(() => expect(screen.getAllByText(expectedReadonlyTestValues[0])).toHaveLength(1));
                else
                    await waitFor(() => expect(screen.getAllByText(expectedTestValues[0])).toHaveLength(1));
            });

            test("Renders list of one, add one, make sure property has changed", async () => {

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
                const property = new AfwProperty({ 
                    propertyName: "myprop", 
                    propertyType: {
                        dataType: "array",
                        dataTypeParameter: dataType,
                        label: "My Prop"
                    },
                    value: [ testValues[0] ]
                });
    
                const onChanged = jest.fn();
                property.addEventListener("onChanged", onChanged);
                property.addEventListener("onChildChanged", onChanged);
                                
                render(      
                    <OperationalContext.Provider value={{ editable: true }}>
                        <PropertyResponsiveWithProfiler       
                            property={property}
                        />
                    </OperationalContext.Provider>,
                    { wrapper }
                );               

                /* \fixme list of nulls doesn't work right; list of boolean (false) also doesn't work */
                if (dataType === "null" || dataType === "boolean")
                    return;
    
                if (testValues.length > 1) {
                    // click the add button
                    fireEvent.click(screen.getByText("add"));
                            
                    // look for the labeled input            
                    const input = await screen.findByLabelText("My Prop");
        
                    // change the text content              
                    if (input.type === "checkbox") {
                        // click the Checkbox to true, leave for false
                        if (testValues[1])
                            fireEvent.click(input);                  
                    } else if (testEditable !== false) {
                        fireEvent.change(input, { target: { value: expectedTestValues[1] }});  
                    }
                    
                    // click the ok button
                    fireEvent.click(screen.getByText("Ok"));
        
                    expect(onChanged).toHaveBeenLastCalledWith(expect.objectContaining({ eventId: "onChildChanged", value: testValues[1] }));
                }
                
                if (dataType === "hybrid")
                    await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                
                
                if (expectedReadonlyTestValues) {
                    await waitFor(() => expect(screen.getAllByText(expectedReadonlyTestValues[0])).toHaveLength(1));
                    if (expectedReadonlyTestValues.length > 1)
                        await waitFor(() => expect(screen.getAllByText(expectedReadonlyTestValues[1])).toHaveLength(1));
                } else {
                    await waitFor(() => expect(screen.getAllByText(expectedTestValues[0])).toHaveLength(1));
                    if (expectedTestValues.length > 1)
                        await waitFor(() => expect(screen.getAllByText(expectedTestValues[1])).toHaveLength(1));
                }

            });
        });        
        
    });    
        
};

/**
 * Main entry point for testing AdaptiveProperty component.  It runs down every
 * dataType/dataTypeParameter enumeration and makes sure each test succeeds.
 */
const Test = (wrapper) => {

    /**
     * General Tests
     */
    describe("General tests", () => {       

        test("Renders properly", async () => {
            const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
            const property = new AfwProperty();
                            
            render(
                <PropertyResponsiveWithProfiler property={property} />,
                { wrapper }
            );    
        });

        test("Renders properly with a property name and value, but no propertyType", async () => {
            const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
            const property = new AfwProperty({ propertyName: "myprop", value: "This is a value!" });
                            
            render(
                <PropertyResponsiveWithProfiler property={property} />,
                { wrapper }
            );             
            
            await waitFor(() => expect(screen.getByLabelText("myprop")).toBeInTheDocument()); 
        });
        
        test("Renders properly (editable) with a property name and value, but no propertyType", async () => {
            const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
            const property = new AfwProperty({ propertyName: "myprop", value: "This is a value!" });                            

            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsiveWithProfiler property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );     

            await waitFor(() => expect(screen.getByLabelText("myprop").value).toBe("This is a value!"));
        });

        test("Renders properly (editable) with a property name and value, label, brief and description", async () => {
            const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "This is a value!",
                propertyType: {
                    label: "My Prop",
                    brief: "This is myprop",
                    description: "This is a more detailed description of myprop."
                } 
            });                            

            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsiveWithProfiler property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );     

            await waitFor(() => expect(screen.getByLabelText("My Prop").value).toBe("This is a value!"));
        });

        test("Renders with new property name, when changed via AfwProperty setName()", async () => {
            const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "This is a value!",
            });                            

            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsiveWithProfiler property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );     

            expect(screen.getByLabelText("myprop")).toBeInTheDocument();

            await act(async () => {
                property.setName("newprop");
                await waitFor(() => expect(screen.getByLabelText("newprop")).toBeInTheDocument());
            }); 
        });
    });


    // Run through every dataType/propertyType
    Object.entries(objectTypeObject.propertyTypes).forEach(([dataType, propertyType]) => {

        // \fixme skip these for now
        if (dataType === "array" || dataType === "object" || dataType === "time" || dataType === "date" || dataType === "dateTime")
            return;

        const {testValues, label, brief, expectedTestValues} = propertyType;

        describe("dataType=" + dataType + " tests", () => {

            test("Renders properly with a property name and value", async () => {    

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);            
                const property = new AfwProperty({ 
                    propertyName: "myprop", 
                    value: testValues[0],
                    propertyType: {
                        dataType
                    } 
                });
                                
                const {container} = render(                
                    <PropertyResponsiveWithProfiler property={property} data-testid={"PropertyResponsive." + dataType} />,
                    { wrapper }
                );   

                await waitFor(() => expect(container.firstChild).not.toBeNull());                            
            });

            test("Renders properly (editable) with a property name, value, label, brief and description", async () => {

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
                const property = new AfwProperty({ 
                    propertyName: "myprop", 
                    value: testValues[0],
                    propertyType: {
                        dataType,
                        label,
                        brief,
                        description: "This is a more detailed description."
                    } 
                });
                                
                render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <PropertyResponsiveWithProfiler property={property} />
                    </OperationalContext.Provider>,
                    { wrapper }
                );   

                if (dataType === "hybrid")
                    await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
    
                await waitFor(() => expect(screen.getByLabelText(label)).toBeInTheDocument()); 
            });

            test("Calls onChanged after input changes", async () => {

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
                const property = new AfwProperty({ 
                    propertyName: "myprop", 
                    value: testValues[0],
                    propertyType: {
                        dataType, 
                        label, 
                        brief
                    } 
                });
                       
                const onChanged = jest.fn();
                property.addEventListener("onChanged", onChanged);
    
                render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <PropertyResponsiveWithProfiler property={property} />
                    </OperationalContext.Provider>,
                    { wrapper }
                );   

                if (dataType === "hybrid")
                    await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
    
                const input = screen.getByLabelText(label);
                await waitFor(() => expect(input).toBeInTheDocument());
                await waitFor(() => expect(input.value).toBe(expectedTestValues[0]));
                    
                if (testValues.length > 1) {
                    if (input.type === "checkbox")
                        fireEvent.click(input);      
                    else
                        fireEvent.change(input, { target: { value: expectedTestValues[1] } });            
        
                    expect(onChanged).toHaveBeenCalledTimes(1);
                    expect(onChanged).toHaveBeenCalledWith(
                        expect.objectContaining({ eventId: "onChanged", value: testValues[1] })
                    );         
                }
            });    

            test("Input value resets after changes are discarded", async () => {

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
                const property = new AfwProperty({ 
                    propertyName: "myprop", 
                    value: testValues[0],
                    propertyType: {
                        dataType,
                        label,
                        brief
                    } 
                });
                       
                const onChanged = jest.fn();
                property.addEventListener("onChanged", onChanged);
            
                trace("initial render", () => {
                    render(
                        <OperationalContext.Provider value={{ editable: true }}>                            
                            <PropertyResponsiveWithProfiler property={property} />
                        </OperationalContext.Provider>,
                        { wrapper }
                    );   
                });

                if (dataType === "hybrid")
                    await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
    
                const input = screen.getByLabelText(label);
                await waitFor(() => expect(input).toBeInTheDocument());
                await waitFor(() => expect(input.value).toBe(expectedTestValues[0]));

                if (testValues.length > 1) {
                    trace("change input", () => {
                        if (input.type === "checkbox")
                            fireEvent.click(input);      
                        else
                            fireEvent.change(input, { target: { value: expectedTestValues[1] } });            
                    });
        
                    expect(onChanged).toHaveBeenCalledTimes(1);
                    expect(onChanged).toHaveBeenCalledWith(expect.objectContaining({ eventId: "onChanged", value: testValues[1] }));      

                    await act(async() => {                       
                        trace("discard changes", () => { 
                            property.discardChanges();                        
                        });
                        const input = screen.getByLabelText(label);
                        await waitFor(() => expect(input.value).toBe(expectedTestValues[0]));
                    });   
                }                     
            }); 

            test("Change property name via AfwProperty setName()", async () => {

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
                const property = new AfwProperty({ 
                    propertyName: "myprop", 
                    value: testValues[0],
                    propertyType: {
                        dataType,
                        brief
                    } 
                });                            
    
                render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <PropertyResponsiveWithProfiler property={property} />
                    </OperationalContext.Provider>,
                    { wrapper }
                );     
    
                await waitFor(() => expect(screen.getByLabelText("myprop")).toBeInTheDocument());
    
                await act(async () => {
                    property.setName("newprop");
                    await waitFor(() => expect(screen.getByLabelText("newprop")).toBeInTheDocument());
                });

            });

            test("Change property value via AfwProperty setValue()", async () => {

                const PropertyResponsiveWithProfiler = withProfiler(PropertyResponsive);
                const property = new AfwProperty({ 
                    propertyName: "myprop", 
                    value: testValues[0],
                    propertyType: {
                        dataType,
                        label,
                        brief
                    } 
                });                            
    
                render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <PropertyResponsiveWithProfiler property={property} />
                    </OperationalContext.Provider>,
                    { wrapper }
                );     
    
                expect(screen.getByLabelText(label).value).toBe(expectedTestValues[0]);
    
                if (testValues.length > 1) {                    
                    await act(async () => {
                        property.setValue(testValues[1]);
                        await waitFor(() => expect(screen.getByLabelText(label).value).toBe(expectedTestValues[1]));
                    });
                }

            });
        });
    });

    /**
     * Date Tests
     */
    describe.skip("dataType=date tests", () => {

        test("Renders properly with a property name and value", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "1978-07-31",
                propertyType: {
                    dataType: "date"
                } 
            });
                            
            render(                
                <PropertyResponsive       
                    property={property}
                />,
                { wrapper }
            );                
        });

        test("Renders properly (editable) with a property name, value, brief and description", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "1978-07-31",
                propertyType: {
                    dataType: "date",
                    label: "My Prop",
                    brief: "This is myprop",
                    description: "This is a more detailed description of myprop."
                } 
            });
                            
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsive property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );   

            // use AllBy, because the label may indicate an input field in addition to a picker control 
            screen.getAllByLabelText("My Prop").forEach(e => expect(e).toBeInTheDocument());            
        });

        test("Calls onChanged after input changes", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "1978-07-31",
                propertyType: {
                    dataType: "date"
                } 
            });
                   
            const onChanged = jest.fn();
            property.addEventListener("onChanged", onChanged);

            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsive property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );   

            const input = screen.getByLabelText("myprop", { selector: "input" });
            expect(input).toBeInTheDocument();
            expect(input.value).toBe("1978-07-31");

            fireEvent.change(input, { target: { value: "1981-08-13" } });            

            expect(onChanged).toHaveBeenCalledTimes(1);            
            expect(onChanged).toHaveBeenCalledWith(expect.objectContaining({ eventId: "onChanged", value: "1981-08-13+00:00" }));
        });        

        test("Input value resets after changes are discarded", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "1978-07-31",
                propertyType: {
                    dataType: "date"
                } 
            });
                   
            const onChanged = jest.fn();
            property.addEventListener("onChanged", onChanged);

            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsive property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );   

            const input = screen.getByLabelText("myprop", { selector: "input" });
            expect(input).toBeInTheDocument();
            expect(input.value).toBe("1978-07-31");

            fireEvent.change(input, { target: { value: "1981-08-13" } });            

            expect(onChanged).toHaveBeenCalledTimes(1);
            // \fixme requirements aren't quite clear on what happens when a TZ is omitted or provided 
            expect(onChanged).toHaveBeenCalledWith(expect.objectContaining({ eventId: "onChanged", value: "1981-08-13+00:00" }));            
           
            await act(async() => {
                property.discardChanges();
                await waitFor(() => expect(input.value).toBe("1978-07-31"));
            });           
        }); 

        test("Change property name via AfwProperty setName()", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "1978-07-31",
                propertyType: {
                    dataType: "date"
                } 
            });                            

            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsive property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );     

            expect(screen.getByText("myprop")).toBeInTheDocument();

            await act(async () => {
                property.setName("newprop");
                await waitFor(() => expect(screen.getByText("newprop")).toBeInTheDocument());
            });
        });

        test("Change property value via AfwProperty setValue()", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "1978-07-31",
                propertyType: {
                    dataType: "date"
                } 
            });                            

            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsive property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );     

            expect(screen.getByLabelText("myprop", { selector: "input" }).value).toBe("1978-07-31");

            await act(async () => {
                property.setValue("1981-08-13");
                await waitFor(() => expect(screen.getByLabelText("myprop", { selector: "input" }).value).toBe("1981-08-13"));
            });
        });
    });
    

    /**
     * List Tests
     */
    describe("dataType=array tests", () => {
        TestList(wrapper);
    });
    

    /**
     * Object Tests
     */
    describe("dataType=object tests", () => {
        TestObject(wrapper);
    });
    

    /**
     * Time Tests
     */
    describe("dataType=time tests", () => {

        test("Renders properly with a property name and value", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "12:00",
                propertyType: {
                    dataType: "time"
                } 
            });
                            
            render(                
                <PropertyResponsive       
                    property={property}
                />,
                { wrapper }
            );                
        });

        test("Renders properly (editable) with a property name and value", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "12:00",
                propertyType: {
                    dataType: "time"
                } 
            });
                            
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsive       
                        property={property}
                    />
                </OperationalContext.Provider>,
                { wrapper }
            );     
        });

        /*

        
        \fixme TimePicker has a lot of work to do yet....


        test("Calls onChanged after input changes", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "12:00",
                propertyType: {
                    dataType: "time"
                } 
            });
                   
            const onChanged = jest.fn();
            property.addEventListener("onChanged", onChanged);

            const {screen.getByLabelText} = render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsive property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );   

            const input = screen.getByLabelText("myprop", { selector: "input" });
            expect(input).toBeInTheDocument();
            expect(input.value).toBe("12:00");

            fireEvent.change(input, { target: { value: "09:30 AM" } });            

            expect(onChanged).toHaveBeenCalledTimes(1);
            expect(onChanged).toHaveBeenCalledWith(expect.objectContaining({ eventId: "onChanged", value: "09:30" }));
        });        

        test("Input value resets after changes are discarded", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "12:00",
                propertyType: {
                    dataType: "time"
                } 
            });
                   
            const onChanged = jest.fn();
            property.addEventListener("onChanged", onChanged);

            const {screen.getByLabelText} = render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsive property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );   

            const input = screen.getByLabelText("myprop", { selector: "input" });
            expect(input).toBeInTheDocument();
            expect(input.value).toBe("12:00");

            fireEvent.change(input, { target: { value: "09:30 AM" } });            

            expect(onChanged).toHaveBeenCalledTimes(1);
            expect(onChanged).toHaveBeenCalledWith(expect.objectContaining({ eventId: "onChanged", value: "09:30" }));   
           
            await act(async() => {
                property.discardChanges();
                await waitFor(() => expect(input.value).toBe("12:00 PM"));
            });           
        }); 

        test("Change property name via AfwProperty setName()", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "12:00",
                propertyType: {
                    dataType: "time"
                } 
            });                            

            const {screen.getByLabelText} = render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsive property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );     

            expect(screen.getByLabelText("myprop")).toBeInTheDocument();

            await act(async () => {
                property.setName("newprop");
                await waitFor(() => expect(screen.getByLabelText("newprop")).toBeInTheDocument());
            });
        });        

        test("Change property value via AfwProperty setValue()", async () => {
            const property = new AfwProperty({ 
                propertyName: "myprop", 
                value: "12:00 PM",
                propertyType: {
                    dataType: "time"
                } 
            });                            

            const {screen.getByLabelText, debug} = render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <PropertyResponsive property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );     

            debug();
            expect(screen.getByLabelText("myprop").value).toBe("12:00 PM");

            await act(async () => {
                property.setValue("09:30 AM");
                await waitFor(() => expect(screen.getByLabelText("myprop").value).toBe("09:30 AM"));
            });
        });
         */
    });    
    
};


export default Test;
