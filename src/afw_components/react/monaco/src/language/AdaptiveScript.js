// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable no-useless-escape */
/**
 * Language Definition for Adaptive Script.
 *
 * This file provides methods that Monaco Editor can use to understand
 * The Adaptive Script language syntax.
 */


/**
 * Programmatic language declarations.
 */
export const constructLanguageConfiguration = (monaco) => ({

    wordPattern: /(-?\d*\.\d\w*)|([^\`\~\!\@\#\%\^\&\*\(\)\-\=\+\[\{\]\}\\\|\;\:\'\"\,\.\<\>\/\?\s]+)/g,
    
    comments: {
        lineComment: '//',
        blockComment: ['/*', '*/']
    },

    brackets: [
        ['{', '}'],
        ['[', ']'],
        ['(', ')']
    ],

    onEnterRules: [
        {
            // e.g. /** | */
            beforeText: /^\s*\/\*\*(?!\/)([^\*]|\*(?!\/))*$/,
            afterText: /^\s*\*\/$/,
            action: { 
                indentAction: monaco.languages.IndentAction.IndentOutdent, 
                appendText: ' * ' 
            }
        },
        {
            // e.g. /** ...|
            beforeText: /^\s*\/\*\*(?!\/)([^\*]|\*(?!\/))*$/,
            action: { 
                indentAction: monaco.languages.IndentAction.None, 
                appendText: ' * ' 
            }
        },
        {
            // e.g.  * ...|
            beforeText: /^(\t|(\ \ ))*\ \*(\ ([^\*]|\*(?!\/))*)?$/,
            action: { 
                indentAction: monaco.languages.IndentAction.None, 
                appendText: '* ' 
            }
        },
        {
            // e.g.  */|
            beforeText: /^(\t|(\ \ ))*\ \*\/\s*$/,
            action: { 
                indentAction: monaco.languages.IndentAction.None, 
                removeText: 1 
            }
        }
    ],

    autoClosingPairs: [
        { open: '{', close: '}' },
        { open: '[', close: ']' },
        { open: '(', close: ')' },
        { open: '"', close: '"', notIn: ['string'] },
        { open: '\'', close: '\'', notIn: ['string', 'comment'] },
        { open: '`', close: '`', notIn: ['string', 'comment'] },
        { open: "/**", close: " */", notIn: ["string"] }
    ],

    folding: {
        markers: {
            start: new RegExp("^\\s*//\\s*#?region\\b"),
            end: new RegExp("^\\s*//\\s*#?endregion\\b")
        }
    }

});


/**
 * Declarative Language definitions.
 */
export const monarchLanguage = {

    defaultToken: "invalid",
    tokenPostfix: ".as",
    ignoreCase: false,

    brackets: [
        { token: 'delimiter.curly', open: '{', close: '}' },
        { token: 'delimiter.parenthesis', open: '(', close: ')' },
        { token: 'delimiter.square', open: '[', close: ']' },
        { token: 'delimiter.angle', open: '<', close: '>' }
    ],

    keywords: [
        "break", "catch", "const", "continue", "do", "else", "false", "for", "foreach", 
        "function", "if", "let", "of", "return", "throw", "true", "try", "undefined", "while"
    ],

    /*! \fixme these should be fetched from dataTypes? */
    typeKeywords: [
        "anyUri", "base64Binary", "boolean", "date", "dateTime", "dayTimeDuration", 
        "dnsName", "double", "expression", "hexBinary", "hybrid", 
        "ia5String", "integer", "ipAddress", "array", "null", "object", "objectId",
        "objectPath", "password", "rfc822Name", "string", "template", "time", "x500Name",
        "xPathExpression", "yearMonthDuration",
    ],

    operators: [
        "=", ">", "<", ":", "==", "===", "<=", ">=", "!=", "!==",
        "+", "-", "*", "**", "/", "%", "..", "&&", "||",
        "+=", "-=", "*=", "**=", "..=", "%=", "/=", "&&=", "||=",
        "->"
    ],

    // we include these common regular expressions
    symbols: /[=><!~?:&|+\-*\/\^%]+/,
    escapes: /\\(?:[abfnrtv\\"']|x[0-9A-Fa-f]{1,4}|u[0-9A-Fa-f]{4}|U[0-9A-Fa-f]{8})/,
    digits: /\d+(_+\d+)*/,
    octaldigits: /[0-7]+(_+[0-7]+)*/,
    binarydigits: /[0-1]+(_+[0-1]+)*/,
    hexdigits: /[[0-9a-fA-F]+(_+[0-9a-fA-F]+)*/,

    regexpctl: /[(){}\[\]\$\^|\-*+?\.]/,
    regexpesc: /\\(?:[bBdDfnrstvwWn0\\\/]|@regexpctl|c[A-Z]|x[0-9a-fA-F]{2}|u[0-9a-fA-F]{4})/,

    tokenizer: {
        root: [
            [/[{}]/, 'delimiter.bracket'],
            { include: 'common' }
        ],

        common: [
            // identifiers and keywords
            [/[a-z_$][\w]*(::)?[\w$]*/, {
                cases: {
                    '@typeKeywords': 'type.identifier',
                    '@keywords': 'keyword',
                    '@default': 'identifier'
                }
            }],
            [/[A-Z][\w\$]*/, 'type.identifier'],  // to show identifier names nicely
            [/[A-Z][\w\$]*/, 'identifier'],

            // whitespace
            { include: '@whitespace' },

            // regular expression: ensure it is terminated before beginning (otherwise it is an operator)
            [/\/(?=([^\\\/]|\\.)+\/([gimsuy]*)(\s*)(\.|;|\/|,|\)|\]|\}|$))/, { token: 'regexp', bracket: '@open', next: '@regexp' }],

            // delimiters and operators
            [/[()\[\]]/, '@brackets'],
            [/[<>](?!@symbols)/, '@brackets'],
            [/!(?=([^=]|$))/, 'delimiter'],
            [
                /@symbols/, 
                {
                    cases: {
                        '@operators': 'delimiter',
                        '@default': ''
                    }
                }
            ],

            // numbers
            [/(@digits)[eE]([\-+]?(@digits))?/, 'number.float'],
            [/(@digits)\.(@digits)([eE][\-+]?(@digits))?/, 'number.float'],
            [/0[xX](@hexdigits)n?/, 'number.hex'],
            [/0[oO]?(@octaldigits)n?/, 'number.octal'],
            [/0[bB](@binarydigits)n?/, 'number.binary'],
            [/(@digits)n?/, 'number'],

            // delimiter: after number because of .\d floats
            [/[;,.]/, 'delimiter'],

            // strings
            [/"([^"\\]|\\.)*$/, 'string.invalid'],  // non-terminated string
            [/'([^'\\]|\\.)*$/, 'string.invalid'],  // non-terminated string
            [/"/, 'string', '@string_double'],
            [/'/, 'string', '@string_single'],
            [/`/, 'string', '@string_backtick'],
        ],

        whitespace: [
            [/[ \t\r\n]+/, ''],
            [/\/\*\*(?!\/)/, 'comment.doc', '@jsdoc'],
            [/\/\*/, 'comment', '@comment'],
            [/\/\/.*$/, 'comment'],
            [/^#!.*$/, 'comment'],
        ],

        comment: [
            [/[^\/*]+/, 'comment'],
            [/^#!.*/, 'comment'],
            [/\*\//, 'comment', '@pop'],
            [/[\/*]/, 'comment']
        ],

        jsdoc: [
            [/[^\/*]+/, 'comment.doc'],
            [/\*\//, 'comment.doc', '@pop'],
            [/[\/*]/, 'comment.doc']
        ],

        // We match regular expression quite precisely
        regexp: [
            [/(\{)(\d+(?:,\d*)?)(\})/, ['regexp.escape.control', 'regexp.escape.control', 'regexp.escape.control']],
            [/(\[)(\^?)(?=(?:[^\]\\\/]|\\.)+)/, ['regexp.escape.control', { token: 'regexp.escape.control', next: '@regexrange' }]],
            [/(\()(\?:|\?=|\?!)/, ['regexp.escape.control', 'regexp.escape.control']],
            [/[()]/, 'regexp.escape.control'],
            [/@regexpctl/, 'regexp.escape.control'],
            [/[^\\\/]/, 'regexp'],
            [/@regexpesc/, 'regexp.escape'],
            [/\\\./, 'regexp.invalid'],
            [/(\/)([gimsuy]*)/, [{ token: 'regexp', bracket: '@close', next: '@pop' }, 'keyword.other']],
        ],

        regexrange: [
            [/-/, 'regexp.escape.control'],
            [/\^/, 'regexp.invalid'],
            [/@regexpesc/, 'regexp.escape'],
            [/[^\]]/, 'regexp'],
            [/\]/, { token: 'regexp.escape.control', next: '@pop', bracket: '@close' }]
        ],

        string_double: [
            [/[^\\"]+/, 'string'],
            [/@escapes/, 'string.escape'],
            [/\\./, 'string.escape.invalid'],
            [/"/, 'string', '@pop']
        ],

        string_single: [
            [/[^\\']+/, 'string'],
            [/@escapes/, 'string.escape'],
            [/\\./, 'string.escape.invalid'],
            [/'/, 'string', '@pop']
        ],

        string_backtick: [
            [/\$\{/, { token: 'delimiter.bracket', next: '@bracketCounting' }],
            [/[^\\`$]+/, 'string'],
            [/@escapes/, 'string.escape'],
            [/\\./, 'string.escape.invalid'],
            [/`/, 'string', '@pop']
        ],

        bracketCounting: [
            [/\{/, 'delimiter.bracket', '@bracketCounting'],
            [/\}/, 'delimiter.bracket', '@pop'],
            { include: 'common' }
        ],
    }
};


/**
 * adaptorsCompletionItemProvider()
 *
 * This routine registers adaptors autocomplete items for the monaco editor.
 */
export const adaptorsCompletionItemProvider = (monaco, adaptors) => {
    const suggestions = [];

    adaptors.forEach(adaptor => {
        const adaptorId = adaptor.adaptorId;

        suggestions.push({
            label: '"' + adaptorId + '"',
            kind: monaco.languages.CompletionItemKind.Constant,
            insertText: '"' + adaptorId + '"',
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet
        });
    });

    return {
        provideCompletionItems: () => {
            return { 
                suggestions: suggestions.map(s => ({ ...s }))
            };
        }
    };
};

/**
 * objectTypeCompletionItemProvider()
 *
 * This routine registers object type autocomplete items for the monaco editor.
 */
export const objectTypeCompletionItemProvider = (monaco, objectTypes) => {
    const suggestions = [];

    objectTypes.forEach(objectType => {
        const objectTypeId = objectType.getObjectId();
        const description = objectType.getPropertyValue("description");

        suggestions.push({
            label: '"' + objectTypeId + '"',
            kind: monaco.languages.CompletionItemKind.Constant,
            detail: description,
            documentation: description,
            insertText: '"' + objectTypeId + '"',
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet
        });
    });

    return {
        provideCompletionItems: () => {
            return { 
                suggestions: suggestions.map(s => ({ ...s }))
            };
        }
    };
};

/**
 * dataTypeCompletionItemProvider()
 *
 * This routine registers dataType autocomplete items for the monaco editor.
 */
export const dataTypeCompletionItemProvider = (monaco, dataTypes) => {
    const suggestions = [];

    dataTypes.forEach(dataType => {
        const {dataTypeId, brief, description} = dataType;

        suggestions.push({
            label: '"' + dataTypeId + '"',
            kind: monaco.languages.CompletionItemKind.Constant,
            detail: brief,
            documentation: description,
            insertText: '"' + dataTypeId + '"',
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet
        });
    });

    return {
        provideCompletionItems: () => {
            return { 
                suggestions: suggestions.map(s => ({ ...s }))
            };
        }
    };
};

/**
 * functionCompletionItemProvider()
 *
 * This routine registers function autocomplete items for the monaco editor.
 */
export const functionCompletionItemProvider = (monaco, functions) => {
    
    let suggestions = [];

    functions.forEach(func => {
        const {functionId, parameters, brief, description} = func;

        let fnDoc = description;
        let params = [];
        if (parameters && parameters.length > 0) {
            fnDoc += "  \n  \n";
            parameters.forEach((param, index) => {
                const parameterName = param.name;
                params.push("${" + (index+1) + ":" + parameterName + "}");

                fnDoc += "  * @param " + parameterName + " - " + param.description + "  \n  \n";
            });
        }

        suggestions.push({
            label: functionId,
            kind: monaco.languages.CompletionItemKind.Function,
            detail: brief,
            documentation: fnDoc,
            insertText: functionId,
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet
        });
    });    

    return {
        provideCompletionItems: (model, position) => {              
            return { 
                suggestions: suggestions.map(s => ({ ...s }))
            };
        }
    };
};

export const polymorphicMethodCompletionItemProvider = (monaco, functions) => {

    let suggestions = [];

    functions.forEach(func => {
        const {functionId, parameters, brief, description, polymorphic} = func;

        if (!polymorphic)
            return;

        let fnDoc = description;
        let params = [];
        if (parameters && parameters.length > 0) {
            fnDoc += "  \n  \n";
            parameters.forEach((param, index) => {
                const parameterName = param.name;
                params.push("${" + (index+1) + ":" + parameterName + "}");

                fnDoc += "  * @param " + parameterName + " - " + param.description + "  \n  \n";
            });
        }

        suggestions.push({
            label: functionId,
            kind: monaco.languages.CompletionItemKind.Function,
            detail: brief,
            documentation: fnDoc,
            insertText: functionId,
            insertTextRules: monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet
        });
    }); 

    /**
     * \fixme
     * Can only suggest every polymorphic function for now.  Monaco doesn't have a public API
     * that I'm aware of to get the tokens and their types during this function call.
     */
    return {
        triggerCharacters: [ ">" ],
        provideCompletionItems: (model, position, context, token) => {
            const {lineNumber, column} = position;                 

            const line = model.getLineContent(lineNumber);            
            if (line.length <= 2)
                return;

            const trigger = line.substring(column-3, column-1);                    
            if (trigger !== "->")
                return;
           
            return ({
                suggestions: suggestions.map(s => ({ ...s }))
            });
        }
    };
};


/**
 * The Hover Provider helps Monaco understand what to display
 * when the user hovers over a section of code.
 */
export function hoverProvider(monaco, functions) {

    const functionDocs = {};
    functions.forEach(func => {
        const {functionId, parameters, brief, description, functionDeclaration : functionSignature} = func;        
        let returns;

        let paramsDoc = "";
        if (parameters && parameters.length > 0) {
            paramsDoc += "**Parameters**  \n  \n";
            parameters.forEach(({name, dataType = "any", description}) => {                                
                if (!dataType)
                    dataType = "any";

                paramsDoc += "  * `" + name + "` - " + description + "  \n  \n";
            });
        }

        let returnsDoc = "";
        if (func.returns) {
            const dataTypeParameter = func.returns.dataTypeParameter;
            let dataType = func.returns.dataType;

            if (!dataType)
                dataType = "any";

            returns = dataTypeParameter ? ("'" + dataType + " " + dataTypeParameter + "'") : dataType;            
            returnsDoc = "  **Returns** \n \n";
            returnsDoc += func.returns.description;
        }

        functionDocs[functionId] = { brief, description, parameters, paramsDoc, returns, functionSignature, returnsDoc };
    });

    return ({
        provideHover: function(model, position, token) {
            const word = model.getWordAtPosition(position);            
            if (word && word.word && functionDocs[word.word]) {                
                const fnDoc = functionDocs[word.word];     
                return Promise.resolve({
                    //range: new monaco.Range(1, 1, model.getLineCount(), model.getLineMaxColumn(model.getLineCount())),
                    contents: [
                        { 
                            value: '```afw\n' + fnDoc.functionSignature + '\n```'
                        },                          
                        {
                            value: fnDoc.brief
                        },
                        {
                            value: fnDoc.description
                        },
                        {
                            value: fnDoc.returnsDoc
                        },
                        {
                            value: fnDoc.paramsDoc
                        },
                    ]
                });
            } else return Promise.resolve({});
        }
    });
}


/**
 * signatureHelpProvider()
 *
 * This routine registers function signatures for the monaco editor, 
 * based on current context.
 */
export const signatureHelpProvider = (monaco, functions, allSignatures) => {

    /* function signatures are needed when we hit '(' or ')' */
    const signatureHelpTriggerCharacters = ["(", ","];

    /* callback routine when the model needs help */
    const provideSignatureHelp = (model, position, token, context) => {
        let signatures = [];
        let activeSignature = 0;

        if (model.isDisposed())
            return;

        if (context.activeSignatureHelp) {
            signatures = context.activeSignatureHelp.signatures;
            activeSignature = context.activeSignatureHelp.activeSignature;
        }

        const line = model.getLineContent(position.lineNumber);
        let start = position.column - 3;
        const c = line.charAt(start+1);

        if (c === ")")
            return;

        /* walk backwards to find first non-alphanumeric character */
        while (line.charAt(start).match(/^[a-zA-Z0-9_]+$/i) && start > 0)
            start--;
        if (start > 0) start++;

        /* calculate last matching token */
        const match = line.substring(start, position.column-2);
        
        let activeParameter = 0;

        // locate parameter position 
        if (activeSignature !== undefined) {
            start = position.column - 2;      

            while (line.charAt(start) !== "(" && start > 0) {
                if (line.charAt(start) === ",")
                    activeParameter++;
                start--;
            }
        }

        if (allSignatures[match]) {
            signatures = [ allSignatures[match] ];
            activeSignature = 0;
        }
        
        return { 
            value: { activeParameter, activeSignature, signatures },
            dispose() {}
        };
    };

    return { signatureHelpTriggerCharacters, provideSignatureHelp };
};


/**
 * signatures()
 *
 * This routine pre-constructs all of the Monaco signatures
 * from adaptive function definitions.  Doing this ahead of time
 * will be more efficient when we need one.
 */
export const signatures = (functions) => {

    const signatures = {};

    functions.forEach((func) => {
        const {functionId, parameters, brief, description, functionSignature} = func;        

        if (!functionSignature)
            return;

        let params = [];
        if (parameters) {
            parameters.forEach((param) => {                
                // eslint-disable-next-line no-unused-vars
                const {name, brief, description} = param;
                
                params.push({                    
                    label: name,
                    documentation: description,
                });

            });
        }

        signatures[functionId] = {
            label: functionSignature,
            documentation: description,
            parameters: params,
            detail: brief,
        };
    });    

    return signatures;
};
