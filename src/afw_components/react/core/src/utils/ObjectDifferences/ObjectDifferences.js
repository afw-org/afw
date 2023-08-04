// See the 'COPYING' file in the project root for licensing information.
import PropTypes from "prop-types";

import {
    Icon,
    Responsive,
    DiffEditor,
    Tabs,
    Tooltip,
    Typography,
    useTheme,
} from "@afw/react";

import {AfwObject} from "@afw/client";
import {stable_stringify, isObject, isArray} from "@afw/react";

/* The property name that is being added/removed/modified */
const ObjectDifferencesPropertyName = (props) => {
    const name = props.name;

    if (isArray(name)) {
        return (
            <Tooltip 
                content={name.join(".")}
                delay={100}
                target={
                    <div>
                        <Typography size="4" color="textSecondary" text={name[name.length-1]} />
                    </div>
                }
            />
        );
    } else {
        return (
            <Typography size="4" color="textSecondary" text={name} />
        );
    }
};

export const ObjectDifferencesList = (props) => {
    const objectDifferences = props.objectDifferences;

    if (objectDifferences) {
        return (
            objectDifferences.map((mod, index) => {
                const [operation, property, value] = mod;

                if (operation === "set_property") {
                    return (
                        <Responsive.Row 
                            key={index}
                            spacing={4}
                            alignItems="flex-start"
                            columns={[
                                {
                                    key: "icon",
                                    breakpoints: {
                                        small: 1,
                                    },
                                    contains: 
                                        <Icon iconName="edit" />
                                },
                                {
                                    key: "operation",
                                    breakpoints: {
                                        small: 3,
                                        medium: 2                                        
                                    },
                                    contains: 
                                        <Typography text="Set Property" color="textSecondary" />
                                },
                                {
                                    key: "property",
                                    breakpoints: {
                                        small: 4,
                                        medium: 2
                                    },
                                    contains:
                                        <ObjectDifferencesPropertyName name={property} />
                                },
                                {
                                    key: "value",
                                    breakpoints: {
                                        small: 4,
                                    },
                                    contains: 
                                        <Tooltip 
                                            content={JSON.stringify(value)}
                                            delay={100}
                                            target={
                                                <div>
                                                    <Typography 
                                                        text={
                                                            JSON.stringify(value).length > 32 ? JSON.stringify(value).substring(0, 32) + "..." :
                                                                JSON.stringify(value)
                                                        } 
                                                    />
                                                </div>
                                            }
                                        />
                                }
                            ]}
                        />
                    );
                } else if (mod[0] === "remove_property") {
                    return (
                        <Responsive.Row 
                            key={index}
                            spacing={4}
                            alignItems="flex-end"
                            columns={[
                                {
                                    key: "icon",
                                    breakpoints: {
                                        small: 1,
                                    },
                                    contains: 
                                        <Icon iconName="delete" />
                                },
                                {
                                    key: "operation",
                                    breakpoints: {
                                        small: 4,
                                        medium: 2                                        
                                    },
                                    contains: 
                                        <Typography text="Remove Property" color="textSecondary" />
                                },
                                {
                                    key: "property",
                                    breakpoints: {
                                        small: 4,
                                        medium: 2                                        
                                    },
                                    contains:
                                        <ObjectDifferencesPropertyName name={mod[1]} />
                                }
                            ]}
                        />
                    );         
                } else if (mod[0] === "add_value") {
                    return (
                        <Responsive.Row 
                            key={index}
                            spacing={4}
                            alignItems="flex-end"
                            columns={[
                                {
                                    key: "icon",
                                    breakpoints: {
                                        small: 1,
                                    },
                                    contains: 
                                        <Icon iconName="add" />
                                },
                                {
                                    key: "operation",
                                    breakpoints: {
                                        small: 4,
                                        medium: 2                                        
                                    },
                                    contains: 
                                        <Typography text="Add Value" color="textSecondary" />
                                },
                                {
                                    key: "property",
                                    breakpoints: {
                                        small: 4,
                                        medium: 2                                        
                                    },
                                    contains:
                                        <Typography text={mod[1]} />
                                },
                                {
                                    key: "value",
                                    breakpoints: {
                                        small: 4,
                                    },
                                    contains: 
                                        <Typography text={JSON.stringify(mod[2])} />
                                }
                            ]}
                        />
                    );                    
                } else if (mod[0] === "remove_value") {
                    return (
                        <Responsive.Row 
                            key={index}
                            spacing={4}
                            alignItems="flex-end"
                            columns={[
                                {
                                    key: "icon",
                                    breakpoints: {
                                        small: 1,
                                    },
                                    contains: 
                                        <Icon iconName="remove" />
                                },
                                {
                                    key: "operation",
                                    breakpoints: {
                                        small: 4,
                                        medium: 2                                        
                                    },
                                    contains: 
                                        <Typography text="Remove Value" color="textSecondary" />
                                },
                                {
                                    key: "property",
                                    breakpoints: {
                                        small: 4,
                                        medium: 2                                        
                                    },
                                    contains:
                                        <Typography text={mod[1]} />
                                },
                                {
                                    key: "value",
                                    breakpoints: {
                                        small: 4,
                                    },
                                    contains: 
                                        <Typography text={JSON.stringify(mod[2])} />
                                }
                            ]}
                        />
                    );                    
                } else return null;

            })
        );
    }

    return null;
};

const replacer = (name, value) => {
    if (name === "_meta_")
        return undefined;
    else
        return value;
};

const sortArrayContents = (arr) => {
    const values = [];
    for (const v of arr) {
        if (isArray(v))
            values.push( sortArrayContents(v) );
        else if (isObject(v)) {
            // eslint-disable-next-line @typescript-eslint/no-use-before-define
            values.push( sortObject(v) );
        } else
            values.push(v);
    }

    return values;
};

const sortObject = (obj) => {
    const ordered = {};

    Object.keys(obj).sort().forEach(key => {
        const value = obj[key];

        if (isArray(value)) {
            ordered[key] = sortArrayContents(value);
        } if (isObject(value)) {
            ordered[key] = sortObject(value);
        } else {
            ordered[key] = value;
        }
    });

    return ordered;
};

/**
 * ObjectDifferences gets the original and modified content from the object
 * and shows the differences in a summary and "diff" navigator.
 * 
 * @param {AfwObject} object 
 */
export const ObjectDifferences = (props) => {
    const object = props.object;

    const theme = useTheme();

    const original = stable_stringify(sortObject(object.getOriginal()), { replacer, space: 4 });
    const modified = stable_stringify(sortObject(object.toJSON()), { replacer, space: 4 });

    return (   
        <Tabs 
            tabs={[
                {
                    text: "Source",
                    contains:
                        <DiffEditor
                            style={{ height: "70vh" }}
                            original={original}
                            modified={modified}
                        />
                },
                {
                    text: "Modifications",
                    contains: 
                        <div style={{ padding: theme.spacing(2), height: "70vh", overflow: "auto" }}>
                            <ObjectDifferencesList {...props} />
                        </div>
                },                
            ]}
        />                  
    );
};

ObjectDifferences.propTypes = {
    object:                     PropTypes.instanceOf(AfwObject),
    objectDifferences:          PropTypes.array,
};

export default ObjectDifferences;
