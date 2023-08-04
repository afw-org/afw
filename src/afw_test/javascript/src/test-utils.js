// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import "@testing-library/jest-dom";
import {render} from "@testing-library/react";
import {ThemeProvider, createTheme} from "@mui/material/styles";
import {waitForElementToBeRemoved} from "@testing-library/react";
import userEvent from "@testing-library/user-event";
import {rest} from "msw";

import {server} from "./__mocks__/server";
import {mswPostCallback, mswGetCallback} from "./__mocks__/handlers";
import {unstable_trace, unstable_wrap} from "scheduler/tracing";

export const withProfiler = (Component, id = "withProfiler") => {

    const onRender = (id, phase, actualDuration, baseDuration, startTime, commitTime, interactions) => {
        // eslint-disable-next-line @typescript-eslint/no-use-before-define
        SnapshotProfiler.__numCommits++;
        SnapshotProfiler.__renderLog.push({
            id, phase, actualDuration, baseDuration, startTime, commitTime, interactions
        });
    };

    const SnapshotProfiler = (props) => 
        React.createElement(
            React.Profiler,
            { id, onRender },
            React.createElement(Component, props)
        );

    SnapshotProfiler.__hasVerifiedInitialCommit = false;
    SnapshotProfiler.__numCommits = 0;
    SnapshotProfiler.__renderLog = [];

    SnapshotProfiler.debug = () => {
        let debug = "";

        SnapshotProfiler.__renderLog.forEach(log => {
            debug += "  id=" + log.id + ",phase=" + log.phase + ",actualDuration=" + log.actualDuration + ",interactions=";
            if (log.interactions) {
                for (let entry of log.interactions) { 
                    debug += JSON.stringify(entry) + ",";
                }
            }
            debug += "\n";
        });

        console.log(debug + "\n");
    };

    SnapshotProfiler.reset = () => {
        SnapshotProfiler.__numCommits = 0;
        SnapshotProfiler.__renderLog = [];
    };

    return SnapshotProfiler;
};

const toMatchNumCommits = (SnapshotProfiler) => {
    if (!SnapshotProfiler.__hasVerifiedInitialCommit) {
        SnapshotProfiler.__hasVerifiedInitialCommit = true;
    }

    //expect(SnapshotProfiler.__numCommits).toMatchSnapshot();
    SnapshotProfiler.reset();
    
    return { pass: true };
};

const toHaveCommittedTimes = (SnapshotProfiler, expectedNumCommits) => {
    expect(SnapshotProfiler.__numCommits).toBe(expectedNumCommits);
};

export const trace = (msg, func) => {
    return unstable_trace(msg, performance.now(), func);
};

export {unstable_wrap as wrap};

expect.extend({
    toHaveCommittedTimes,
    toMatchNumCommits
});

const KEY = "_mui_cleaned_";

const getNodes = (node, nodes = []) => {
    if (typeof node === "object") {
        nodes.push(node);
    }

    if (node.children) {
        Array.from(node.children).forEach(child => getNodes(child, nodes));
    }

    return nodes;
};

const getClassNamesFromDOM = node => Array.from(node.classList);
const getClassNamesFromProps = node => {
    const classNameProp = node.props && (node.props.class || node.props.className);

    if (classNameProp) {
        return classNameProp.trim().split(/\s+/);
    }

    return [];
};

const getClassNames = nodes =>
    nodes.reduce((classNames, node) => {
        let newClassNames = null;

        if (global.Element && node instanceof global.Element) {
            newClassNames = getClassNamesFromDOM(node);
        } else {
            newClassNames = getClassNamesFromProps(node);
        }

        newClassNames.forEach(className => classNames.add(className));

        return classNames;
    }, new Set());

const markNodes = nodes => nodes.forEach(node => (node[KEY] = true));

// https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Regular_Expressions#Escaping
const escapeRegExp = (string) => string.replace(/[.*+\-?^${}()|[\]\\]/g, "\\$&");

const removeIncrementalSuffix = (code, classNames) =>
    Array.from(classNames)
        .filter(className => className.match(/-\d+$/))
        .reduce(
            (acc, val) => {
                const exp = new RegExp(escapeRegExp(val), "g");
                return acc.replace(exp, val.slice(0, val.lastIndexOf("-")));
            },
            code,
        );

expect.addSnapshotSerializer({
    test: function(val) {
        return (
            val &&
            !val[KEY] &&
            (val.$$typeof === Symbol.for("react.test.json") ||
                (global.Element && val instanceof global.Element))
        );
    },
    print: function(val, print) {        
        const nodes = getNodes(val);
        const classNames = getClassNames(nodes);        
        markNodes(nodes);  
        
        return removeIncrementalSuffix(print(val), classNames);        
    }
});

/* Transform mui- non-deterministic styling classes that can change upon compiles */
expect.addSnapshotSerializer({
    test: function(val) {
        return val && typeof val === "string" && (val.match(/mui-[0-9]+/) != null);
    },
    print: function(val) {
        let str = val;
        str = str.replace(/mui-[0-9]+/g, "mui-00000");        
        
        return `"${str}"`;
    }
});

