# Snackbar

A component that renders a Snackbar to be displayed.

## Description
This component pops up a small bar at the bottom of the screen to alert the user of a recent activity, such as a success or error status, typically following an action that was taken by the user.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|duration|number|false|null|The amount of time, in milliseconds to display the Snackbar.|
|message|string|false|null|The message to be displayed in the Snackbar component.|
|open|boolean|false|null|Specifies whether the Snackbar is open or not.|
|type|string|false|null|The type of message for the Snackbar component (error, warning, info, success).|

## Usage
```js
import {Snackbar} from "@afw/react";

const App = (props) => {
    return <Snackbar />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Snackbar.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Snackbar.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Snackbar.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Snackbar.test.js`

   Test cases used by @testing-library/jest.

 * `Snackbar.stories.js`

   Storybook stories for showcasing the component.

 * `Snackbar.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Snackbar.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Snackbar.json)

