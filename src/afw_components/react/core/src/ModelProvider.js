// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {ModelContext} from "./context";
import {AfwModel} from "@afw/client";

/**
 * ModelProvider
 *
 * This component provides an AfwModel object for applications
 * that wish to use it to retrieve and manage adaptive objects
 * using its modeling capabilities, such as caching, event 
 * dispatching and debouncing, object type dependency resolution, 
 * and getter/setter functions.
 */
export const ModelProvider = ({ client, debounce, children }) => {

    const [model, setModel] = useState();

    useEffect(() => {
        /* create a default AfwModel object */
        if (client)
            setModel( new AfwModel({ client, debounce }) );                
    }, [client, debounce]);

    return (
        <ModelContext.Provider value={model}>
            { children }
        </ModelContext.Provider>
    );
};

export default ModelProvider;
