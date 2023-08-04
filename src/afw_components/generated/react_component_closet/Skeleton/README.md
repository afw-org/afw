# Skeleton

A component that displays a skeleton, or shape, of a component that may shimmer to indicate that an actual component is waiting to be loaded.

## Description
The Skeleton is a feedback utility component, similar to the Spinner, but it takes on a shape that gives the user the perspective that data is nearly ready and the component is attempting to render. It's a component placeholder, which often animates or shimmers.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|height||false|null|The CSS height of the skeleton.|
|type|string|false|null|The shape of the skeleton to display (line or circle).|
|width||false|null|The CSS width of the skeleton.|

## Usage
```js
import {Skeleton} from "@afw/react";

const App = (props) => {
    return <Skeleton />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Skeleton.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Skeleton.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Skeleton.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Skeleton.test.js`

   Test cases used by @testing-library/jest.

 * `Skeleton.stories.js`

   Storybook stories for showcasing the component.

 * `Skeleton.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Skeleton.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Skeleton.json)

