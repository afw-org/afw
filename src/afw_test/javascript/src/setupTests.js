// See the 'COPYING' file in the project root for licensing information.
import {configure} from "@testing-library/dom";
import {server} from "@afw/test";
import {ReadableStream} from "web-streams-polyfill/es6";

global.ReadableStream = ReadableStream;

if (typeof TextEncoder === "undefined")
    global.TextEncoder = require("util").TextEncoder;
if (typeof TextDecoder === "undefined")
    global.TextDecoder = require("util").TextDecoder;

const timeout = 500000;

jest.setTimeout(timeout);
configure({ asyncUtilTimeout: timeout / 10 });

/* setup some fake routines for the tests that require them */
if (global.document) {

    global.document.body.createTextRange = function() {
        return {
            setEnd: function(){},
            setStart: function(){},
            getBoundingClientRect: function(){
                return {right: 0};
            },
            getClientRects: function(){
                return {
                    length: 0,
                    left: 0,
                    right: 0
                };
            }
        };
    };

    global.document.elementFromPoint = function() {
        return null;
    };
}

Date.now = function() {    
    return (1487076708000);
};

// Establish API mocking before all tests.
beforeAll(() => server.listen({
    onUnhandledRequest(req) {
        server.printHandlers();
        console.error("Found an unhandled %s request to %s", req.method, req.url.href);
    }
}));
// Reset any request handlers that we may add during the tests,
// so they don't affect other tests.
afterEach(() => server.resetHandlers());
// Clean up after the tests are finished.
afterAll(() => server.close());
