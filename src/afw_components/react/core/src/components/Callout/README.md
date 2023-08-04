# Callout

A layout container that displays content inside a popup.

## Description
This component container is visible when a target element is clicked.  It's used to quickly show or grab input from the user in a popup area and can be dismissed.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|contains|object|false|null|Specifies the placeholder text displayed when no value is present.|
|gapSpace|number|false|null|The gap space between target and the Callout component.|
|open|boolean|true|null|Specifies whether this Callout component is open.|
|targetPosition|object|false|null|Specifies the position of the callout, relative to the target component.|

## Usage
```js
import {Callout} from "@afw/react";

const App = (props) => {
    return <Callout />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Callout.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Callout.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Callout.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Callout.test.js`

   Test cases used by @testing-library/jest.

 * `Callout.stories.js`

   Storybook stories for showcasing the component.

 * `Callout.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Callout.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Callout.json)

