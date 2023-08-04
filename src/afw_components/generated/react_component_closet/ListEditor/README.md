# ListEditor

A component that allows the user to manage a list of text items.

## Description
This component renders a list of text and optional secondary text, items, which allows the user to add, edit or delete items from the list.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|editable|boolean|false|null|Specifies whether this list is currently editable.|
|items|array|false|null|The list of items to be managed.|

## Usage
```js
import {ListEditor} from "@afw/react";

const App = (props) => {
    return <ListEditor />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `ListEditor.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `ListEditor.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `ListEditor.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `ListEditor.test.js`

   Test cases used by @testing-library/jest.

 * `ListEditor.stories.js`

   Storybook stories for showcasing the component.

 * `ListEditor.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/ListEditor.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_ListEditor.json)

