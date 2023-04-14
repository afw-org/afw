// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";
import {useLocation, useHistory} from "react-router";

import useMediaQuery from "@mui/material/useMediaQuery";
import Drawer from "@mui/material/Drawer";
import Paper from "@mui/material/Paper";

import {
    Button,
    Link,
    Nav,
    Skeleton,
    Tooltip,
    Typography,
} from "@afw/react";

import {useApplication, useAppCore, useTheme, useClasses} from "../hooks";

const drawerWidth = 250;

const styles = theme => ({
    drawerHeader: {
        display: "flex",
        justifyContent: "flex-end",
        padding: "0 8px",
        ...theme.mixins.denseToolbar,
        backgroundColor: theme.palette.mode === "dark" ? theme.palette.background.paper : theme.palette.primary.dark,
        alignItems: "center",
        borderRadius: 0,
    },
    drawerPaper: {
        width: drawerWidth,
        backgroundColor: theme.palette.background.default,
    },
    miniDrawerPaper: {
        width: 64,
        backgroundColor: theme.palette.background.default,
    },
    mobileDrawerPaper: {
        width: 64,
        height: 48,
        backgroundColor: theme.palette.background.default,
    },
});

const AppNavMobile = (props) => {

    const classes = useClasses(styles);
    const {className, onCollapse} = props;

    return (
        <>
            <div className={className} position="static">
                <div className={classes.drawerHeader}>
                    <Tooltip 
                        style={{ margin: "0 auto" }}
                        content="Expand Sidebar Nav"
                        target={
                            <div>
                                <Button
                                    style={{ color: "white" }}
                                    type="icon"
                                    label="Expand Sidebar Nav"
                                    icon="menu"                                    
                                    onClick={() => onCollapse(false)}
                                />
                            </div>
                        }
                    />
                </div>
            </div>            
        </>
    );
};

/**
 * AppNavMini
 * 
 * This is a version of the AppNav that is shown on medium sized devices.
 * Instead of full text links, icons are displayed on the side that the user 
 * can click and navigate through the app. 
 */
const AppNavMini = (props) => {

    const classes = useClasses(styles);
    const theme = useTheme();
    const location = useLocation();
    const history = useHistory();

    const {className, onCollapse} = props;

    const isSelected = (item) => {
        if (item.container)
            return location.pathname === item.url;
        else
            return location.pathname.startsWith(item.url);
    };

    const handleLink = (url) => {
        history.push(url);

        if (window.innerWidth < theme.breakpoints.values.md)
            onCollapse(true);

        return false;
    };

    const handleEventLink = (event, url) => {
        event.preventDefault();
        handleLink(url);

        return false;
    };

    return (
        <>
            <div className={className} position="static">
                <div className={classes.drawerHeader}>
                    <Tooltip 
                        style={{ margin: "0 auto" }}
                        content="Expand Menu"
                        target={
                            <div>
                                <Button
                                    style={{ color: "white" }}
                                    type="icon"
                                    label="Expand Nav"
                                    icon="menu"                                    
                                    onClick={() => onCollapse(false)}
                                />
                            </div>
                        }
                    />
                </div>
            </div>
            <div 
                id="admin-app-nav-mini"
                role="navigation"                 
            >
                <Nav
                    isMini={true}
                    onClick={(item, event) => handleEventLink(event, item.url)}
                    isSelected={(item) => isSelected(item)}
                    items={[
                        {
                            label: "Home",
                            icon: "home",
                            url: "/Home",
                        },
                        {
                            label: "Objects",
                            icon: "edit",
                            url : "/Objects",
                        },
                        {
                            label: "Admin",
                            icon: "equalizer",
                            url: "/Admin",
                        },
                        {
                            label: "Documentation",
                            icon: "bookmarks",
                            url: "/Documentation",
                        },
                        {
                            label: "Tools",
                            icon: "build",
                            url: "/Tools",
                        }
                    ]}
                />
            </div>
        </>
    );
};

