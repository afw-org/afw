// See the 'COPYING' file in the project root for licensing information.
import {fireEvent, render, waitFor, waitForElementToBeRemoved, screen} from "@testing-library/react";
import {renderHook} from "@testing-library/react-hooks";
import {withProfiler} from "@afw/test";

import {ObjectEditor, ObjectResponsive, useModel} from "@afw/react";
import {AfwObject} from "@afw/client";

import objectTypeObject from "../../../__mocks__/objectTypeObject";

/* eslint-disable react-hooks/rules-of-hooks */

const Test = (wrapper) => {
    
    const { result } = renderHook(() => useModel(), { wrapper });
    const model = result.current;    

    describe("General tests", () => {        

        test("Renders properly", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {},
                object: {}
            });

            await object.initialize();
                   
            render(                
                <ObjectEditorWithProfiler object={object} />,
                { wrapper }
            );     
            
        });

        test("Renders a simple object and layout", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {
                    propertyTypes: {
                        prop1: {
                            label: "Prop 1",
                            dataType: "string"
                        }
                    }
                },
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();
                   
            render(                
                <ObjectEditorWithProfiler
                    object={object}     
                    layout={<ObjectResponsive />}                
                />,                
                { wrapper }
            );     


            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());            
            
        });

        test("Switch between layout and source", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {
                    propertyTypes: {
                        prop1: {
                            label: "Prop 1",
                            dataType: "string"
                        }
                    }
                },
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();
                   
            render(                
                <ObjectEditorWithProfiler
                    object={object}     
                    layout={<ObjectResponsive />}                
                />,                
                { wrapper }
            );     

            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());   
            
            const sourceBtn = screen.getByLabelText("Show Object Source");
            sourceBtn.click();

            await waitFor(() => expect(screen.getByDisplayValue(/"prop1": "value1"/)).toBeInTheDocument());      
            
            
            const layoutBtn = screen.getByLabelText("Show Object in Layout");
            layoutBtn.click();

            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());   
            
        });

        test("Edit Object Cancel", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {
                    propertyTypes: {
                        prop1: {
                            label: "Prop 1",
                            dataType: "string"
                        }
                    }
                },
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();

            const onChildChanged = jest.fn();
            object.addEventListener("onChildChanged", onChildChanged);
                   
            render(                
                <ObjectEditorWithProfiler
                    object={object}     
                    layout={<ObjectResponsive />}                
                />,                
                { wrapper }
            );     

            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());   
            
            const editBtn = screen.getByLabelText("Edit Object");
            editBtn.click();

            await waitFor(() => expect(screen.getByLabelText("Prop 1")).toBeInTheDocument());   

            
            const cancelBtn = screen.getByLabelText("Cancel");
            cancelBtn.click();
            
            expect(onChildChanged).not.toHaveBeenCalled();
            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());   
            
        });

        test("Edit Object calls onChildChanged", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {
                    propertyTypes: {
                        prop1: {
                            label: "Prop 1",
                            dataType: "string"
                        }
                    }
                },
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();

            const onChildChanged = jest.fn();
            object.addEventListener("onChildChanged", onChildChanged);
                   
            render(                
                <ObjectEditorWithProfiler
                    object={object}     
                    layout={<ObjectResponsive />}                
                />,                
                { wrapper }
            );     

            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());   
            
            const editBtn = screen.getByLabelText("Edit Object");
            editBtn.click();

            await waitFor(() => expect(screen.getByLabelText("Prop 1")).toBeInTheDocument());        
            
            const prop1 = screen.getByLabelText("Prop 1");
            fireEvent.change(prop1, { target: { value: "value2" }});

            await waitFor(() => expect(onChildChanged).toHaveBeenLastCalledWith(
                expect.objectContaining({ eventId: "onChildChanged", value: "value2" })
            ));            
            
        });

        test("Edit Object changes source", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {
                    propertyTypes: {
                        prop1: {
                            label: "Prop 1",
                            dataType: "string"
                        }
                    }
                },
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();
                   
            render(                
                <ObjectEditorWithProfiler
                    object={object}     
                    layout={<ObjectResponsive />}                
                />,                
                { wrapper }
            );     

            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());   
            
            const editBtn = screen.getByLabelText("Edit Object");
            editBtn.click();

            await waitFor(() => expect(screen.getByLabelText("Prop 1")).toBeInTheDocument());        
            
            const prop1 = screen.getByLabelText("Prop 1");
            fireEvent.change(prop1, { target: { value: "value2" }});

            const sourceBtn = screen.getByLabelText("Show Object Source");
            sourceBtn.click();

            await waitFor(() => expect(screen.getByDisplayValue(/"prop1": "value2"/)).toBeInTheDocument());    
            await waitFor(() => expect(screen.getByRole("button", { name: "Save" })).toBeEnabled());
            
        });

        test("Edit Source changes object", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {
                    propertyTypes: {
                        prop1: {
                            label: "Prop 1",
                            dataType: "string"
                        }
                    }
                },
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();
                   
            render(                
                <ObjectEditorWithProfiler
                    object={object}     
                    layout={<ObjectResponsive />}                
                />,                
                { wrapper }
            );     

            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());   
            
            const editBtn = screen.getByLabelText("Edit Object");
            editBtn.click();

            await waitFor(() => expect(screen.getByLabelText("Prop 1")).toBeInTheDocument());   
            
            const sourceBtn = screen.getByLabelText("Show Object Source");
            sourceBtn.click();            

            await waitFor(() => expect(screen.getByDisplayValue(/"prop1": "value1"/)).toBeInTheDocument());    

            const textarea = screen.getByDisplayValue(/"prop1": "value1"/);
            fireEvent.change(textarea, { target: { value: "{ \"prop1\": \"value2\" }" } });

            const layoutBtn = screen.getByLabelText("Show Object in Layout");
            layoutBtn.click();

            await waitFor(() => expect(screen.getByLabelText("Prop 1")).toHaveValue("value2"));
            
        });

        test("Edit Object, Cancel, Confirm Discard Changes", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {
                    propertyTypes: {
                        prop1: {
                            label: "Prop 1",
                            dataType: "string"
                        }
                    }
                },
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();

            const onChanged = jest.fn();
            object.addEventListener("onChildChanged", onChanged);
            object.addEventListener("onDiscardChanges", onChanged);
                   
            render(                
                <ObjectEditorWithProfiler
                    object={object}     
                    layout={<ObjectResponsive />}                
                />,                
                { wrapper }
            );     

            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());   
            
            const editBtn = screen.getByLabelText("Edit Object");
            editBtn.click();

            await waitFor(() => expect(screen.getByLabelText("Prop 1")).toBeInTheDocument());        
            
            const prop1 = screen.getByLabelText("Prop 1");
            fireEvent.change(prop1, { target: { value: "value2" }});

            await waitFor(() => expect(onChanged).toHaveBeenCalledWith(
                expect.objectContaining({ eventId: "onChildChanged" })
            ));

            const cancelBtn = screen.getByLabelText("Cancel");
            cancelBtn.click();
             
            await waitFor(() => expect(screen.getByText(/Discard the changes?/)).toBeInTheDocument());


            const yesBtn = screen.getByLabelText("Yes");
            yesBtn.click();

            await waitForElementToBeRemoved(() => screen.queryByText(/Discard the changes?/));            

            expect(onChanged).toHaveBeenLastCalledWith(
                expect.objectContaining({ eventId: "onDiscardChanges" })
            );
            
        });   

        test("Edit Object, Cancel, Don't Confirm Discard Changes", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {
                    propertyTypes: {
                        prop1: {
                            label: "Prop 1",
                            dataType: "string"
                        }
                    }
                },
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();

            const onChanged = jest.fn();
            object.addEventListener("onChildChanged", onChanged);
            object.addEventListener("onDiscardChanges", onChanged);
                
            render(                
                <ObjectEditorWithProfiler
                    object={object}     
                    layout={<ObjectResponsive />}                
                />,                
                { wrapper }
            );     

            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());   
            
            const editBtn = screen.getByLabelText("Edit Object");
            editBtn.click();

            await waitFor(() => expect(screen.getByLabelText("Prop 1")).toBeInTheDocument());        
            
            const prop1 = screen.getByLabelText("Prop 1");
            fireEvent.change(prop1, { target: { value: "value2" }});

            await waitFor(() => expect(onChanged).toHaveBeenCalledWith(
                expect.objectContaining({ eventId: "onChildChanged" })
            ));

            const cancelBtn = screen.getByLabelText("Cancel");
            cancelBtn.click();
            
            await waitFor(() => expect(screen.getByText(/Discard the changes?/)).toBeInTheDocument());            

            const noBtn = screen.getByLabelText("No");
            noBtn.click();

            await waitForElementToBeRemoved(() => screen.queryByText(/Discard the changes?/));            

            expect(onChanged).not.toHaveBeenLastCalledWith(
                expect.objectContaining({ eventId: "onDiscardChanges" })
            );

            await waitFor(() => expect(screen.getByLabelText("Cancel")).toBeInTheDocument());
            
        });

        test("Edit Object, Save Changes", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            const object = new AfwObject({                 
                model,
                objectTypeObject: {
                    propertyTypes: {
                        prop1: {
                            label: "Prop 1",
                            dataType: "string"
                        }
                    }
                },
                object: {
                    prop1: "value1"
                }
            });

            await object.initialize();
                
            render(                
                <ObjectEditorWithProfiler
                    object={object}     
                    layout={<ObjectResponsive />}                
                />,                
                { wrapper }
            );     

            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value1")).toBeInTheDocument());   
            
            const editBtn = screen.getByLabelText("Edit Object");
            editBtn.click();

            await waitFor(() => expect(screen.getByLabelText("Prop 1")).toBeInTheDocument());        
            
            const prop1 = screen.getByLabelText("Prop 1");
            fireEvent.change(prop1, { target: { value: "value2" }});

            const saveBtn = screen.getByLabelText("Save");
            saveBtn.click();                               
            
            await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

            await waitFor(() => expect(screen.getByText("Prop 1")).toBeInTheDocument());            
            await waitFor(() => expect(screen.getByText("value2")).toBeInTheDocument());
            
        });

        test("Edit Complex Object", async () => {

            const ObjectEditorWithProfiler = withProfiler(ObjectEditor);
            
            let obj = {};
            for (const [propertyName, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {
                obj[propertyName] = propertyType.testValues[0];
            }

            const object = new AfwObject({                 
                model,
                objectTypeObject,
                object: obj,
            });
            
            await object.initialize();
                
            render(                
                <ObjectEditorWithProfiler
                    object={object}     
                    layout={<ObjectResponsive />}                
                />,                
                { wrapper }
            );     

            // skip some dataTypes for now, as they're not well defined in non-editable mode 
            const skipDataTypes = [ 
                "list", "object", "time", "base64Binary", "date", "dateTime", "hybrid", "password"
            ];

            for (const [dataType, propertyType] of Object.entries(objectTypeObject.propertyTypes)) {

                const {label, expectedTestValues} = propertyType;
                
                if (skipDataTypes.includes(dataType))
                    continue;

                await waitFor(() => expect(screen.getByText(label)).toBeInTheDocument());            
                await waitFor(() => expect(screen.getByText(expectedTestValues[0])).toBeInTheDocument());   
            }
                        
            
            const editBtn = screen.getByLabelText("Edit Object");
            editBtn.click();

            const cancelBtn = screen.getByLabelText("Cancel");
            cancelBtn.click();                               
            
            await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());
            
        });
    });
};

export default Test;
