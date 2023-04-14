# Toggle

A component that renders a boolean value as a toggleable switch.

## Description
This component takes a boolean value and displays it as a switch that can be toggled on or off to represent true or false.  It may also have text labels to interpret its meaning.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|disabled|boolean|false|null|Specifies whether the Toggle component is disabled.|
|label|string|false|null|The label for the Toggle component.|
|offText|string|false|null|The text displayed when the Toggle component is off.|
|onText|string|false|null|The text displayed when the Toggle component is on.|
|value|boolean|false|null|The value for the Toggle component.|

## Usage
```js
import {Toggle} from "@afw/react";

const App = (props) => {
    return <Toggle />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Toggle.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Toggle.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Toggle.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Toggle.test.js`

   Test cases used by @testing-library/jest.

 * `Toggle.stories.js`

   Storybook stories for showcasing the component.

 * `Toggle.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Toggle.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Toggle.json)