const AppNav = (props) => {

    const [docsOpen, setDocsOpen] = useState(true);
    const [adminOpen, setAdminOpen] = useState(true);
    const [toolsOpen, setToolsOpen] = useState(true);
    const [docRefOpen, setDocRefOpen] = useState(true);    
    const [servicesOpen, setServicesOpen] = useState(true);

    const theme = useTheme();
    const classes = useClasses(styles);
    const {menuExpanded} = useApplication();
    const {application} = useAppCore();
    const location = useLocation();
    const history = useHistory();
    const {className, onCollapse} = props;

    const smDown = useMediaQuery(theme => theme.breakpoints.down("sm"));
    const smUp = useMediaQuery(theme => theme.breakpoints.up("sm"));
    const mdUp = useMediaQuery(theme => theme.breakpoints.up("md"));

    const handleLink = (url) => {        
        history.push(url);

        if (window.innerWidth < theme.breakpoints.values.md)
            props.onCollapse(true);
        
        return false;
    };

    const handleEventLink = (event, url) => {   
              
        event.preventDefault();
        handleLink(url);
        event.stopPropagation();

        return false;        
    };

    useEffect(() => {
        if (location) {
            const pathname = location.pathname;
            let docsOpen = false, adminOpen = false, toolsOpen = false, servicesOpen = false, docRefOpen = false;

            if (pathname.startsWith("/Admin/Services"))
                adminOpen = servicesOpen = true;
            else if (pathname.startsWith("/Admin/Adaptors") || pathname.startsWith("/Admin/Logs") || pathname.startsWith("/Admin/AuthHandlers"))
                adminOpen = servicesOpen = true;
            else if (pathname.startsWith("/Admin"))
                adminOpen = true;
            else if (pathname.startsWith("/Documentation/Reference"))
                docRefOpen = docsOpen = true;
            else if (pathname.startsWith("/Documentation/Guides"))
                docsOpen = true;
            else if (pathname.startsWith("/Documentation"))
                docsOpen = true;
            else if (pathname.startsWith("/Tools"))
                toolsOpen = true;

            setDocsOpen(docsOpen);
            setAdminOpen(adminOpen);
            setToolsOpen(toolsOpen);
            setServicesOpen(servicesOpen);
            setDocRefOpen(docRefOpen);
        }
    }, [location]);

    const isSelected = (item) => {
        if (item.container)
            return location.pathname === item.url;
        else
            return location.pathname.startsWith(item.url);
    };

    // eslint-disable-next-line no-undef
    const version = process.env.REACT_APP_VERSION;

    const title = application ? 
        (application.getPropertyValue("title") ? application.getPropertyValue("title") : application.getPropertyValue("applicationId")) :
        "Loading...";

    const drawer = (
        <>
            <div className={className} position="static">
                <Paper className={classes.drawerHeader}>
                    <div style={{ paddingLeft: theme.spacing(1), flex: 1, display: "flex", alignItems: "baseline", justifyContent: "space-between" }}>
                        <div>
                            {
                                application ? 
                                    <Typography style={{ color: "white" }} size="5" component="h2" text={title} /> : 
                                    <Skeleton width={100} />
                            }                            
                        </div>
                        <div>
                            <Link style={{ fontSize: "smaller", color: "white" }} url="/Versions" text={version} />
                        </div>
                    </div>                    
                </Paper>
            </div>
            <div 
                id="admin-app-nav"                
                role="navigation" 
                aria-label="App Navigation"
            >
                <Nav
                    onClick={(item, event) => handleEventLink(event, item.url)}
                    isSelected={(item) => isSelected(item)}
                    items={[
                        {
                            label: "Home",
                            icon: "home",
                            url: "/Home",
                        },
                        {
                            label: "Objects",
                            icon: "edit",
                            url : "/Objects",
                        },
                        {
                            label: "Admin",
                            container: true,
                            icon: "equalizer",
                            onGroupClick: () => setAdminOpen(!adminOpen),
                            url: "/Admin",
                            isExpanded: adminOpen,
                            items: [
                                {
                                    label: "Server",
                                    url: "/Admin/Server",
                                },
                                {
                                    label: "Application",
                                    url: "/Admin/Application",
                                },
                                {
                                    label: "Extensions",
                                    url: "/Admin/Extensions",
                                },
                                {
                                    label: "Services",
                                    url: "/Admin/Services",
                                    container: true,
                                    onGroupClick: () => setServicesOpen(!servicesOpen),
                                    isExpanded: servicesOpen,
                                    items: [
                                        {
                                            label: "Adaptors",
                                            url: "/Admin/Adaptors",
                                        },
                                        {
                                            label: "Logs",
                                            url: "/Admin/Logs",
                                        },
                                        {
                                            label: "Auth Handlers",
                                            url: "/Admin/AuthHandlers",
                                        },
                                    ]
                                },
                                {
                                    label: "Schema",
                                    url: "/Admin/Schema"
                                },
                                {
                                    label: "Models",
                                    url: "/Admin/Models",
                                },
                                {
                                    label: "Provisioning",
                                    url: "/Admin/Provisioning",
                                },
                                {
                                    label: "Authorization",
                                    url: "/Admin/Authorization",
                                },
                                {
                                    label: "Request Handlers",
                                    url: "/Admin/RequestHandlers",
                                },
                            ]
                        },
                        {
                            label: "Documentation",
                            icon: "bookmarks",
                            container: true,
                            onGroupClick: () => setDocsOpen(!docsOpen),
                            url: "/Documentation",
                            isExpanded: docsOpen,
                            items: [                                
                                {
                                    label: "Reference",
                                    onGroupClick: () => {
                                        setDocRefOpen(!docRefOpen);
                                    },
                                    url: "/Documentation/Reference",
                                    container: true,
                                    isExpanded: docRefOpen,
                                    items: [
                                        {
                                            label: "Data Types",
                                            url: "/Documentation/Reference/DataTypes",
                                        },
                                        {
                                            label: "Functions",
                                            url: "/Documentation/Reference/Functions",
                                        },
                                        {
                                            label: "Schema",
                                            url: "/Documentation/Reference/Schema",
                                        },
                                        {
                                            label: "Components",
                                            url: "/Documentation/Reference/Components",
                                        },
                                        {
                                            label: "Language",
                                            url: "/Documentation/Reference/Language",
                                        },
                                        {
                                            label: "Glossary",
                                            url: "/Documentation/Reference/Glossary",
                                        }
                                    ]
                                },
                                {
                                    label: "Full Docs",
                                    url: "/doc",
                                    external: true,
                                }
                            ]
                        },
                        {
                            label: "Tools",
                            icon: "build",
                            container: true,
                            onGroupClick: () => setToolsOpen(!toolsOpen),
                            url: "/Tools",
                            isExpanded: toolsOpen,
                            items: [
                                {
                                    label: "Layouts",
                                    url: "/Tools/Layouts",
                                },
                                {
                                    label: "Fiddle",
                                    url: "/Tools/Fiddle",
                                },
                                {
                                    label: "Requests",
                                    url: "/Tools/Requests",
                                },
                            ]
                        }
                    ]}
                />
            </div>
        </>
    );

    return (
        <>
            { !mdUp && 
                <Drawer
                    open={menuExpanded}
                    onClose={() => onCollapse(true)}
                    variant="temporary"
                    classes={{
                        paper: classes.drawerPaper,
                    }}
                    anchor="left"
                >
                    {drawer}
                </Drawer>
            }
            { !smDown && !menuExpanded && (                
                <Drawer
                    open={!menuExpanded}
                    variant="persistent"
                    classes={{ paper: classes.miniDrawerPaper }}
                    anchor="left"
                >
                    <AppNavMini {...props} />
                </Drawer>
            )}
            { !smDown && menuExpanded && (                
                <Drawer
                    open={menuExpanded}
                    variant="persistent"
                    classes={{ paper: classes.drawerPaper }}
                    anchor="left"
                    onClose={onCollapse}
                >
                    {drawer}
                </Drawer>                
            )}            
            {
                !smUp && !menuExpanded &&
                <Drawer
                    open={!menuExpanded}
                    variant="persistent"
                    classes={{ paper: classes.mobileDrawerPaper }}
                    anchor="left"                        
                >
                    <AppNavMobile {...props} />
                </Drawer>
            }
            { 
                !smUp && menuExpanded &&
                <Drawer
                    open={menuExpanded}
                    variant="temporary"
                    classes={{ paper: classes.drawerPaper }}
                    anchor="left"
                    onClose={onCollapse}
                >
                    {drawer}
                </Drawer>
            }    
        </>
    );
};

export default AppNav;
