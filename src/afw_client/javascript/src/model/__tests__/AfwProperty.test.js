// See the 'COPYING' file in the project root for licensing information.
import {AfwClient} from "../../AfwClient";

import {AfwModel, AfwObject, AfwProperty} from "..";

describe("AfwProperty Tests", () => {

    const client = new AfwClient();
    const model = new AfwModel({ client });

    test("Create new AfwProperty", async () => {        
        
        const property = new AfwProperty({
            propertyName: "prop1",
            value: "abc"
        });

        expect(property).toBeInstanceOf(AfwProperty);
    });

    test("Test remove()", async () => {

        const obj = new AfwObject({
            model, 
            object: {
                prop1: "abc"
            }
        });

        await obj.initialize();

        const prop = obj.getProperty("prop1");
        expect(prop).toBeInstanceOf(AfwProperty);
        prop.remove();
        
        expect(obj.getPropertyValue("prop1")).toBeUndefined();
    });

    test("Test isOtherPropertyType()", async () => {

        const obj = new AfwObject({
            model, 
            object: {
                prop1: "abc"
            },
            objectTypeObject: {
                propertyTypes: {
                    "prop1": {
                        dataType: "string"
                    }
                },
                otherProperties: {
                    dataType: "string"
                }
            }
        });

        await obj.initialize();

        const prop1 = obj.getProperty("prop1");
        const prop2 = obj.getProperty("prop2");

        expect(prop1.isOtherPropertyType()).toBe(false);
        expect(prop2.isOtherPropertyType()).toBe(true);
    });

    test("Test getName()", async () => {

        const obj = new AfwObject({
            model, 
            object: {
                prop1: "abc"
            }
        });

        await obj.initialize();

        const prop = obj.getProperty("prop1");
        expect(prop.getName()).toBe("prop1");
    });

    test("Test getPath()", async () => {

        const obj = new AfwObject({
            model, 
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test"
                },
                prop1: {
                    prop2: "abc"
                }
            }
        });

        await obj.initialize();

        const prop1 = obj.getProperty("prop1");
        const prop2 = prop1.getValue().getProperty("prop2");
        expect(prop1.getPath()).toBe("/afw/_AdaptiveObject_/test/prop1");
        expect(prop2.getPath()).toBe("/afw/_AdaptiveObject_/test/prop1/prop2");
    });

    test("Test getPropertyType()", async () => {

        const objectTypeObject = {
            propertyTypes: {
                prop1: {
                    dataType: "string"
                }
            }
        };

        const obj = new AfwObject({
            model, 
            object: {
                prop1: "abc"
            },
            objectTypeObject
        });

        await obj.initialize();

        const prop = obj.getProperty("prop1");
        const pType = prop.getPropertyType();
        expect(pType).toEqual(objectTypeObject.propertyTypes.prop1);
    });

    test("Test getObjectTypeObject()", async () => {

        const objectTypeObject = {
            propertyTypes: {
                prop1: {
                    dataType: "object",
                    dataTypeParameter: "_AdaptiveObject_"
                }
            }
        };

        const obj = new AfwObject({
            model, 
            object: {
                prop1: {
                    prop2: "abc"
                }
            },
            objectTypeObject
        });

        await obj.initialize();

        //const prop = obj.getProperty("prop1");
        //const oType = prop.getObjectTypeObject();
        
        // \fixme implement
    });

    test("Test setName()", async () => {

        const obj = new AfwObject({
            model, 
            object: {
                prop1: "abc"
            }
        });

        await obj.initialize();

        const prop = obj.getProperty("prop1");
        prop.setName("prop2");
        
        expect(prop.getName()).toBe("prop2");
        expect(obj.getPropertyValue("prop1")).toBeUndefined();
        expect(obj.getPropertyValue("prop2")).toBe("abc");
    });

    test("Test validate()", async () => {

        const objectTypeObject = {
            propertyTypes: {
                prop1: {
                    dataType: "string",
                    required: true
                }
            }
        };

        const obj = new AfwObject({
            model, 
            object: {
                prop1: "abc"
            },
            objectTypeObject
        });

        await obj.initialize();

        const prop = obj.getProperty("prop1");
        expect(prop.validate().length).toBe(0);

        // \fixme check invalid value
    });

    test("Test meta properties", async () => {

        const property = new AfwProperty({ 
            propertyName: "mylist", 
            propertyType: {
                dataType: "array",
                dataTypeParameter: "string",
                label: "My List"
            },                
            value: [ "List Value 1" ]
        });
        
        expect(property.getLabel()).toBe("My List");

    });

});
