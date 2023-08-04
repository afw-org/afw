# Tooltip

A component that surrounds content with a tooltip.

## Description
This component renders a small popup with text when the user hovers over an element for a specific amount of time. This may provide helpful hints to explain to the user what they are looking at.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|contains|object|false|null|Content that can be rendered inside this tooltip display.|
|content|string|false|null|The content to display in the Tooltip.|
|delay|number|false|null|The time delay, in milliseconds, before displaying tooltip.|
|target|object|false|null|The target to wrap inside this tooltip.|

## Usage
```js
import {Tooltip} from "@afw/react";

const App = (props) => {
    return <Tooltip />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Tooltip.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Tooltip.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Tooltip.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Tooltip.test.js`

   Test cases used by @testing-library/jest.

 * `Tooltip.stories.js`

   Storybook stories for showcasing the component.

 * `Tooltip.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Tooltip.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Tooltip.json)

