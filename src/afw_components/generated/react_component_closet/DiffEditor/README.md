# DiffEditor

A component that compares two objects.

## Description
This component displays the differences between two objects.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|modified|string|false|null|The modified source value for the DiffEditor component to render.|
|original|string|false|null|The original source value for the DiffEditor component to render.|
|showLineNumbers|boolean|false|null|Specifies whether the line numbers should be visible in DiffEditor component.|

## Usage
```js
import {DiffEditor} from "@afw/react";

const App = (props) => {
    return <DiffEditor />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `DiffEditor.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `DiffEditor.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `DiffEditor.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `DiffEditor.test.js`

   Test cases used by @testing-library/jest.

 * `DiffEditor.stories.js`

   Storybook stories for showcasing the component.

 * `DiffEditor.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/DiffEditor.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_DiffEditor.json)

