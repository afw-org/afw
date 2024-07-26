// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for ObjectId
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {
    TextField, 
    Typography, 
    Link,
    useOperational,
    useRouteBasePath,
    useAfwProperty,
} from "@afw/react";

/**
 * Handles dataType=objectId
 */
export const ObjectId = (props) => {    
    
    const routeBasePath = useRouteBasePath();
    const {editable} = useOperational(props);
    const property = useAfwProperty(props);

    const {id, autoFocus, value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description, dataTypeParameter : objectTypeId} = valueMeta;    

    /*! \fixme use an object selector to allow to browse for an object id? */
    if (editable) {
        return (
            <TextField
                id={id}
                // eslint-disable-next-line jsx-a11y/no-autofocus
                autoFocus={autoFocus}
                {...valueMeta}
                description={brief || description}
                type="text"
                value={value}
                onChanged={onChanged}
                onValidate={onValidate}
            />
        );
    }

    else {
        const embeddingObject = property?.getEmbeddingObject();
        const adapterId = embeddingObject?.getAdapterId();

        if (objectTypeId && adapterId && value) {            
            let url = routeBasePath + "/" + adapterId + "/" + objectTypeId + "/" + value;

            return (
                <Link text={value} url={url} />
            );
        } else
            return <Typography text={value || ""} />;
    }
};

ObjectId.displayName = "ObjectId";

export default ObjectId;
