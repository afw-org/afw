# Hidden

A component that hides content under screen breakpoint conditions.

## Description
This is a utility component that gives a layout the ability to hide content under screen size conditions. This is necessary for responsive layouts to provide both complex or simply designs that are accessible and usable from a variety of screen devices.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|contains|object|false|null|The contained component that will be hidden.|
|for|array|false|null|The breakpoint conditions for which the contained component will be hidden (xs, sm, md, lg, xl).|
|lgDown|boolean|false|null|This content will only be displayed on large displays or smaller.|
|mdDown|boolean|false|null|This content will only be displayed on medium displays or smaller.|
|smDown|boolean|false|null|This content will only be displayed on small displays or smaller.|
|xlDown|boolean|false|null|This content will only be displayed on extra large displays or smaller.|
|xsDown|boolean|false|null|This content will only be displayed on extra small displays.|

## Usage
```js
import {Hidden} from "@afw/react";

const App = (props) => {
    return <Hidden />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Hidden.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Hidden.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Hidden.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Hidden.test.js`

   Test cases used by @testing-library/jest.

 * `Hidden.stories.js`

   Storybook stories for showcasing the component.

 * `Hidden.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Hidden.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Hidden.json)

