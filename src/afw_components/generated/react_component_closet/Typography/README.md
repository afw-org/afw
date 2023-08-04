# Typography

A component that renders a text label.

## Description
This component is a very simple one, which renders text into a static label with specified color, size or style..

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|color|string|false|null|The color of the text. This may be a theme definition, or an actual color.|
|size|string|false|null|The size of the text (1-11).|
|tag|string|false|null|This allows you to explicitly set the underlying HTML tag in order to get the semantics correct.|
|text|string|true|null|The actual text content to be rendered by this component.|

## Usage
```js
import {Typography} from "@afw/react";

const App = (props) => {
    return <Typography />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Typography.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Typography.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Typography.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Typography.test.js`

   Test cases used by @testing-library/jest.

 * `Typography.stories.js`

   Storybook stories for showcasing the component.

 * `Typography.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Typography.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Typography.json)

