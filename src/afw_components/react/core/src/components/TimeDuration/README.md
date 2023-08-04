# TimeDuration

A component that renders a time duration.

## Description
This component understands a time duration data type and renders a convenient way to select a duration of time.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|dayTimeDuration|string|false|null|The value of the TimeDuration component expressed as a dayTime.|
|description|string|false|null|The description for the TimeDuration component that describes this input.|
|disabled|boolean|false|null|Specifies whether the the TimeDuration is disabled.|
|label|string|false|null|The label for the TimeDuration component.|
|required|boolean|false|null|Specifies whether the the TimeDuration is required.|
|yearMonthDuration|string|false|null|The value of the TimeDuration component expressed as a yearMonth.|

## Usage
```js
import {TimeDuration} from "@afw/react";

const App = (props) => {
    return <TimeDuration />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `TimeDuration.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `TimeDuration.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `TimeDuration.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `TimeDuration.test.js`

   Test cases used by @testing-library/jest.

 * `TimeDuration.stories.js`

   Storybook stories for showcasing the component.

 * `TimeDuration.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/TimeDuration.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_TimeDuration.json)

