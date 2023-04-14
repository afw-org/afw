# CodeEditor

A component that renders a rich text editor.

## Description
This component displays a rich text editor that allows the user to manually edit the source code for a particular piece of data.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|readOnly|boolean|false|null|The source value for the CodeEditor component should be read-only.|
|showGutter|boolean|false|null|Specifies whether the gutter should be visible in CodeEditor component.|
|showLineNumbers|boolean|false|null|Specifies whether the line numbers should be visible in CodeEditor component.|
|showPrintMargin|boolean|false|null|Specifies whether the print margin should be visible in CodeEditor component.|
|source|string|false|null|The source value for the CodeEditor component to render.|

## Usage
```js
import {CodeEditor} from "@afw/react";

const App = (props) => {
    return <CodeEditor />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `CodeEditor.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `CodeEditor.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `CodeEditor.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `CodeEditor.test.js`

   Test cases used by @testing-library/jest.

 * `CodeEditor.stories.js`

   Storybook stories for showcasing the component.

 * `CodeEditor.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/CodeEditor.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_CodeEditor.json)

