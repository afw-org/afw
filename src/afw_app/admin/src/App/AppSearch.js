// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect, useRef, useMemo} from "react";
import {useHistory} from "react-router";

import Button from "@mui/material/Button";
import TextField from "@mui/material/TextField";
import Popper from "@mui/material/Popper";
import InputAdornment from "@mui/material/InputAdornment";
import ClickAwayListener from "@mui/material/ClickAwayListener";
import Search from "@mui/icons-material/Search";

import {useTheme} from "../hooks";

import {
    Dialog,
    Divider,
    Hidden,
    Typography
} from "@afw/react";
import {debounce} from "@afw/client";

import {useAppCore, useClasses} from "../hooks";

const styles = theme => ({
    helpTextField: {
        paddingLeft: theme.spacing(1),
        color: theme.palette.primary.light,
        borderStyle: "solid",
        borderWidth: "1px",
        backgroundColor: theme.palette.mode === "dark" ? 
            theme.palette.background.default : 
            theme.palette.primary.main,    
        borderColor: theme.palette.mode === "dark" ? 
            theme.palette.background.default : 
            theme.palette.primary.main,
    },
    searchResult: {
        paddingLeft: theme.spacing(5), 
        cursor: "pointer",
        "&:hover": {
            backgroundColor: theme.palette.mode === "dark" ? 
                theme.palette.background.default : "#efefef",
        }
    }
});

let generalSearchTopics = [
    {
        category: "General",
        text: "Objects",
        url: "/Objects",    
        description: "View, create or update Objects with the Object Editor."    
    },
    {
        category: "General",
        text: "Server",
        url: "/Admin/Server",
        description: "View properties of this Adaptive Server instance."    
    },
    {
        category: "General",
        text: "Application",
        url: "/Admin/Application",
        description: "Define the Application properties."    
    },
    {
        category: "General",
        text: "Extensions",
        url: "/Admin/Extensions",
        description: "View or Load Extensions."    
    },
    {
        category: "General",
        text: "Services",
        url: "/Admin/Services",
        description: "Manage, Start or Stop Services."    
    },
    {
        category: "General",
        text: "Adapters",
        url: "/Admin/Adapters",
        description: "View or Edit Adapter Services."    
    },
    {
        category: "General",
        text: "Logs",
        url: "/Admin/Logs",
        description: "View or Edit Log Services."    
    },
    {
        category: "General",
        text: "Models",
        url: "/Admin/Models",
        description: "View or Edit Model definitions."    
    },
    {
        category: "General",
        text: "Authorization",
        url: "/Admin/Authorization",
        description: "View or Edit Authorization policies."    
    },
    {
        category: "General",
        text: "Data Types",
        url: "/Documentation/Reference/DataTypes",
        description: "View Adaptive Data Type reference manual."    
    },
    {
        category: "General",
        text: "Functions",
        url: "/Documentation/Reference/Functions",
        description: "View Adaptive Function reference manual."    
    },
    {
        category: "General",
        text: "Fiddle",
        url: "/Tools/Fiddle",
        description: "Use the Fiddle Developer tool."    
    },
];


