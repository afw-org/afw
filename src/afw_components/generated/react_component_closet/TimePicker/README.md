# TimePicker

A component that allows a time to be chosen.

## Description
This component allows a time to be chosen, typically from a clock or spin buttons.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|description|string|false|null|The text description for this TimePicker component.|
|label|string|false|null|The text label for this TimePicker component.|

## Usage
```js
import {TimePicker} from "@afw/react";

const App = (props) => {
    return <TimePicker />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `TimePicker.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `TimePicker.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `TimePicker.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `TimePicker.test.js`

   Test cases used by @testing-library/jest.

 * `TimePicker.stories.js`

   Storybook stories for showcasing the component.

 * `TimePicker.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/TimePicker.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_TimePicker.json)

