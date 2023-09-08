// See the 'COPYING' file in the project root for licensing information.
import {fireEvent, render, waitFor, screen, act} from "@testing-library/react";
import {renderHook} from "@testing-library/react-hooks";
import {withProfiler} from "@afw/test";

import {ObjectResponsive, useModel} from "@afw/react";
import {AfwObject} from "@afw/client";
import {OperationalContext, AdaptiveLayoutContext} from "@afw/react";

import objectTypeObject from "../../../__mocks__/objectTypeObject";

/* eslint-disable react-hooks/rules-of-hooks */

export const TestAddProperties = (wrapper) => {

    const propertyTypes = Object.entries(objectTypeObject.propertyTypes);

    test.each(propertyTypes)(
        "Add property of dataType=%s", 
        async (dataType, propertyType) => {    

            let { result } = renderHook(() => useModel(), { wrapper });
            let model = result.current;        

            const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
            const onChanged = jest.fn();

            const object = new AfwObject({                 
                model,
                objectTypeObject,
                object: {}
            });

            object.addEventListener("onChildChanged", onChanged);
    
            const layoutParameters = {
                selectable: true,
                headerOptions: "all",
                filterOptions: {
                    filterValuedProperties: true
                }
            };

            await object.initialize();
                
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                        <ObjectResponsiveWithProfiler />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }                    
            );

            const {label} = propertyType;
                
            // look for the Add New Property button                
            const addBtn = await screen.findByLabelText("Add New Property");    
                
            // click the Add button                
            fireEvent.click(addBtn);
                                
            const addPropertyMenu = await screen.findByRole("menuitem", { name: label });

            fireEvent.click(addPropertyMenu);
            await waitFor(() => expect(screen.getByRole("dialog")).toBeInTheDocument());

            const ok = screen.getByLabelText("Ok");
                
            fireEvent.click(ok);

            // \fixme onChildChanged or onPropertiesChanged (review AdaptiveEvent)
            await waitFor(() => expect(onChanged).toHaveBeenLastCalledWith( expect.objectContaining({ eventId: "onChildChanged" })));
        }
    );
};

export const TestRemoveProperties = (wrapper) => {

    const propertyTypes = Object.entries(objectTypeObject.propertyTypes);

    test.each(propertyTypes)(
        "Removing property of dataType=%s", 
        async (dataType, propertyType) => {   
            
            let { result } = renderHook(() => useModel(), { wrapper });
            let model = result.current;        
            const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);

            let obj = {
                [dataType] : propertyType.testValues[0]
            };
                         
            const onChanged = jest.fn();

            const object = new AfwObject({                 
                model,
                objectTypeObject,
                object: obj
            });

            object.addEventListener("onPropertiesChanged", onChanged);                
            //object.addEventListener("onChildChanged", onChanged);
    
            const layoutParameters = {
                selectable: true,
                headerOptions: "all",
                filterOptions: {
                    filterValuedProperties: true
                }
            };

            await object.initialize();
                
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                        <ObjectResponsiveWithProfiler />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );  

            //const {label} = propertyType;
                
            const selectCheck = await screen.findByLabelText("Select Property " + dataType);                
            fireEvent.click(selectCheck);
                
            const removeBtn = await screen.findByLabelText("Remove Property");            
            fireEvent.click(removeBtn);

            // \fixme onChildChanged or onPropertiesChanged (review AdaptiveEvent)
            await waitFor(() => expect(onChanged).toHaveBeenLastCalledWith( expect.objectContaining({ eventId: "onPropertiesChanged" })));

            // verify the change via toJSON()
            const newObj = object.toJSON();
            expect(newObj.dataType).not.toBeDefined();

        }
    );
};

