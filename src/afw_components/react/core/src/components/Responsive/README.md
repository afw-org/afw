# Responsive

A layout container that renders content in a grid format, with breakpoints defined for a variety of screen sizes.

## Description
This container component defines a Row/Column grid of components to display content horizontally and vertically, depending on the screen size.  For larger displays, you may assign breakpoints that allow for all content to fit, while on smaller mobile devices, content will be divided up into chunks.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|alignContent|string|false|null|Align content.|
|alignItems|string|false|null|Align items.|
|justify|string|false|null|Justify content.|
|rows|array|true|null|The layout row components that are contained by this Responsive.|
|spacing|number|false|null|Spacing between rows.|

## Usage
```js
import {Responsive} from "@afw/react";

const App = (props) => {
    return <Responsive />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Responsive.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Responsive.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Responsive.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Responsive.test.js`

   Test cases used by @testing-library/jest.

 * `Responsive.stories.js`

   Storybook stories for showcasing the component.

 * `Responsive.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Responsive.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Responsive.json)

