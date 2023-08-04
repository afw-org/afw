# Dropdown

A component that displays a set of options that can be selected from a dropdown.

## Description
This component, given a set of options, provides the user with the ability to select one of the values by clicking on the dropdown icon and selecting the appropriate value from a list.

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
import {Dropdown} from "@afw/react";

const App = (props) => {
    return <Dropdown />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Dropdown.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Dropdown.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Dropdown.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Dropdown.test.js`

   Test cases used by @testing-library/jest.

 * `Dropdown.stories.js`

   Storybook stories for showcasing the component.

 * `Dropdown.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Dropdown.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Dropdown.json)

