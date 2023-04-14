// See the 'COPYING' file in the project root for licensing information.
import * as mui from "./components";

/**
 * index.js
 * 
 * Registers the Material UI AFW Component Implementation package.
 */
export const componentRegistry = {
    implementationId:   "@afw/react-material-ui",
    initialize:         () => undefined,
    setTheme:           () => undefined,
    components:         { ...mui }
};

export default componentRegistry;
