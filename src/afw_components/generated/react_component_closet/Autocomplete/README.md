# Autocomplete

Component to display a list of options that may be selected or can be autocompleted by typing.

## Description
This component provides a dropdown picker with possible values to quickly select from. By typing, it will narrow down the selection to make the list even shorter to choose a selection from. Additionally, it will offer the user the ability to enter a new value, if allowed.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|allowWrite|boolean|false|null|Specifies whether the Autocomplete component can be changed.|
|disabled|boolean|false|false|Specifies whether the Autocomplete component is disabled.|
|isLoading|boolean|false|false|Specifies whether the Autocomplete component is currently loading.|
|label|string|false|null|The label for the Autocomplete component.|
|options|array|false|null|The available options for the Autocomplete component.|
|placeholder|string|false|null|Specifies the placeholder text displayed when no value is present.|
|required|boolean|false|null|Specifies whether this Autocomplete component is required.|
|value|object|false|null|The option value for the Autocomplete component.|

## Usage
```js
import {Autocomplete} from "@afw/react";

const App = (props) => {
    return <Autocomplete />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Autocomplete.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Autocomplete.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Autocomplete.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Autocomplete.test.js`

   Test cases used by @testing-library/jest.

 * `Autocomplete.stories.js`

   Storybook stories for showcasing the component.

 * `Autocomplete.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Autocomplete.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Autocomplete.json)

