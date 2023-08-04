# Menu

A component that renders a Menu to be displayed.

## Description
This component pops up a Menu of items, each of which take an appropriate action when clicked.  Menu items may also open up new submenus in order to organize or filter a set of items in a way that's easy for the end user to understand and select from.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|items|array|false|null|Item to display under this Menu.|
|open|boolean|false|null|Specifies whether this menu is open.|

## Usage
```js
import {Menu} from "@afw/react";

const App = (props) => {
    return <Menu />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Menu.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Menu.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Menu.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Menu.test.js`

   Test cases used by @testing-library/jest.

 * `Menu.stories.js`

   Storybook stories for showcasing the component.

 * `Menu.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Menu.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Menu.json)

