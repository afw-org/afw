// See the 'COPYING' file in the project root for licensing information.
import {AfwClient} from "../../AfwClient";

import {AfwModel, AfwObject, AfwProperty, AfwArray, AfwArrayEntry} from "..";

describe("AfwArray Tests", () => {

    const client = new AfwClient();
    const model = new AfwModel({ client });

    test("Create new AfwArray", async () => {        
        
        const list = new AfwArray({ 
            value: [
                "abc",
                "def"
            ]
        });

        expect(list).toBeInstanceOf(AfwArray);
    });

    test("Create new AfwArray, iterate through values", async () => {

        const value = [ "abc", true, 42 ];

        const list = new AfwArray({ value });

        expect(list).toBeInstanceOf(AfwArray);

        /* iterate through properties */
        for (const [ind, val] of list.entries()) {            
            expect(val).toBeInstanceOf(AfwArrayEntry);

            expect(value[ind]).toBe(val.getValue());
        }         
    });

    test("Arrays of objects", async () => {

        const value = [
            {
                prop1: "abc",
                prop2: true,
            },
            {
                prop3: "def",
                prop4: 42
            }
        ];

        const list = new AfwArray({ value });

        expect(list).toBeInstanceOf(AfwArray);

        for (const [ind, val] of list.entries()) {
            expect(val).toBeInstanceOf(AfwArrayEntry);            
            expect(val.getValue()).toBeInstanceOf(AfwObject);

            // need to initialize any AfwObject
            await val.getValue().initialize(model);

            const objEntry = value[ind];
            for (const [objEntryP, objEntryV] of Object.entries(objEntry)) {   
                const property = val.getValue().getProperty( objEntryP );
                expect(property).toBeInstanceOf(AfwProperty);
                expect(property.getName()).toBe(objEntryP);
                expect(property.getValue()).toBe(objEntryV);
            }
        }

    });

    test("Test entries()", async () => {
        
        let value = [ "abc", "def", "ghi" ];
        let list = new AfwArray({ value });

        for (const [ind, val] of list.entries()) {
            expect(val).toBeInstanceOf(AfwArrayEntry);
            expect(val.getValue()).toBe(value[ind]);
        }

        value = [ ];
        list = new AfwArray({ value });

        for (const [ind, val] of list.entries()) {
            expect(val).toBeInstanceOf(AfwArrayEntry);
            expect(val.getValue()).toBe(value[ind]);
        }
    });

    test("Test getAt()", async () => {

        const value = [ "abc", "def", "ghi" ];
        const list = new AfwArray({ value });        

        expect(list.getAt(0).getValue()).toBe(value[0]);
        expect(list.getAt(1).getValue()).toBe(value[1]);
        expect(list.getAt(2).getValue()).toBe(value[2]);
    });

    test("Test forEach()", async () => {

        const value = [ "abc", "def", "ghi" ];
        const list = new AfwArray({ value });        

        list.forEach((v, ind) => {
            expect(v).toBeInstanceOf(AfwArrayEntry);
            expect(v.getValue()).toBe(value[ind]);
        });
    });

    test("Test map()", async () => {
        const value = [ "abc", "def", "ghi" ];
        const list = new AfwArray({ value });  

        const values = list.map(entry => entry.getValue());

        values.forEach((v, ind) => {
            expect(v).toBe(value[ind]);
        });
    });

    test("Test newValue()", async () => {
        const value = [ "abc", true, 42 ];

        const list = new AfwArray({ value });

        const newValue = list.newValue();
        expect(newValue).toBeInstanceOf(AfwArrayEntry);

        newValue.setValue("xyz");
        expect(list.getAt(3).getValue()).toBe("xyz");
    });

    test("Test removeValue", async () => {
        const value = [ "abc", true, 42 ];

        const list = new AfwArray({ value });
        expect(list.length).toBe(3);
        
        list.removeValue(list.getAt(1));        
        expect(list.length).toBe(2);
        expect(list.getAt(1).getValue()).toBe(42);

        list.removeValue(list.getAt(1));        
        expect(list.length).toBe(1);
        expect(list.getAt(0).getValue()).toBe("abc");

        list.removeValue(list.getAt(0));
        expect(list.length).toBe(0);
    });

    test("Test iterator()", async () => {

        const value = [ "abc", "def", "ghi" ];
        const list = new AfwArray({ value });        

        let ind = 0;
        for (const v of list) {
            expect(v).toBeInstanceOf(AfwArrayEntry);
            expect(v.getValue()).toBe(value[ind]);

            ind++;
        }
    });

    test("Test includes()", async () => {

        const value = [ "abc", "def", "ghi" ];
        const list = new AfwArray({ value }); 

        expect(list.includes("abc")).toBe(true);
        expect(list.includes(123)).toBe(false);
    });

    test("Test toJSON", async () => {

        const value = [
            "abc", "def", "ghi"
        ];

        const list = new AfwArray({ value });

        expect(list.toJSON()).toEqual([
            "abc", "def", "ghi"
        ]);
    });

    test("Test validate()", async () => {

        const value = [ "abc", "def", "ghi" ];
        const list = new AfwArray({ value }); 

        // \fixme not sure how to test this yet
        expect(list.validate()).toBe(true);
    });

    test("Test onChanged", async () => {

        const onChanged = jest.fn();

        const value = [ "abc", "def", "ghi" ];
        const list = new AfwArray({ value }); 

        list.addEventListener("onChanged", onChanged);

        list.removeValue(list.getAt(0));
        expect(onChanged).toHaveBeenCalled();
    });
    
});
