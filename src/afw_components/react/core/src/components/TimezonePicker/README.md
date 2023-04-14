# TimezonePicker

A component that allows a timezone to be chosen.

## Description
This component allows a timezone to be chosen, typically from a dropdown of timezones, or from a world map.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|description|string|false|null|The text description for this TimezonePicker component.|
|label|string|false|null|The text label for this TimezonePicker component.|

## Usage
```js
import {TimezonePicker} from "@afw/react";

const App = (props) => {
    return <TimezonePicker />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `TimezonePicker.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `TimezonePicker.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `TimezonePicker.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `TimezonePicker.test.js`

   Test cases used by @testing-library/jest.

 * `TimezonePicker.stories.js`

   Storybook stories for showcasing the component.

 * `TimezonePicker.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/TimezonePicker.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_TimezonePicker.json)

