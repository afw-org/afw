# Chart

Component to display data in a Chart.

## Description
This component can display a variety of simple or complex chart data in forms such as Pie, Line or Bar to offer a visual representation of the provided data.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|animateDuration|number|false|null|Duration in milliseconds to animate the chart data.|
|data|array|false|null|The data for the Chart.|
|tooltip|boolean|false|null|Specifies whether or not the Chart contains tooltips.|
|type|string|false|null|The type of Chart to display (bar, area, line, pie).|

## Usage
```js
import {Chart} from "@afw/react";

const App = (props) => {
    return <Chart />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Chart.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Chart.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Chart.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Chart.test.js`

   Test cases used by @testing-library/jest.

 * `Chart.stories.js`

   Storybook stories for showcasing the component.

 * `Chart.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Chart.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Chart.json)

