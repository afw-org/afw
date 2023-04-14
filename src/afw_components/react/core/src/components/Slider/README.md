# Slider

A component that displays numeric values in a slider control.

## Description
This component is used for numeric values, and allows the user to slide a controller to increase or decrease its value without having to enter data manually.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|disabled|boolean|false|null|Specifies whether the Slider component is disabled.|
|label|string|false|null|The label for the Slider component.|
|max|number|false|null|The maximum value for the Slider component.|
|min|number|false|null|The minimum value for the Slider component.|
|showNumber|boolean|false|null|Indicates whether the value of the Slider should be visible.|
|step|number|false|null|The incremental step value for the Slider component.|
|value|number|false|null|The value for the Slider component.|
|vertical|boolean|false|null|Indicates whether the Slider component is vertical oriented.|

## Usage
```js
import {Slider} from "@afw/react";

const App = (props) => {
    return <Slider />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Slider.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Slider.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Slider.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Slider.test.js`

   Test cases used by @testing-library/jest.

 * `Slider.stories.js`

   Storybook stories for showcasing the component.

 * `Slider.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Slider.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Slider.json)

