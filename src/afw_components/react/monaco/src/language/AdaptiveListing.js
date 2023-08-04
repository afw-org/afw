// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable no-useless-escape */
/**
 * Language Definition for Adaptive Listing.
 *
 * This file provides methods that Monaco Editor can use to understand
 * The Adaptive Listing language syntax.
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
            [/[0-9:]+ \| .*/, {token: "comment", fontStyle: "bold"}],
            [/#.*/, "comment"],
            [/^.*\ \+/, "annotation"],
            [/^---.*/, "string"]
        ],
    }
};
