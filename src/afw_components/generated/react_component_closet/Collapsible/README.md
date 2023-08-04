# Collapsible

A layout container that displays content that can be hidden by a collapsible control.

## Description
This component helps organize data into a group, which can be temporarily concealed or revealed by clicking on the collapse or expand icons. This may be important for preserving real estate on the screen, or reducing the amount of data that may be present at a given time, to avoid overloading the end user with information.

## Props
| Name | Type | Required | Default | Description |
|:----------|:----------|:----|:------------|:------------|
|contains|object|false|null|Specifies the component contained within this Collapsible Body.|
|headerItems|object|false|null|Specifies the component contained within this Collapsible Header.|
|isCollapsed|boolean|false|true|Specifies whether the Collapsible content is collapsed or not.|
|showCollapse|boolean|false|true|Specifies whether the Collapsible should render the collapse feature.|

## Usage
```js
import {Collapsible} from "@afw/react";

const App = (props) => {
    return <Collapsible />
}
```

## Files
Each file in this directory has been generated for specific purposes.
 * `Collapsible.js`

   This is the ReactJS component directly imported from this package.

   It will use registered implementations, or call a fallback for default.
 * `Collapsible.propTypes.js`

   Generated propTypes, which declare the parameters using React propType definitions.

 * `Collapsible.fallback.js`

   A default fallback implementation, if none are supplied by the client.

 * `Collapsible.test.js`

   Test cases used by @testing-library/jest.

 * `Collapsible.stories.js`

   Storybook stories for showcasing the component.

 * `Collapsible.d.ts`

   Typescript declaration file for declaring the type information for this component.

 * `index.js`

   An index that exports all relevant Component and auxiliary functions or objects for easy import.

 * `index.d.ts`

   A typescript declaration index that exports all relevant type information for third-party tools to use.

 * `README.md`

   This file that you are reading.

## Reference
This README was generated from:
  * The Component Definition, located [here](/src/afw_components/generate/objects/_AdaptiveLayoutComponentType_/Collapsible.json)
  * The Object Type definition, located [here](/src/afw_components/generate/objects/_AdaptiveObjectType_/_AdaptiveLayoutComponentType_Collapsible.json)

