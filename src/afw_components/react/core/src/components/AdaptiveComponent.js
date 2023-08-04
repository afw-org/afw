// See the 'COPYING' file in the project root for licensing information.
import {forwardRef, isValidElement, cloneElement, createElement} from "react";
import {useComponents} from "../hooks";
import {cls} from "@afw/react";

/**
 * This is the main entry point for rendering a React component, given basic layout 
 * component information.  Adaptive Components use componentType and parameters, which
 * are translated into React-specific <ComponentType> and props.
 * 
 * @param {*} componentDefinitions 
 * @param {*} layoutComponent 
 * @param {*} fallback 
 * @param {*} props 
 * @returns 
 */
export const render = (componentDefinitions, layoutComponent, fallback, props) => {

    // check for native React elements
    if (isValidElement(layoutComponent)) {         
        return cloneElement(layoutComponent, props);
    }

    if (!layoutComponent)
        return null;

    const {componentType, parameters} = layoutComponent;
    if (componentDefinitions[componentType] !== undefined) {
        return createElement(
            componentDefinitions[componentType],
            { ...props, ...parameters },
        );
    }
    
    else if (fallback) {
        return fallback({ ...props, ...parameters });
    }

    // The UI is going to have a missing component, so warn with a message
    console.warn("Warning:  You are trying to use an Adaptive Component that has no implementation or fallback provided: " + componentType);
    
    return null;
};

/**
 * AdaptiveComponent
 * 
 * Renders a React component using the component definition provided by props.  It will leverage
 * the Components registered in its context, lookup the appropriate componentType and return the 
 * actual React component if it exists, or use the fallback, if provided.
 * 
 * This is primarily used by @afw/react.  But, also in component implementations that have nested 
 * components, where the nested component may also use Adaptive Components.
 */
export const AdaptiveComponent = forwardRef((props, ref) => {
    
    // Get the components registered in our context
    const componentDefinitions = useComponents();

    const {layoutComponent, fallback, ...rest} = props;
    const newProps = { ...rest, ref, className: cls(props, "AdaptiveComponent") };

    return render(componentDefinitions, layoutComponent, fallback, newProps);
});

export default AdaptiveComponent;
