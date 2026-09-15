// See the 'COPYING' file in the project root for licensing information.
import {defineConfig} from "vitest/config";

export default defineConfig({
    // The codebase uses JSX in plain .js files (a CRA/babel convention,
    // instead of .jsx) - esbuild's default loader for .js doesn't enable JSX.
    esbuild: {
        jsx: "automatic",
        loader: "jsx",
        include: /\.jsx?$/,
        exclude: []
    },
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
