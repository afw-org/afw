// See the 'COPYING' file in the project root for licensing information.
import {vi} from "vitest";
import {configure} from "@testing-library/dom";
import {server} from "@afw/test";
import {ReadableStream} from "web-streams-polyfill";
import "@testing-library/jest-dom";

/*
 * Vitest's jsdom environment doesn't reliably pick up plain `global.X = Y`
 * assignments made from a setup file (the test file's own global scope can
 * end up resolving a different, jsdom-provided binding) - vi.stubGlobal is
 * Vitest's own API for this and reaches the right realm.
 */
vi.stubGlobal("ReadableStream", ReadableStream);

/*
 * AfwClient calls the bare global `fetch()`. MSW's setupServer() patches
 * this itself via @mswjs/interceptors' fetch interceptor, which builds its
 * mocked Response using whatever global `Response` is in scope - Node's own
 * native fetch/Response/Headers/Request (undici-based) are already real,
 * spec-compliant globals here, with a genuine ReadableStream body, so they
 * are left alone rather than shadowed with a polyfill.
 */

/*
 * Node provides real global TextEncoder/TextDecoder natively - no polyfill
 * needed here. The `node:` prefix matters if this ever does: a bare
 * require("util")/import "util" resolves through Vite/Vitest's bundler-style
 * package resolution, which finds the (unrelated, browser-polyfill) "util"
 * npm package before Node's own built-in of the same name - unlike Jest,
 * which checks core modules first.
 */

const timeout = 500000;

vi.setConfig({testTimeout: timeout});
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
    onUnhandledRequest(request) {
        server.listHandlers().forEach((handler) => console.log(handler.info.header));
        console.error("Found an unhandled %s request to %s", request.method, request.url);
    }
}));
/*
 * Real browsers resolve a relative fetch() URL ("/afw") against the
 * document's location automatically. MSW's fetch interceptor normalizes
 * every call into a native (undici) Request before it even looks at
 * handlers - regardless of what `fetch` itself is bound to above - and that
 * native Request has no notion of a "current document" to resolve a bare
 * path against, so it throws ("Invalid URL"). Resolve it against jsdom's
 * window.location first, same as a browser would. Registered (and so
 * running) after the server.listen() beforeAll above, since listen()
 * re-patches global fetch with its own interceptor and would otherwise
 * clobber this wrapper if installed earlier.
 */
beforeAll(() => {
    const patchedFetch = fetch;
    vi.stubGlobal("fetch", (input, init) => {
        if (typeof input === "string" && !/^[a-z][a-z0-9+.-]*:/i.test(input)) {
            input = new URL(input, window.location.href).href;
        }
        return patchedFetch(input, init);
    });
});
// Reset any request handlers that we may add during the tests,
// so they don't affect other tests.
afterEach(() => server.resetHandlers());
// Clean up after the tests are finished.
afterAll(() => server.close());
