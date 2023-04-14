# Icon

A component that displays an icon.

## Description
This component displays an icon, which references a common dictionary of icon names, each of which may map to the appropriate icon library depending on the framework that is being used. The icon itself may be clickable.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|iconName|string|false|null|The name of the icon to render.|

## Usage
```js
import {Icon} from "@afw/react";

const App = (props) => {
    return <Icon />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Icon.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Icon.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Icon.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Icon.test.js`

   Test cases used by @testing-library/jest.

 * `Icon.stories.js`

   Storybook stories for showcasing the component.

 * `Icon.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Icon.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Icon.json)

