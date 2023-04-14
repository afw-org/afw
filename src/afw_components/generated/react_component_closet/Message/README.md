# Message

A component that renders a message to be displayed.

## Description
This component displays a text message within a surface that may alert the user to additional information, errors or warnings.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|canDismiss|boolean|false|null|Specifies whether the Message can be dismissed.|
|contains|object|false|null|Optional components to be displayed for the Message component.|
|isMultiline|boolean|false|null|Specifies whether the Message message is multilined.|
|message|string|false|null|Optional message to be displayed for the Message component.|
|status|string|false|null|Status of message (error, info, warning).|

## Usage
```js
import {Message} from "@afw/react";

const App = (props) => {
    return <Message />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Message.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Message.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Message.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Message.test.js`

   Test cases used by @testing-library/jest.

 * `Message.stories.js`

   Storybook stories for showcasing the component.

 * `Message.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Message.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Message.json)

