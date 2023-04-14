# List

A layout container that renders a list of components.

## Description
This component is a simple container component that renders its contents in a sequential, scrollable list.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|contains|array|false|null|The layout components that are contained by this List component.|
|selectable|boolean|false|null|Can the List items be selected?|

## Usage
```js
import {List} from "@afw/react";

const App = (props) => {
    return <List />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `List.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `List.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `List.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `List.test.js`

   Test cases used by @testing-library/jest.

 * `List.stories.js`

   Storybook stories for showcasing the component.

 * `List.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/List.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_List.json)

