# EditableLabel

A component that renders an label, which can be edited when clicked.

## Description
This component provides a very quick way for a label to become 'editable' simply by clicking on the text label. After clicking on the label, the component changes from a static label to an editable TextField component.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|multiline|boolean|false|null|Specifies whether the TextField for the EditableLabel needs to be multilined.|
|placeholder|string|false|null|The placeholder value for the text field when no value exists.|
|value|string|false|null|The value for the EditableLabel component.|

## Usage
```js
import {EditableLabel} from "@afw/react";

const App = (props) => {
    return <EditableLabel />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `EditableLabel.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `EditableLabel.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `EditableLabel.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `EditableLabel.test.js`

   Test cases used by @testing-library/jest.

 * `EditableLabel.stories.js`

   Storybook stories for showcasing the component.

 * `EditableLabel.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/EditableLabel.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_EditableLabel.json)

