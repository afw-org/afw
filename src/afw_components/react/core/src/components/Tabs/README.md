# Tabs

A layout container that renders a series of tabs.

## Description
This component displays a list of components, one at a time, by containing the content inside of their individual tabs.  A tab has a clickable link, which when clicked, will switch the content to the desired component.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|gapSpace|number|false|null|The amount of space between the tabs and tabbed content.|
|tabs|array|false|null|The tabs that are defined for this Tabs component.|

## Usage
```js
import {Tabs} from "@afw/react";

const App = (props) => {
    return <Tabs />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Tabs.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Tabs.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Tabs.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Tabs.test.js`

   Test cases used by @testing-library/jest.

 * `Tabs.stories.js`

   Storybook stories for showcasing the component.

 * `Tabs.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Tabs.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Tabs.json)

