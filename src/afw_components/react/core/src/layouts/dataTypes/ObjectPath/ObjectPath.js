// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for AnyURI
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {
    TextField, 
    Link,
    useOperational,
    useRouteBasePath,
    useAfwProperty,
} from "@afw/react";

/**
 * Handles dataType=objectPath
 */
export const ObjectPath = (props) => {
    
    const {editable} = useOperational(props);
    const routeBasePath = useRouteBasePath();
    const property = useAfwProperty(props);

    const {id, autoFocus, value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    if (editable) {
        /*! \fixme use an object selector to browse for path? */
        return (
            <TextField
                id={id}
                // eslint-disable-next-line jsx-a11y/no-autofocus
                autoFocus={autoFocus}
                {...valueMeta}
                description={brief ? brief : description}
                type="text"
                value={value}
                onChanged={onChanged}
                onValidate={onValidate}
            />
        );
    }

    else {
        if (!value || !property)
            return null;

        /*! 
            \fixme 

            This requires more complex path resolution and some common
            code to do this, as it may include scheme://, etc..  

            Quick fix here for *most* cases. 
         */
        let url = routeBasePath;

        /* includes adapterId */
        if (value.startsWith("/"))
            url = url + value;

        /* includes objectTypeId */
        else if (value.indexOf("/") > 0)
            url = url + "/" + property.getEmbeddingObject().getAdapterId() + "/" +  value;

        /* just objectId */
        else
            url = url + "/" + property.getEmbeddingObject().getAdapterId() + "/" + property.getEmbeddingObject().getObjectTypeId() + "/" + value;

        return (
            <Link text={value} url={url} />
        );
    }
};

ObjectPath.displayName = "ObjectPath";

export default ObjectPath;
