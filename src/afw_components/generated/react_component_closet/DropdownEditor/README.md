# DropdownEditor

A component that manages a list of options in a Dropdown picker.

## Description
This component, given a set of options, provides the user with the ability to select, edit, add or delete one of the values by clicking on the dropdown icon and selecting the appropriate value and button from a list.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|description|string|false|null|The description for this dropdown editor.|
|editable|boolean|false|null|Whether the options managed by this Dropdown Editor are editable.|
|label|string|false|null|The label for the Dropdown component.|
|multiselect|boolean|false|null|Whether this Dropdown allows for multiple selections.|
|options|array|false|null|Specifies the available options for the dropdown.|
|selectedKeys|array|false|null|The key(s) that are selected for the Dropdown component.|

## Usage
```js
import {DropdownEditor} from "@afw/react";

const App = (props) => {
    return <DropdownEditor />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `DropdownEditor.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `DropdownEditor.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `DropdownEditor.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `DropdownEditor.test.js`

   Test cases used by @testing-library/jest.

 * `DropdownEditor.stories.js`

   Storybook stories for showcasing the component.

 * `DropdownEditor.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/DropdownEditor.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_DropdownEditor.json)

