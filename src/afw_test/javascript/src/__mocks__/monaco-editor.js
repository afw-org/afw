// See the 'COPYING' file in the project root for licensing information.
/**
 * Stub for "monaco-editor" in tests. Real Monaco needs a browser (Canvas,
 * Web Workers, real layout) jsdom can't provide, and no test should be
 * instantiating a real editor instance - this only needs to cover the API
 * surface @afw/react-monaco touches during a normal render, so components
 * that use it don't crash mounting/unmounting.
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
    IndentAction: new Proxy({}, {get: () => 0}),
    json: {
        jsonDefaults: {
            setDiagnosticsOptions: noop
        }
    }
};

const model = {
    getFullModelRange: () => ({}),
    applyEdits: noop,
    setEOL: noop,
    dispose: noop,
    getPositionAt: () => ({lineNumber: 1, column: 1})
};

export const editor = {
    create: vi.fn(() => ({
        getModel: () => model,
        setModel: noop,
        getValue: () => "",
        onDidChangeModelContent: disposable,
        updateOptions: noop,
        setPosition: noop,
        revealLineInCenter: noop,
        deltaDecorations: () => [],
        focus: noop,
        dispose: noop
    })),
    createDiffEditor: vi.fn(() => ({
        setModel: noop,
        updateOptions: noop,
        dispose: noop
    })),
    createModel: () => model,
    getModel: () => null,
    createDiffNavigator: () => ({next: noop, previous: noop}),
    defineTheme: noop,
    setModelLanguage: noop,
    EndOfLineSequence: {LF: 0, CRLF: 1}
};

export const Uri = {parse: (uri) => ({toString: () => uri})};

export class Range {
    constructor(startLineNumber, startColumn, endLineNumber, endColumn) {
        this.startLineNumber = startLineNumber;
        this.startColumn = startColumn;
        this.endLineNumber = endLineNumber;
        this.endColumn = endColumn;
    }
}
