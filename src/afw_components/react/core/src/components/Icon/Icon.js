// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Icon
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

import {memo} from "react";
import {propTypes, defaultProps} from "./Icon.propTypes";
import fallback from "./Icon.fallback";

import {AdaptiveComponent} from "@afw/react";
import {ctx} from "@afw/react";

import Atom from "mdi-material-ui/Atom";
import Cloud from "mdi-material-ui/Cloud";
import Cogs from "mdi-material-ui/Cogs";
import Database from "mdi-material-ui/Database";
import FileMultiple from "mdi-material-ui/FileMultiple";
import Flash from "mdi-material-ui/Flash";
import Function from "mdi-material-ui/Function";
import FunctionVariant from "mdi-material-ui/FunctionVariant";
import Server from "mdi-material-ui/Server";
import Sitemap from "mdi-material-ui/Sitemap";
import FileTree from "mdi-material-ui/FileTree";
import Pin from "mdi-material-ui/Pin";
import PinOff from "mdi-material-ui/PinOff";
import Variable from "mdi-material-ui/Variable";


const importableIcons = {
    Atom,
    Cloud,
    Cogs,
    Database,
    FileMultiple,
    Flash,
    Function,
    Variable,
    FunctionVariant,
    Sitemap,
    FileTree,
    Pin,
    Unpin: PinOff,    
    Server,
};

export const canImportIcon = (iconName) => importableIcons[iconName] !== undefined;


/* helper function to import some extra icons out of mdi-material-ui that may not exist in other 3rd-party libraries */
export const importIcon = async (iconName) => {
    return importableIcons[iconName] ? importableIcons[iconName] : null;    
};


/**
 * Implementation Id : Icon
 * Category          : content
 * 
 * A component that displays an icon.
 * 
 * This component displays an icon, which references a common dictionary of
 * icon names, each of which may map to the appropriate icon library
 * depending on the framework that is being used.  The icon itself may be
 * clickable.
 * 
 */
export const Icon = (props) => {

    return (
        <AdaptiveComponent 
            {...props} 
            data-component-type={ctx(props, "Icon")}
            fallback={fallback}
            layoutComponent={{
                componentType: "Icon",
                parameters: props
            }}
        />
    );
};


Icon.propTypes = propTypes;
Icon.defaultProps = defaultProps;
Icon.displayName = "Icon";

export default memo(Icon);
