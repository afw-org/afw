// See the 'COPYING' file in the project root for licensing information.
/**
 * Useful hooks to use with Monaco environment.
 */

import {useContext} from "react";

import {MonacoContext} from "./context";

/* hook to get the global monaco object */
export const useMonaco = () => {
    return useContext(MonacoContext);
};
