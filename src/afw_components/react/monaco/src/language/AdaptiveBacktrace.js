// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable no-useless-escape */
/**
 * Language Definition for Adaptive Backtrace.
 *
 * This file provides methods that Monaco Editor can use to understand
 * The Adaptive Backtrace language syntax.
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
            [/^0x[a-f0-9]+/, { token: "comment", next: '@in' }],            
        ],
        'in': [
            [/ in /, { token: "white", next: "@func" }]
        ],
        func: [
            [/[a-zA-Z_]+/, { token: "constant", next: '@at' }]
        ],
        at: [
            [/ at /, { token: "white", next: "@source" }]
        ],
        source: [
            [/[a-zA-Z_]+\.[a-zA-Z]+/, {token: "string", next: '@colon'}],
            [/\+0x[a-f0-9]+/, {token: "string", next: '@colon'}]
        ],
        colon: [
            [/:/, { token: "white", next: "@location" }],
            [/(?!:)/, {token: "white", next: "@root"}]
        ],
        location: [
            [/[0-9]+/, {token: "white", next: "@root" }]
        ]
    }
};
