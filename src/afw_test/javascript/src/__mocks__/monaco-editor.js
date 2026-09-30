// See the 'COPYING' file in the project root for licensing information.
/**
 * Stub for "monaco-editor" and its monaco-editor/... entry points (other
 * than the JSON language service - see monaco-editor-json.js) in tests.
 * Real Monaco needs a browser (Canvas, Web Workers, real layout) jsdom can't
 * provide, and no test should be instantiating a real editor instance - this
 * only needs to cover the API surface @afw/react-monaco touches during a
 * normal render, so components that use it don't crash mounting/unmounting.
 */
import {vi} from "vitest";

const noop = () => undefined;
const disposable = () => ({dispose: noop});

export const languages = {
    register: noop,
    setLanguageConfiguration: disposable,
    setMonarchTokensProvider: disposable,
    registerCompletionItemProvider: disposable,
    registerSignatureHelpProvider: disposable,
    registerHoverProvider: disposable,
    CompletionItemKind: new Proxy({}, {get: () => 0}),
    CompletionItemInsertTextRule: new Proxy({}, {get: () => 0}),
    IndentAction: new Proxy({}, {get: () => 0})
};

const model = {
    getFullModelRange: () => ({}),
    applyEdits: noop,
    setEOL: noop,
    dispose: noop,
    getPositionAt: () => ({lineNumber: 1, column: 1})
};

export const editor = {
    // Each call gets its own value/changeHandler closure (not the shared
    // `model` above) so onDidChangeModelContent and applyEdits() actually
    // do something instead of silently discarding it - tests that need to
    // simulate typing (jsdom can't drive Monaco's real DOM/canvas surface)
    // drive it through the returned instance's __setValueAndFireChange(),
    // a test-only helper that isn't part of the real monaco API.
    create: vi.fn((container, options) => {
        let value = options?.value ?? "";
        let changeHandler;

        const instanceModel = {
            getFullModelRange: () => ({}),
            applyEdits: (edits) => {
                if (edits?.[0] && typeof edits[0].text === "string")
                    value = edits[0].text;
            },
            setEOL: noop,
            dispose: noop,
            getPositionAt: () => ({lineNumber: 1, column: 1})
        };

        return {
            getModel: () => instanceModel,
            setModel: noop,
            getValue: () => value,
            onDidChangeModelContent: (fn) => { changeHandler = fn; return disposable(); },
            // TabbedCodeEditor's onCreateEditor callback (and anything else
            // that receives the created editor instance directly, rather
            // than going through the componentType-registry CodeEditor)
            // exercises more of the real monaco.editor.IStandaloneCodeEditor
            // surface than the base create()/getModel() path above does.
            onDidChangeCursorPosition: disposable,
            onDidChangeCursorSelection: disposable,
            getSelection: () => ({startLineNumber: 1, endLineNumber: 1, startColumn: 1, endColumn: 1}),
            addAction: disposable,
            addCommand: noop,
            updateOptions: noop,
            setPosition: noop,
            revealLineInCenter: noop,
            deltaDecorations: () => [],
            focus: noop,
            dispose: noop,
            __setValueAndFireChange: (newValue) => {
                value = newValue;
                changeHandler?.({changes: []});
            },
        };
    }),
    createDiffEditor: vi.fn(() => ({
        setModel: noop,
        updateOptions: noop,
        goToDiff: noop,
        dispose: noop
    })),
    createModel: () => model,
    getModel: () => null,
    defineTheme: noop,
    setModelLanguage: noop,
    EndOfLineSequence: {LF: 0, CRLF: 1}
};

export const Uri = {parse: (uri) => ({toString: () => uri})};

// TabbedCodeEditor combines these with a bitwise OR to build a keybinding
// (e.g. `KeyMod.CtrlCmd | KeyCode.KeyS`) for editor.addCommand(). Only the
// real names in use are defined (values from monaco-editor), so a misspelled
// or renamed one - KEY_S became KeyS - fails a test instead of silently
// binding nothing.
const strict = (name, values) => new Proxy(values, {
    get: (target, key) => {
        if (typeof key === "string" && !(key in target))
            throw new Error(`monaco-editor mock: ${name}.${key} is not defined`);
        return target[key];
    }
});
export const KeyMod = strict("KeyMod", {CtrlCmd: 2048});
export const KeyCode = strict("KeyCode", {KeyS: 49});

export class Range {
    constructor(startLineNumber, startColumn, endLineNumber, endColumn) {
        this.startLineNumber = startLineNumber;
        this.startColumn = startColumn;
        this.endLineNumber = endLineNumber;
        this.endColumn = endColumn;
    }
}
