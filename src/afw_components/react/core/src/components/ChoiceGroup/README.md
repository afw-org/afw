# ChoiceGroup

A component to display a selectable choice of options.

## Description
This component provides a mutually exclusive list of options, organized as radio buttons, that may be selected.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|label|string|false|null|The label for the ChoiceGroup component.|
|options|array|true|null|The available options for the ChoiceGroup component.|
|orientation|string|false|null|The orientation of the Choice Group (horizontal or vertical).|
|required|boolean|false|null|Specifies whether this ChoiceGroup component is required.|
|selectedKey|string|false|null|The value of the selected key for the ChoiceGroup options.|

## Usage
```js
import {ChoiceGroup} from "@afw/react";

const App = (props) => {
    return <ChoiceGroup />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `ChoiceGroup.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `ChoiceGroup.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `ChoiceGroup.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `ChoiceGroup.test.js`

   Test cases used by @testing-library/jest.

 * `ChoiceGroup.stories.js`

   Storybook stories for showcasing the component.

 * `ChoiceGroup.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/ChoiceGroup.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_ChoiceGroup.json)

