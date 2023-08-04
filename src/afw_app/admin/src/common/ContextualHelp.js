// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useState} from "react";
import {matchPath, useLocation} from "react-router";

import Drawer from "@mui/material/Drawer";
import useMediaQuery from "@mui/material/useMediaQuery";

import {
    Button,
    Divider,
    Typography
} from "@afw/react";

import {useTheme} from "../hooks";

const DefaultHelp = () => {
    const theme = useTheme();

    return (
        <>
            <Typography color="primary" size="7" text="Help" />
            <div style={{ height: theme.spacing(2) }} />
        </>
    );
};

export const HelpText = ({ text }) => {
    return (
        <Typography
            size="3"
            style={{ whiteSpace: "pre-line" }}
            paragraph={true}
            text={text}
        />
    );
};


export const ContextualHelpButton = ({ showHelp }) => {

    return (
        <Button 
            variant="outlined"
            size="small"
            label="Help"
            color="primary"
            icon="help"
            onClick={() => showHelp(true)}
        />
    );
};


/**
 * Temporary contextual help content.
 */
export const ContextualHelp = (props) => {

    const [open, setOpen] = useState(props.open);
    const [drawerVariant, setDrawerVariant] = useState("temporary");
    const {pathname} = useLocation();
    const theme = useTheme();

    const isSmall = !useMediaQuery(theme.breakpoints.up("sm"));
    const isMedium = !useMediaQuery(theme.breakpoints.up("md"));
    const isLarge = !useMediaQuery(theme.breakpoints.up("lg"));
    
    const {onClose, routes} = props;

    useEffect(() => {
        
        const onKeyUp = (e) => {
            if (e.srcElement.nodeName === "BODY" && (e.key === "h" || e.key === "H")) {
                setOpen(open => !open);
            } else if (e.srcElement.nodeName === "BODY" && (e.key === "Escape"))
                setOpen(false);
        };

        window.addEventListener("keyup", onKeyUp);

        return () => { window.removeEventListener("keyup", onKeyUp); };

    }, []);    

    let Help = DefaultHelp;
    let Title = "Help";

    if (pathname && routes) {
        for (const route of routes) {
            const {path, title, Component} = route;                
            if (matchPath(pathname, { path })) {
                Help = Component;
                Title = title;

                break;
            }
        }    
    }

    return (
        <Drawer 
            open={open || props.open}
            anchor="right"
            onClose={() => {
                setOpen(false);
                onClose();
            }}
            variant={drawerVariant}
        >
            <div 
                style={{ 
                    minWidth: "375px", 
                    width: isSmall ? "100vw" : isMedium ? "75vw" : isLarge ? "50vw" : "25vw",                     
                    height: "100%" 
                }}
            >
                <div style={{ display: "flex", flexDirection: "column", height: "100%" }}>
                    <div style={{ display: "flex", alignItems: "center", padding: theme.spacing(2) }}>
                        <div style={{ flex: 1 }}>
                            <Typography color="primary" size="7" text={Title} />
                        </div>  
                        { !(isSmall || isMedium) &&
                            <Button 
                                type="icon"
                                label="Pin Window"
                                tooltip="Pin Window"
                                icon={drawerVariant === "permanent" ? "Unpin" : "Pin"}
                                onClick={() => {
                                    if (drawerVariant === "permanent")
                                        setDrawerVariant("temporary");                                
                                    else 
                                        setDrawerVariant("permanent");
                                }}
                            />                     
                        }
                        <Button 
                            type="icon"
                            icon="close"
                            label="Close Window"
                            tooltip="Close Window"
                            onClick={() => {
                                setOpen(false);
                                onClose();
                            }}
                        />                                           
                    </div>
                    <Divider style={{ paddingLeft: theme.spacing(2), paddingRight: theme.spacing(2) }} />                    
                    <div style={{ height: "100%", overflow: "auto", padding: theme.spacing(2) }}>
                        <Help {...props} />
                    </div>                    
                </div>                                
            </div>
        </Drawer>
    );
};

export default ContextualHelp;
