# DateTimePicker

A component that allows you to select a date and time.

## Description
This component understands both dates and time formats, which allows the user to select a date and time from a calendar and clock for higher date precision.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|description|string|false|null|The description for the DateTimePicker component.|
|disabled|boolean|false|null|Specifies whether the DateTimePicker component is disabled.|
|label|string|false|null|The label for the DateTimePicker component.|
|required|boolean|false|null|Specifies whether the Date Picker value is required.|
|value|string|false|null|The value for the DateTimePicker component.|

## Usage
```js
import {DateTimePicker} from "@afw/react";

const App = (props) => {
    return <DateTimePicker />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `DateTimePicker.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `DateTimePicker.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `DateTimePicker.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `DateTimePicker.test.js`

   Test cases used by @testing-library/jest.

 * `DateTimePicker.stories.js`

   Storybook stories for showcasing the component.

 * `DateTimePicker.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/DateTimePicker.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_DateTimePicker.json)

