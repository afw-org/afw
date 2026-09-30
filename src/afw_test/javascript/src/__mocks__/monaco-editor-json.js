// See the 'COPYING' file in the project root for licensing information.
/**
 * Stub for "monaco-editor/languages/features/json/register" in tests -
 * @afw/react-monaco re-exports it as the `json` namespace (see its
 * src/monaco.js), which CodeEditor's schema validation uses.
 */
const noop = () => undefined;

export const jsonDefaults = {
    setDiagnosticsOptions: noop
};
