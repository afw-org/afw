// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import {BrowserRouter} from "react-router-dom";

import {createTheme, responsiveFontSizes} from "@mui/material/styles";
import {ThemeProvider} from "@mui/material/styles";
import CssBaseline from "@mui/material/CssBaseline";
import {StyledEngineProvider} from "@mui/material/styles";

import {AppCoreProvider} from "./AppCoreProvider";

import AppNav from "./AppNav";
import AppBar from "./AppBar";
import AppError from "./AppError";
import AppRoutes from "./AppRoutes";
import AppKeyboardShortcuts from "./AppKeyboardShortcuts";
import {withMediaQuery, lightTheme, darkTheme} from "./AppTheme";

import {AfwObject, AfwProperty, AfwArray} from "@afw/client";
import {AfwClient} from "@afw/client";

import {copyToClipboard} from "../utils";

import {Snackbar} from "@afw/react";
import {AppContext, NotificationContext} from "../context";

import {AdaptiveProvider, combineComponentRegistries} from "@afw/react";
import muiComponentRegistry from "@afw/react-material-ui";
import monacoComponentRegistry, {MonacoProvider} from "@afw/react-monaco";


/**
 * App
 * 
 * This is the main component for the entire Web Application.
 * 
 * It's responsible for rendering the main sections of the app,
 * watching for window resizing, catching errors as a last resort,
 * setting initial settings from Local Storage preferences, and 
 * centrally managing global notifications.
 * 
 * It also sets up the Providers for many global things that are passed
 * down via React Context, including which components to use, 
 * client objects, and clipboard functionality.
 */
class App extends React.Component {

    constructor(props) {
        super(props);

        const client = new AfwClient({
            url: "/afw"
        });
        client.addOn("onResponse", this.onHandleResponse);
        client.addOn("onResponseError", this.onHandleResponseError);

        /* default built-in theme */
        const theme = responsiveFontSizes(createTheme(props.prefersDarkMode ? { ...darkTheme } : { ...lightTheme }));

        const isMobile = (window.innerWidth <= theme.breakpoints.values.sm);

        /* construct a default state, that can be changed later */
        this.state = {
            client,
            theme,
            prefersDarkMode: props.prefersDarkMode,
            menuExpanded: window.innerWidth >= theme.breakpoints.values.md ? true : false,
            isMobile,
            marginHeight: isMobile ? "180px" : "80px",
            snackbarOpen: false,
            notifications: [],
            componentRegistry: combineComponentRegistries("admin", [ muiComponentRegistry, monacoComponentRegistry ]),
            clipboard: [],
        };
    }

    /**
     * Once our main "App" is mounted, then make request(s) to the
     *   server to obtain our initial application settings.
     */
    componentDidMount() {
        window.addEventListener("resize", this.onResize);                

        this.processLocalStorage();

        /* initialize component libraries */        
        muiComponentRegistry.initialize();
        monacoComponentRegistry.initialize();

        muiComponentRegistry.setTheme(this.state.theme);
        monacoComponentRegistry.setTheme(this.state.theme);

        this._isMounted = true;
    }

    componentWillUnmount() {
        window.removeEventListener("resize", this.onResize);       

        this._isMounted = false;        
    }

    componentDidUpdate(prevProps) {
        if (prevProps.prefersDarkMode !== this.props.prefersDarkMode) {
            const theme = responsiveFontSizes(createTheme(this.props.prefersDarkMode ? {...darkTheme} : {...lightTheme}));

            this.setState({ theme });

            muiComponentRegistry.setTheme(this.state.theme);
            monacoComponentRegistry.setTheme(this.state.theme);
        }
    }

    /**
     * Catch any React errors that have not been handled by child components.
     */
    componentDidCatch(catchError, catchInfo) {
        if (catchInfo && catchInfo.componentStack)
            /*eslint-disable no-console */
            console.log(catchInfo.componentStack);

        this.setState({ catchError, catchInfo });
    }

    /**
     * onHandleAppRequestError()
     *
     * This routine handles network request errors and checks for whether we
     * need to re-authenticate by telling the browser to refresh.
     *
     * \fixme localStorage calls can fail in "incognito mode".     
     */
    onHandleAppRequestError = (error) => {        
        if (error && error.response && error.response.status === 401) {
            /* force a reload of the current URL to re-invoke authentication */
            if (localStorage.getItem("reloadingApp")) {
                /* we've already loaded the app and still have a 401, so quit with error */
                /* \fixme we should also display an error */
                console.log("Got consecutive 401 errors!");
            } else {
                /* set flag to let us know we're reloading, so next time we can prevent a loop */
                localStorage.setItem("reloadingApp", true);
                /* \fixme also, need to prompt before reloading, in case they have unsaved data. */
                window.location.reload();
            }
        } else if (!(error && error.response && error.response.status)) {
            /* check server via xhr to see if we are in a 401 situation, as we couldn't tell from the error */
            if (localStorage.getItem("checkingServer")) {
                /* already checked, so remove the flag */
                localStorage.removeItem("checkingServer");
            } else {
                localStorage.setItem("checkingServer", true);
                /* \fixme test the server by fetching the application app extension? */
            }
        }
    };
   
