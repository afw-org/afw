// See the 'COPYING' file in the project root for licensing information.
import AfwClient from "../../AfwClient";

import {AfwModel, AfwObject, AfwProperty, AfwArray, AfwArrayEntry} from "..";
import {isObject, isArray} from "../../utils";
import {waitFor, mswPostCallback} from "@afw/test";

describe("AfwObject Tests", () => {

    const client = new AfwClient();
    const model = new AfwModel({ client });

    test("Create new AfwObject", async () => {        
        
        const obj = new AfwObject({ 
            model, 
            object: {
                prop: "abc"
            }
        });

        expect(obj).toBeInstanceOf(AfwObject);
    });

    test("Create new AfwObject, iterate through properties", async () => {

        const object = {
            prop1: "abc",
            prop2: true,
            prop3: 10,   
        };

        const obj = new AfwObject({ model, object });

        expect(obj).toBeInstanceOf(AfwObject);

        /* initialize it for dependencies */
        await obj.initialize();

        /* iterate through properties */
        for (const property of obj.getProperties()) {            
            expect(property).toBeInstanceOf(AfwProperty);
        }
        
        /* iterate through properties, using for of */
        for (const property of obj) {
            expect(property).toBeInstanceOf(AfwProperty);
        }
        
        /* make sure all property values match */
        for (const [key, val] of Object.entries(object)) {
            expect(obj.getProperty(key).getValue()).toBe(val);
        }        
    });

    test("Create new AfwObject, with embedded object", async () => {

        const object = {
            prop1: {
                prop2: "abc"
            }  
        };

        const obj = new AfwObject({ model, object });

        expect(obj).toBeInstanceOf(AfwObject);

        /* initialize it for dependencies */
        await obj.initialize();

        const property = obj.getProperty("prop1");
        expect(property).toBeInstanceOf(AfwProperty);

        const prop1 = property.getValue();
        expect(prop1).toBeInstanceOf(AfwObject);

        const prop2 = prop1.getProperty("prop2");
        expect(prop2).toBeInstanceOf(AfwProperty);
        expect(prop2.getValue()).toBe("abc");
        
    });

    test("Create new AfwObject, with list", async () => {

        const object = {
            prop1: [
                "abc",
                "def"
            ] 
        };

        const obj = new AfwObject({ model, object });

        expect(obj).toBeInstanceOf(AfwObject);

        /* initialize it for dependencies */
        await obj.initialize();

        const property = obj.getProperty("prop1");
        expect(property).toBeInstanceOf(AfwProperty);

        const prop1 = property.getValue();
        expect(prop1).toBeInstanceOf(AfwArray);

        property.getValue().forEach((value, index) => {
            expect(value).toBeInstanceOf(AfwArrayEntry);

            expect(value.getValue()).toBe(object.prop1[index]);            
        });
    });

    test("custom objectTypeObject", async () => {

        const object = {
            prop1: "abc",
            prop2: 42,
            prop3: true,
            prop4: { prop4a: "abc" },
            prop5: [ 1, 2, 3 ]
        };

        const objectTypeObject = {
            propertyTypes: {
                prop1: {
                    dataType: "string"
                },
                prop2: {
                    dataType: "integer"
                },
                prop3: {
                    dataType: "boolean"
                },
                prop4: {
                    dataType: "object"
                },
                prop5: {
                    dataType: "array"
                }
            }
        };

        const obj = new AfwObject({ object, objectTypeObject });

        expect(obj.getProperty("prop1")).toBeInstanceOf(AfwProperty);
        expect(obj.getProperty("prop1").getDataType()).toBe("string");

        expect(obj.getProperty("prop2")).toBeInstanceOf(AfwProperty);
        expect(obj.getProperty("prop2").getDataType()).toBe("integer");

        expect(obj.getProperty("prop3")).toBeInstanceOf(AfwProperty);
        expect(obj.getProperty("prop3").getDataType()).toBe("boolean");

        expect(obj.getProperty("prop4")).toBeInstanceOf(AfwProperty);
        expect(obj.getProperty("prop4").getDataType()).toBe("object");

        expect(obj.getProperty("prop5")).toBeInstanceOf(AfwProperty);
        expect(obj.getProperty("prop5").getDataType()).toBe("array");

        expect(obj.getProperty("prop6")).toBeUndefined();

    });

    test("clone", async () => {

        const obj = new AfwObject({ 
            model, 
            object: {
                prop: "abc"
            }
        });

        await obj.initialize();

        const clone = obj.clone();        

        expect(clone).toBeInstanceOf(AfwObject);
        expect(clone).not.toBe(obj);

        // make sure their stringify is equivalent
        expect(obj.stringify()).toBe(clone.stringify());

    });

    test("discardChanges", async () => {

        const obj = new AfwObject({ 
            model, 
            object: {
                prop: "abc"
            }
        });

        await obj.initialize();

        obj.getProperty("prop").setValue("def");
        expect(obj.getProperty("prop").getValue()).toBe("def");

        obj.discardChanges();
        expect(obj.getProperty("prop").getValue()).toBe("abc");

    });

    test("getEmbeddingObject", async () => {

        const obj = new AfwObject({
            model,
            object: {
                embedded: {
                    prop: "abc"
                }
            }
        });

        await obj.initialize();

        const embeddedObj = obj.getProperty("embedded").getValue();
        expect(embeddedObj.getEmbeddingObject()).toBe(obj);

    });

    test("getModel()", async () => {

        const obj = new AfwObject({
            model,
            object: {                
                prop: "abc"                
            }
        });

        await obj.initialize();

        expect(obj.getModel()).toBe(model);
    });

    test("getPropertyType()", async () => {

        const object = { prop1: "abc" };
        const objectTypeObject = {
            propertyTypes: {
                prop1: {
                    dataType: "string"
                }
            }
        };

        const obj = new AfwObject({ model, object, objectTypeObject });

        await obj.initialize();

        expect(obj.getPropertyType("prop1")).toBe(objectTypeObject.propertyTypes.prop1);      
        expect(obj.getPropertyType("prop2")).toBeUndefined();

    });

    test("getPropertyType() with otherProperties", async () => {

        const object = { prop1: "abc" };
        const objectTypeObject = {
            propertyTypes: {
                prop1: {
                    dataType: "string"
                }
            },
            otherProperties: {
                dataType: "integer"
            }
        };

        const obj = new AfwObject({ model, object, objectTypeObject });

        await obj.initialize();

        expect(obj.getPropertyType("prop1")).toBe(objectTypeObject.propertyTypes.prop1);      
        expect(obj.getPropertyType("prop2")).toBe(objectTypeObject.otherProperties);

        expect(obj.getPropertyType("prop2", false)).toBeUndefined();

    });

    test("getProperties()/getProperty()", async () => {

        const object = { 
            prop1: "abc",
            prop2: 123,
            prop3: true,
            prop4: {
                prop4a: "xyz"
            },
            prop5: [
                "def",
                "ghi"
            ]
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        const properties = obj.getProperties();
        for (const prop of properties) {
            expect(prop).toBeInstanceOf(AfwProperty);

            expect(obj.getProperty(prop.getName())).toBe(prop);
        }
    });

    test("getPropertyValue()", async () => {

        const object = { 
            prop1: "abc",
            prop2: 123,
            prop3: true,
            prop4: {
                prop4a: "xyz"
            },
            prop5: [
                "def",
                "ghi"
            ]
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        for (const [p, v] of Object.entries(object)) {
            if (isObject(v)) 
                expect(obj.getPropertyValue(p)).toBeInstanceOf(AfwObject);                
            else if (isArray(v))
                expect(obj.getPropertyValue(p)).toBeInstanceOf(AfwArray);
            else
                expect(obj.getPropertyValue(p)).toBe(v);
        }

    });

    test("getPropertyValues()", async () => {

        const object = { 
            prop1: "abc",
            prop2: 123,
            prop3: true,
            prop4: {
                prop4a: "xyz"
            },
            prop5: [
                "def",
                "ghi"
            ]
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        const props = obj.getPropertyValues();
        expect(Object.entries(props)).toHaveLength(Object.entries(object).length);

        for (const [k, v] of Object.entries(props)) {
            expect(obj.getPropertyValue(k)).toBe(v);
        }
    });

    test("getPropertyValues() after adding values", async () => {

        const object = { 
            prop1: "abc",
            prop2: 123,
            prop3: true,
            prop4: {
                prop4a: "xyz"
            },
            prop5: [
                "def",
                "ghi"
            ]
        };

        const obj = new AfwObject({ model, object: {} });

        await obj.initialize();

        for (const [k, v] of Object.entries(object)) {
            obj.setPropertyValue(k, v);
        }

        const props = obj.getPropertyValues();        
        expect(Object.entries(props)).toHaveLength(Object.entries(object).length);

        for (const [k, v] of Object.entries(props)) {
            expect(obj.getPropertyValue(k)).toBe(v);
        }
    });

    test("getObjectTypeObject()", async () => {

        const object = { prop1: "abc" };
        const objectTypeObject = {
            propertyTypes: {
                prop1: {
                    dataType: "string"
                }
            },
            otherProperties: {
                dataType: "integer"
            }
        };

        const obj = new AfwObject({ model, object, objectTypeObject });

        await obj.initialize();

        expect(obj.getObjectTypeObject()).toBe(objectTypeObject);
    });

    test("getAdapterId()", async () => {

        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {}
        });

        await obj.initialize();

        expect(obj.getAdapterId()).toBe("afw");
    });

    test("getMetaProperty()", async () => {

        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",    
                    allowChange: true,           
                }
            }
        });

        await obj.initialize();

        expect(obj.getMetaProperty("path")).toBe("/afw/_AdaptiveObject_/test");
        expect(obj.getPath()).toBe("/afw/_AdaptiveObject_/test");

        expect(obj.getMetaProperty("allowChange")).toBe(true);
        expect(obj.getAllowChange()).toBe(true);

        expect(obj.getMetaProperty("none")).toBeUndefined();    
    });

    test("getMetaObject()", async () => {

        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",    
                    allowChange: true,               
                }
            }
        });

        await obj.initialize();

        const metaObject = obj.getMetaObject();

        expect(metaObject.getPropertyValue("path")).toBe("/afw/_AdaptiveObject_/test");
        expect(metaObject.getPropertyValue("allowChange")).toBe(true);       

        expect(metaObject.getPropertyValue("none")).toBeUndefined();    
    });

    test("renameProperty()", async () => {

        const object = { prop1: "abc" };
        const objectTypeObject = {            
            otherProperties: {
                dataType: "integer"
            }
        };

        const obj = new AfwObject({ model, object, objectTypeObject });

        await obj.initialize();

        expect(obj.getPropertyValue("prop1")).toBe("abc");

        obj.renameProperty("prop1", "prop2");

        expect(obj.getPropertyValue("prop2")).toBe("abc");
        expect(obj.getPropertyValue("prop1")).toBeUndefined();
    });

    test("removeProperty()", async () => {

        const object = { prop1: "abc" };
        const objectTypeObject = {            
            otherProperties: {
                dataType: "integer"
            }
        };

        const obj = new AfwObject({ model, object, objectTypeObject });

        await obj.initialize();

        expect(obj.getPropertyValue("prop1")).toBe("abc");

        obj.removeProperty("prop1");

        expect(obj.getPropertyValue("prop1")).toBeUndefined();
    });

    test("setMetaProperty()", async () => {
        const object = {
            _meta_: {
                path: "/afw/_AdaptiveObject_/test",
                allowChange: true
            },
            prop1: "abc" 
        };       

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        expect(obj.getMetaProperty("allowChange")).toBe(true);

        obj.setMetaProperty("allowChange", false);
        expect(obj.getMetaProperty("allowChange")).toBe(false);
    });

    test("setAdapterId()", async () => {
        const object = {
            _meta_: {
                path: "/afw/_AdaptiveObject_/test"
            },
            prop1: "abc" 
        };       

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        expect(obj.getAdapterId()).toBe("afw");

        obj.setAdapterId("test");
        expect(obj.getAdapterId()).toBe("test");
    });

    test("setObjectTypeId()", async () => {
        const object = {
            _meta_: {
                path: "/afw/_AdaptiveObject_/test"
            },
            prop1: "abc" 
        };       

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        expect(obj.getObjectTypeId()).toBe("_AdaptiveObject_");

        obj.setObjectTypeId("_NewObjectType_");
        expect(obj.getObjectTypeId()).toBe("_NewObjectType_");
    });

    test("setObjectId()", async () => {

        const object = {
            _meta_: {
                path: "/afw/_AdaptiveObject_/test"
            },
            prop1: "abc" 
        };       

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        expect(obj.getObjectId()).toBe("test");

        obj.setObjectId("test2");
        expect(obj.getObjectId()).toBe("test2");
    });

    test("setPath()", async () => {

        const object = {
            _meta_: {
                path: "/afw/_AdaptiveObject_/test"
            },
            prop1: "abc" 
        };       

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        expect(obj.getPath()).toBe("/afw/_AdaptiveObject_/test");

        obj.setPath("/some/new/path");
        expect(obj.getPath()).toBe("/some/new/path");

    });

    test("newProperty()", async () => {

        const object = { prop1: "abc" };
        const objectTypeObject = {            
            otherProperties: {
                dataType: "integer"
            }
        };

        const obj = new AfwObject({ model, object, objectTypeObject });

        await obj.initialize();

        expect(obj.getPropertyValue("prop2")).toBeUndefined();

        const prop = obj.newProperty("prop2");
        prop.setValue(123);

        expect(obj.getPropertyValue("prop2")).toBe(123);
    });

    test("setPropertyValue()", async () => {
        const object = { prop1: "abc" };
        const objectTypeObject = {            
            propertyTypes: {
                prop1: {
                    dataType: "string"
                },
                prop2: {
                    dataType: "integer"
                },
                prop3: {
                    dataType: "boolean"
                },
                prop4: {
                    dataType: "object"
                },
                prop5: {
                    dataType: "array"
                },
            }
        };

        const obj = new AfwObject({ model, object, objectTypeObject });

        await obj.initialize();

        expect(obj.getPropertyValue("prop1")).toBe("abc");

        obj.setPropertyValue("prop1", "def");
        expect(obj.getPropertyValue("prop1")).toBe("def");
        
        obj.setPropertyValue("prop2", 123);
        expect(obj.getPropertyValue("prop2")).toBe(123);

        obj.setPropertyValue("prop3", true);
        expect(obj.getPropertyValue("prop3")).toBe(true);

        obj.setPropertyValue("prop4", {});
        expect(obj.getPropertyValue("prop4")).toBeInstanceOf(AfwObject);

        obj.setPropertyValue("prop5", []);
        expect(obj.getPropertyValue("prop5")).toBeInstanceOf(AfwArray);
    });
    
    test("forEach()", async () => {

        const object = { 
            prop1: "abc",
            prop2: 123,
            prop3: true,
            prop4: {
                prop4a: "xyz"
            },
            prop5: [
                "def",
                "ghi"
            ]
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();
        
        obj.forEach(property => {
            expect(property).toBeInstanceOf(AfwProperty);
            expect(obj.getPropertyValue(property.getName())).toBe(property.getValue());
        });
    });

    test("map()", async () => {

        const object = { 
            prop1: "abc",
            prop2: 123,
            prop3: true,
            prop4: {
                prop4a: "xyz"
            },
            prop5: [
                "def",
                "ghi"
            ]
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();
        
        const values = obj.map(property => property.getName());

        for (const v of values) {
            expect(object[v]).toBeDefined();
        }
    });

    test("getEmbeddingProperty()", async () => {

        const object = {             
            prop1: {
                prop2: "xyz"
            }            
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        expect(obj.getPropertyValue("prop1").getEmbeddingProperty()).toBe("prop1");

    });

    test("getOriginal()", async () => {

        const object = {             
            prop1: "abc"
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        obj.setPropertyValue("prop1", "xyz");
        expect(obj.getOriginal()).toEqual({
            prop1: "abc"
        });
    });

    test("getOriginal() with embedded object", async () => {

        const object = {             
            prop1: "abc",
            prop2: {
                prop3: "x"
            }
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        obj.setPropertyValue(["prop2", "prop3", "y"]);
        expect(obj.getOriginal()).toEqual({
            prop1: "abc",
            prop2: {
                prop3: "x"
            }
        });
    });

    test("getOriginal() with renaming an embedded object property", async () => {

        const object = {             
            prop1: "abc",
            prop2: {
                prop3: "x"
            }
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        const p = obj.getProperty("prop2");
        p.setName("newProp2");

        expect(obj.getOriginal()).toEqual({
            prop1: "abc",
            prop2: {
                prop3: "x"
            }
        });
    });

    test("hasProperty()", async () => {
        const object = {             
            prop1: "abc"
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        expect(obj.hasProperty("prop1")).toBe(true);
        expect(obj.hasProperty("prop2")).toBe(false);
    });

    test("fromJSON()", async () => {

        const object = { 
            prop1: "abc",
            prop2: 123,
            prop3: true,
            prop4: {
                prop4a: "xyz"
            },
            prop5: [
                "def",
                "ghi"
            ]
        };

        const obj = new AfwObject({ model });

        await obj.initialize();
        
        obj.fromJSON( JSON.stringify(object) );
        
        for (const p of obj.getProperties()) {  
            if (p.getDataType() === "object")
                expect(JSON.stringify(object[p.getName()], null, 4)).toBe(obj.getPropertyValue(p.getName()).stringify());
            else if (p.getDataType() === "array")                  
                expect(JSON.stringify(object[p.getName()], null, 4)).toBe(obj.getPropertyValue(p.getName()).stringify());                
            else
                expect(object[p.getName()]).toBe(obj.getPropertyValue(p.getName()));
        }
        
    });

    test("getErrors()", async () => {

        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                prop1: "abc",                
                prop2: {
                    prop3: {
                        abc: "xyz"
                    },
                    _meta_: {
                        hasErrors: true,
                        propertyTypes: {
                            prop3: {
                                errors: [
                                    "Another error"
                                ]
                            }
                        }
                    }
                },
                _meta_: {                    
                    path: "/afw/_AdaptiveObject_/test",    
                    hasErrors: true,
                    propertyTypes: {
                        prop1: {
                            errors: [
                                "An error occurred"
                            ]
                        },                        
                    }                    
                },
            }
        });
        
        await obj.initialize();
        
        expect(obj.getErrors()).toHaveLength(2);
    });

    test("hasErrors()", async () => {
        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                prop1: "abc",
                _meta_: {                    
                    path: "/afw/_AdaptiveObject_/test",    
                    hasErrors: true,
                    propertyTypes: {
                        prop1: {
                            errors: [
                                "An error occurred"
                            ]
                        }
                    }                    
                }
            }
        });
        
        await obj.initialize();
        
        expect(obj.hasErrors()).toBe(true);
    });

    test("stringify()", async () => {
        const object = { 
            prop1: "abc",
            prop2: 123,
            prop3: true,
            prop4: {
                prop4a: "xyz"
            },
            prop5: [
                "def",
                "ghi"
            ]
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        expect(obj.stringify()).toBe(`{
    "prop1": "abc",
    "prop2": 123,
    "prop3": true,
    "prop4": {
        "prop4a": "xyz"
    },
    "prop5": [
        "def",
        "ghi"
    ]
}`);
        
    });

    test("toJSON()", async () => {

        const object = { 
            prop1: "abc",
            prop2: 123,
            prop3: true,
            prop4: {
                prop4a: "xyz"
            },
            prop5: [
                "def",
                "ghi"
            ]
        };

        const obj = new AfwObject({ model, object });

        await obj.initialize();

        expect(obj.toJSON()).toStrictEqual(object);
    });

    test("validate()", async () => {

        const object = { prop1: "abc" };        

        const objectTypeObject = {            
            propertyTypes: {
                prop1: {
                    dataType: "string",
                    required: true
                },
                prop2: {
                    dataType: "integer"
                },
                prop3: {
                    dataType: "boolean"
                },
                prop4: {
                    dataType: "object"
                },
                prop5: {
                    dataType: "array"
                },
            }
        };

        const obj = new AfwObject({ model, object, objectTypeObject });

        await obj.initialize();

        expect(obj.validate()).toHaveLength(0);

        obj.removeProperty("prop1");

        expect(obj.validate()).not.toHaveLength(0);
        
    });

    test("getJSONSchema()", async () => {
        const object = { 
            prop1: "abc",
            prop2: 123,
            prop3: true,
            prop4: {
                prop4a: "xyz"
            },
            prop5: [
                "def",
                "ghi"
            ]
        };

        const objectTypeObject = {
            propertyTypes: {
                prop1: {
                    dataType: "string"
                },
                prop2: {
                    dataType: "integer"
                },
                prop3: {
                    dataType: "boolean"
                },
                prop4: {
                    dataType: "object"
                },
                prop5: {
                    dataType: "array"
                }
            }
        };

        const jsonSchema = {
            type: "object",
            properties: {                
                prop1: {
                    type: "string",
                },
                prop2: {
                    type: "number",
                },
                prop3: {
                    type: "boolean",
                },
                prop4: {
                    type: "object",
                },
                prop5: {
                    type: "array",
                },
            }
        };

        const obj = new AfwObject({ model, object, objectTypeObject });

        await obj.initialize();

        expect(obj.getJSONSchema()).toEqual(expect.objectContaining({ type: "object" }));
        expect(obj.getJSONSchema().properties).toEqual(expect.objectContaining(jsonSchema.properties));
    });

    test("refresh()", async () => {

        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",                                  
                },
                prop1: "abc"
            }
        });

        await obj.initialize();
        
        mswPostCallback.mockClear();
        await obj.refresh();
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
    });

    test("reconcile()", async () => {
        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",                                  
                },
                prop1: "abc"
            }
        });

        await obj.initialize();
        
        mswPostCallback.mockClear();
        const response = obj.reconcile();        
        await response.result();        
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
    });

    test("replace()", async () => {
        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",                                  
                },
                prop1: "abc"
            }
        });

        await obj.initialize();
        
        mswPostCallback.mockClear();
        const response = obj.replace();
        await response.result();
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
    });

    test("modify()", async () => {
        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",                                  
                },
                prop1: "abc"
            }
        });

        await obj.initialize();
        
        mswPostCallback.mockClear();
        await obj.modify();
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
    });

    test("add()", async () => {
        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",                                  
                },
                prop1: "abc"
            }
        });

        await obj.initialize();
        
        mswPostCallback.mockClear();
        const response = obj.add();
        await response.result();
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
    });

    test("onChanged", async () => {

        const onChanged = jest.fn();

        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",                                  
                },
                prop1: "abc"
            }
        });

        await obj.initialize();
        
        obj.addEventListener("onChanged", onChanged);

        obj.setPropertyValue("prop1", "def");
        await waitFor(() => expect(onChanged).not.toHaveBeenCalled());

        obj.removeProperty("prop1");
        await waitFor(() => expect(onChanged).toHaveBeenCalled());
    });

    test("onSavable", async () => {

        const onSavable = jest.fn();

        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",                                  
                },
                prop1: "abc"
            }
        });

        await obj.initialize();
        
        obj.addEventListener("onSavable", onSavable);

        obj.setPropertyValue("prop1", "def");
        await waitFor(() => expect(onSavable).toHaveBeenCalled());

    });

    test("onPropertiesChanged", async () => {

        const onPropertiesChanged = jest.fn();

        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",                                  
                },
                prop1: "abc"
            }
        });

        await obj.initialize();

        obj.addEventListener("onPropertiesChanged", onPropertiesChanged);
        obj.renameProperty("prop1", "prop2");
        await waitFor(() => expect(onPropertiesChanged).toHaveBeenCalled());
    });

    test("onDiscardChanges", async () => {

        const onDiscardChanges = jest.fn();

        const obj = new AfwObject({
            model,
            adapterId: "afw",
            objectTypeId: "_AdaptiveObject_",
            objectId: "test",
            object: {
                _meta_: {
                    path: "/afw/_AdaptiveObject_/test",                                  
                },
                prop1: "abc"
            }
        });

        await obj.initialize();

        obj.addEventListener("onDiscardChanges", onDiscardChanges);
        obj.renameProperty("prop1", "prop2");

        obj.discardChanges();
        await waitFor(() => expect(onDiscardChanges).toHaveBeenCalled());
    });

});
