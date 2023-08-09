// See the 'COPYING' file in the project root for licensing information.
import React from "react";

import {fireEvent, render, waitFor, screen} from "@testing-library/react";
import {ListResponsive} from "@afw/react";
import {AfwProperty} from "@afw/client";
import {OperationalContext, AdaptiveLayoutContext} from "@afw/react";


export const AddListValue = async ({ label, value }) => {

    // click the add button
    fireEvent.click(screen.getByText("add"));
            
    // look for the labeled input
    const input = await screen.findByLabelText(label);
    fireEvent.change(input, { target: { value }});

    // click the ok button
    fireEvent.click(screen.getByText("Ok"));

    // verify it's no longer in the list
    expect(screen.getByText(value)).toBeInTheDocument();
};


export const RemoveListValue = async ({ value }) => {
    // select the entry
    fireEvent.click(screen.getByText(value));

    // click remove button
    fireEvent.click(screen.getByText("remove"));
                
    expect(screen.queryByText(value)).not.toBeInTheDocument();
};


const Test = (wrapper) => {

    describe("General tests", () => {

        test("Renders properly", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string"
                },
                value: [ "This is a value!" ]
            });
                   
            render(
                <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                    <ListResponsive    
                        property={property}
                    />
                </AdaptiveLayoutContext.Provider>,
                { wrapper }
            );                    
        });

        test("Renders properly (editable)", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string"
                },
                value: [ "This is a value!" ]
            });

            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <ListResponsive property={property} />
                </OperationalContext.Provider>,
                { wrapper }
            );     
        });      
        
        test("Renders with 0 values", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string"
                },
            });
                   
            render(
                <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                    <ListResponsive    
                        property={property}
                    />
                </AdaptiveLayoutContext.Provider>,
                { wrapper }
            );     
        });

        test("Renders (editable) with 0 values", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string"
                },
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     
        });

        test("Renders with 1 value", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string"
                },
                value: [ "This is a value!" ]
            });
                   
            render(
                <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                    <ListResponsive    
                        property={property}
                    />
                </AdaptiveLayoutContext.Provider>,
                { wrapper }
            );     

            expect(screen.getByText("This is a value!")).toBeInTheDocument();
        });

        test("Renders (editable) with 1 value", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string"
                },
                value: [ "This is a value!" ]
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     

            expect(screen.getByText("This is a value!")).toBeInTheDocument();
        });

        test("Renders with 2 values", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string"
                },
                value: [ "This is a value!", "This is value2" ]
            });
                   
            render(
                <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                    <ListResponsive    
                        property={property}
                    />
                </AdaptiveLayoutContext.Provider>,
                { wrapper }
            );     

            expect(screen.getByText("This is a value!")).toBeInTheDocument();
            expect(screen.getByText("This is value2")).toBeInTheDocument();
        });

        test("Renders (editable) with 2 values", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string"
                },
                value: [ "This is a value!", "This is value2" ]
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     

            expect(screen.getByText("This is a value!")).toBeInTheDocument();
            expect(screen.getByText("This is value2")).toBeInTheDocument();
        });

        test("Renders with 100 values", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string"
                }
            });

            let values = [];
            for (let i = 0; i < 100; i++) {
                values.push("This is value " + i);
            }
            property.setValue(values);
                   
            render(
                <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                    <ListResponsive    
                        property={property}
                    />
                </AdaptiveLayoutContext.Provider>,
                { wrapper }
            );     

            for (let i = 0; i < 100; i++) {
                expect(screen.getByText("This is value " + i)).toBeInTheDocument();
            }            
            
        });

        test("Renders (editable) with 100 values", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string"
                }
            });

            let values = [];
            for (let i = 0; i < 100; i++) {
                values.push("This is value " + i);
            }
            property.setValue(values);
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     

            for (let i = 0; i < 100; i++) {
                expect(screen.getByText("This is value " + i)).toBeInTheDocument();
            }                       
        });

    });

    describe("Adding/Removing tests", () => {    
      
        test("Start with 0 values, add 1 value", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string",
                    label: "My List"
                },
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );                           
            
            await AddListValue({ label: "My List", value: "List Value 1" });
        });

        test("Start with 1 value, add 1 value", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string",
                    label: "My List"
                },
                value: [ "List Value 1" ]
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     

            await AddListValue({ label: "My List", value: "List Value 2" });

            expect(screen.getByText("List Value 1")).toBeInTheDocument();
            expect(screen.getByText("List Value 2")).toBeInTheDocument();
        });

        test("Start with 1 value, remove 1 value", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string",
                    label: "My List"
                },
                value: [ "List Value 1" ]
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     

            await RemoveListValue({ value: "List Value 1" });
        });

        test("Start with 2 values, remove first value, then second value", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string",
                    label: "My List"
                },
                value: [ "List Value 1", "List Value 2" ]
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     

            await RemoveListValue({ value: "List Value 1" });
            await RemoveListValue({ value: "List Value 2" });

            expect(screen.queryByText("List Value 1")).not.toBeInTheDocument();                        
            expect(screen.queryByText("List Value 2")).not.toBeInTheDocument();
        });

        test("Start with 0 values, add 1 value, then remove it", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string",
                    label: "My List"
                },                
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     

            await AddListValue({ label: "My List", value: "List Value 1" });
            await RemoveListValue({ value: "List Value 1" });
        });

        test("Start with 0 values, add 100 new values, then remove them", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string",
                    label: "My List"
                },                
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     
            
            for (let i = 0; i < 100; i++) {
                await AddListValue({ label: "My List", value: "List Value " + i });                         
            }

            for (let i = 0; i < 100; i++) {                
                await RemoveListValue({ value: "List Value " + i });
            }
        });

        test("Start with 0 values, add a new value, then cancel", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string",
                    label: "My List"
                },                
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     
            
            fireEvent.click(screen.getByText("add"));
            const input = await screen.findByLabelText("My List");
            fireEvent.change(input, { target: { value: "List Value 1" }});  
            
            fireEvent.click(screen.getByText("Cancel"));
            expect(screen.queryByText("List Value 1")).not.toBeInTheDocument();
        });

        test("Start with 1 value, add a new value, then cancel", async () => {
            const property = new AfwProperty({ 
                propertyName: "mylist", 
                propertyType: {
                    dataType: "array",
                    dataTypeParameter: "string",
                    label: "My List"
                },                
                value: [ "List Value 1" ]
            });
                   
            render(
                <OperationalContext.Provider value={{ editable: true }}>
                    <AdaptiveLayoutContext.Provider value={{ data: { property, value: property } }}>
                        <ListResponsive    
                            property={property}
                        />
                    </AdaptiveLayoutContext.Provider>
                </OperationalContext.Provider>,
                { wrapper }
            );     
            
            fireEvent.click(screen.getByText("add"));
            const input = await screen.findByLabelText("My List");
            fireEvent.change(input, { target: { value: "List Value 2" }});  
            
            fireEvent.click(screen.getByText("Cancel"));

            expect(screen.queryByText("List Value 1")).toBeInTheDocument();
            expect(screen.queryByText("List Value 2")).not.toBeInTheDocument();
        });

    });
};

export default Test;
