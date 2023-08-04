# Box

A component that renders a flexible Box of content.

## Description
This component is a very powerful container, which implements a box allowing items to be added in a flexible way.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|alignContent|string|false|null|This aligns a flex container's lines within when there is extra space in the cross-axis, similar to how "Justify Content" aligns individual items within the main-axis.|
|alignItems|string|false|"stretch"|This defines the default behavior for how flex items are laid out along the cross axis on the current line. Think of it as the "Justify Content" version for the cross axis (perpendicular to the main-axis).|
|contains|array|false|null|This declares a list of items to be rendered inside the Box.|
|direction|string|false|"row"|This declares the direction that items are oriented inside the Box.|
|justifyContent|string|false|"flex-start"|This defines the alignment along the main axis. It helps distribute extra free space leftover when either all the flex items on a line are inflexible, or are flexible but have reached their maximum size. It also exerts some control over the alignment of items when they overflow the line.|
|wrap|string|false|"nowrap"|By default, flex items will all try to fit onto one line. You can change that and allow the items to wrap as needed with this property.|

## Usage
```js
import {Box} from "@afw/react";

const App = (props) => {
    return <Box />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Box.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Box.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Box.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Box.test.js`

   Test cases used by @testing-library/jest.

 * `Box.stories.js`

   Storybook stories for showcasing the component.

 * `Box.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Box.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Box.json)

