# Spinner

A component that displays a spinner progress indicator.

## Description
This utility component displays a spinning circle to indicate to the user to wait for processing to complete.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|label|string|false|null|The label for the Spinner component.|
|size|string|false|null|The size of the Spinner component.|

## Usage
```js
import {Spinner} from "@afw/react";

const App = (props) => {
    return <Spinner />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Spinner.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Spinner.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Spinner.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Spinner.test.js`

   Test cases used by @testing-library/jest.

 * `Spinner.stories.js`

   Storybook stories for showcasing the component.

 * `Spinner.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Spinner.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Spinner.json)