const AppSearchResults = (props) => {

    const theme = useTheme();
    const classes = useClasses(styles);

    const handleLinkSelect = (result) => {
        props.handleLinkSelect(result);
    };

    const {
        filteredGeneralTopics, 
        filteredServiceTopics, 
        filteredDataTypeTopics,
        filteredFunctionTopics,
        filteredExtensionTopics,
        filteredObjectTypeTopics,        
    } = props;


    return (
        <>               
            {
                (filteredGeneralTopics.length > 0) && 
                <div>
                    <Typography text="General" />
                    <Divider />
                    {
                        filteredGeneralTopics.map((result, index) => 
                            <div 
                                className={classes.searchResult} 
                                key={index} 
                                role="button" 
                                onClick={() => handleLinkSelect(result)}
                                onKeyDown={() => handleLinkSelect(result)}
                                tabIndex={-1}
                            >
                                <div style={{ height: theme.spacing(1) }} />
                                <Typography text={result.text} size="6" />
                                <Typography text={result.description} size="2" color="textSecondary" />
                                <div style={{ height: theme.spacing(1) }} />
                                {
                                    (index < filteredGeneralTopics.length-1) &&
                                        <Divider />
                                }
                            </div>
                        )
                    }                            
                </div>
            }     
            {
                (filteredServiceTopics.length > 0) && 
            <div>
                <Typography text="Services" />
                <Divider />
                {
                    filteredServiceTopics.map((result, index) => 
                        <div 
                            className={classes.searchResult} 
                            key={index} 
                            role="button" 
                            onClick={() => handleLinkSelect(result)}
                            onKeyDown={() => handleLinkSelect(result)}
                            tabIndex={-1}
                        >
                            <div style={{ height: theme.spacing(1) }} />
                            <Typography text={result.text} size="6" />
                            <Typography text={result.description} size="2" color="textSecondary" />
                            <div style={{ height: theme.spacing(1) }} />
                            {
                                (index < filteredServiceTopics.length-1) &&
                                    <Divider />
                            }
                        </div>
                    )
                }                            
            </div>
            }      
            {
                (filteredExtensionTopics.length > 0) && 
            <>
                <Typography text="Extensions" />
                <Divider />
                {
                    filteredExtensionTopics.map((result, index) => 
                        <div 
                            className={classes.searchResult} 
                            key={index} 
                            role="button" 
                            onClick={() => handleLinkSelect(result)}
                            onKeyDown={() => handleLinkSelect(result)}
                            tabIndex={-1}
                        >
                            <div style={{ height: theme.spacing(1) }} />
                            <Typography text={result.text} size="6" />
                            <Typography text={result.description} size="2" color="textSecondary" />
                            <div style={{ height: theme.spacing(1) }} />
                            {
                                (index < filteredExtensionTopics.length-1) &&
                                    <Divider />
                            }
                        </div>
                    )
                }                            
            </>
            }      
            {
                (filteredDataTypeTopics.length > 0) && 
            <div>
                <Typography text="Data Types" />
                <Divider />
                {
                    filteredDataTypeTopics.map((result, index) => 
                        <div 
                            className={classes.searchResult} 
                            key={index} 
                            role="button" 
                            onClick={() => handleLinkSelect(result)}
                            onKeyDown={() => handleLinkSelect(result)}
                            tabIndex={-1}
                        >
                            <div style={{ height: theme.spacing(1) }} />
                            <Typography text={result.text} size="6" />
                            <Typography text={result.description} size="2" color="textSecondary" />
                            <div style={{ height: theme.spacing(1) }} />
                            {
                                (index < filteredDataTypeTopics.length-1) &&
                                    <Divider />
                            }
                        </div>
                    )
                }                            
            </div>
            }    
            {
                (filteredFunctionTopics.length > 0) && 
            <div>
                <Typography text="Functions" />
                <Divider />
                {
                    filteredFunctionTopics.map((result, index) => 
                        <div 
                            className={classes.searchResult} 
                            key={index} 
                            role="button" 
                            onClick={() => handleLinkSelect(result)}
                            onKeyDown={() => handleLinkSelect(result)}
                            tabIndex={-1}
                        >
                            <div style={{ height: theme.spacing(1) }} />
                            <Typography text={result.text} size="6" />
                            <Typography text={result.description} size="2" color="textSecondary" />
                            <div style={{ height: theme.spacing(1) }} />
                            {
                                (index < filteredFunctionTopics.length-1) &&
                                    <Divider />
                            }
                        </div>
                    )
                }                            
            </div>
            }  
            {
                (filteredObjectTypeTopics.length > 0) && 
                <div>
                    <Typography text="Object Types" />
                    <Divider />
                    {
                        filteredObjectTypeTopics.map((result, index) => 
                            <div 
                                className={classes.searchResult} 
                                key={index} 
                                role="button" 
                                onClick={() => handleLinkSelect(result)}
                                onKeyDown={() => handleLinkSelect(result)}
                                tabIndex={-1}
                            >
                                <div style={{ height: theme.spacing(1) }} />
                                <Typography text={result.text} size="6" />
                                <Typography text={result.description} size="2" color="textSecondary" />
                                <div style={{ height: theme.spacing(1) }} />
                                {
                                    (index < filteredObjectTypeTopics.length-1) &&
                                        <Divider />
                                }
                            </div>
                        )
                    }                            
                </div>
            }  
        </>
    );
};


/**
 * AppSearch
 * 
 * This component uses a searchable TextField to allow the user 
 * to lookup topics inside the App and quickly locate information
 * that may be indexed.
 */
