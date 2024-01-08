// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for AnyURI
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import propTypes from "../dataTypes.propTypes";

import {TextField, Link} from "@afw/react";
import {useOperational, useRouteBasePath} from "@afw/react";

/**
 * dataType:  AnyURI
 * 
 * This is a built-in metadata handler for values who's dataType is anyURI.
 */
export const AnyURI = (props) => {
    const {editable} = useOperational(props);
    const routeBasePath = useRouteBasePath();

    const {id, autoFocus, "aria-label": ariaLabel, value, valueMeta = {}, onChanged, onValidate} = props;
    const {brief, description} = valueMeta;

    if (editable) { 
        return (
            <TextField
                id={id}
                // eslint-disable-next-line jsx-a11y/no-autofocus
                autoFocus={autoFocus}
                {...valueMeta}
                description={brief ? brief : description}
                type="text"
                onChanged={onChanged}
                onValidate={onValidate}
                value={value}                
            />
        );
    }

    else {        
        /* check to see if we need the link to be relative to the application route */
        if (value && value.indexOf("://") > 0)
            return <Link aria-label={ariaLabel} text={value} url={value} />;
        else if (value)
            return <Link aria-label={ariaLabel} text={value} url={(routeBasePath || "") + value} />;        
        else 
            return null;
    }
};

AnyURI.propTypes = propTypes;
AnyURI.displayName = "AnyURI";

export default memo(AnyURI);
