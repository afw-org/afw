# TextField

A component that renders a text field with a label and description.

## Description
This is a simple component for inputting text. It may display an outlined or underlined field from which the user can enter text into. It may also display a label and description to help the user understand its contents.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|description|string|false|null|The description for the TextField component that describes this input.|
|disabled|boolean|false|null|Specifies whether the the TextField is disabled.|
|label|string|false|null|The label for the TextField component.|
|multiline|boolean|false|null|Specifies whether this TextField component is multilined.|
|placeholder|string|false|null|The placeholder text for this component.|
|required|boolean|false|null|Specifies whether this TextField component is required.|
|rows|number|false|null|Specifies the number of rows to display.|
|value|string|false|null|The value of the TextField component.|

## Usage
```js
import {TextField} from "@afw/react";

const App = (props) => {
    return <TextField />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `TextField.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `TextField.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `TextField.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `TextField.test.js`

   Test cases used by @testing-library/jest.

 * `TextField.stories.js`

   Storybook stories for showcasing the component.

 * `TextField.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/TextField.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_TextField.json)

