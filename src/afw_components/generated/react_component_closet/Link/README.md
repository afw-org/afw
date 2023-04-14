# Link

A component that renders a url link.

## Description
This component understands how to render a link with the appropriate framework style, and handles the React Routing, if the link references another component within this same application.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|disabled|boolean|false|null|Specifies whether the the Link is disabled.|
|text|string|false|null|The text value displayed for the Link component.|
|url|string|false|null|The URL for the link.|

## Usage
```js
import {Link} from "@afw/react";

const App = (props) => {
    return <Link />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Link.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Link.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Link.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Link.test.js`

   Test cases used by @testing-library/jest.

 * `Link.stories.js`

   Storybook stories for showcasing the component.

 * `Link.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Link.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Link.json)

