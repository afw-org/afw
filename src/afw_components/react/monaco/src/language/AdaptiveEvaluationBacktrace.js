// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable no-useless-escape */
/**
 * Language Definition for Adaptive Evaluation Backtrace.
 *
 * This file provides methods that Monaco Editor can use to understand
 * The Adaptive Evaluation Backtrace language syntax.
 */


export const constructLanguageConfiguration = (monaco) => {

    return ({

        wordPattern: /(-?\d*\.\d\w*)|([^\`\~\!\@\#\%\^\&\*\(\)\-\=\+\[\{\]\}\\\|\;\:\'\"\,\.\<\>\/\?\s]+)/g,

        comments: {
            lineComment: '#',
        },

        brackets: [
            ['{', '}'],
            ['[', ']'],
            ['(', ')']
        ],
    });

};

/**
 * Declarative Language definitions.
 */
export const monarchLanguage = {

    tokenizer: {
        root: [
            [/^---CompiledValue .*/, { token: "string", next: "@CompiledValue" } ],
            [/^---Compile error.*/, { token: "comment", next: "@CompileError" } ],
        ],

        CompileError: [
            [/^---CompiledValue .*/, { token: "string", next: "@CompiledValue" } ],
            [/^[ 0-9]+ \>.*/, {token: "invalid"}],
            [/^[ 0-9]+ \|/, {token: "annotation"}],            
        ],

        CompiledValue: [
            [/^---Source .*/, { token: "comment", next: "@Source"}],
        ],

        Source: [
            [/^[ 0-9]+ \>.*/, {token: "invalid"}],
            [/^[ 0-9]+ \|/, {token: "annotation"}],
            [/^---Backtrace/, { token: "variable", next: "@Backtrace"}],
        ],

        Backtrace: [
            [/^[ 0-9]+ \|/, {token: "annotation"}],
            [/^---CompiledValue .*/, {token: "string", next: "@CompiledValue"}]
        ]
    }
};
