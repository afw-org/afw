// See the 'COPYING' file in the project root for licensing information.
import AfwClient from "../../AfwClient";

import {AfwModel, AfwObject} from "..";
import {mswPostCallback, waitFor} from "@afw/test";

describe("AfwModel Tests", () => {

    const client = new AfwClient();

    test("Create new AfwModel", async () => {        
        const model = new AfwModel({ client });

        expect(model).toBeInstanceOf(AfwModel);
    });

    test("Retrieve objects", async () => {
        const model = new AfwModel({ client });

        const response = model.retrieveObjects({ adapterId: "afw", objectTypeId: "_AdaptiveObjectType_" });
        const objects = await response.objects;

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());     

        for (const obj of objects) {            
            expect(obj).toBeInstanceOf(AfwObject);
        }
    });

    test("Get object", async () => {
        const model = new AfwModel({ client });

        const response = model.getObject({ adapterId: "afw", objectTypeId: "_AdaptiveObjectType_", objectId: "_AdaptiveObject_" });
        const obj = await response.object;

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());     

        expect(obj).toBeInstanceOf(AfwObject);
    });

    test("Get object with uri", async () => {
        const model = new AfwModel({ client });

        const response = model.getObjectWithUri({ uri: "/afw/_AdaptiveObjectType_/_AdaptiveObject_" });
        const obj = await response.object;

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());     

        expect(obj).toBeInstanceOf(AfwObject);
    });

    test("New object", async () => {
        const model = new AfwModel({ client });

        const obj = model.newObject({ 
            adapterId: "myAdapter", 
            objectTypeId: "MyObjectType", 
            objectId: "myObjectId",
            object: {
                myProperty: "myValue"
            }
        });

        expect(obj).toBeInstanceOf(AfwObject);
    });

    test("Load Object Types", async () => {

        const model = new AfwModel({ client });

        const objects = await model.loadObjectTypes({ adapterId: "afw" });

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());     

        for (const obj of objects) {
            expect(obj).toBeInstanceOf(AfwObject);
        }
    });

    describe("Caching", () => {

        test("Getting returns correct item set", async () => {
            const model = new AfwModel({ client });

            const object = {
                "test": "value"
            };

            model.cacheSet("key1", object);

            const cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);
        });

        test("Getting wrong key", async () => {
            const model = new AfwModel({ client });

            const object = {
                "test": "value"
            };

            model.cacheSet("key1", object);

            const cachedObject = model.cacheGet("key2");
            expect(cachedObject).not.toBe(object);
        });

        test("Setting cache item calls invalidate callback", async () => {

            const model = new AfwModel({ client });
            const cb = jest.fn();

            const object = {
                "test": "value"
            };
            const newObject = {
                "test2": "value2"
            };

            model.cacheSet("key1", object);
            model.cacheOnInvalidate("key1", cb);

            model.cacheSet("key1", newObject);
            expect(cb).toHaveBeenCalled();
            expect(model.cacheGet("key1")).toBe(newObject);
        });

        test("Invalidate", async () => {
            const model = new AfwModel({ client });

            const object = {
                "test": "value"
            };

            model.cacheSet("key1", object);
            model.cacheSet("key2", object);

            let cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);

            cachedObject = model.cacheGet("key2");
            expect(cachedObject).toBe(object);

            model.cacheInvalidate();

            cachedObject = model.cacheGet("key1");
            expect(cachedObject).not.toBe(object);

            cachedObject = model.cacheGet("key2");
            expect(cachedObject).not.toBe(object);
        });

        test("Invalidate a key", async () => {
            const model = new AfwModel({ client });

            const object = {
                "test": "value"
            };

            model.cacheSet("key1", object);
            model.cacheSet("key2", object);

            let cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);

            model.cacheInvalidate("key1");

            cachedObject = model.cacheGet("key1");
            expect(cachedObject).not.toBe(object);

            cachedObject = model.cacheGet("key2");
            expect(cachedObject).toBe(object);
        });

        test("Invalidate a key with a new value", async () => {

            const model = new AfwModel({ client });
            const cb = jest.fn();            

            const object = {
                "test": "value"
            };

            const object2 = {
                "test2": "value2"
            };

            model.cacheSet("key1", object);
            model.cacheOnInvalidate("key1", cb);

            let cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);

            model.cacheInvalidate("key1", object2);

            cachedObject = model.cacheGet("key1");
            expect(cachedObject).not.toBe(object);
            expect(cachedObject).toBe(object2);
            expect(cb).toHaveBeenCalled();
        });

        test("Invalidate callback", async () => {

            const model = new AfwModel({ client });
            const cb = jest.fn();            

            const object = {
                "test": "value"
            };

            model.cacheSet("key1", object);

            let cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);

            model.cacheOnInvalidate("key1", cb);
            model.cacheInvalidate("key1");

            cachedObject = model.cacheGet("key1");
            expect(cachedObject).not.toBe(object);

            expect(cb).toHaveBeenCalled();

        });

        test("Invalidate does not callback", async () => {

            const model = new AfwModel({ client });
            const cb = jest.fn();            

            const object = {
                "test": "value"
            };

            model.cacheSet("key1", object);

            let cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);

            model.cacheOnInvalidate("key1", cb);
            model.cacheInvalidate("key2");

            cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);

            expect(cb).not.toHaveBeenCalled();

        });

        test("Invalidate callback for all keys", async () => {

            const model = new AfwModel({ client });
            const cb = jest.fn();            

            const object = {
                "test": "value"
            };

            model.cacheSet("key1", object);

            let cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);

            model.cacheOnInvalidate("key1", cb);
            model.cacheInvalidate();

            cachedObject = model.cacheGet("key1");
            expect(cachedObject).not.toBe(object);

            expect(cb).toHaveBeenCalled();

        });

        test("Invalidate callback remove", async () => {

            const model = new AfwModel({ client });
            const cb = jest.fn();            

            const object = {
                "test": "value"
            };

            model.cacheSet("key1", object);

            let cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);

            model.cacheOnInvalidate("key1", cb);
            cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);

            model.cacheOffInvalidate(cb);

            cachedObject = model.cacheGet("key1");
            expect(cachedObject).toBe(object);

            expect(cb).not.toHaveBeenCalled();
        });
    });
});
