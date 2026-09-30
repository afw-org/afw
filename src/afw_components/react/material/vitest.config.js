// See the 'COPYING' file in the project root for licensing information.
import {defineConfig} from "vitest/config";
import {transformWithOxc} from "vite";

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
                inline: [
                    "@afw/client",
                    "@afw/react",
                    "@afw/react-material-ui",
                    "@afw/react-monaco",
                    "@afw/test"
                ]
            }
        },
        setupFiles: [
            "../../../afw_test/javascript/src/vitestGlobals.js",
            "../../../afw_test/javascript/src/setupTests.js"
        ],
        // Vitest's own default only matches *.test.js/*.spec.js - this
        // codebase instead follows Jest's other convention (any file inside
        // a __tests__/ directory, no suffix required), so that needs adding.
        include: [
            "**/*.{test,spec}.?(c|m)[jt]s?(x)",
            "**/__tests__/**/*.[jt]s?(x)"
        ],
        exclude: ["**/node_modules/**", "**/build/**"]
    }
});