    /* interceptor for handling all responses directly after they've been sent back from the server */
    onHandleResponse = (response) => {
        
        /* successful response from server, so clear the reloading flag */
        if (localStorage.getItem("reloadingApp"))
            localStorage.removeItem("reloadingApp");

        /* simply return the response to be handled */
        return response;
    };

    /* interceptor for handling all response errors, directly after they've been sent back from the server */
    onHandleResponseError = (error) => {    
        this.onHandleAppRequestError(error);
    };

    /* Use localStorage to setup preferences for the web app */
    processLocalStorage = () => {
        try {
            const afw = localStorage.getItem("afw");

            if (afw) {
                const parsed = JSON.parse(afw);
                if (parsed && parsed["settings"]) {
                    const {flags, timeout, theme} = parsed["settings"];
                    if (timeout) {
                        this.state.client.setTimeout(timeout);
                    }

                    if (flags) {
                        this.state.client.setActionFlags(flags);
                    }
                    
                    if (theme) {
                        /* \fixme */
                    }
                                            
                    this.setState({
                        componentRegistry: combineComponentRegistries("admin",
                            [ muiComponentRegistry, monacoComponentRegistry ]),
                    });                        
                    
                }
            }
        } catch (error) {
            /* could be running in incognito mode, or a browser that doesn't support localStorage */
            console.log(error);
        }
    };

    /**
     * notification()
     *
     * This routine can be called by any component, and is used to alert the user that a notification is
     * available.  This may be an error, or it may be an informative message.  The "type" is used to 
     * differentiate the two.  Additionally, if a "Snackbar" alert should be displayed, set the "snackbar"
     * and optional "duration" fields to send a message at the bottom of the screen.
     */
    notification = ({ message, type, snackbar = true, duration }) => {
        let messageStr = message;
        let notifications = this.state.notifications;

        if (type === "error") {
            /* if the user passed in an Error object, look for a response, which would be passed back by the server */
            if (message && (message instanceof Error)) {
                if (message.response && message.response.data) {
                    if (message.response.data.message) {
                        messageStr = message.response.data.message;
                        notifications = [...notifications, {...message.response.data, timestamp: new Date()}];
                    } else {
                        messageStr = message.message;
                        notifications = [...notifications, {...message, timestamp: new Date()}];
                    }
                } else {
                    notifications = [...notifications, {
                        timestamp: new Date(), ...message
                    }];
                }
            } else if (typeof(message) === "object") {
                /* \fixme there should be an adaptive object of type error message here */
                messageStr = message["message"];
                notifications = [...notifications, {message: messageStr, error: message, timestamp: new Date()}];
            } else {
                notifications = [...notifications, {message, timestamp: new Date()}];
            }
        }

        this.setState({
            snackbarOpen: snackbar,
            snackbarMessage: (snackbar ? String(messageStr) : undefined),
            snackbarType: type,
            snackbarDuration: duration,
            notifications,
        });
    };

    /**
     * Capture resize event and decide whether to dock or expand the Navbar.     
     */
    onResize = () => {
        const {menuExpanded, theme} = this.state;

        if (menuExpanded && window.innerWidth < theme.breakpoints.values.md)
            this.setState({ menuExpanded: false });
        else if (!menuExpanded && window.innerWidth >= theme.breakpoints.values.md)
            this.setState({ menuExpanded: true });

        const isMobile = window.innerWidth <= theme.breakpoints.values.sm;
        this.setState({ 
            isMobile,
            marginHeight: isMobile ? "180px" : "80px",                      
        });
    };

    /**
     * Capture application settings changes.
     */
    onChanged = (options) => {
        if (options) {
            const {theme} = options;            
                            
            this.setState({
                componentRegistry: combineComponentRegistries("admin",
                    [ muiComponentRegistry, monacoComponentRegistry ])
            });                            

            /*! \fixme figure out where to store raw and when to compute actual theme */
            if (theme) {
                this.setState({ theme: responsiveFontSizes(createTheme({...theme})) });

                muiComponentRegistry.setTheme(theme);
                monacoComponentRegistry.setTheme(theme);
            }
        }
    };

    /**
     * When the user copies something anywhere in the App, this will copy the text to the
     * clipboard and saves it locally so it can be pasted somewhere else in the app.
     */
    onCopy = (content) => {
        let string;

        /* if the content is an object, we need to stringify it for the actual clipboard */
        if (typeof(content) === "object") {
            if (content instanceof AfwObject || content instanceof AfwProperty || content instanceof AfwArray) {
                string = JSON.stringify(content.toJSON());
            } else
                string = JSON.stringify(content);
        } else {
            /* primitive values remain primitives */
            string = content;
        }

        /* common routine to copy content to user's clipboard, which relies on a series of heuristics */        
        copyToClipboard(string);        

        this.state.clipboard.push( content );
    };

    /**
     * Cancels an array of activeRequests.
     */
    onCancelActiveRequests = (/*requests*/) => {
       
    };

