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

    /* Allow a qualifier::name reference (e.g. current::foo) to be treated as one word. */
    wordPattern: /(-?\d*\.\d\w*)|([^\`\~\!\@\#\%\^\&\*\(\)\-\=\+\[\{\]\}\\\|\;\:\'\"\,\.\<\>\/\?\s]+(?:::[^\`\~\!\@\#\%\^\&\*\(\)\-\=\+\[\{\]\}\\\|\;\:\'\"\,\.\<\>\/\?\s]+)*)/g,

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

    surroundingPairs: [
        { open: '{', close: '}' },
        { open: '[', close: ']' },
        { open: '(', close: ')' },
        { open: '"', close: '"' },
        { open: '\'', close: '\'' },
        { open: '`', close: '`' },
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

    /*
     * Reserved words per afw_compile_lexical.c ReservedWords: statement /
     * clause keywords, predefined const literals ('null' is listed here
     * rather than typeKeywords so it colors consistently with true/false/
     * undefined), plus words that are reserved but not implemented as
     * syntax today (they still cannot be used as identifiers). 'of' and
     * 'using' are contextual (for-of, switch-using) and not reserved, but
     * are included for highlighting since they are structural.
     */
    keywords: [
        "break", "case", "catch", "const", "continue", "default", "do",
        "else", "extends", "false", "finally", "for", "function", "if",
        "interface", "let", "null", "of", "return", "switch", "throw",
        "true", "try", "type", "undefined", "using", "void", "while",
        // reserved but not implemented as syntax (afw_compile_lexical.c UnusedButReservedWords)
        "as", "async", "await", "class", "delete", "export", "from",
        "import", "in", "instanceof", "super", "this", "typeof", "var", "with",
    ],

    // Adaptive data type ids valid in Type position (afw_compile_parse_expression.c DataType).
    // 'function' and 'void' are DataType names too, but are left out here (as
    // 'null' already is) since they collide with 'keywords' below and are far
    // more commonly typed as the statement/lambda keyword and unary operator,
    // respectively, than as a bare type annotation - cases checks typeKeywords
    // first, so keeping them here would mis-tag every 'function'/'void' use.
    typeKeywords: [
        "any", "anyURI", "array", "base64Binary", "boolean", "date", "dateTime",
        "dayTimeDuration", "dnsName", "double", "expression",
        "hexBinary", "ia5String", "integer", "ipAddress", "object", "objectId",
        "objectPath", "password", "regexp", "rfc822Name", "script", "string",
        "template", "time", "unevaluated", "unknown", "x500Name",
        "xpathExpression", "yearMonthDuration",
    ],

    // Predefined numeric const literals (afw_compile_lexical.c); not identifiers.
    numberKeywords: [
        "Infinity", "INF", "NaN",
    ],

    operators: [
        "=", ">", "<", ":", "==", "===", "<=", ">=", "!=", "!==",
        "+", "-", "*", "**", "/", "%", "&", "|", "&&", "||", "??",
        "+=", "-=", "*=", "**=", "%=", "/=", "&&=", "||=", "??=",
        "++", "--", "->", "?->", "?", "=>", "..."
    ],

    // we include these common regular expressions
    symbols: /[=><!~?:&|+\-*\/\^%]+/,
    // Char escapes per afw_compile_lexical.c: known short forms + \xHH (2 hex
    // digits) + \uXXXX or \u{H+}. Any other backslash + non-digit is a valid
    // NonEscapeSequence (identity escape, e.g. \A -> A); backslash + 1-9 is invalid.
    escapes: /\\(?:[bfnrtv0\\"'`\/]|x[0-9A-Fa-f]{2}|u[0-9A-Fa-f]{4}|u\{[0-9A-Fa-f]+\})/,
    digits: /\d+/,
    octaldigits: /[0-7]+/,
    binarydigits: /[0-1]+/,
    hexdigits: /[0-9a-fA-F]+/,

    tokenizer: {
        root: [
            [/[{}]/, 'delimiter.bracket'],
            { include: 'common' }
        ],

        common: [
            // identifiers and keywords
            [/[a-z_$][\w]*(::)?[\w$]*/, {
                cases: {
                    '@numberKeywords': 'number',
                    '@typeKeywords': 'type.identifier',
                    '@keywords': 'keyword',
                    '@default': 'identifier'
                }
            }],
            [/[A-Z][\w\$]*/, {
                cases: {
                    '@numberKeywords': 'number',
                    '@default': 'type.identifier'
                }
            }],

            // whitespace (also claims the '#!...' shebang line, so it must come
            // before the generic pound-identifier rules below)
            { include: '@whitespace' },

            // compiler-internal / pragma pound-identifiers: #compile, #pi, #block(...), etc.
            // (afw_compile_parse_compiler_internal.c, afw_compile_parse_pragma.c)
            [/#\{/, { token: 'delimiter.bracket', next: '@bracketCounting' }],
            [/#[a-zA-Z_$][\w$]*/, 'annotation'],
            [/#/, 'delimiter'],

            // delimiters and operators
            [/[()\[\]]/, '@brackets'],
            [/[<>](?!@symbols)/, '@brackets'],
            [/!(?=([^=]|$))/, 'delimiter'],
            [/\?\./, 'delimiter'],
            [/\.\.\./, 'delimiter'],
            [
                /@symbols/,
                {
                    cases: {
                        '@operators': 'delimiter',
                        '@default': ''
                    }
                }
            ],

            // numbers (afw_compile_lexical.c: no digit separators, no BigInt
            // suffix; octal/hex/binary require an explicit 0o/0x/0b prefix)
            [/(@digits)\.(@digits)?([eE][\-+]?(@digits))?/, 'number.float'],
            [/\.(@digits)([eE][\-+]?(@digits))?/, 'number.float'],
            [/(@digits)[eE][\-+]?(@digits)/, 'number.float'],
            [/0[xX](@hexdigits)/, 'number.hex'],
            [/0[oO](@octaldigits)/, 'number.octal'],
            [/0[bB](@binarydigits)/, 'number.binary'],
            [/(@digits)/, 'number'],

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
            [/\*\//, 'comment', '@pop'],
            [/[\/*]/, 'comment']
        ],

        jsdoc: [
            [/[^\/*]+/, 'comment.doc'],
            [/\*\//, 'comment.doc', '@pop'],
            [/[\/*]/, 'comment.doc']
        ],

        // Adaptive Script has no regex-literal syntax ('/' is always divide,
        // see afw_compile_lexical.c) so there is deliberately no regexp state here.

        string_double: [
            [/[^\\"]+/, 'string'],
            [/@escapes/, 'string.escape'],
            [/\\[1-9]/, 'string.escape.invalid'],
            [/\\./, 'string.escape'],
            [/"/, 'string', '@pop']
        ],

        string_single: [
            [/[^\\']+/, 'string'],
            [/@escapes/, 'string.escape'],
            [/\\[1-9]/, 'string.escape.invalid'],
            [/\\./, 'string.escape'],
            [/'/, 'string', '@pop']
        ],

        // Template body (afw_compile_parse_template.c): both '${' (eval-time)
        // and '#{' (compile-time) substitutions are recognized, and a lone
        // '$' or '#' not starting a substitution is plain text.
        string_backtick: [
            [/\$\{/, { token: 'delimiter.bracket', next: '@bracketCounting' }],
            [/#\{/, { token: 'delimiter.bracket', next: '@bracketCounting' }],
            [/(?:[^\\`$#]|\$(?!\{)|#(?!\{))+/, 'string'],
            [/@escapes/, 'string.escape'],
            [/\\[1-9]/, 'string.escape.invalid'],
            [/\\./, 'string.escape'],
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
 * adaptersCompletionItemProvider()
 *
 * This routine registers adapters autocomplete items for the monaco editor.
 */
export const adaptersCompletionItemProvider = (monaco, adapters) => {
    const suggestions = [];

    adapters.forEach(adapter => {
        const adapterId = adapter.adapterId;

        suggestions.push({
            label: '"' + adapterId + '"',
            kind: monaco.languages.CompletionItemKind.Constant,
            insertText: '"' + adapterId + '"',
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

    dataTypes.forEach(({dataType: dataTypeId, brief, description}) => {

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


/*
 * Flattens monaco.editor.tokenize()'s per-line Token[][] (offsets only) into
 * a single array of { text, type, line, column } tokens with the source
 * text sliced in, using the same Monarch classification that already
 * drives syntax highlighting - so declarations inside strings/comments are
 * never mistaken for real ones the way a plain text/regex scan could.
 */
const tokenizeToFlatTokens = (monaco, model) => {
    const text = model.getValue();
    const lines = text.split(/\r\n|\r|\n/);
    const tokensByLine = monaco.editor.tokenize(text, model.getLanguageId());

    const tokens = [];
    for (let lineIndex = 0; lineIndex < tokensByLine.length; lineIndex++) {
        const lineTokens = tokensByLine[lineIndex];
        const lineText = lines[lineIndex] ?? "";

        for (let i = 0; i < lineTokens.length; i++) {
            const start = lineTokens[i].offset;
            const end = (i + 1 < lineTokens.length) ? lineTokens[i + 1].offset : lineText.length;
            const raw = lineText.substring(start, end);
            const text = raw.trim();

            if (text) {
                tokens.push({
                    text,
                    type: lineTokens[i].type,
                    line: lineIndex + 1,
                    column: start + 1 + (raw.length - raw.trimStart().length),
                });
            }
        }
    }

    return tokens;
};

/*
 * Scans a flat token stream (see tokenizeToFlatTokens) for local variable
 * declarations that are in scope at the given cursor position, following
 * the shapes of the real grammar (afw_compile_parse_script.c /
 * afw_compile_parse_expression.c): LetDeclaration, ConstDeclaration,
 * FunctionStatement/Lambda ParameterBinding + EllipsisParameter, and Catch
 * bindings.
 *
 * Scoping is approximated with brace depth: a name is attributed to the
 * depth its declaration is visible at, and is dropped as soon as a '}' at
 * that depth (or shallower) is seen. The token stream is truncated to the
 * cursor position first, so a name is never offered before its own
 * declaration, and any block that has already closed before the cursor has
 * already had its names dropped by the time the scan ends - whatever
 * remains is exactly what is visible at the cursor. Destructuring patterns
 * ([a, b] / {a, b}) are skipped as a whole rather than expanded; this is a
 * lightweight scanner, not a parser, so it favors simple, common cases.
 */
const collectLocalVariableNames = (monaco, model, position) => {
    const allTokens = tokenizeToFlatTokens(monaco, model);
    const tokens = allTokens.filter(t =>
        t.line < position.lineNumber ||
        (t.line === position.lineNumber && t.column <= position.column)
    );

    const isText = (t, str) => t !== undefined && t.text === str;
    const isType = (t, suffix) => t !== undefined && t.type === (suffix + ".as");
    const isKeyword = (t, word) => isType(t, "keyword") && t.text === word;
    const isIdentifier = (t) => isType(t, "identifier") || isType(t, "type.identifier");
    /* '${' and '#{' (template substitutions) open with a two-character
       token but still close with a plain '}', so they must count as
       openers here too or brace-depth tracking goes out of balance the
       moment a template literal containing a substitution is scanned. */
    const isOpenBrace = (t) => isText(t, "{") || isText(t, "${") || isText(t, "#{");

    const names = [];
    let depth = 0;

    /* Skip one balanced sub-expression (default value, ': Type', etc.)
       starting at i. Stops - without consuming it - at the first ','
       or unmatched closing bracket/';' seen while locally back at
       depth 0, so the caller can see what ended it. */
    const skipExpression = (i) => {
        let nested = 0;
        while (i < tokens.length) {
            const t = tokens[i];
            if (isText(t, "(") || isText(t, "[") || isOpenBrace(t)) {
                nested++;
            } else if (isText(t, ")") || isText(t, "]") || isText(t, "}")) {
                if (nested === 0)
                    return i;
                nested--;
            } else if (nested === 0 && (isText(t, ",") || isText(t, ";"))) {
                return i;
            }
            i++;
        }
        return i;
    };

    /* Collects a comma-separated ParameterBinding / AssignmentTarget list
       (identifiers, optionally with a destructure target, ': Type', '?',
       or '= default') starting at i, attributing each name to declDepth.
       Returns the index of whatever token ended the list (a closing
       bracket, ';', or the 'of' of a for-of target) without consuming it. */
    const collectBindingList = (i, declDepth) => {
        while (i < tokens.length) {
            const t = tokens[i];

            if (isIdentifier(t)) {
                names.push({ name: t.text, depth: declDepth });
                i++;
            } else if (isText(t, "[") || isText(t, "{")) {
                i = skipExpression(i) + 1;
                continue;
            } else if (isText(t, "...")) {
                i++;
                continue;
            } else {
                return i;
            }

            i = skipExpression(i);
            if (isText(tokens[i], ","))
                i++;
            else
                return i;
        }
        return i;
    };

    let i = 0;
    while (i < tokens.length) {
        const t = tokens[i];

        if (isOpenBrace(t)) {
            depth++;
            i++;
            continue;
        }

        if (isText(t, "}")) {
            for (let n = names.length - 1; n >= 0; n--) {
                if (names[n].depth >= depth)
                    names.splice(n, 1);
            }
            depth = Math.max(0, depth - 1);
            i++;
            continue;
        }

        if (isKeyword(t, "let") || isKeyword(t, "const")) {
            i = collectBindingList(i + 1, depth);
            continue;
        }

        if (isKeyword(t, "catch") && isText(tokens[i + 1], "(")) {
            i = collectBindingList(i + 2, depth + 1) + 1;
            continue;
        }

        if (isKeyword(t, "function")) {
            let j = i + 1;
            if (isIdentifier(tokens[j])) {
                /* named function statement: the name is visible in the
                   enclosing scope, not just inside its own body */
                names.push({ name: tokens[j].text, depth });
                j++;
            }
            if (isText(tokens[j], "("))
                j = collectBindingList(j + 1, depth + 1) + 1;
            i = j;
            continue;
        }

        i++;
    }

    return Array.from(new Set(names.map(n => n.name)));
};

/**
 * localVariableCompletionItemProvider()
 *
 * This routine suggests local variables, function parameters and catch
 * bindings that are in scope at the cursor, using Monaco's own tokenizer
 * output (see collectLocalVariableNames) rather than the document's raw
 * text, so declarations are recognized the way the language actually
 * classifies them (and not, say, inside a string or comment).
 */
export const localVariableCompletionItemProvider = (monaco) => {
    return {
        provideCompletionItems: (model, position) => {
            const names = collectLocalVariableNames(monaco, model, position);

            return {
                suggestions: names.map(name => ({
                    label: name,
                    kind: monaco.languages.CompletionItemKind.Variable,
                    insertText: name,
                }))
            };
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
