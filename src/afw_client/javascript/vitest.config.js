// See the 'COPYING' file in the project root for licensing information.
import {defineConfig} from "vitest/config";

export default defineConfig({
    test: {
        globals: true,
        environment: "jsdom",
        // Forked child processes are killed reliably by the OS even when
        // Node's own event loop doesn't naturally end (worker_threads, the
        // default pool, can leave the test run stuck at teardown - "Failed
        // to terminate worker" / Vite server not exiting - once native
        // fetch's undici Agent is exercised for real).
        pool: "forks",
        setupFiles: [
            "../../afw_test/javascript/src/vitestGlobals.js",
            "../../afw_test/javascript/src/setupTests.js"
        ],
        exclude: ["**/node_modules/**", "**/build/**"]
    }
});
