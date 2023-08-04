// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import Drawer from "@mui/material/Drawer";

import {
    Button,
    Typography
} from "@afw/react";

import {useClasses, useTheme} from "../hooks";

const styles = theme => ({
    paper: {
        width: "350px",
    },
    paperAnchorBottom: {
        left: "auto",
        right: theme.spacing(3)
    },
    keyTable: {
        display: "table",
        padding: theme.spacing(2)
    },
    keyRow: {
        display: "table-row",
        padding: theme.spacing(1)
    },
    keyCell: {
        display: "table-cell",
        padding: theme.spacing(1)
    },
    keyChar: {
        display: "inline-block",
        border: "solid thin grey",
        borderRadius: theme.shape.borderRadius,
        paddingLeft: theme.spacing(1),
        paddingRight: theme.spacing(1),
        marginRight: theme.spacing(3)
    },    
});

export const AppKeyboardShortcuts = () => {

    const theme = useTheme();
    const [open, setOpen] = useState(false);
    const classes = useClasses(styles);

    useEffect(() => {
        
        const onKeyUp = (e) => {

            const filtered = [ "INPUT", "SELECT", "TEXTAREA" ];            
            const nodeName = e.srcElement.nodeName;
            const key = e.key;

            if (filtered.includes(nodeName))
                return;

            if (nodeName && (key === "?")) {
                setOpen(open => !open);
            } else if (nodeName && (key === "Escape"))
                setOpen(false);
        };

        window.addEventListener("keyup", onKeyUp);

        return () => { window.removeEventListener("keyup", onKeyUp); };

    }, []);

    return (
        <Drawer 
            open={open}
            anchor="bottom"
            onClose={() => {
                setOpen(false);
            }}
            variant="persistent"
            classes={{
                paper: classes.paper,
                paperAnchorBottom: classes.paperAnchorBottom
            }}
        >
            <>
                <Button 
                    type="icon"
                    icon="close"
                    label="Close Help"
                    tooltip="Close Help"
                    style={{ position: "absolute", top: 8, right: 8 }}
                    onClick={() => setOpen()}
                />
                <div style={{ padding: theme.spacing(2) }}>
                    <Typography size="6" color="primary" text="Keyboard Shortcut Help" />
                </div>
                <div className={classes.keyTable}>
                    <div className={classes.keyRow}>
                        <div className={classes.keyCell}>
                            <div className={classes.keyChar}>?</div>
                        </div>
                        <div className={classes.keyCell}>
                            <span>Show this Keyboard Help</span>
                        </div>
                    </div>
                    <div className={classes.keyRow}>
                        <div className={classes.keyCell}>
                            <div className={classes.keyChar}>/</div>      
                        </div>
                        <div className={classes.keyCell}>                  
                            <span>Search</span>
                        </div>
                    </div>
                    <div className={classes.keyRow}>
                        <div className={classes.keyCell}>
                            <div className={classes.keyChar}>h</div>
                        </div>
                        <div className={classes.keyCell}>
                            <span>Contextual Help</span>
                        </div>
                    </div>
                    <div className={classes.keyRow}>
                        <div className={classes.keyCell}>
                            <div className={classes.keyChar}>e</div>
                        </div>
                        <div className={classes.keyCell}>
                            <span>Enter Edit Mode</span>
                        </div>
                    </div>
                    <div className={classes.keyRow}>
                        <div className={classes.keyCell}>
                            <div className={classes.keyChar}>n</div>
                        </div>
                        <div className={classes.keyCell}>
                            <span>Create New</span>
                        </div>
                    </div>
                    <div className={classes.keyRow}>
                        <div className={classes.keyCell}>
                            <div className={classes.keyChar}>ESC</div>
                        </div>
                        <div className={classes.keyCell}>
                            <span>Cancel/Close</span>
                        </div>
                    </div>
                </div>
            </>
        </Drawer>
    );
};


export default AppKeyboardShortcuts;
