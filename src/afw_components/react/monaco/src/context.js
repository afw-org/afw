// See the 'COPYING' file in the project root for licensing information.
/**
 * @afw/react-monaco React Context
 *
 * Provides React Context for the Monaco environment.
 */

import {createContext} from "react";

export const MonacoContext = createContext();

/* export an HOC for consumers */
export const withMonacoContext = (Component) => (props) => 
    <MonacoContext.Consumer>
        { monaco => <Component monaco={monaco} {...props} /> }
    </MonacoContext.Consumer>;
