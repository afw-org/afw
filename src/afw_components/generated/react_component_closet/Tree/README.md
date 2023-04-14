# Tree

A component that renders data in a Tree structure.

## Description
This component helps to organize content into a Tree structure, where nodes can be selected, expanded, or collapsed to allow the user to navigate more complex nested data or component structures.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|children|array|true|null|The children of this Tree.|
|dense|boolean|false|null|The tree nodes should be condensed in space.|
|expandedKeys|array|false|null|The keys of nodes in this tree that are expanded.|
|selectedKeys|array|false|null|The keys of nodes in this tree that are selected.|

## Usage
```js
import {Tree} from "@afw/react";

const App = (props) => {
    return <Tree />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Tree.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Tree.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Tree.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Tree.test.js`

   Test cases used by @testing-library/jest.

 * `Tree.stories.js`

   Storybook stories for showcasing the component.

 * `Tree.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Tree.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Tree.json)

