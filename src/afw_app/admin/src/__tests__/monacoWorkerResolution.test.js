// See the 'COPYING' file in the project root for licensing information.
// @vitest-environment node

/*
 * The Vite dev server serves Monaco's web worker bundles (editor.worker,
 * json.worker, ...) by resolving each one's entry point with Node's
 * require.resolve() against the *actual installed* monaco-editor package
 * (see resolveMonacoPath in
 * node_modules/vite-plugin-monaco-editor/dist/index.js). That resolution
 * only happens against the real package on disk - Vitest's own jsdom test
 * config aliases "monaco-editor" away entirely (see vitest.config.js),
 * since jsdom can't provide the Canvas/Worker APIs the real editor needs,
 * so nothing else in this suite exercises this path.
 *
 * A monaco-editor release whose package.json "exports" map doesn't match
 * what this (unmaintained, last-updated-for-older-monaco) plugin expects
 * breaks this silently in every jsdom/unit test, surfacing only when a
 * real browser actually opens a source window in Fiddle - as a thrown
 * "Cannot find module '.../monaco-editor/esm/vs/esm/vs/editor/editor.
 * worker.js'" (a doubled path: monaco-editor's own "./*": "./esm/vs/*.js"
 * exports wildcard applied a second time on top of the plugin's
 * already-"esm/vs"-prefixed entry string). This landed here once already
 * when a newer, unpinned monaco-editor got hoisted to the workspace root
 * in place of @afw/react-monaco's pinned ^0.33.0 - fixed via the root
 * package.json "overrides" entry pinning monaco-editor workspace-wide.
 */
import {resolveMonacoPath, getWorks} from "vite-plugin-monaco-editor/dist/index.js";

// keep in sync with the monacoEditorPlugin(...) call in vite.config.js
const languageWorkers = ["editorWorkerService", "json"];

describe("Monaco worker resolution (vite-plugin-monaco-editor)", () => {

    test.each(getWorks({languageWorkers, customWorkers: []}))(
        "resolves a real file on disk for the $label worker",
        ({entry}) => {
            const resolved = resolveMonacoPath(entry);
            expect(resolved).toMatch(/\.js$/);
            expect(resolved).not.toMatch(/esm[/\\]vs[/\\].*esm[/\\]vs/);
        }
    );

});
