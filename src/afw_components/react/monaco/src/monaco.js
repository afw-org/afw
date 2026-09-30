// See the 'COPYING' file in the project root for licensing information.
/**
 * monaco.js
 *
 * The monaco namespace this package uses, assembled from monaco-editor's
 * tree-shakeable entry points (0.56+) instead of the full "monaco-editor"
 * entry, which also loads the TypeScript, CSS and HTML language services and
 * their workers (several MB) that nothing here uses:
 *
 *   - the core editor API and every editor feature (find, folding, ...);
 *   - every Monarch syntax definition - each registers lazily and loads its
 *     tokenizer only when a model uses that language (xml, yaml, ...);
 *   - the JSON language service, for CodeEditor's schema validation.
 *
 * The JSON language service starts its own web worker with
 * `new Worker(new URL(..., import.meta.url))`, which Vite bundles as is. The
 * base editor worker needs MonacoEnvironment.getWorker below: Monaco only
 * references it through a bare `new URL()`, which Vite inlines as a data: URL
 * whose relative imports can't load, so it would silently fall back to
 * running on the main thread.
 */
import "monaco-editor/features/register.all";
import "monaco-editor/languages/definitions/register.all";
import EditorWorker from "monaco-editor/editor/editor.worker?worker";

export * as json from "monaco-editor/languages/features/json/register";
export * from "monaco-editor/editor";

/* Returning undefined for any other label keeps Monaco's own default. */
self.MonacoEnvironment ??= {
    getWorker: (_workerId, label) =>
        (label === "editorWorkerService") ? new EditorWorker() : undefined
};
