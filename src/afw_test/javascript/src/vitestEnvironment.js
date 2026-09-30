// See the 'COPYING' file in the project root for licensing information.
/**
 * Vitest's jsdom environment, but with Node's own AbortController and
 * AbortSignal left in place. Point each package's vitest.config.js
 * `environment` at this file instead of "jsdom".
 *
 * jsdom supplies its own AbortController/AbortSignal, while fetch() here is
 * Node's native (undici) fetch - jsdom has none. undici 7 (Node 24+) brand
 * checks `init.signal` against Node's AbortSignal, so a jsdom signal throws
 * "RequestInit: Expected signal to be an instance of AbortSignal" (undici 6,
 * in Node 20/22, accepted it). A real browser has one AbortSignal, so keeping
 * the one that matches fetch is the browser-faithful choice.
 *
 * This has to be an environment rather than a setup file: vitest overwrites
 * these globals before any setup file runs and keeps Node's originals
 * private until teardown.
 */
import {builtinEnvironments} from "vitest/environments";

const jsdom = builtinEnvironments.jsdom;
const nodeGlobals = ["AbortController", "AbortSignal"];

export default {
    ...jsdom,
    name: "afw-jsdom",
    async setup(global, options) {
        const saved = nodeGlobals.map(key => [key, global[key]]);
        const env = await jsdom.setup(global, options);
        saved.forEach(([key, value]) => global[key] = value);

        /* jsdom's teardown restores its saved originals - the same values. */
        return env;
    }
};
