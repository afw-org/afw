// See the 'COPYING' file in the project root for licensing information.
import {useReducer} from "react";

import {
    Button,    
    Dropdown,       
    TextField,
} from "@afw/react";

import {TabbedCodeEditor} from "../../common/TabbedCodeEditor";
import {useTheme} from "../../hooks";

const initialState = {
    method:         "POST",
    uri:            "/afw",
    headers:        {
        "Content-Type": "application/json",
        "Accept": "application/x-afw"
    },
    tabs:           [
        {
            id: "headers",
            "data-testid": "headers",
            label: "Headers",
            source: "{\n  \"Content-Type\": \"application/json\",\n  \"Accept\": \"application/x-afw\"\n}",
            canClose: false,
            format: "json"
        },
        {
            id: "request",
            "data-testid": "request",
            label: "Request",
            source: "",            
            canClose: false,
            format: "json",
        },
        {
            id: "response",
            "data-testid": "response",
            label: "Response",
            source: "",
            canClose: false,
            format: "plaintext",
        }
    ],
    activeTab:      1,
    contentType:    "application/json",
    accept:         "application/x-afw",
};

const requestsReducer = (state, action) => {

    switch (action.type) {

    case "LOADING":
        return ({
            ...state,
            spinnerText: action.spinnerText
        });

    case "SWITCH_TAB": 
        return ({
            ...state,
            activeTab: action.activeTab,
        });

    case "METHOD": 
        return ({
            ...state,
            method: action.method,
        });

    case "URI": 
        return ({
            ...state,
            uri: action.uri,
        });

    case "CONTENT_TYPE": {
        let updatedTabs = [ ...state.tabs ];
        
        switch (action.contentType) {
        case "application/json":
            updatedTabs[1].format = "json";
            break;
        case "application/xml":
            updatedTabs[1].format = "xml";
            break;
        default:
            updatedTabs[1].format = "plaintext";
            break;
        }        

        const contentType = action.contentType;

        try {
            const headers = JSON.parse(updatedTabs[0].source);
            headers["Content-Type"] = contentType;
            updatedTabs[0].source = JSON.stringify(headers, null, 4);
        } catch (e) {
            //eslint-disable-next-line
        }

        return ({
            ...state,
            tabs: updatedTabs,
            contentType: action.contentType,
            headers: {
                "Content-Type": contentType,
                "Accept": contentType,
            }
        });
    }

    case "ACCEPT": {
        let updatedTabs = [ ...state.tabs ];

        const accept = action.accept ? action.accept : "application/plaintext";               

        try {
            const headers = JSON.parse(updatedTabs[0].source);
            headers["Accept"] = accept;
            updatedTabs[0].source = JSON.stringify(headers, null, 4);
        } catch (e) {
            //eslint-disable-next-line
        }

        return ({
            ...state,
            tabs: updatedTabs,
            accept: action.accept,
            headers: {
                "Content-Type": state.contentType,
                "Accept": accept,
            }
        });
    }

    case "SOURCE_CHANGED": {
        let updatedTabs = [ ...state.tabs ];

        updatedTabs[state.activeTab].source = action.source;

        let headers = state.headers;
        if (state.activeTab === 0) {
            try {
                headers = JSON.parse(action.source);                
                // valid JSON                
            // eslint-disable-next-line no-empty
            } catch (e) {}
        }

        return ({
            ...state,
            headers,
            tabs: updatedTabs,                        
        });
    }

    case "RESPONSE": {
        let updatedTabs = [ ...state.tabs ];

        if (action.contentType === "application/json") {
            updatedTabs[2].format = "json";
            updatedTabs[2].source = JSON.stringify(action.text, null, 4);
        } else if (action.contentType === "application/xml") {
            updatedTabs[2].format = "xml";
            updatedTabs[2].source = action.text;
        } else {
            updatedTabs[2].format = "plaintext";
            updatedTabs[2].source = action.text;
        }

        return ({
            ...state,
            activeTab: 2,
            tabs: updatedTabs,
        });
    }

    default:
        return state;

    }
};

