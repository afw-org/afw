# Picker

A component that allows one or more items to be picked from a list.

## Description
This component allows for zero or more items to be selected from a list.  When an item is selected, it appears as a tag in the selection, which may also be later removed.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|description|string|false|null|The text description for this Picker component.|
|items|array|false|null|Items to be displayed by this Picker layout.|
|label|string|false|null|The text label for this Picker component.|

## Usage
```js
import {Picker} from "@afw/react";

const App = (props) => {
    return <Picker />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Picker.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Picker.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Picker.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Picker.test.js`

   Test cases used by @testing-library/jest.

 * `Picker.stories.js`

   Storybook stories for showcasing the component.

 * `Picker.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Picker.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Picker.json)

