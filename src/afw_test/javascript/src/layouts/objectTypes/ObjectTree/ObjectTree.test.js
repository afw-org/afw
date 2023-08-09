// See the 'COPYING' file in the project root for licensing information.
import {render, waitFor, fireEvent, screen} from "@testing-library/react";
import {renderHook} from "@testing-library/react-hooks";
import {withProfiler} from "@afw/test";

import {ObjectTree, useModel} from "@afw/react";
import {AfwObject} from "@afw/client";
import {OperationalContext} from "@afw/react";

import objectTypeObject from "../../../__mocks__/objectTypeObject";

/* eslint-disable react-hooks/rules-of-hooks */

const Test = (wrapper) => {
    
    const { result } = renderHook(() => useModel(), { wrapper });
    const model = result.current;    

    describe("General tests", () => {        

        test("Renders properly", async () => {

            const ObjectTreeWithProfiler = withProfiler(ObjectTree);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {},
                object: {}
            });

            await object.initialize();
                   
            render(                
                <ObjectTreeWithProfiler object={object} />,                
                { wrapper }
            );     
            
        });

        test("Renders properly with a simple object", async () => {

            const ObjectTreeWithProfiler = withProfiler(ObjectTree);
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
                <ObjectTreeWithProfiler object={object} />,
                { wrapper }
            );            
            
            await waitFor(() => expect(screen.getByText("prop1")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByText("prop2")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByText("prop3")).toBeInTheDocument());

        });
        
        
        test("Renders properly with a simple object and objectType", async () => {

            const ObjectTreeWithProfiler = withProfiler(ObjectTree);
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
                <ObjectTreeWithProfiler object={object} />,
                { wrapper }
            );              

            await waitFor(() => expect(screen.getByText("Property 1")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByText("Property 2")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByText("Property 3")).toBeInTheDocument());


            fireEvent.click(screen.getByText("Property 1"));
            await waitFor(() => expect(screen.getByText("some value")).toBeInTheDocument());


            fireEvent.click(screen.getByText("Property 2"));
            await waitFor(() => expect(screen.getByText("1000")).toBeInTheDocument());


            fireEvent.click(screen.getByText("Property 3"));
            await waitFor(() => expect(screen.getByText("true")).toBeInTheDocument());            

        });
                
        test("Renders properly, with a simple object and objectType, editable mode", async () => {

            const ObjectTreeWithProfiler = withProfiler(ObjectTree);
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
                    <ObjectTreeWithProfiler object={object} />                
                </OperationalContext.Provider>,
                { wrapper }
            );                      

            await waitFor(() => expect(screen.getByText("Property 1")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByText("Property 2")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByText("Property 3")).toBeInTheDocument());


            fireEvent.click(screen.getByText("Property 1"));
            await waitFor(() => expect(screen.getByLabelText("Property 1").value).toBe("some value"));


            fireEvent.click(screen.getByText("Property 2"));
            await waitFor(() => expect(screen.getByLabelText("Property 2").value).toBe("1000"));


            fireEvent.click(screen.getByText("Property 3"));
            await waitFor(() => expect(screen.getByLabelText("Property 3").value).toBe("true"));         

        });
                
        test("Renders every propertyType (non editable mode)", async () => {

            const ObjectTreeWithProfiler = withProfiler(ObjectTree);

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
                <ObjectTreeWithProfiler object={object} />,
                { wrapper }
            ); 
                                    
            for (let [dataType, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
                const {expectedTestValues, label} = propertyType;

                // skip some dataTypes for now, as they're not well defined in non-editable mode 
                if (dataType === "array" || dataType === "object" || dataType === "time" || dataType === "base64Binary" || 
                    dataType === "date" || dataType === "dateTime" || dataType === "hybrid" || dataType === "password" )
                    continue;

                if (dataType === "null")
                    continue;
                
                await waitFor(() => expect(screen.getByText(label)).toBeInTheDocument());

                fireEvent.click(screen.getByText(label));
                await waitFor(() => expect(screen.getByText(expectedTestValues[0])).toBeInTheDocument());                    
            }

        });
                
        
        test("Renders every propertyType, editable mode", async () => {

            const ObjectTreeWithProfiler = withProfiler(ObjectTree);
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
                    <ObjectTreeWithProfiler object={object} />                    
                </OperationalContext.Provider>,
                { wrapper }
            ); 

            for (let [dataType, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
                const {expectedTestValues, label} = propertyType;

                // skip some dataTypes for now, as they're not well defined in non-editable mode 
                if (dataType === "array" || dataType === "object" || dataType === "time" || dataType === "base64Binary" || 
                    dataType === "date" || dataType === "dateTime" || dataType === "hybrid" || dataType === "password" )
                    continue;

                if (dataType === "null")
                    continue;
                
                await waitFor(() => expect(screen.getByText(label)).toBeInTheDocument());

                fireEvent.click(screen.getByText(label));
                await waitFor(() => expect(screen.getByLabelText(label).value).toBe(expectedTestValues[0]));                    
            }
                                               

        });

        /*
        test("Changing each property causes object onChildChanged to be called", async () => {

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
                   
            const {container} = render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                        <ObjectTreeWithProfiler />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );                                     
            
            for (let [dataType, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
                const {label, brief, testValues, expectedTestValues} = propertyType;

                // skip certain dataTypes for now 
                if (dataType === "array" || dataType === "object" || dataType === "time" || dataType === "date")
                    continue;
                
                const input = screen.getByLabelText(label);                                             
                
                await waitFor(() => expect(input.value).toBe(expectedTestValues[0]));
                await waitFor(() => expect(screen.getByText(brief)).toBeInTheDocument());

                if (testValues.length > 1) {
                    if (input.type === "checkbox")
                        fireEvent.click(input);
                    else
                        fireEvent.change(input, { target: { value: expectedTestValues[1] } });

                    expect(onChildChanged).toHaveBeenLastCalledWith( 
                        expect.objectContaining({ eventId: "onChildChanged", value: testValues[1] }) 
                    );
                }
            }
            
        });

        test("Discarding changes to object causes all property values to be reset", async () => {

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
                   
            const {container} = render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                        <ObjectTreeWithProfiler />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );                                     
            
            // change each property value 
            for (let [dataType, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
                const {label, testValues, expectedTestValues} = propertyType;

                // skip certain dataTypes for now 
                if (dataType === "array" || dataType === "object" || dataType === "time" || dataType === "date")
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
                    if (dataType === "array" || dataType === "object" || dataType === "time" || dataType === "date")
                        continue;
                    
                    const input = screen.getByLabelText(label);                                             
                    
                    await waitFor(() => expect(input.value).toBe(expectedTestValues[0]));                
                }
            });

        });
        
        test("Change a property's name with setName()", async () => {

            const object = new AfwObject({                 
                model,
                objectTypeObject: {},
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();

            const {container} = render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                        <ObjectTreeWithProfiler />
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

            const object = new AfwObject({                 
                model,
                objectTypeObject: {},
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();

            const {container} = render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { object } }}>
                        <ObjectTreeWithProfiler />
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
    });
    
    describe("Test layoutParameters", () => {

        describe("Test filterOptions", () => {

            test("Test filterProperties", async () => {

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

                const {container} = render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                            <ObjectTreeWithProfiler />
                        </AdaptiveLayoutContext.Provider>
                    </OperationalContext.Provider>,
                    { wrapper }
                );  

                expect(screen.queryByLabelText("prop1")).toBeInTheDocument();
                expect(screen.queryByLabelText("prop2")).not.toBeInTheDocument();

            });
            
            test("Test filterRequired", async () => {

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

                const {container} = render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                            <ObjectTreeWithProfiler />
                        </AdaptiveLayoutContext.Provider>
                    </OperationalContext.Provider>,
                    { wrapper }
                );  
                
                // \fixme the * shouldn't be necessary.  We can use screen.getByRole("textbox", { name: "prop1" }), but it's kinda slow...
                expect(screen.queryByLabelText("prop1 *")).toBeInTheDocument();
                expect(screen.queryByLabelText("prop2")).not.toBeInTheDocument();

            });

            test("Test filterExcludeProperties", async () => {

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
    
                const {container} = render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                            <ObjectTreeWithProfiler />
                        </AdaptiveLayoutContext.Provider>
                    </OperationalContext.Provider>,
                    { wrapper }
                );  
                
                expect(screen.queryByLabelText("prop1")).not.toBeInTheDocument();
                expect(screen.queryByLabelText("prop2")).toBeInTheDocument();

            });         
            
            test("Test filterValuedProperties=true", async () => {

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
    
                const {container} = render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                            <ObjectTreeWithProfiler />
                        </AdaptiveLayoutContext.Provider>
                    </OperationalContext.Provider>,
                    { wrapper }
                );  
                
                expect(screen.queryByLabelText("prop1")).toBeInTheDocument();
                expect(screen.queryByLabelText("prop2")).toBeInTheDocument();
                expect(screen.queryByLabelText("prop3")).not.toBeInTheDocument();

            });     

            test("Test filterValuedProperties=false", async () => {

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
    
                const {container} = render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                            <ObjectTreeWithProfiler />
                        </AdaptiveLayoutContext.Provider>
                    </OperationalContext.Provider>,
                    { wrapper }
                );  
                
                expect(screen.queryByLabelText("prop1")).toBeInTheDocument();
                expect(screen.queryByLabelText("prop2")).toBeInTheDocument();
                expect(screen.queryByLabelText("prop3")).toBeInTheDocument();

            });  

            test("Test filterExcludeEmbeddedProperties", async () => {

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
    
                const {container} = render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                            <ObjectTreeWithProfiler />
                        </AdaptiveLayoutContext.Provider>
                    </OperationalContext.Provider>,
                    { wrapper }
                );  
                
                expect(screen.queryByLabelText("prop1")).toBeInTheDocument();
                expect(screen.queryByLabelText("prop2")).toBeInTheDocument();
                expect(screen.queryByLabelText("prop3")).not.toBeInTheDocument();
                expect(screen.queryByLabelText("prop4")).not.toBeInTheDocument();

            });         

        });    

        // not sure how to test DOM order yet.. 
        //describe("Test sortOptions", () => {
        //  
        //
        //});
        

        test("Test showMeta", async () => {
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

            const {container} = render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                        <ObjectTreeWithProfiler />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );  

            expect(screen.getByText("Object Metadata")).toBeInTheDocument();
            expect(screen.getByLabelText("prop1")).toBeInTheDocument();
            expect(screen.getByLabelText("prop2")).toBeInTheDocument();    
            
        });

        test("Test selectable", async () => {
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

            const {container} = render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                        <ObjectTreeWithProfiler />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );  

            expect(screen.getByLabelText("prop1")).toBeInTheDocument();
            expect(screen.getByLabelText("prop2")).toBeInTheDocument();     
            expect(screen.getByLabelText("Select Property prop1")).toBeInTheDocument();
            expect(screen.getByLabelText("Select Property prop2")).toBeInTheDocument();       
            
        });

    });      

    
    describe("Test adding of property", () => {

        Object.entries(objectTypeObject.propertyTypes).forEach(([dataType, propertyType]) => {

            test("Add property of dataType=" + dataType, async () => {
                
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
                
                const {container} = render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                            <ObjectTreeWithProfiler />
                        </AdaptiveLayoutContext.Provider>
                    </OperationalContext.Provider>,
                    { wrapper }
                );  

                const {label} = propertyType;
                
                // look for the Add New Property button
                await waitFor(() => expect(screen.getByLabelText("Add New Property")).toBeInTheDocument());
                const addBtn = screen.getByLabelText("Add New Property");    
                
                // click the Add button
                fireEvent.click(addBtn);

                await waitFor(() => expect(screen.getByLabelText(label)).toBeInTheDocument());
                const addPropertyMenu = screen.getByLabelText(label);
                
                fireEvent.click(addPropertyMenu);
                await waitFor(() => expect(screen.getByRole("dialog")).toBeInTheDocument());

                const ok = screen.getByLabelText("Ok");
                fireEvent.click(ok);

                // \fixme onChildChanged or onPropertiesChanged (review AdaptiveEvent)
                await waitFor(() => expect(onChanged).toHaveBeenLastCalledWith( expect.objectContaining({ eventId: "onChildChanged" })));   
                
            });

        });
        
    });

    
    describe("Testing Removing Properties", () => {

        Object.entries(objectTypeObject.propertyTypes).forEach(([dataType, propertyType]) => {

            test("Removing property of dataType=" + dataType, async () => {

                let obj = {};
                for (const [propertyName, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
                    obj[ propertyName ] = propertyType.testValues[0];
                }
                
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
                
                const {container} = render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                            <ObjectTreeWithProfiler />
                        </AdaptiveLayoutContext.Provider>
                    </OperationalContext.Provider>,
                    { wrapper }
                );  

                const {label} = propertyType;
                
                await waitFor(() => expect(screen.getByLabelText("Select Property " + label)).toBeInTheDocument());
                const selectCheck = screen.getByLabelText("Select Property " + label);                
                fireEvent.click(selectCheck);

                await waitFor(() => expect(screen.getByLabelText("Remove Property")).toBeInTheDocument());
                const removeBtn = screen.getByLabelText("Remove Property");
                fireEvent.click(removeBtn);

                // \fixme onChildChanged or onPropertiesChanged (review AdaptiveEvent)
                await waitFor(() => expect(onChanged).toHaveBeenLastCalledWith( expect.objectContaining({ eventId: "onPropertiesChanged" })));

                // verify the change via toJSON()
                const newObj = object.toJSON();
                expect(newObj.dataType).not.toBeDefined();
                
            });

        });

    });

    
    describe("Testing Editing Properties", () => {

        Object.entries(objectTypeObject.propertyTypes).forEach(([dataType, propertyType]) => {

            // skip these for now
            if (dataType === "array" || dataType === "object" || dataType === "time" || dataType === "date")
                return;

            test("Editing property of dataType=" + dataType, async () => {

                let obj = {};
                for (const [propertyName, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
                    obj[ propertyName ] = propertyType.testValues[0];
                }
                
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
                
                const {container} = render(
                    <OperationalContext.Provider value={{ editable: true }}>
                        <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters }}>
                            <ObjectTreeWithProfiler />
                        </AdaptiveLayoutContext.Provider>
                    </OperationalContext.Provider>,
                    { wrapper }
                );  

                const {label, testValues, expectedTestValues} = propertyType;
                
                await waitFor(() => screen.getByLabelText(label));
                const input = screen.getByLabelText(label);

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
                
            });

        });

    });

    describe("Testing Embedded Objects", () => {

        test("Render Embedded Object", async () => {
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
                   
            const {container} = render(
                <AdaptiveLayoutContext.Provider value={{ data: { object }, layoutParameters: { isCollapsed: false } }}>
                    <ObjectTreeWithProfiler />
                </AdaptiveLayoutContext.Provider>,
                { wrapper }
            );    
            
            expect(screen.getByLabelText("Property 1")).toBeInTheDocument();
            expect(screen.getByText("This is property 1")).toBeInTheDocument();

            expect(screen.getByLabelText("prop2").textContent).toBe("string");
            expect(screen.getByLabelText("prop3").textContent).toBe("1000");
            expect(screen.getByLabelText("prop4").textContent).toBe("true");     
            
        });
         */
    });    

};

export default Test;
