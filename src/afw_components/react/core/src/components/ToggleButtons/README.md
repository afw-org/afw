# ToggleButtons

A component to display a selectable choice of options, presented as buttons that can be toggled on or off.

## Description
This component provides a list of options, organized as buttons, that may be toggled on or off.  They may be inclusive or exclusive.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|exclusive|boolean|false|true|Specifies whether the choices are mutually exclusive.|
|label|string|false|null|The label for the ToggleButtons component.|
|options|array|true|null|The available options for the ToggleButtons component.|
|required|boolean|false|null|Specifies whether this ToggleButtons component is required.|

## Usage
```js
import {ToggleButtons} from "@afw/react";

const App = (props) => {
    return <ToggleButtons />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `ToggleButtons.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `ToggleButtons.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `ToggleButtons.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `ToggleButtons.test.js`

   Test cases used by @testing-library/jest.

 * `ToggleButtons.stories.js`

   Storybook stories for showcasing the component.

 * `ToggleButtons.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/ToggleButtons.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_ToggleButtons.json)