export const Requests = () => {

    const theme = useTheme();
    const [state, dispatch] = useReducer(requestsReducer, initialState);

    // \fixme quick hack to format XML
    // At this point in time, a third-party lib is needed to parse/serialize XML properly
    function format(xml) {
        var tab = "\t";
        var result = "";
        var indent= "";
    
        xml.split(/>\s*</).forEach(function(element) {
            if (element.match( /^\/\w/ )) {
                indent = indent.substring(tab.length);
            }
    
            result += indent + "<" + element + ">\r\n";
    
            if (element.match( /^<?\w[^>]*[^/]$/ )) { 
                indent += tab;              
            }
        });
    
        return result.substring(1, result.length-3);
    }

    const onSend = async () => {

        let contentType = state.contentType;
        const {headers, method, uri, tabs} = state;
        let text;
        
        try {
            const response = await fetch(uri, {
                method,
                headers,
                body: tabs[1].source ? tabs[1].source : undefined
            });       

            contentType = response.headers.get("content-type");
            if (contentType === "application/json") {
                text = await response.json();                
                dispatch({ type: "RESPONSE", text, contentType });
            } else if (contentType === "application/xml") {
                text = await response.text();                
                dispatch({ type: "RESPONSE", text: format(text), contentType });
            } else {
                text = await response.text();                        
                dispatch({ type: "RESPONSE", text, contentType });
            }            
            
        } catch(error) {
            text = JSON.parse(JSON.stringify(error, Object.getOwnPropertyNames(error)));
            dispatch({ type: "RESPONSE", text, contentType: "application/json" });
        }                
    };

    return (
        <div data-testid="admin-tools-requests" style={{ display: "flex", flexDirection: "column", height: "calc(100% - 30px)" }}>
            <div style={{ display: "flex", alignItems: "flex-end", marginBottom: theme.spacing(4) }}>
                <div style={{ marginRight: theme.spacing(2), width: "120px" }}>
                    <Dropdown 
                        id="requests-method"
                        data-testid="requests-method"
                        label="Method"
                        aria-label="Method"                        
                        selectedKey={state.method}
                        options={[
                            {
                                key: "POST",
                                text: "POST"
                            },
                            {
                                key: "GET",
                                text: "GET"
                            },
                            {
                                key: "PUT",
                                text: "PUT"
                            },
                            {
                                key: "DELETE",
                                text: "DELETE"
                            },
                            {
                                key: "PATCH",
                                text: "PATCH"
                            },
                            {
                                key: "HEAD",
                                text: "HEAD"
                            },
                        ]}
                        onChanged={opt => {
                            dispatch({ type: "METHOD", method: opt.key });                                             
                        }}
                    />
                </div>      
                <div style={{ marginRight: theme.spacing(2), flex: 1 }}>
                    <TextField 
                        id="requests-uri"
                        data-testid="requests-uri"
                        label="URI"                        
                        value={state.uri}
                        onChanged={uri => dispatch({ type: "URI", uri })}
                    />
                </div>                     
                <div style={{ marginRight: theme.spacing(2), width: "120px" }}>
                    <Dropdown 
                        id="requests-content-type"
                        data-testid="requests-content-type"
                        label="Content-Type"
                        selectedKey={state.contentType}
                        options={[
                            {
                                key: "application/json",
                                text: "JSON"
                            },
                            {
                                key: "application/xml",
                                text: "XML"
                            },
                            {
                                key: "application/plaintext",
                                text: "Plaintext"
                            },                     
                        ]}
                        onChanged={opt => {
                            dispatch({ type: "CONTENT_TYPE", contentType: opt.key });                            
                        }}
                    />
                </div>  
                <div style={{ marginRight: theme.spacing(2), width: "120px" }}>
                    <Dropdown 
                        id="requests-accept"
                        data-testid="requests-accept"
                        label="Accept"
                        selectedKey={state.accept}
                        options={[
                            {
                                key: "application/json",
                                text: "JSON"
                            },
                            {
                                key: "application/xml",
                                text: "XML"
                            },
                            {
                                key: "application/x-afw",
                                text: "X-AFW"
                            },     
                            {
                                key: "application/plaintext",
                                text: "Plaintext"
                            },                     
                        ]}
                        onChanged={opt => {
                            dispatch({ type: "ACCEPT", accept: opt.key });                            
                        }}
                    />
                </div>         
                <div>
                    <Button 
                        id="requests-send"
                        data-testid="requests-send"
                        variant="contained"
                        color="primary"
                        label="Send" 
                        icon="send"
                        onClick={onSend}
                    />
                </div>     
            </div>       
            <div style={{ flex: 1, height: "100%" }}>
                <TabbedCodeEditor 
                    tabs={state.tabs}
                    activeTab={state.activeTab}
                    onTabSwitch={activeTab => dispatch({ type: "SWITCH_TAB", activeTab })}
                    onSourceChanged={(newValue) => {
                        dispatch({ type: "SOURCE_CHANGED", source: newValue });
                    }}
                />
            </div>            
        </div>
    );
};

export default Requests;
