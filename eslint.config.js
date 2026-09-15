// See the 'COPYING' file in the project root for licensing information.
const js = require("@eslint/js");
const tseslint = require("typescript-eslint");
const react = require("eslint-plugin-react");
const reactHooks = require("eslint-plugin-react-hooks");
const jsxA11y = require("eslint-plugin-jsx-a11y");
const jest = require("eslint-plugin-jest");
const globals = require("globals");

module.exports = tseslint.config(
    {
        ignores: [
            "**/node_modules/**",
            "**/build/**",
            "**/generated/**",
            "**/dist/**",
            "**/coverage/**",
            "**/*.d.ts",
        ],
    },
    js.configs.recommended,
    tseslint.configs.recommended,
    react.configs.flat.recommended,
    react.configs.flat["jsx-runtime"],
    jsxA11y.flatConfigs.recommended,
    {
        plugins: {
            "react-hooks": reactHooks,
        },
        rules: reactHooks.configs["recommended-latest"].rules,
    },
    {
        languageOptions: {
            globals: {
                ...globals.browser,
                ...globals.node,
                ...globals.es2021,
            },
            parserOptions: {
                ecmaFeatures: {
                    jsx: true,
                },
            },
        },
        settings: {
            react: {
                version: "detect",
            },
        },
        rules: {
            "indent": ["error", 4],
            "linebreak-style": ["error", "unix"],
            "quotes": ["error", "double"],
            "semi": ["error", "always"],
            "@typescript-eslint/no-use-before-define": ["error"],
            // `condition && doSomething()` and `condition ? doA() : doB()` as
            // statements are an established idiom throughout this codebase.
            "@typescript-eslint/no-unused-expressions": ["error", {
                allowShortCircuit: true,
                allowTernary: true,
            }],
            "react/jsx-uses-vars": 1,
            "react/jsx-uses-react": 1,
            // Structurally mismatched with this codebase: components widely
            // spread ...rest/dynamic props, and prop validation already
            // happens via separate hand-maintained *.propTypes.js files per
            // component rather than full-coverage inline PropTypes.
            "react/prop-types": "off",
        },
    },
    {
        // Vitest's globals are Jest-API-compatible, so eslint-plugin-jest's
        // rules apply directly - but its jest/no-deprecated-functions rule
        // needs a real "jest" package to detect a version from, which this
        // Vitest-only repo doesn't have. Set one explicitly instead.
        files: [
            "**/*.test.js",
            "**/__tests__/**/*.js",
            "**/test-utils.js",
            "**/setupTests.js",
        ],
        plugins: {
            jest,
        },
        languageOptions: {
            globals: {
                ...globals.jest,
            },
        },
        settings: {
            jest: {
                version: 29,
            },
        },
        rules: {
            ...jest.configs["flat/recommended"].rules,
        },
    }
);
