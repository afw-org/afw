# Button

Component to display a Button.

## Description
This component encapsulates a wide range of Button features, including labels, icons and colors.  The Button component may even allow for a dropdown of selected actions to perform when clicked.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|color|string|false|null|Specifies an optional color for this Button (primary, secondary, error).|
|disabled|boolean|false|null|Specifies whether this Button component is disabled.|
|dropdown|boolean|false|null|Specifies an optional dropdown icon for this Button.|
|icon|string|false|null|Specifies an optional icon for this Button.|
|label|string|false|null|Specifies the text on the Button.|
|secondaryText|string|false|null|Specifies optional text to be added below the main text label.|
|size|string|false|null|Specifies a size for the button (small)|
|type|string|false|null|The type of Button (icon, split, compound, default).|

## Usage
```js
import {Button} from "@afw/react";

const App = (props) => {
    return <Button />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Button.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Button.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Button.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Button.test.js`

   Test cases used by @testing-library/jest.

 * `Button.stories.js`

   Storybook stories for showcasing the component.

 * `Button.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Button.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Button.json)

