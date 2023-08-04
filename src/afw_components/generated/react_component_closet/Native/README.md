# Native

A component that displays a native HTML element.

## Description
This component represents any additional native HTML node that is required by the end user, which is not covered by other components within this library.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|tag|string|false|null|The HTML Tag name.|

## Usage
```js
import {Native} from "@afw/react";

const App = (props) => {
    return <Native />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Native.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Native.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Native.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Native.test.js`

   Test cases used by @testing-library/jest.

 * `Native.stories.js`

   Storybook stories for showcasing the component.

 * `Native.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Native.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Native.json)