    /**
     * Notifications can be cleared here by the app.
     */
    onClearNotifications = (selectedNotifications) => {
        if (selectedNotifications) {
            const notifications = this.state.notifications.filter(e => {
                for (let e2 of selectedNotifications) {
                    if (e2.timestamp === e.timestamp)
                        return false;
                }
                return true;
            });

            this.setState({ notifications });
        } else
            this.setState({ notifications: [] });
    };

    /**
     * Some operations require the whole App needs to reload (such as loading a new Extension).  Or, if an 
     * unrecoverable error occurs.
     */
    onReloadApp = () => {
        window.location.reload();
    };

    /**
     * The App render() routine sets up the Router rules for navigating the
     *   admin app, and declares React Context Providers for children components.
     */
    render() {
        const {
            client, theme, afw_components, menuExpanded, flags,
            clipboard, componentRegistry,notifications, catchError, error, catchInfo, 
            snackbarOpen, snackbarDuration, snackbarMessage, snackbarType, prefersDarkMode, 
            isMobile, marginHeight
        } = this.state;

        /* define the applicationProps that all consumers will get */
        const applicationProps = {
            client, theme, afw_components, menuExpanded, flags,
            clipboard, prefersDarkMode, isMobile, marginHeight,
            onHandleAppRequestError:    this.onHandleAppRequestError,
            notification:               this.notification,
            onChanged:                  this.onChanged,
            onCopy:                     this.onCopy,            
        };

        /* define the notificationProps that all consumers will get */
        const notificationProps = {
            notifications, 
            onClearNotifications: this.onClearNotifications,
            onCancelActiveRequests: this.onCancelActiveRequests,
        };

        /* let monaco inherit our theme */
        const monacoTheme = (theme.palette.mode === "dark" ? (this.props.prefersHighContrast ? "hc-black" : "custom-dark") : "vs-light");

        /* determine which component will be returned, based on error states */
        let component;
        if (error) {
            /* There was an error loading a critical application setting (server unavailable?) */
            component = (
                <AppError
                    title="Error Loading Application Data"
                    message="There was an error loading the application.  You may click Refresh or Browser Refresh to retry the request.
                    If the problem persists, make sure afwfcgi service is running, or check the server for errors."
                    error={error}
                    reload={this.onReloadApp}
                />
            );
        }

        else if (catchError) {
            /* A component crashed and we caught the error, report the error and component stacks for troubleshooting */
            component = (
                <AppError
                    title="Application Crashed!"
                    message="The application crashed due to a server or client application bug."
                    error={catchError}
                    info={catchInfo}
                    reload={this.onReloadApp}
                />
            );
        }

        else {
            /* No errors, so process the normal App routes */
            component = (
                <BrowserRouter                 
                    basename={process.env.PUBLIC_URL}
                    getUserConfirmation={(payload, callback) => {          
                        const allowTransition = window.confirm(payload);
                        callback(allowTransition);                        
                    }}
                >
                    <div id="admin" style={{ width: "100%", zIndex: 1, overflow: "hidden", position: "relative" }}>
                        <AppBar
                            menuExpanded={menuExpanded}
                            onToggleMenu={() => this.setState({ menuExpanded: !menuExpanded })}
                        />
                        <AppNav
                            menuExpanded={menuExpanded}
                            onCollapse={(collapsed) => this.setState({ menuExpanded: !collapsed })}
                        />
                        <div 
                            id="admin-main"                            
                            role="main" 
                            style={{
                                marginLeft: (menuExpanded === true && !isMobile) ? "250px" : (isMobile ? "0px" : "64px"), 
                                paddingTop: (isMobile ? "64px" : "80px"), 
                                paddingBottom: (isMobile) ? "64px": "0px",
                            }}
                        >
                            <AppRoutes {...this.props} />
                        </div>
                        <Snackbar
                            id="admin-notification"                            
                            open={snackbarOpen}
                            message={snackbarMessage}
                            type={snackbarType}
                            showClose={!snackbarDuration}
                            duration={snackbarDuration}
                            onClose={() => this.setState({ snackbarOpen: false })}
                        />
                    </div>
                </BrowserRouter>
            );
        }

        return (
            <ThemeProvider theme={theme}>
                <CssBaseline />
                <AppContext.Provider value={{ ...applicationProps }}>
                    <NotificationContext.Provider value={notificationProps}>
                        <AdaptiveProvider 
                            client={client}                             
                            componentRegistry={componentRegistry} 
                            onCopy={this.onCopy} 
                            clipboard={clipboard}
                        >
                            <AppCoreProvider>
                                <MonacoProvider theme={monacoTheme}>
                                    <StyledEngineProvider injectFirst>
                                        { component }
                                    </StyledEngineProvider>
                                    <AppKeyboardShortcuts />
                                </MonacoProvider>
                            </AppCoreProvider>
                        </AdaptiveProvider>
                    </NotificationContext.Provider>
                </AppContext.Provider>
            </ThemeProvider>
        );
    }
}

export default withMediaQuery(App);
