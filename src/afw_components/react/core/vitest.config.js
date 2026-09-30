// See the 'COPYING' file in the project root for licensing information.
import {defineConfig} from "vitest/config";
import {transformWithOxc} from "vite";

// The codebase uses JSX in plain .js files (a CRA/babel convention, instead
// of .jsx) - Vite's Oxc transform picks the language from the file
// extension, so .js never gets JSX, and Vite's `oxc` option has no `lang`
// override. This only transforms .js files, leaving Vite's normal .ts/.tsx
// handling untouched.
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
        setupFiles: [
            "../../../afw_test/javascript/src/vitestGlobals.js",
            "../../../afw_test/javascript/src/setupTests.js"
        ],
        exclude: ["**/node_modules/**", "**/build/**"]
    }
});