const AppSearch = () => {    
    
    const [searchText, setSearchText] = useState("");
    const [anchorEl, setAnchorEl] = useState();    
    const [showSearchHelp, setShowSearchHelp] = useState(false);

    const searchTextField = useRef(null);
    const mobileSearchTextField = useRef(null);

    const classes = useClasses(styles);
    const theme = useTheme();

    const {services, extensions, dataTypes, functions, objectTypeObjects} = useAppCore();
    const history = useHistory();

    useEffect(() => {
        
        const onKeyUp = (e) => {
            
            if (e.srcElement.nodeName === "BODY" && (e.key === "/")) {
                searchTextField.current.focus();
            }
        };

        window.addEventListener("keyup", onKeyUp);

        return () => { window.removeEventListener("keyup", onKeyUp); };

    }, []);

    const serviceTopics = useMemo(() => {
        if (services) {
            return services.map(service => {
                const serviceId = service.getPropertyValue("serviceId");

                return ({
                    text: serviceId,
                    url: "/Admin/Services/" + serviceId,
                    description: "Service definition for " + serviceId,
                });
            });
        } else return [];
    }, [services]);

    const extensionTopics = useMemo(() => {
        if (extensions) {
            return extensions.map(extension => {
                const extensionId = extension.extensionId;

                return ({
                    text: extensionId,
                    url: "/Admin/Extensions/" + extensionId,
                    description: "Extension definition for " + extensionId,
                });
            });
        } else return [];
    }, [extensions]);

    const dataTypeTopics = useMemo(() => {
        if (dataTypes) {
            return dataTypes.map(d => {
                const dataType = d.dataType;

                return ({
                    text: dataType,
                    url: "/Documentation/Reference/DataTypes/" + dataType,
                    description: "Reference for Data Type " + dataType,
                });
            });
        } else return [];
    }, [dataTypes]);

    const functionTopics = useMemo(() => {
        if (functions) {
            return functions.map(func => {
                const functionId = func.functionId;

                return ({
                    text: functionId,
                    url: "/Documentation/Reference/Functions/" + func.category + "/" + functionId,
                    description: "Reference for Function " + functionId,
                });
            });
        } else return [];
    }, [functions]);

    const objectTypeTopics = useMemo(() => {
        if (objectTypeObjects) {
            return Object.values(objectTypeObjects).map(objectTypeObject => {                            
                const objectTypeId = objectTypeObject.getPropertyValue("objectType");

                return ({
                    text: objectTypeId,
                    url: "/Documentation/Reference/Schema/" + objectTypeObject.getAdapterId() + "/" + objectTypeId,
                    description: "Reference manual for Object Type " + objectTypeId
                });
            });
        } else return [];
    }, [objectTypeObjects]);    
    
    const showPopper = () => {
        setAnchorEl(searchTextField.current);       
    };
    
    const closePopper = () => {
        setAnchorEl();
    };

    const handleSearchChange = () => {                        
        const searchText = searchTextField?.current?.value ?? "";        
        setSearchText(searchText);        

        if (searchText && !anchorEl)
            showPopper();
        else if (!searchText)
            closePopper();
    };    

    const handleMobileSearchChange = () => {        
        const searchText = mobileSearchTextField?.current?.value ?? "";
        setSearchText(searchText);        

        if (searchText && !anchorEl)
            showPopper();
        else if (!searchText)
            closePopper();
    };

    const handleLinkSelect = (result) => {
        closePopper();        
        setShowSearchHelp(false);        
        if (searchTextField.current)
            searchTextField.current.value = "";
        setSearchText("");

        history.push(result.url);
    };

    const filteredGeneralTopics = generalSearchTopics.filter(result => result.text.toLowerCase().indexOf(searchText.toLowerCase()) >= 0);
    const filteredServiceTopics = serviceTopics.filter(result => result.text.toLowerCase().indexOf(searchText.toLowerCase()) >= 0);
    const filteredExtensionTopics = extensionTopics.filter(result => result.text.toLowerCase().indexOf(searchText.toLowerCase()) >= 0);
    const filteredDataTypeTopics = dataTypeTopics.filter(result => result.text.toLowerCase().indexOf(searchText.toLowerCase()) >= 0);
    const filteredFunctionTopics = functionTopics.filter(result => result.text.toLowerCase().indexOf(searchText.toLowerCase()) >= 0);
    const filteredObjectTypeTopics = objectTypeTopics.filter(result => result.text.toLowerCase().indexOf(searchText.toLowerCase()) >= 0);    

    const totalResultsCount =   filteredGeneralTopics.length + filteredServiceTopics.length +
                                filteredDataTypeTopics.length +filteredFunctionTopics.length +
                                filteredExtensionTopics.length + filteredObjectTypeTopics.length;

    return (
        <>
            <Hidden 
                smDown={true}
                contains={
                    <>
                        <TextField           
                            inputRef={searchTextField}                 
                            placeholder="Search..."                                      
                            InputProps={{
                                disableUnderline: true,
                                startAdornment: (
                                    <InputAdornment position="start">
                                        <Search />
                                    </InputAdornment>
                                ),
                                className: classes.helpTextField,          
                                inputProps: {
                                    "aria-label": "Search"
                                }                                
                            }}                            
                            variant="standard"
                            onChange={debounce(handleSearchChange, 150)}
                            id="help-search-field"                
                        />  
                        <Popper
                            open={Boolean(anchorEl)}                
                            anchorEl={anchorEl}                            
                            placement="bottom-start" 
                            style={{ zIndex: 2 }}       
                            modifiers={[
                                {
                                    name: "offset",
                                    options: {
                                        offset: {
                                            offset: "-50%, 20px"
                                        }
                                    }
                                }
                            ]}                      
                        > 
                            <ClickAwayListener onClickAway={() => { setSearchText(""); closePopper(); }}>
                                <div style={{
                                    minWidth: "350px", maxWidth: "800px", overflowY: "auto", overflowX: "hidden", backgroundColor: theme.palette.background.paper,
                                    boxShadow: "rgba(0, 0, 0, 0.2) 0px 1px 8px 0px, rgba(0, 0, 0, 0.14) 0px 3px 4px 0px, rgba(0, 0, 0, 0.12) 0px 3px 3px -2px"
                                }}   
                                >
                                    <div style={{ maxHeight: "400px", overflowY: "auto", padding: theme.spacing(3), overflowX: "hidden" }}>                                                    
                                        <AppSearchResults 
                                            handleLinkSelect={handleLinkSelect}
                                            filteredGeneralTopics={filteredGeneralTopics}
                                            filteredServiceTopics={filteredServiceTopics}
                                            filteredExtensionTopics={filteredExtensionTopics}
                                            filteredDataTypeTopics={filteredDataTypeTopics}
                                            filteredFunctionTopics={filteredFunctionTopics}
                                            filteredObjectTypeTopics={filteredObjectTypeTopics}
                                        />                                      
                                    </div>
                                    <div style={{ padding: theme.spacing(2), textAlign: "right" }}>
                                        <Typography size="3" component="span" color="textSecondary" text={String(totalResultsCount) + " Results Found."} />
                                    </div> 
                                </div>                                       
                            </ClickAwayListener>          
                        </Popper>
                    </>
                }
            />
            <Hidden 
                mdUp={true}
                contains={
                    <>
                        <Button 
                            style={{ color: "white" }}
                            onClick={() => setShowSearchHelp(true)}
                        >
                            <Search />
                        </Button>
                        <Dialog 
                            open={showSearchHelp}
                            onDismiss={() => setShowSearchHelp(false)}
                            showClose={true}
                            fullScreen={true}
                            contains={
                                <div style={{ padding: theme.spacing(5) }}>
                                    <TextField                    
                                        inputRef={mobileSearchTextField}                                     
                                        //eslint-disable-next-line
                                        autoFocus={true}
                                        aria-label="Search"                                                      
                                        placeholder="Search..."                                      
                                        InputProps={{
                                            startAdornment: (
                                                <InputAdornment position="start">
                                                    <Search />
                                                </InputAdornment>
                                            ),   
                                            "aria-label": "Search"                                                                                                                
                                        }}
                                        onChange={debounce(handleMobileSearchChange, 150)}
                                        id="mobile-search-field"
                                    />  
                                    <div style={{ height: theme.spacing(5) }} />
                                    { searchText &&
                                        <AppSearchResults 
                                            handleLinkSelect={handleLinkSelect}
                                            filteredGeneralTopics={filteredGeneralTopics}
                                            filteredServiceTopics={filteredServiceTopics}
                                            filteredExtensionTopics={filteredExtensionTopics}
                                            filteredDataTypeTopics={filteredDataTypeTopics}
                                            filteredFunctionTopics={filteredFunctionTopics}
                                            filteredObjectTypeTopics={filteredObjectTypeTopics}
                                        />
                                    }
                                </div>
                            }
                        />
                    </>
                }
            />
        </>
    );
};

export default AppSearch;
