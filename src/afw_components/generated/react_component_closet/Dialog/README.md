# Dialog

A layout container that displays content inside a popup, with a confirmation response.

## Description
This component container is visible when a target element is clicked. It's used to quickly show or grab input from the user in a popup area and can be dismissed by confirming or cancelling via buttons.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|contains|object|false|null|Any contents that needs to be rendered inside the Dialog.|
|footer|object|false|null|Any contents that needs to be rendered inside the Dialog's footer area.|
|fullScreen|boolean|false|false|Should the Dialog be full screen?|
|isBlocking|boolean|false|null|Determines if the Dialog should block dismissing until the user clicks a button.|
|maxWidth|string|false|null|The maximum width of the Dialog.|
|open|boolean|false|null|Specifies whether this Dialog component is open.|
|showClose|boolean|false|false|Should a close button be rendered, to allow the user to dismiss the Dialog.|
|subText|string|false|null|The content text to display inside the dialog box, directly under the Title.|
|title|string|false|null|The title text to display at the top of the dialog box.|

## Usage
```js
import {Dialog} from "@afw/react";

const App = (props) => {
    return <Dialog />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Dialog.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Dialog.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Dialog.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Dialog.test.js`

   Test cases used by @testing-library/jest.

 * `Dialog.stories.js`

   Storybook stories for showcasing the component.

 * `Dialog.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Dialog.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Dialog.json)

