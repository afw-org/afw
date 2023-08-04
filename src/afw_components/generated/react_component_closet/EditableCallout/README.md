# EditableCallout

A component that calls out with an editable multiline text area.

## Description
This component wraps an editable text field inside a Callout component. This is useful when a label needs to be quickly edited by popping up an editor, rather than navigating them to a new page. The EditableLabel component that it displays may be a multiline text field.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|allowWrite|boolean|false|null|Specifies whether the EditableCallout component is writable.|
|disabled|boolean|false|null|Specifies whether the EditableCallout component is disabled.|
|open|boolean|true|null|Specifies whether the EditableCallout component is visible.|
|placeholder|string|false|null|The placeholder value for the text field when no value exists.|
|value|string|false|null|The value for the EditableCallout component.|

## Usage
```js
import {EditableCallout} from "@afw/react";

const App = (props) => {
    return <EditableCallout />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `EditableCallout.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `EditableCallout.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `EditableCallout.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `EditableCallout.test.js`

   Test cases used by @testing-library/jest.

 * `EditableCallout.stories.js`

   Storybook stories for showcasing the component.

 * `EditableCallout.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/EditableCallout.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_EditableCallout.json)