export const TestEditProperties = (wrapper) => {

    const filteredPropertyTypes = Object.entries(objectTypeObject.propertyTypes).filter(([dataType]) => 
        !["array", "object", "time", "date", "dateTime"].includes(dataType)
    );
        
    test.each(filteredPropertyTypes)(
        "Editing property of dataType=%s", 
        async (dataType, propertyType) => { 
            
            let { result } = renderHook(() => useModel(), { wrapper });
            let model = result.current;        

            const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
            
            let obj = {
                [dataType] : propertyType.testValues[0]
            };          
                
            const onChanged = jest.fn();

            const object = new AfwObject({                 
                model,
                objectTypeObject,
                object: obj
            });
                
            object.addEventListener("onChildChanged", onChanged);
    
            const layoutParameters = {
                selectable: true,
                headerOptions: "all",
                filterOptions: {
                    filterValuedProperties: true
                }
            };

            await object.initialize();
                
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                        <ObjectResponsiveWithProfiler />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );  

            const {label, testValues, expectedTestValues} = propertyType;
                                
            const input = await screen.findByLabelText(label);

            await waitFor(() => expect(input.value).toBe(expectedTestValues[0]));                

            if (testValues.length > 1) {                
                if (input.type === "checkbox")
                    fireEvent.click(input);
                else
                    fireEvent.change(input, { target: { value: expectedTestValues[1] } });                

                await waitFor(() => expect(onChanged).toHaveBeenLastCalledWith( expect.objectContaining({ eventId: "onChildChanged" }) ));

                // verify the change via toJSON()
                const newObj = object.toJSON();
                await waitFor(() => expect(newObj[dataType]).toBe(testValues[1]));
            }
        }
    );    
};

