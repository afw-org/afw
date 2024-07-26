// See the 'COPYING' file in the project root for licensing information.
import {AppCoreContext} from "../context";
import {useLoadApplicationData} from "@afw/react";

import {AppError} from "./AppError";

/**
 * AppCoreProvider
 * 
 * This component loads and provides all core objects, data types, functions, 
 * extensions, adapters, services and other pieces of Adaptive Framework that 
 * will be used throughout the administrative app.
 * 
 */
export const AppCoreProvider = ({ children }) => {
    const {
        appData, 
        environment, 
        afwComponents, 
        objectTypeObjects, 
        services, 
        application,
        error,
        refresh,
    } = useLoadApplicationData();    

    const reloadApp = async () => {
        refresh();
    };

    const reloadServices = async () => {
        refresh();
    };

    const reloadAdapters = async () => {
        refresh();
    };

    const value = { 
        ...appData,         
        afwComponents,
        objectTypeObjects,
        environment, 
        services, 
        application,
        error, 
        reloadApp, 
        reloadServices, 
        reloadAdapters 
    };

    /* wait for appData to load before loading anything else */
    if (!appData && (error)) {
        return (
            <AppError
                title="Error Loading Application Data"
                message="There was an error loading the application.  You may click Refresh or Browser Refresh to retry the request.
                If the problem persists, make sure afwfcgi service is running, or check the server for errors."
                error={error}
                reload={reloadApp}
            />
        );
    }        

    if (!environment)
        return null;

    return (
        <AppCoreContext.Provider value={value}>
            { children }
        </AppCoreContext.Provider>
    );
};

export default AppCoreProvider;
