// See the 'COPYING' file in the project root for licensing information.
// Deliberately separate from vite.config.mjs (used for the dev server/build) -
// this stays a plain, test-only config so build-time-only plugins (the
// JSX-in-.js build workaround, the bundle visualizer) don't get pulled into
// test runs, where they're unnecessary.
import {defineConfig} from "vitest/config";
import {transformWithOxc} from "vite";
import path from "path";

// The codebase uses JSX in plain .js files (a CRA/babel convention, instead
// of .jsx) - Vite's Oxc transform picks the language from the file
// extension, so .js never gets JSX, and Vite's `oxc` option has no `lang`
// override. A sibling workspace package consumed here (@afw/client) has real
// .ts source using things like `enum`, so .js has to be handled on its own -
// this only transforms .js files, leaving Vite's normal .ts/.tsx handling
// untouched.
function jsxInJs() {
    return {
        name: "jsx-in-js",
        enforce: "pre",
        async transform(code, id) {
            if (!id.endsWith(".js")) return;
            const result = await transformWithOxc(code, id, {
                lang: "jsx",
                jsx: {runtime: "automatic", importSource: "react"},
                sourcemap: true
            });
            return {code: result.code, map: result.map};
        }
    };
}

export default defineConfig({
    plugins: [jsxInJs()],
    resolve: {
        alias: [
            // Real Monaco needs a browser (Canvas, Web Workers, real layout)
            // jsdom can't provide. Every admin test mounts MonacoProvider
            // (via test-utils.js's AllTheProviders), which eagerly imports
            // @afw/react-monaco's monaco-editor entry points on mount (see
            // its src/monaco.js), so these need stand-ins regardless. First
            // match wins: the JSON language service, the editor worker, then
            // everything else.
            {
                find: /^monaco-editor\/languages\/features\/json\/register$/,
                replacement: path.resolve(import.meta.dirname, "../../afw_test/javascript/src/__mocks__/monaco-editor-json.js")
            },
            {
                find: /^monaco-editor\/.*\?worker$/,
                replacement: path.resolve(import.meta.dirname, "../../afw_test/javascript/src/__mocks__/monaco-editor-worker.js")
            },
            {
                find: /^monaco-editor(\/.*)?$/,
                replacement: path.resolve(import.meta.dirname, "../../afw_test/javascript/src/__mocks__/monaco-editor.js")
            }
        ],
        // Workspace packages (inlined below) and the app itself must resolve
        // to the exact same react/react-dom instance, or hooks break
        // ("Cannot read properties of null (reading 'useContext')") across
        // the package boundary - same fix as vite.config.mjs's dev/build path.
        dedupe: ["react", "react-dom", "@emotion/react", "@emotion/styled"]
    },
    test: {
        globals: true,
        environment: "jsdom",
        // Forked child processes are killed reliably by the OS even when
        // Node's own event loop doesn't naturally end (worker_threads, the
        // default pool, can leave the test run stuck at teardown - "Failed
        // to terminate worker" / Vite server not exiting - once native
        // fetch's undici Agent is exercised for real).
        pool: "forks",
        // These sibling workspace packages are consumed as source (JSX-in-.js,
        // or real .ts with things like enums) rather than prebuilt dist -
        // without this, Vitest treats them as already-built externals and
        // loads them via plain Node/Rollup parsing, which understands
        // neither JSX nor TypeScript syntax.
        server: {
            deps: {
                // react/react-dom also need to be inlined, not just deduped:
                // `resolve.dedupe` only applies to Vite's own resolution -
                // packages left as externals go through plain Node require()
                // instead, which bypasses it entirely.
                inline: [
                    "@afw/client",
                    "@afw/react",
                    "@afw/react-material-ui",
                    "@afw/react-monaco",
                    "@afw/test",
                    "react",
                    "react-dom"
                ]
            }
        },
        setupFiles: [
            "../../afw_test/javascript/src/vitestGlobals.js",
            "../../afw_test/javascript/src/setupTests.js"
        ],
        exclude: ["**/node_modules/**", "**/build/**"]
    }
});
