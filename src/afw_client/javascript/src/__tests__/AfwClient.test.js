// See the 'COPYING' file in the project root for licensing information.
import {server, rest} from "@afw/test";

import {AfwClient} from "../AfwClient";

describe("AfwClient Tests", () => {

    test("Create a client", async () => {
        const client = new AfwClient();

        expect(client).toBeInstanceOf(AfwClient);
    });

    test("onRequest should be called", async () => {
        const onRequest = jest.fn();

        server.use(            
            rest.get("http://localhost/afw", (req, res, ctx) => {                
                return res(
                    ctx.status(200),
                );
            })
        );
        
        const client = new AfwClient();        
        client.addOn("onRequest", onRequest);
        await client.get("http://localhost/afw").response;        
        
        expect(onRequest).toHaveBeenCalled();
    });

    test("onRequest should not be called, after removing on", async () => {
        const onRequest = jest.fn();

        server.use(            
            rest.get("http://localhost/afw", (req, res, ctx) => {                
                return res(
                    ctx.status(200),
                );
            })
        );
        
        const client = new AfwClient();        
        client.addOn("onRequest", onRequest);
        client.removeOn("onRequest", onRequest);

        expect(onRequest).not.toHaveBeenCalled();
        await client.get("http://localhost/afw").response;        
        
        expect(onRequest).not.toHaveBeenCalled();
    });

    test("onResponse should be called", async () => {
        const onResponse = jest.fn();

        server.use(            
            rest.get("http://localhost/afw", (req, res, ctx) => {                
                return res(
                    ctx.status(200),
                );
            })
        );
        
        const client = new AfwClient();        
        client.addOn("onResponse", onResponse);
        await client.get("http://localhost/afw").response;        
        
        expect(onResponse).toHaveBeenCalled();
    });

    test("onResponseError should be called", async () => {
        const onResponseError = jest.fn();

        server.use(
            rest.get("/afw", (req, res, ctx) => {
                return res(
                    ctx.status(500),
                );
            })
        );
        
        const client = new AfwClient();        
        client.addOn("onResponseError", onResponseError);
        const {response} = client.get("http://localhost/afw");
        await response;    
        
        expect(onResponseError).toHaveBeenCalled();
    });

    test("onResponseError should not be called", async () => {
        const onResponseError = jest.fn();

        server.use(
            rest.get("/afw", (req, res, ctx) => {
                return res(
                    ctx.status(200),
                    ctx.json({})
                );
            })
        );
        
        const client = new AfwClient();
        client.addOn("onResponseError", onResponseError);
        const {response} = client.get("http://localhost/afw");
        await response;    
        
        expect(onResponseError).not.toHaveBeenCalled();
    });

    describe("Test Perform()", () => {

        test("Test result", async () => {
            server.use(
                rest.post("/afw", (req, res, ctx) => {
                    return res(
                        ctx.status(200),
                        ctx.json({ result: 2, status: "success" })
                    );
                })
            );

            const client = new AfwClient();

            const result = await client.perform({ 
                "function": "add",
                "args": [1, 1]
            }).result();

            expect(result).toBe(2);
        });

        test("Test response", async () => {
            server.use(
                rest.post("http://localhost/afw", (req, res, ctx) => {
                    return res(
                        ctx.status(200),
                        ctx.json({ result: 2, status: "success" })
                    );
                })
            );

            const client = new AfwClient();

            const {result, status} = await client.perform({ 
                "function": "add",
                "args": [1, 1]
            }).response();

            expect(result).toBe(2);
            expect(status).toBe("success");
        });

        test("Perform returns an error", async () => {
            server.use(
                rest.post("http://localhost/afw", (req, res, ctx) => {
                    return res(
                        ctx.status(200),
                        ctx.json({ status: "error", error: "It didn't work" })
                    );
                })
            );

            const client = new AfwClient();

            let thrownError;
            try {
                await client.perform({ 
                    "function": "add",
                    "args": [1, 1]
                }).result();
            } catch (error) {
                thrownError = error;
            }

            expect(thrownError).toEqual("It didn't work");
        });

        test("Perform over streams", async () => {
            server.use(
                rest.post("/afw", (req, res, ctx) => {
                    return res(
                        ctx.status(200),
                        ctx.set("Content-Type", "application/x-afw"),
                        ctx.body("1 31 response\n{\"result\":2,\"status\":\"success\"}2 0 end\n")
                    );
                })
            );

            const client = new AfwClient();

            const result = await client.perform({ 
                "function": "add",
                "args": [1, 1]
            }).result();

            expect(result).toBe(2);
        });

        test("Handle error with HTML response", async () => {
        
            server.use(
                rest.post("/afw", (req, res, ctx) => {                    
                    return res(
                        ctx.status(500),                        
                        ctx.body("<html><h1>An Error occurred.</h1></html>")
                    );
                })
            );
    
            const client = new AfwClient();

            let thrownError;
            try {
                await client.perform({ 
                    "function": "add",
                    "args": [1, 1]
                }).result();           
            } catch (error) {
                thrownError = error;
            }
         
            expect(thrownError).toBe("<html><h1>An Error occurred.</h1></html>");
        });

        test("controller abort()", async () => {
            
            server.use(
                rest.post("/afw", (req, res, ctx) => {                    
                    return res(
                        ctx.status(200),
                        ctx.json({ status: "success" })
                    );
                })
            );
            
            const client = new AfwClient();

            let controller, result;
            let thrownError;
            try {                                
                ({result, controller} = client.perform({ 
                    "function": "add",
                    "args": [1, 1]
                }));
                controller.abort();                
                await result();
            } catch (e) {     
                thrownError = e;           
            }

            expect(controller.signal.aborted).toBe(true);                
            expect(thrownError).toBeDefined();
        });
        
    });    

});
