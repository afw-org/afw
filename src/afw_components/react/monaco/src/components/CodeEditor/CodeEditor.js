// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable @typescript-eslint/no-use-before-define */
// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for CodeEditor
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import React, {useEffect, useRef, useCallback} from "react";

import {useMonaco} from "../../hooks";
import {useQualifiers, useIsMounted} from "@afw/react";
import {debounce} from "@afw/client";

import "./CodeEditor.css";

const defaultStyle      = { width: "100%", height: "100%" };
const defaultUri        = "http://afw-monaco/object/uri.json";
const defaultSchemaUri  = "http://afw-monaco/schema/uri.json";

/**
 * Implementation Id : CodeEditor
 * Category          : editors
 * 
 * A component that renders a rich text editor.
 * 
 * This component displays a rich text editor that allows the user to
 * manually edit the source for a particular piece of data.
 * 
 */
export const CodeEditor = React.forwardRef((props, ref) => {

    const containerRef = useRef();
    const ignoreChange = useRef();
    const editorRef = useRef();
    const decorationsRef = useRef([]);
    
    const qualifiedVars = useQualifiers();    
    const isMounted = useIsMounted();

    const {
        id,
        "data-testid": dataTestId,
        style = defaultStyle,
        autoFocus, 
        foldingStrategy,
        label,
        language, 
        source, 
        showLineNumbers, 
        readOnly, 
        onChanged, 
        showMinimap,    
        errorOffset, 
        errorHoverMessage,
        cursorPosition,
        theme,
        onCreateEditor,
        glyphMargin,
        uri = defaultUri,
        schema,
        schemaUri = defaultSchemaUri,
    } = props;

    const { monaco, theme: globalTheme } = useMonaco();
    
    useEffect(() => {
        if (monaco) {
            ignoreChange.current = true;
            
            const editor = monaco.editor.create(containerRef.current, {
                theme: theme ? theme : globalTheme,
                language: language ? language : "json",                
                automaticLayout: true, 
                hideCursorInOverviewRuler: true,
                overviewRulerBorder: false,
                lineDecorationsWidth: 5,
                foldingStrategy,
                value: source,
                lineNumbers: (showLineNumbers ? "on" : "off"),
                readOnly,
                scrollBeyondLastLine: false,
                wordWrap: "on",
                glyphMargin: glyphMargin ? glyphMargin : false,
                minimap: {
                    enabled: showMinimap
                },
                fixedOverflowWidgets: true,
                "bracketPairColorization.enabled": true,       
                ariaLabel: label,         
            });
            if (ref) ref.current = containerRef.current;
            editorRef.current = editor;
            
            if (autoFocus)
                editor.focus();

            /* set EOL to NL */
            let model = editor.getModel();                        
            if (schema) {
                const modelUri = monaco.Uri.parse(uri);     
                model = monaco.editor.getModel(modelUri);
                if (!model) {
                    model = monaco.editor.createModel(source, language ? language : "json", modelUri);                                       
                }                                                         

                monaco.languages.json.jsonDefaults.setDiagnosticsOptions({
                    validate: true,                    
                    schemas: [{
                        uri: schemaUri,
                        fileMatch: [modelUri.toString()],
                        schema,                        
                    }],
                    enableSchemaRequest: false,
                });
            }            
            model.setEOL(monaco.editor.EndOfLineSequence.LF);                                

            editor.onDidChangeModelContent(debounce(e => {
                if (isMounted())
                    onDidChangeModelContent(e);
            }, 100));

            if (onCreateEditor)
                onCreateEditor(editor);

            ignoreChange.current = false;

            return () => {
                const model = editor.getModel();
                if (model)
                    model.dispose();

                editor.dispose();
            };
        }
    }, [monaco]);

    useEffect(() => {
        if (editorRef.current && (cursorPosition !== undefined)) {
            const position = editorRef.current.getModel().getPositionAt(cursorPosition);
            editorRef.current.setPosition(position);
        }
    }, [cursorPosition]);

    useEffect(() => {
        if (monaco && qualifiedVars && qualifiedVars.length > 0) {            
            /* register any hover or completion providers for each qualified variable */
            let suggestions = [];
            
            qualifiedVars.forEach( ({ qualifierName, qualifierDef }) => {
                const label = qualifierDef.label ? qualifierDef.label : qualifierName;
                Object.entries(qualifierDef).forEach( ([varName, variable]) => {
                    if (varName === "_meta_")
                        return;

                    suggestions.push({
                        label: label + "::" + varName,
                        kind: monaco.languages.CompletionItemKind.Variable,
                        title: variable.label,
                        detail: variable.brief ? variable.brief : variable.description,
                        documentation: variable.brief ? variable.description : undefined,
                        insertText: qualifierName + "::" + varName,
                        insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet
                    });
                });
            });

            const provider = {
                provideCompletionItems: () => {
                    return { 
                        suggestions: suggestions.map(s => ({ ...s }))
                    };
                }
            };

            const itemProvider = monaco.languages.registerCompletionItemProvider("afw", provider);
           
            return () => itemProvider.dispose(); 
        }
    }, [monaco, qualifiedVars]);

    useEffect(() => {
        if (editorRef.current && editorRef.current.getValue() !== source && (source !== undefined)) {
            ignoreChange.current = true;            
            const model = editorRef.current.getModel();               
            model.applyEdits([{ text: source, range: model.getFullModelRange() }]);
            ignoreChange.current = false;
        }
    }, [source]);

    useEffect(() => {
        if (editorRef.current && autoFocus)
            editorRef.current.focus();
    }, [autoFocus]);

    useEffect(() => {
        if (editorRef.current) {
            editorRef.current.updateOptions({ 
                readOnly, 
                lineNumbers: (showLineNumbers ? "on" : "off"), 
                wordWrap: "on", 
                scrollBeyondLastLine: false,
                minimap: {
                    enabled: showMinimap
                }
            });
        } 
    }, [readOnly, showLineNumbers, showMinimap]);

    useEffect(() => {
        if (editorRef.current) {
            window.monaco.editor.setModelLanguage(editorRef.current.getModel(), language);
        }
    }, [language]);

    useEffect(() => {
        if (editorRef.current && uri && schema && schemaUri) {            
            const modelUri = monaco.Uri.parse(uri);
            let model = monaco.editor.getModel(modelUri);
            if (!model) {
                model = monaco.editor.createModel(editorRef.current.getValue(), "json", modelUri);   
                model.setEOL(monaco.editor.EndOfLineSequence.LF);                         
            }
            monaco.languages.json.jsonDefaults.setDiagnosticsOptions({
                validate: true,                    
                schemas: [{
                    uri: schemaUri,
                    fileMatch: [modelUri.toString()],
                    schema,                    
                }],
                enableSchemaRequest: false,
            });
            editorRef.current.setModel(model);
        }
    }, [monaco, schema, schemaUri, uri]);

    useEffect(() => {
        if (editorRef.current && (errorOffset !== undefined)) {
            /* find line from offset */
            const position = editorRef.current.getModel().getPositionAt(errorOffset);
            const {lineNumber, column} = position;
            const d = editorRef.current.deltaDecorations([], [
                {
                    range: new monaco.Range(lineNumber, column, lineNumber, column),
                    options: {
                        isWholeLine: true,
                        glyphMarginClassName: "highlightGlyph",
                        className: "highlightLine",
                        minimap: {
                            position
                        },
                        overviewRuler: {
                            position
                        },
                        hoverMessage: { value: errorHoverMessage }
                    }
                },
                {
                    range: new monaco.Range(lineNumber, column, lineNumber, column+1),
                    options: {
                        className: "highlightToken"                        
                    }
                }
            ]);

            decorationsRef.current = d;

            editorRef.current.setPosition(position);
            editorRef.current.revealLineInCenter(lineNumber);
        } else if (editorRef.current) {
            editorRef.current.deltaDecorations(decorationsRef.current, []);  
            decorationsRef.current = [];          
        }
    }, [monaco, errorOffset, errorHoverMessage]);

    const onDidChangeModelContent = useCallback((e) => {
        if (ignoreChange.current)
            return;

        if (editorRef.current && onChanged) {     
            const {changes} = e;
            // eslint-disable-next-line no-unused-vars
            changes.forEach(change => {
                // \fixme what to do with the change item (forceMoveMarkers, range, rangeOffset, rangeLength, text)?                
            });
                   
            onChanged( editorRef.current.getValue() );
        }
    }, [onChanged]);

    return (
        <div id={id} data-testid={dataTestId} ref={containerRef} style={style} />
    );
});


export default CodeEditor;
