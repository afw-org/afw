// See the 'COPYING' file in the project root for licensing information.
/**
 * Custom ReactJS Hooks for the app to use.
 */

import {useContext, useEffect, useState, useCallback, useLayoutEffect, useMemo} from "react";
import {useLocation} from "react-router";
import {css} from "@emotion/css";
import {useTheme} from "@emotion/react";

import {AppContext, ConfigContext, AppCoreContext, NotificationContext} from "./context";

/**
 * A hook to get Application data via the useContext hook.
 */
export const useApplication = () => {  
    return useContext(AppContext);
};

/**
 * A hook to get the underlying app client 
 */
export const useClient = () => {
    const {client} = useContext(AppContext);

    return client;
};

/**
 * A hook to get the Configuration data via the useContext hook.
 */
export const useConfig = () => {
    return useContext(ConfigContext);
};


/**
 * A hook to get the core objects via the useContext hook.
 */
export const useAppCore = () => {
    return useContext(AppCoreContext);
};

/**
 * A hook to get the Notification alerts.
 */
export const useNotifications = () => {
    return useContext(NotificationContext);
};


export const useBreadcrumbs = (root, reducer) => {

    const {pathname} = useLocation();
    
    let breadcrumbItems = [];

    if (pathname) {
        if (root)
            breadcrumbItems.push(root);

        if (reducer)
            breadcrumbItems = reducer(breadcrumbItems);

        else {
            const parts = pathname.split("/");
            let link = root ? root.link : "";
            for (let i = 2; i < parts.length; i++) {
                link += "/" + parts[i];
                breadcrumbItems.push({
                    text: parts[i],
                    link
                });
            }
        }
    }

    return breadcrumbItems;
};

/**
 * useLocalJSONStorage()
 *
 * A hook that provides the user with the value of Local Storage identifier
 * (within "afw") and provides notifications/update routines when they are
 * changed.
 *
 * All values in/out are in JSON formatted strings.
 *
 * @param {string} identifier
 * @param {any} initialValue
 */
export const useLocalJSONStorage = (identifier, initialValue) => {

    const [storage, setStorage] = useState();
    const [error, setError] = useState();

    useEffect(() => {
        if (identifier) {
            let afwStorage = localStorage.getItem("afw");
            if (!afwStorage) {
                /* if we don't have "afw" in storage yet, go ahead and create it */
                localStorage.setItem("afw", "{}");
                afwStorage = "{}";
            }

            try {
                /* parse the JSON result from localStorage back into a value */
                afwStorage = JSON.parse(afwStorage);
                if (!afwStorage[identifier] && initialValue) {
                    afwStorage[identifier] = initialValue;
                    localStorage.setItem("afw", JSON.stringify(afwStorage));
                }
                setStorage(afwStorage[identifier]);
            } catch (e) {
                /* incognito mode?  size limit? */
                setError(e);
            }
        }
    }, [identifier, initialValue]);

    const onChanged = useCallback((s) => {
        try {
            let afwStorage = localStorage.getItem("afw");
            if (afwStorage) {
                afwStorage = JSON.parse(afwStorage);

                afwStorage[identifier] = s;
                localStorage.setItem("afw", JSON.stringify(afwStorage));

                setStorage(s);
            }
        } catch (e) {
            /* incognito mode?  size limit? */
            setError(e);
        }
    }, [identifier]);

    return { storage, onChanged, error };
};

/**
 * useDimensions()
 * 
 * A hook to grab the current component's width and height.
 */
export const useDimensions = ({ targetRef }) => {    
    const [dimensions, setDimensions] = useState({ width: 0, height: 0 });

    useLayoutEffect(() => {
        if (targetRef && targetRef.current) {
            setDimensions({
                width: targetRef.current.offsetWidth,
                height: targetRef.current.offsetHeight,
            });
        }
    }, [targetRef]);

    return dimensions;
};

export const useClasses = stylesElement => {
    const theme = useTheme();
    return useMemo(() => {
        const rawClasses = typeof stylesElement === "function" ? stylesElement(theme) : stylesElement;
        const prepared = {};

        Object.entries(rawClasses).forEach(([key, value = {}]) => {
            prepared[key] = css(value);
        });

        return prepared;
    }, [stylesElement, theme]);
};

export {useTheme};
