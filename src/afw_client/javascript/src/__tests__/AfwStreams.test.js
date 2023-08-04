// See the 'COPYING' file in the project root for licensing information.

import {AfwStreams} from "../AfwStreams";
import {ReadableStream} from "web-streams-polyfill/es6";
import {TextEncoder, TextDecoder} from "util";

describe("AfwStreams tests", () => {

    test("Test readFull", async () => {
        const encoder = new TextEncoder();
        const decoder = new TextDecoder("utf-8");

        const stream = new ReadableStream({
            start(controller) {                
                controller.enqueue(
                    encoder.encode(
                        "1 1 result\n62 20 response\n{\"status\":\"success\"}3 0 end"
                    )
                );
                controller.close();
            }
        });

        const streams = new AfwStreams(stream);
        const fullText = await streams.readFull();
      
        expect(decoder.decode(fullText)).toEqual("1 1 result\n62 20 response\n{\"status\":\"success\"}3 0 end");
    });

    describe("Test readFrame", () => {

        test("Single chunk", async () => {
        
            const encoder = new TextEncoder();
            const decoder = new TextDecoder();

            const stream = new ReadableStream({
                start(controller) {                
                    controller.enqueue(
                        encoder.encode(
                            "1 1 result\n62 20 response\n{\"status\":\"success\"}3 0 end"
                        )
                    );
                    controller.close();
                }
            });

            const streams = new AfwStreams(stream);

            // read first frame
            let {sequence, size, streamId, info, data} = await streams.readFrame();

            expect(sequence).toBe(1);
            expect(size).toBe(1);
            expect(streamId).toBe("result");
            expect(info).toBeUndefined();            
            expect(JSON.parse(decoder.decode(data))).toBe(6);

            // read second frame
            ({sequence, size, streamId, info, data} = await streams.readFrame());

            expect(sequence).toBe(2);
            expect(size).toBe(20);
            expect(streamId).toBe("response");
            expect(info).toBeUndefined();
            expect(JSON.parse(decoder.decode(data))).toEqual(expect.objectContaining({
                "status": "success"
            }));

            // read last frame
            ({sequence, size, streamId, info, data} = await streams.readFrame());
            expect(sequence).toBe(3);
            expect(size).toBe(0);
            expect(streamId).toBe("end");
            expect(info).toBeUndefined();
            expect(data).toBeUndefined();
        });

        test("Multiple chunks (one per frame)", async () => {

            const encoder = new TextEncoder();
            const decoder = new TextDecoder();

            const stream = new ReadableStream({
                start(controller) {                
                    controller.enqueue(
                        encoder.encode(
                            "1 1 result\n6",
                        )                    
                    );
                    controller.enqueue(
                        encoder.encode(
                            "2 20 response\n{\"status\":\"success\"}",
                        )
                    );
                    controller.enqueue(
                        encoder.encode(
                            "3 0 end",
                        )
                    );
                    controller.close();
                }
            });

            const streams = new AfwStreams(stream);

            // read first frame
            let {sequence, size, streamId, info, data} = await streams.readFrame();

            expect(sequence).toBe(1);
            expect(size).toBe(1);
            expect(streamId).toBe("result");
            expect(info).toBeUndefined();
            expect(JSON.parse(decoder.decode(data))).toBe(6);

            // read second frame            
            ({sequence, size, streamId, info, data} = await streams.readFrame());

            expect(sequence).toBe(2);
            expect(size).toBe(20);
            expect(streamId).toBe("response");
            expect(info).toBeUndefined();
            expect(JSON.parse(decoder.decode(data))).toEqual(expect.objectContaining({
                "status": "success"
            }));
            
            // read last frame
            ({sequence, size, streamId, info, data} = await streams.readFrame());
            expect(sequence).toBe(3);
            expect(size).toBe(0);
            expect(streamId).toBe("end");
            expect(info).toBeUndefined();
            expect(data).toBeUndefined();
        });

        test("Multiple chunks in between frames", async () => {

            const encoder = new TextEncoder();
            const decoder = new TextDecoder();

            const stream = new ReadableStream({
                start(controller) {                
                    controller.enqueue(
                        encoder.encode(
                            "1 1 res",
                        )                    
                    );
                    controller.enqueue(
                        encoder.encode(
                            "ult\n62 20 response\n{\"status\"",
                        )
                    );                    
                    controller.enqueue(
                        encoder.encode(
                            ":\"success\"}3 0 end",
                        )
                    );
                    controller.close();
                }
            });

            const streams = new AfwStreams(stream);

            // read first frame
            let {sequence, size, streamId, info, data} = await streams.readFrame();

            expect(sequence).toBe(1);
            expect(size).toBe(1);
            expect(streamId).toBe("result");
            expect(info).toBeUndefined();
            expect(JSON.parse(decoder.decode(data))).toBe(6);

            // read second frame            
            ({sequence, size, streamId, info, data} = await streams.readFrame());

            expect(sequence).toBe(2);
            expect(size).toBe(20);
            expect(streamId).toBe("response");
            expect(info).toBeUndefined();
            expect(JSON.parse(decoder.decode(data))).toEqual(expect.objectContaining({
                "status": "success"
            }));
            
            // read last frame
            ({sequence, size, streamId, info, data} = await streams.readFrame());
            expect(sequence).toBe(3);
            expect(size).toBe(0);
            expect(streamId).toBe("end");
            expect(info).toBeUndefined();
            expect(data).toBeUndefined();
        });     
        
        test("Multiple chunks, delimiter on new chunk offset 0", async () => {

            const encoder = new TextEncoder();
            const decoder = new TextDecoder();

            const stream = new ReadableStream({
                start(controller) {                
                    controller.enqueue(
                        encoder.encode(
                            "1 1",
                        )                    
                    );
                    controller.enqueue(
                        encoder.encode(
                            " result\n62 20 response\n{\"status\"",
                        )
                    );                    
                    controller.enqueue(
                        encoder.encode(
                            ":\"success\"}3 0 end",
                        )
                    );
                    controller.close();
                }
            });

            const streams = new AfwStreams(stream);

            // read first frame
            let {sequence, size, streamId, info, data} = await streams.readFrame();

            expect(sequence).toBe(1);
            expect(size).toBe(1);
            expect(streamId).toBe("result");
            expect(info).toBeUndefined();
            expect(JSON.parse(decoder.decode(data))).toBe(6);

            // read second frame            
            ({sequence, size, streamId, info, data} = await streams.readFrame());

            expect(sequence).toBe(2);
            expect(size).toBe(20);
            expect(streamId).toBe("response");
            expect(info).toBeUndefined();
            expect(JSON.parse(decoder.decode(data))).toEqual(expect.objectContaining({
                "status": "success"
            }));
            
            // read last frame
            ({sequence, size, streamId, info, data} = await streams.readFrame());
            expect(sequence).toBe(3);
            expect(size).toBe(0);
            expect(streamId).toBe("end");
            expect(info).toBeUndefined();
            expect(data).toBeUndefined();
        });   
    });

    describe("test readStreams()", () => {

        test("Simple single frame streams", async () => {
            const encoder = new TextEncoder();

            const stream = new ReadableStream({
                start(controller) {                
                    controller.enqueue(
                        encoder.encode(
                            "1 1 result\n62 20 response\n{\"status\":\"success\"}3 0 end",
                        )
                    );
                    controller.close();
                }
            });

            const s = new AfwStreams(stream);
            const {result, response} = await s.readStreams();

            expect(result).toBe(6);
            expect(response).toEqual(expect.objectContaining({
                "status": "success"
            }));
            
        });
    });
});
