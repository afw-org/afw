# Table

A component that displays data in a table format.

## Description
This component arranges data into rows and columns, which can be optionally sorted, selected and edited through actions.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|columns|array|true|null|The columns that are defined for this Table component.|
|compact|boolean|false|null|Indicates whether this Table compact appears compact.|
|isHeaderVisible|boolean|false|null|Indicates whether this Table displays column headers.|
|rows|array|true|null|The rows that are defined for this Table component.|
|selectionMode|string|false|null|The selection mode for this table (none, single, multiple).|

## Usage
```js
import {Table} from "@afw/react";

const App = (props) => {
    return <Table />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Table.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Table.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Table.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Table.test.js`

   Test cases used by @testing-library/jest.

 * `Table.stories.js`

   Storybook stories for showcasing the component.

 * `Table.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Table.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Table.json)

