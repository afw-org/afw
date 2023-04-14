# DatePicker

A component that allows you to select a date from a calendar.

## Description
This component understands date values and allows the user to select a date from a calendar, or enter it in the appropriate format.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|description|string|false|null|The description for the DatePicker component.|
|disabled|boolean|false|null|Specifies whether the DatePicker component is disabled.|
|label|string|false|null|The label for the DatePicker component.|
|required|boolean|false|null|Specifies whether the Date Picker value is required.|
|value|string|false|null|The value for the DatePicker component.|

## Usage
```js
import {DatePicker} from "@afw/react";

const App = (props) => {
    return <DatePicker />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `DatePicker.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `DatePicker.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `DatePicker.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `DatePicker.test.js`

   Test cases used by @testing-library/jest.

 * `DatePicker.stories.js`

   Storybook stories for showcasing the component.

 * `DatePicker.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/DatePicker.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_DatePicker.json)