export const TestBasic = (wrapper) => {

    test("General Test - Renders properly", async () => {
        
        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {},
            object: {}
        });

        await object.initialize();
                   
        render(
            <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                <ObjectResponsiveWithProfiler />
            </AdaptiveLayoutContext.Provider>,
            { wrapper }
        );    
            
    });

    test("Renders properly with a simple object", async () => {
        
        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {},
            object: {
                prop1: "some value",
                prop2: 1000,
                prop3: true
            }
        });

        await object.initialize();
                   
        render(
            <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                <ObjectResponsiveWithProfiler />
            </AdaptiveLayoutContext.Provider>,
            { wrapper }
        );            
            
        expect(screen.getByText("some value")).toBeInTheDocument();
        expect(screen.getByText("1000")).toBeInTheDocument();
        expect(screen.getByText("true")).toBeInTheDocument();

    });

    test("Renders properly with a simple object and objectType", async () => {
        
        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {
                propertyTypes: {
                    prop1: {
                        dataType: "string",
                        label: "Property 1",
                        brief: "This is property 1",
                    },
                    prop2: {
                        dataType: "integer",
                        label: "Property 2",
                        brief: "This is property 2",
                    },
                    prop3: {
                        dataType: "boolean",
                        label: "Property 3",
                        brief: "This is property 3",
                    }
                }
            },
            object: {
                prop1: "some value",
                prop2: 1000,
                prop3: true
            }
        });

        await object.initialize();
                   
        render(
            <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                <ObjectResponsiveWithProfiler />
            </AdaptiveLayoutContext.Provider>,
            { wrapper }
        );              

        expect(screen.getByText("Property 1")).toBeInTheDocument();
        expect(screen.getByText("Property 2")).toBeInTheDocument();
        expect(screen.getByText("Property 3")).toBeInTheDocument();

        expect(screen.getByText("some value")).toBeInTheDocument();
        expect(screen.getByText("1000")).toBeInTheDocument();
        expect(screen.getByText("true")).toBeInTheDocument();

    });

    test("Renders properly, with a simple object and objectType, editable mode", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {
                propertyTypes: {
                    prop1: {
                        dataType: "string",
                        label: "Property 1",
                        brief: "This is property 1",
                    },
                    prop2: {
                        dataType: "integer",
                        label: "Property 2",
                        brief: "This is property 2",
                    },
                    prop3: {
                        dataType: "boolean",
                        label: "Property 3",
                        brief: "This is property 3",
                    }
                }
            },
            object: {
                prop1: "some value",
                prop2: 1000,
                prop3: true
            }
        });

        await object.initialize();
                   
        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );                      

        expect(screen.getByLabelText("Property 1").value).toBe("some value");
        expect(screen.getByLabelText("Property 2").value).toBe("1000");
        expect(screen.getByLabelText("Property 3").value).toBe("true");

        expect(screen.getByText("This is property 1")).toBeInTheDocument();
        expect(screen.getByText("This is property 2")).toBeInTheDocument();
        expect(screen.getByText("This is property 3")).toBeInTheDocument();

    });

    test("Renders every propertyType (non editable mode)", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);

        let obj = {};
        for (const [propertyName, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
            obj[ propertyName ] = propertyType.testValues[0];
        }

        const object = new AfwObject({                 
            model,
            objectTypeObject,
            object: obj,
        });

        await object.initialize();
                   
        render(                
            <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                <ObjectResponsiveWithProfiler />
            </AdaptiveLayoutContext.Provider>,
            { wrapper }
        ); 
                 
        // skip some dataTypes for now, as they're not well defined in non-editable mode 
        const skip = [
            "array", "object", "time", "date", "dateTime",
            "base64Binary", "password", "template", "script"
        ];
        for (let [dataType, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
            const {label, expectedTestValues} = propertyType;
            
            if (skip.includes(dataType))
                continue;
                
            await waitFor(() => expect(screen.getByText(label)).toBeInTheDocument());
            await waitFor(() => expect(screen.getByText(expectedTestValues[0])).toBeInTheDocument());                
        }

    });
        
    test("Renders every propertyType, editable mode", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const skipDataTypes = [
            "array", "object", "time", "date", "dateTime"
        ];

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);

        let obj = {};
        for (const [propertyName, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
            obj[ propertyName ] = propertyType.testValues[0];
        }

        const object = new AfwObject({                 
            model,
            objectTypeObject,
            object: obj,
        });

        await object.initialize();
                   
        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        ); 
                                    
        for (let [dataType, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
            const {label, brief, expectedTestValues} = propertyType;

            if (skipDataTypes.includes(dataType))
                continue;
                
            const input = screen.getByLabelText(label);                     
                
            await waitFor(() => expect(input.value).toBe(expectedTestValues[0]));
            await waitFor(() => expect(screen.getByText(brief)).toBeInTheDocument());
        }

    });

    test("Changing each property causes object onChildChanged to be called", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const skipDataTypes = [
            "array", "object", "time", "date", "dateTime"
        ];

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);

        let obj = {};
        for (const [propertyName, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
            obj[ propertyName ] = propertyType.testValues[0];
        }

        const object = new AfwObject({                 
            model,
            objectTypeObject,
            object: obj,
        });

        await object.initialize();

        const onChildChanged = jest.fn();
        object.addEventListener("onChildChanged", onChildChanged);
                   
        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );                                     
            
        for (let [dataType, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
            const {label, brief, testValues, expectedTestValues} = propertyType;

            // skip certain dataTypes for now 
            if (skipDataTypes.includes(dataType))
                continue;
                
            const input = screen.getByLabelText(label);                                             
                
            await waitFor(() => expect(input.value).toBe(expectedTestValues[0]));
            await waitFor(() => expect(screen.getByText(brief)).toBeInTheDocument());

            if (testValues.length > 1) {
                if (input.type === "checkbox")
                    fireEvent.click(input);
                else
                    fireEvent.change(input, { target: { value: expectedTestValues[1] } });

                await waitFor(() => expect(onChildChanged).toHaveBeenLastCalledWith( 
                    expect.objectContaining({ eventId: "onChildChanged", value: testValues[1] }) 
                ));
            }
        }
            
    });

    test("Discarding changes to object causes all property values to be reset", async () => {

        const skipDataTypes = [
            "array", "object", "time", "date", "dateTime"
        ];

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);

        let obj = {};
        for (const [propertyName, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
            obj[ propertyName ] = propertyType.testValues[0];
        }

        const object = new AfwObject({                 
            model,
            objectTypeObject,
            object: obj,
        });

        await object.initialize();
                   
        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );                                     
            
        // change each property value 
        for (let [dataType, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
            const {label, testValues, expectedTestValues} = propertyType;

            // skip certain dataTypes for now 
            if (skipDataTypes.includes(dataType))
                continue;
                
            const input = screen.getByLabelText(label);                                             
                
            await waitFor(() => expect(input.value).toBe(expectedTestValues[0]));                

            if (testValues.length > 1) {
                if (input.type === "checkbox")
                    fireEvent.click(input);
                else
                    fireEvent.change(input, { target: { value: expectedTestValues[1] } });
            }
        }
            
        await act(async() => {
            // now, discard the changes
            object.discardChanges();

            await new Promise(resolve => setTimeout(resolve, 2000));

            // verify each property value has been reset
            for (let [dataType, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
                const {label, expectedTestValues} = propertyType;

                // skip certain dataTypes for now 
                if (dataType === "array" || dataType === "object" || dataType === "time" || dataType === "date" || dataType === "dateTime")
                    continue;
                    
                const input = screen.getByLabelText(label);                                             
                    
                await waitFor(() => expect(input.value).toBe(expectedTestValues[0]));                
            }
        });

    });
        
    test("Change a property's name with setName()", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);

        const object = new AfwObject({                 
            model,
            objectTypeObject: {},
            object: {
                prop1: "value1"
            }
        });

        await object.initialize();

        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );   

        expect(screen.getByLabelText("prop1").value).toBe("value1");

        await act(async () => {
            const property = object.getProperty("prop1");
            property.setName("prop2");

            await waitFor(() => expect(screen.getByLabelText("prop2").value).toBe("value1"));
        });

    });
        
    test("Change a property's value with setValue()", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {},
            object: {
                prop1: "value1"
            }
        });

        await object.initialize();

        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );   

        expect(screen.getByLabelText("prop1").value).toBe("value1");

        await act(async () => {
            const property = object.getProperty("prop1");
            property.setValue("value2");

            await waitFor(() => expect(screen.getByLabelText("prop1").value).toBe("value2"));
        });

    });
    

    test("Test layoutParameters, filterOptions, filterProperties", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {},
            object: {
                prop1: "value1",
                prop2: "value2",
            }
        });

        const layoutParameters = {
            filterOptions: {
                filterProperties: [ "prop1" ]
            }
        };

        await object.initialize();

        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );  

        expect(screen.queryByLabelText("prop1")).toBeInTheDocument();
        expect(screen.queryByLabelText("prop2")).not.toBeInTheDocument();

    });
            
    test("Test layoutParameters, filterOptions, filterRequired", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {
                propertyTypes: {
                    prop1: {
                        required: true
                    },
                    prop2: {
                        required: false
                    }
                }
            },
            object: {
                prop1: "value1",
                prop2: "value2",
            }
        });

        const layoutParameters = {
            filterOptions: {
                filterRequiredProperties: true
            }
        };

        await object.initialize();

        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );  
                
        // \fixme the * shouldn't be necessary.  We can use screen.getByRole("textbox", { name: "prop1" }), but it's kinda slow...
        expect(screen.queryByLabelText("prop1 *")).toBeInTheDocument();
        expect(screen.queryByLabelText("prop2")).not.toBeInTheDocument();

    });

    test("Test layoutParameters, filterOptions, filterExcludeProperties", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {},
            object: {
                prop1: "value1",
                prop2: "value2",
            }
        });
    
        const layoutParameters = {
            filterOptions: {
                filterExcludeProperties: [ "prop1" ]
            }
        };
    
        await object.initialize();
    
        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );  
                
        expect(screen.queryByLabelText("prop1")).not.toBeInTheDocument();
        expect(screen.queryByLabelText("prop2")).toBeInTheDocument();

    });         
            
    test("Test layoutParameters, filterOptions, filterValuedProperties=true", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {},
            object: {
                prop1: "value1",
                prop2: "value2",
                prop3: undefined
            }
        });
    
        const layoutParameters = {
            filterOptions: {
                filterValuedProperties: true
            }
        };
    
        await object.initialize();
    
        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );  
                
        expect(screen.queryByLabelText("prop1")).toBeInTheDocument();
        expect(screen.queryByLabelText("prop2")).toBeInTheDocument();
        expect(screen.queryByLabelText("prop3")).not.toBeInTheDocument();

    });     

    test("Test layoutParameters, filterOptions, filterValuedProperties=false", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {
                propertyTypes: {
                    prop3: {
                        dataType: "string"
                    }
                }
            },
            object: {
                prop1: "value1",
                prop2: "value2",
                prop3: undefined
            }
        });
    
        const layoutParameters = {
            filterOptions: {
                filterValuedProperties: false
            }
        };
    
        await object.initialize();
    
        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );  
                
        expect(screen.queryByLabelText("prop1")).toBeInTheDocument();
        expect(screen.queryByLabelText("prop2")).toBeInTheDocument();
        expect(screen.queryByLabelText("prop3")).toBeInTheDocument();

    });  

    test("Test layoutParameters, filterOptions, filterExcludeEmbeddedProperties", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {                    
            },
            object: {
                prop1: "value1",
                prop2: "value2",
                prop3: {
                    prop4: "value4"
                }
            }
        });
    
        const layoutParameters = {
            filterOptions: {
                filterExcludeEmbeddedProperties: true
            }
        };
    
        await object.initialize();
    
        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );  
                
        expect(screen.queryByLabelText("prop1")).toBeInTheDocument();
        expect(screen.queryByLabelText("prop2")).toBeInTheDocument();
        expect(screen.queryByLabelText("prop3")).not.toBeInTheDocument();
        expect(screen.queryByLabelText("prop4")).not.toBeInTheDocument();

    });         
  

    // not sure how to test DOM order yet.. 
    //test("Test sortOptions", async () => {
    //  
    //
    //});
        

    test("Test layoutParameters, showMeta", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {        
                propertyTypes: {
                    prop1: {
                        dataType: "string"
                    },
                    prop2: {
                        dataType: "string"
                    },
                }            
            },
            object: {
                prop1: "value1",
                prop2: "value2",
            }
        });

        const layoutParameters = {
            showMeta: true
        };

        await object.initialize();

        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );  

        expect(screen.getByText("Object Metadata")).toBeInTheDocument();
        expect(screen.getByLabelText("prop1")).toBeInTheDocument();
        expect(screen.getByLabelText("prop2")).toBeInTheDocument();    
            
    });

    test("Test layoutParameters, selectable", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {        
                propertyTypes: {
                    prop1: {
                        dataType: "string"
                    },
                    prop2: {
                        dataType: "string"
                    },
                }            
            },
            object: {
                prop1: "value1",
                prop2: "value2",
            }
        });

        const layoutParameters = {
            selectable: true,
            headerOptions: "all"
        };

        await object.initialize();

        render(
            <OperationalContext.Provider value={{ editable: true }}>
                <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                    <ObjectResponsiveWithProfiler />
                </AdaptiveLayoutContext.Provider>
            </OperationalContext.Provider>,
            { wrapper }
        );  

        expect(screen.getByLabelText("prop1")).toBeInTheDocument();
        expect(screen.getByLabelText("prop2")).toBeInTheDocument();     
        expect(screen.getByLabelText("Select Property prop1")).toBeInTheDocument();
        expect(screen.getByLabelText("Select Property prop2")).toBeInTheDocument();       
            
    });    

    test("Render Embedded Object", async () => {

        let { result } = renderHook(() => useModel(), { wrapper });
        let model = result.current;        

        const ObjectResponsiveWithProfiler = withProfiler(ObjectResponsive);
        const object = new AfwObject({                 
            model,
            objectTypeObject: {
                propertyTypes: {
                    prop1: {
                        dataType: "object",
                        label: "Property 1",
                        brief: "This is property 1",
                        description: "This is property 1 description."
                    }
                }
            },
            object: {
                prop1: {
                    prop2: "string",
                    prop3: 1000,
                    prop4: true
                }
            }
        });

        await object.initialize();
                   
        render(
            <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters: { isCollapsed: false } }}>
                <ObjectResponsiveWithProfiler />
            </AdaptiveLayoutContext.Provider>,
            { wrapper }
        );    
            
        expect(screen.getByLabelText("Property 1")).toBeInTheDocument();
        expect(screen.getByText("This is property 1")).toBeInTheDocument();

        expect(screen.getByLabelText("prop2")).toHaveTextContent("string");
        expect(screen.getByLabelText("prop3")).toHaveTextContent("1000");  
        expect(screen.getByLabelText("prop4")).toHaveTextContent("true");  
            
    });   

};

export const Test = (wrapper) => {           
        
    TestBasic(wrapper);
    TestAddProperties(wrapper);
    TestRemoveProperties(wrapper);
    TestEditProperties(wrapper);    

};


Test.basic = TestBasic;
Test.addProperties = TestAddProperties;
Test.removeProperties = TestRemoveProperties;
Test.editProperties = TestEditProperties;

export default Test;
