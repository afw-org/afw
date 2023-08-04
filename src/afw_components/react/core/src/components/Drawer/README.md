# Drawer

A component that renders content inside a side sheet anchored to one of the edges.

## Description
This component, which contains other content, is anchored to one of the edges of the main window.  It's often animated to slide into view when an action occurs, but can be optionally docked, or permanent.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|anchor|string|false|"left"|This property describes which side of the page, the drawer should be anchored to.  It may be one of 'bottom', 'top', 'left' or 'right'.|
|contains|array|false|null|This declares a list of items to be rendered inside the Drawer.|
|open|boolean|true|null|Specifies whether this Drawer component is open.|
|variant|string|false|"temporary"|This property describes whether the drawer should be anchored temporary, persistent or permanent. If temporary, the drawer will be dismissed when the user unblocks its focus.  If persistent, the drawer will be displayed until the user manually closes it.  If permanent, the drawer will always remain open.|

## Usage
```js
import {Drawer} from "@afw/react";

const App = (props) => {
    return <Drawer />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Drawer.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Drawer.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Drawer.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Drawer.test.js`

   Test cases used by @testing-library/jest.

 * `Drawer.stories.js`

   Storybook stories for showcasing the component.

 * `Drawer.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Drawer.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Drawer.json)

