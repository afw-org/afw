// See the 'COPYING' file in the project root for licensing information.
import {
    ComponentsContext, 
    QualifiersContext,   
    ClipboardContext,
} from "./context";

import {AdaptiveLayoutRegistry} from "./layouts";
import {ModelProvider} from "./ModelProvider";
import {isArray} from "@afw/react";


/**
 * combineComponentRegistries(implementationId, registries)
 *
 * This function will take one or more component registries and combine
 * them together to form a composite provider.  This is useful, if you 
 * need to use multiple component libraries.  For example, Material UI
 * for Form components and Victory for chart components.
 */
export const combineComponentRegistries = (implementationId, registries) => {
    let compositeRegistry = { implementationId };

    if (isArray(registries)) {
        for (const registry of registries) {
            // merge existing registries with new components
            compositeRegistry.components = {                 
                ...compositeRegistry.components, 
                ...registry.components 
            };
        }
    } else {
        throw new Error("Error:  combineComponentRegistries expects an array of component registries.");
    }

    return compositeRegistry;
};


/**
 * AdaptiveProvider
 *
 * This component is a high-level component that takes 
 *   1.  componentRegistry (component registries)
 *   2.  model (AfwModel instance)
 *   3.  children to provide this adaptive instance for.
 */
export const AdaptiveProvider = ({ 
    componentRegistry, 
    client,
    debounce,
    children, 
    qualifiers = [], 
    onCopy, 
    clipboard 
}) => {
    const AfwLayoutRegistry = new AdaptiveLayoutRegistry();

    /* 
     *  export the layout registry to the global window object, so third-party extensions 
     *  can locate it and register their layouts and components 
     */
    window.AfwLayoutRegistry = AfwLayoutRegistry;

    return (
        <ComponentsContext.Provider value={componentRegistry}>
            <ClipboardContext.Provider value={{ onCopy, clipboard }}>                
                <ModelProvider client={client} debounce={debounce}>
                    <QualifiersContext.Provider value={qualifiers}>
                        { children }                    
                    </QualifiersContext.Provider>
                </ModelProvider>
            </ClipboardContext.Provider>
        </ComponentsContext.Provider>
    );
};

export default AdaptiveProvider;
