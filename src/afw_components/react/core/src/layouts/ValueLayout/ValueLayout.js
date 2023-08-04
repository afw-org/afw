// See the 'COPYING' file in the project root for licensing information.
import {useCallback, useState, useMemo} from "react";

import {
    useOperational, 
    useAfwValue,
} from "@afw/react";

import {AfwValue} from "@afw/client";
import {layoutByDataType} from "../registry";

/**
 * ValueLayout
 *
 * This component renders an Adaptive Value, using its metadata.  It expects, first, there will be 
 * a value derived from its React context.  If there is no context "value", we create one internally 
 * and memoize it.
 * 
 * The React context may contain an AfwValue instance.  In this case, it will use that 
 * for the value.  If the React context contains a primitive JSON value, it creates an AfwValue out of 
 * it anyway.  The value may also be an AfwProperty or AfwListEntry, as they extend AfwValue.
 * 
 * This all gives us some flexibility to use <ValueLayout value="myStr" />, which is handy 
 * for tests, or if we just don't care to use the Afw* data model interfaces and prefer to use simple 
 * JSON representation.
 * 
 * 
 * \fixme this does not have support in for AfwObject and AfwList values yet.
 */
export const ValueLayout = (props) => {

    const [, setNewValue] = useState();
    const {editable, add} = useOperational(props);
    const contextValue = useAfwValue(props);
    const {valueMeta, onChanged, onValidate} = props;

    /**
     * This routine grabs the value and meta by looking inside React
     * context.  If it's not there, then it'll be created and stored
     * in React (memoized) state here.
     */
    const [value, meta] = useMemo(() => {
        let value, meta;

        if (contextValue) {
            /* React context contains a value, so use it */
            if (contextValue instanceof AfwValue) {                             
                /* The metadata can be grabbed by calling getMeta() */   
                value = contextValue;
                meta = contextValue.getMeta();            
            } else {
                /* 
                    The context value is not an AfwProperty/AfwValue, so create 
                    an AfwValue here, relying on on the valueMeta from props 
                 */                
                const v = new AfwValue({ propertyType: valueMeta, value: contextValue });
                value = v;
                meta = v.getMeta();
            }
        } else {   
            /* 
                There is nothing in React context, so create an AfwValue instance and 
                rely on value and valueMeta from props.
             */
            const v = new AfwValue({ propertyType: valueMeta, value: props.value });
            value = v;
            meta = v.getMeta();
        }

        return [value, meta];
    }, [valueMeta, contextValue, props.value]);

    /**
     * _onChanged determines of the value can be changed, and does two jobs:  
     * 
     *   (1) Set the underlying AfwValue/AfwProperty value.
     *   (2) Call the props onChanged, if declared.
     */
    const _onChanged = useCallback(newValue => {
        const {allowWrite, required} = meta;       

        // write back to the AfwValue instance only if editable and allowed
        if (editable !== false && (allowWrite !== false || (required && add))) {            
            value.setValue(newValue);
            if (onChanged)
                onChanged(newValue);            
        }

        // force an update
        setNewValue([ newValue ]);
    }, [value, editable, add, meta, onChanged]);

    /**
     * _onValidate calls the onValidate provided by props, or 
     * simply returns true, if none is provided.
     */
    const _onValidate = useCallback((newValue) => {
        if (onValidate)
            return onValidate(newValue);
        else return true;
    }, [onValidate]);

    if (!value)
        return null;
 
    
    /* Use the value's dataType to determine the appropriate metadata layout */
    const Layout = layoutByDataType[ value.getDataType() ];
    if (!Layout)
        return null;

    return (
        <Layout 
            {...props}     
            valueMeta={valueMeta ? valueMeta : meta}       
            value={value.getValue()}
            data-component-type="ValueLayout"
            onChanged={_onChanged}
            onValidate={_onValidate}
        />
    );
};

ValueLayout.displayName = "ValueLayout";

export default ValueLayout;
