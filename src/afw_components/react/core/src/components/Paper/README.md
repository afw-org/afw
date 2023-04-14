# Paper

A component that allows contents to be displayed on a Paper like surface.

## Description
This component allows data on its surface to stand out by providing an edged, or raised outline, with shadow depth and optional corner radius style.  This component is another useful container to help separate a section of data from its surrounding contents.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|contains|object|false|null|Contained component on this Paper layout.|
|elevation|number|false|null|Depth elevation appearance of the Paper surface.|
|square|boolean|false|false|Use square corners instead of rounded.|

## Usage
```js
import {Paper} from "@afw/react";

const App = (props) => {
    return <Paper />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Paper.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Paper.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Paper.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Paper.test.js`

   Test cases used by @testing-library/jest.

 * `Paper.stories.js`

   Storybook stories for showcasing the component.

 * `Paper.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Paper.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Paper.json)

