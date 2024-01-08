// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {useState, useEffect} from "react";

import {
    useEventId, 
    useModel,
    useAfwProperty,    
    useOperational,
    useIsMounted,    
} from "../../../hooks";

import {AdaptiveLayoutMergeProviders} from "../../AdaptiveLayout";
import {layoutByObject} from "../../registry";

/**
 * Handles dataType=object
 */
export const _Object = (props) => {
    const [object, setObject] = useState();

    const property = useAfwProperty(props);
    const changed = useEventId({ object, eventId: "onChildChanged" });
    const model = useModel();
    const operational = useOperational(props);
    const isMounted = useIsMounted();

    const {value, valueMeta, onChanged, onValidate} = props;    

    /**
     * Embedded objects that do not have values yet, sometimes still need to be 
     * rendered, if the configured view options requests it.  We do this by creating
     * an object on the fly here.  Once it's contents change, we can attach it to the
     * property that is embedding it.
     */
    useEffect(() => {

        const newObject = async () => {                 
            const parent = property.getEmbeddingObject();

            const object = model.newObject({
                adaptorId: parent ? parent.getAdaptorId() : undefined,
                objectTypeId: property.getDataTypeParameter(),
                object: {}
            });
            await object.initialize();
           
            if (isMounted()) 
                setObject(object);
        };

        if (model && (value === undefined) && property)
            newObject();

    }, [model, value, property, isMounted]);

    /** 
     *  This routine captures object changes made to new, embedded objects.  When a new embedded
     *  object changes, then we go ahead and assign it to the property.
     */
    useEffect(() => {
        if (object && property && changed) {
            property.setValue(object);
        }
    }, [object, property, changed]);

    if (!object && !value)
        return null;

    /* lookup the appropriate layout for this object */
    const Layout = layoutByObject({ object: object || value, operational });

    return (
        <AdaptiveLayoutMergeProviders data={{ object: object || value }}>
            <Layout
                {...props} 
                {...valueMeta}
                aria-label={property?.getLabel() || property?.getName()}                
                onChanged={onChanged}
                onValidate={onValidate}
            />
        </AdaptiveLayoutMergeProviders>
    );
};

_Object.displayName = "Object";

export default _Object;
