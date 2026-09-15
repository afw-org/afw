// See the 'COPYING' file in the project root for licensing information.
/**
 * index.js
 * 
 * Registers the Monaco Editor AFW Component Implementation package.
 */

/* import every component to be exported via componentRegistry */
import {
    CodeEditor,
    DiffEditor
} from "./components";

export {MonacoProvider} from "./components/MonacoProvider";
export * from "./hooks";

/*
 * initialize()
 *
 * Returns the monaco module namespace, ready for the editor/language APIs
 * used throughout this package. A dynamic import so monaco-editor (which
 * bundles every basic language, ~2MB) lands in its own chunk instead of
 * inflating the app's main bundle - MonacoProvider mounts this unconditionally
 * at the app root, so it still loads immediately, just as a separate
 * parallel fetch rather than part of the main bundle's parse/eval cost.
 */
export const initialize = async () => import("monaco-editor");

const setTheme = () => {
};

export const componentRegistry = {
    implementationId:   "@afw/react-monaco",
    initialize: () => undefined,
    setTheme,
    components: {
        CodeEditor,
        DiffEditor,
    }
};

export default componentRegistry;
