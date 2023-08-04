# SpinButton

A component that renders a numeric field that an be incremented or decremented.

## Description
A component that renders a numeric field that an be incremented or decremented.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|disabled|boolean|false|null|Specifies whether the SpinButton component is disabled.|
|label|string|false|null|The label for the SpinButton component.|
|max|number|false|null|The maximum value for the SpinButton component.|
|min|number|false|null|The minimum value for the SpinButton component.|
|step|number|false|null|The incremental step value for the SpinButton component.|
|value|number|false|null|The value for the SpinButton component.|

## Usage
```js
import {SpinButton} from "@afw/react";

const App = (props) => {
    return <SpinButton />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `SpinButton.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `SpinButton.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `SpinButton.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `SpinButton.test.js`

   Test cases used by @testing-library/jest.

 * `SpinButton.stories.js`

   Storybook stories for showcasing the component.

 * `SpinButton.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/SpinButton.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_SpinButton.json)