/* Strip out <script> tags */
expect.addSnapshotSerializer({
    test: function(val) {        
        return val && typeof(val) === "object" && (val.tagName === "SCRIPT");
    },
    print: function(val) {
        return "";
    }
});

expect.extend({
    toHaveBeenCalledWithObjectContaining(received, property, value) {   
        if (received.mock.calls.length === 0)
            return {
                pass: false,
                message: () => "No calls made"
            };

        for (const call of received.mock.calls) {
            if (call[1] && call[1].indexOf(JSON.stringify(property) + ":" + JSON.stringify(value)) >= 0)
                return { pass: true };
        }
        
        return {
            pass: false,
            message: () => "Unable to find property: \"" + property + "\" with matching value " + JSON.stringify(value),
        };        
        
    }
});

expect.extend({   

    toHaveCalledAdaptiveFunction(received, functionId) {        

        if (received.mock.calls.length === 0)
            return {
                pass: false,
                message: () => "No calls made"
            };

        for (const call of received.mock.calls) {
            const objGot = JSON.stringify(call[1]);
            if (objGot.indexOf(JSON.stringify("function") + ":" + JSON.stringify(functionId)) >= 0)
                return {
                    pass: true,
                    message: () => "Adaptive function " + functionId + " was called."
                };            
        }

        return {
            pass: false,
            message: () => "Adaptive function " + functionId + " was not called.",            
        };
    },

    toHaveBeenCalledWithObjectContainingSparse(received, obj) {

        if (received.mock.calls.length === 0) {            
            return {
                pass: false,
                message: () => "No calls made"
            };
        }
  
        for (const call of received.mock.calls) {
            const objGot = call[1].body;

            let match = true;
            for (const [k, v] of Object.entries(obj)) {
                if (objGot[k] !== v)
                    match = false;
            }

            if (match) {
                return {
                    pass: true,
                    message: () => "All keys/values matched"
                };
            }
        }
        
        return {
            pass: false,
            message: () => "Not all keys matched values.",            
        };
    },

    toHaveBeenCalledWithObjectContainingDeep(received, property, value) {
  
        if (received.mock.calls.length === 0)
            return {
                pass: false,
                message: () => "No calls made"
            };
  
        for (const call of received.mock.calls) {
            const objGot = JSON.stringify(call[1]);
            if (property && (value !== undefined)) {
                if (objGot.indexOf(JSON.stringify(property) + ":" + JSON.stringify(value)) >= 0)      
                    return { 
                        pass: true,
                        message: () => "Found matching property: \"" + property + "\" with matching value " + JSON.stringify(value),
                    }; 
    
            } else if (property) {
                if (objGot.indexOf(JSON.stringify(property) + ":") >= 0)
                    return { 
                        pass: true,
                        message: () => "Found matching property: \"" + property + "\"",
                    };
    
            } else if ((value !== undefined) && objGot.indexOf(JSON.stringify(value)) > 0) {
                return { 
                    pass: true,
                    message: () => "Found matching value " + JSON.stringify(value),
                };  
            }           
        }
  
        return {
            pass: false,
            message: () => "Unable to find property: \"" + property + "\" with matching value " + JSON.stringify(value),
            //message: () => "Received calls: " + JSON.stringify(received.mock.calls)
        };
    },

    toHaveBeenLastCalledWithObjectContainingDeep(received, property, value) {
  
        if (received.mock.calls.length === 0)
            return {
                pass: false,
                message: () => "No calls made"
            };    
        
        const objGot = JSON.stringify(received.mock.calls[ received.mock.calls.length - 1 ]);
        if (property && (value !== undefined)) {
            if (objGot.indexOf(JSON.stringify(property) + ":" + JSON.stringify(value)) >= 0)      
                return { 
                    pass: true,
                    message: () => "Found matching property: \"" + property + "\" with matching value " + JSON.stringify(value),
                }; 

        } else if (property) {
            if (objGot.indexOf(JSON.stringify(property) + ":") >= 0)
                return { 
                    pass: true,
                    message: () => "Found matching property: \"" + property + "\"",
                };

        } else if ((value !== undefined) && objGot.indexOf(JSON.stringify(value)) > 0) {
            return { 
                pass: true,
                message: () => "Found matching value " + JSON.stringify(value),
            };  
        }
            
        return {
            pass: false,
            message: () => "Unable to find property: " + JSON.stringify(property) + "\" with matching value " + JSON.stringify(value) + "\nObject Received: " + objGot,
        };
    }
});

/* code that uses Date.now() will alter snapshots all the time, so just return a static one */
jest.spyOn(Date, "now").mockImplementation(() => 1594224000000);  


const AllTheProviders = ({ children }) => {    
    return (                
        <ThemeProvider theme={createTheme({})}>
            { children }
        </ThemeProvider>
    );
};

const customRender = (ui, options) =>
    render(ui, { wrapper: AllTheProviders, ...options });

export const waitForSpinner = async () => {
    if (document.querySelector("[data-component-type='Spinner']"))
        return waitForElementToBeRemoved(() => document.querySelector("[data-component-type='Spinner']"));
};

// re-export everything
export * from "@testing-library/react";
export { customRender as render, AllTheProviders };
export { rest, server, userEvent, mswPostCallback, mswGetCallback };
