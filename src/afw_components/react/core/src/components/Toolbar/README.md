# Toolbar

A container component that arranges a series of buttons or other minor content in a horizontal bar.

## Description
This component is used to provide actions, via buttons, labels or other small components, condensed into a common location so the user may use it to view or change data that is presented in the layout below.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|alignContent|string|false|null|Defines the align-content style property.|
|alignItems|string|false|null|Defines the align-items style property.|
|contains|array|true|null|A list of components to be contained by this Toolbar.|
|dense|boolean|false|null|Specifies whether the Toolbar should be condensed as much as possible.|
|justify|string|false|null|Defines the justify-content style property.|
|spacing|number|false|null|The spacing between items in the Toolbar.|

## Usage
```js
import {Toolbar} from "@afw/react";

const App = (props) => {
    return <Toolbar />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Toolbar.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Toolbar.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Toolbar.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Toolbar.test.js`

   Test cases used by @testing-library/jest.

 * `Toolbar.stories.js`

   Storybook stories for showcasing the component.

 * `Toolbar.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Toolbar.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Toolbar.json)

