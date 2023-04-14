# Checkbox

A component that displays a checkbox.

## Description
This component is simply a checkbox that allows the user to click to enable or disable its value.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|boxSide|string|false|null|Specifies which side the checkbox is on (start or end).|
|description|string|false|null|The description for the Checkbox component.|
|disabled|boolean|false|null|Specifies whether the Checkbox component is disabled.|
|label|string|false|null|The label for the Checkbox component.|
|value|boolean|false|null|The value for the Checkbox component.|

## Usage
```js
import {Checkbox} from "@afw/react";

const App = (props) => {
    return <Checkbox />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Checkbox.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Checkbox.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Checkbox.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Checkbox.test.js`

   Test cases used by @testing-library/jest.

 * `Checkbox.stories.js`

   Storybook stories for showcasing the component.

 * `Checkbox.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Checkbox.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Checkbox.json)

