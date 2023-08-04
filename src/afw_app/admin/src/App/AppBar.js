// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";
import {useLocation} from "react-router";

import MuiAppBar from "@mui/material/AppBar";
import Button from "@mui/material/Button";
import Toolbar from "@mui/material/Toolbar";
import Settings from "@mui/icons-material/Settings";
import Tooltip from "@mui/material/Tooltip";

import {
    Typography
} from "@afw/react";

import AppStatus from "./AppStatus";
import AppSearch from "./AppSearch";
import AppSettings from "./AppSettings";


import {useApplication, useClasses} from "../hooks";

const styles = theme => ({
    appBarCollapsed: {
        flexGrow: 1,
        paddingLeft: "64px",
        width: "100%",
        backgroundColor: theme.palette.mode === "dark" ? theme.palette.background.paper : theme.palette.primary.dark,
    },
    appBarExpanded: {
        flexGrow: 1,
        paddingLeft: 250,
        backgroundColor: theme.palette.mode === "dark" ? theme.palette.background.paper : theme.palette.primary.dark,
        width: "100%",
    },
    appBarTitle: {
        minWidth: 0,
        flex: 1,        
    },
    title: {
        color: "white",
        whiteSpace: "nowrap",
        overflow: "hidden",
        textOverflow: "ellipsis",
    }
});


const AppBar = () => {
    const classes = useClasses(styles);
    const location = useLocation();
    const {menuExpanded} = useApplication();

    /* eslint no-unused-vars: [2, {"args": "after-used", "varsIgnorePattern": "ignore"}] */
    let [ignore, pathname, ...rest] = location.pathname.split("/");
    if (pathname === "/")
        pathname = "/Home";

    const title = decodeURIComponent((rest.length > 0) ? rest[rest.length-1] : pathname);
    const className = menuExpanded ? classes.appBarExpanded : classes.appBarCollapsed;

    const [settingsOpen, setSettingsOpen] = useState(false);

    return (
        <>
            <MuiAppBar id="admin-appbar" className={className} color="primary" position="fixed">
                <Toolbar variant="dense">
                    <div className={classes.appBarTitle}>
                        <Typography component="h1" size="6" className={classes.title} text={title} />
                    </div>
                    <AppSearch />
                    <AppStatus />
                    <Tooltip title="Show Settings">
                        <Button
                            style={{ color: "white" }}
                            onClick={() => setSettingsOpen(true)}
                            aria-label="Settings"
                        >
                            <Settings />
                        </Button>
                    </Tooltip>
                </Toolbar>
            </MuiAppBar>
            <AppSettings
                open={settingsOpen}
                onDismiss={() => setSettingsOpen(false)}
            />
        </>
    );
};

export default AppBar;
