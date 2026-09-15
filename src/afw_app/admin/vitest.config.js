// See the 'COPYING' file in the project root for licensing information.
// Deliberately separate from vite.config.js (used for the dev server/build) -
// this stays a plain, test-only config so build-time-only plugins (Monaco's
// worker bundling, the JSX-in-.js build workaround) don't get pulled into
// test runs, where they're unnecessary.
import {defineConfig} from "vitest/config";
import {transform as esbuildTransform} from "esbuild";
import path from "path";

// The codebase uses JSX in plain .js files (a CRA/babel convention, instead
// of .jsx) - esbuild's default loader for .js doesn't enable JSX. Vite's
// top-level `esbuild` option only accepts a single loader for every matched
// file, and a sibling workspace package consumed here (@afw/client) has
// real .ts source using things like `enum`, so a single blanket "jsx" loader
// can't cover both without breaking the other - this only transforms .js
// files specifically, leaving Vite's normal .ts/.tsx handling untouched.
function jsxInJs() {
    return {
        name: "jsx-in-js",
        enforce: "pre",
        async transform(code, id) {
            if (!id.endsWith(".js")) return;
            const result = await esbuildTransform(code, {
                loader: "jsx",
                jsx: "automatic",
                jsxImportSource: "react",
                sourcefile: id,
                sourcemap: true
            });
            return {code: result.code, map: result.map};
        }
    };
}

export default defineConfig({
    plugins: [jsxInJs()],
    resolve: {
        alias: {
            // Real Monaco needs a browser (Canvas, Web Workers, real layout)
            // jsdom can't provide, and its package.json only declares a
            // "module" field - Vite's browser-target dev/build resolution
            // tolerates that, but Vitest's Node-target resolution can't find
            // a usable entry point at all. Every admin test mounts
            // MonacoProvider (via test-utils.js's AllTheProviders), which
            // eagerly imports "monaco-editor" on mount, so this needs a
            // stand-in regardless.
            "monaco-editor": path.resolve(__dirname, "../../afw_test/javascript/src/__mocks__/monaco-editor.js")
        },
        // Workspace packages (inlined below) and the app itself must resolve
        // to the exact same react/react-dom instance, or hooks break
        // ("Cannot read properties of null (reading 'useContext')") across
        // the package boundary - same fix as vite.config.js's dev/build path.
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
