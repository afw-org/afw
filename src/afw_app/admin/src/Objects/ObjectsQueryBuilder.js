// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect, useMemo} from "react";

import {
    Button,    
    Dialog,
    Dropdown,
    Paper,
    PropertyTypeDropdown,
    TextField,
    ToggleButtons,    
    Typography,    
} from "@afw/react";

import {useTheme} from "../hooks";

const rqlOperations = {
    "eq": {
        name:       "eq",
        operation:   "=",
        text:       "equals",
    },
    "ne": {
        name:       "ne",
        operation:   "=ne=",
        text:       "does not equal",
    },
    "lt": {
        name:       "lt",
        operation:   "=lt=",
        text:       "is less than",
    },
    "lte": {
        name:       "lte",
        operation:   "=lte=",
        text:       "is less than or equal to",
    },
    "gt": {
        name:       "gt",
        operation:   "=gt=",
        text:       "is greater than",
    },
    "gte": {
        name:       "gte",
        operation:   "=gte=",
        text:       "is greater than or equal to",
    },
    "match": {
        name:       "match",
        operation:   "=match=",
        text:       "matches",
    },
    "differ": {
        name:       "differ",
        operation:   "=differ=",
        text:       "does not match",
    },
    "contains": {
        name:       "contains",
        operation:   "=contains=",
        text:       "contains",
    },
    "in": {
        name:       "in",
        operation:   "=in=",
        text:       "is in",
    },
    "out": {
        name:       "out",
        operation:   "=out=",
        text:       "is not in",
    },
};

const rqlOperators = {
    "and": {
        name:       "and",
        operator:   "&",
        text:       "And"
    },
    "or": {
        name:       "or",
        operator:   "|",
        text:       "Or"
    },
};


/** 
 * Condition
 * 
 * {
 *     name:"eq",
 *     args:["foo","bar"]
 * }
 */
const Condition = ({ total, index, objectTypeObject, operator, arg, onChanged }) => {

    const [propertyName, setPropertyName] = useState();
    const [operation, setOperation] = useState();
    const [value, setValue] = useState();

    const theme = useTheme();

    useEffect(() => {        
        if (arg.name)
            setOperation(arg.name);
        if (arg.args && arg.args[0])
            setPropertyName(arg.args[0]);
        if (arg.args && arg.args[1])
            setValue(arg.args[1]);
    }, [arg, arg.name, arg.args]);

    const onChange = (operation, propertyName, value) => {
        if (operation && propertyName && value) {
            onChanged({
                name: operation,
                args: [ propertyName, value ]
            });
        }        
    };

    const onSelectProperty = (p) => {
        setPropertyName(p);
        onChange(operation, p, value);
    };

    const onSelectOperator = ({ key, name }) => {
        setOperation(key);
        onChange(name, propertyName, value);
    };

    const onSelectValue = (v) => {
        setValue(v);
        onChange(operation, propertyName, v);
    };

    const description = (total > 1 && index) ? 
        ((operator === "and") ? "And" : "Or") + " If Property" : "If Property";   
        
    return (
        <div style={{ display: "flex", alignItems: "center" }}>
            <div style={{ marginRight: theme.spacing(2), minWidth: "120px" }}>
                <Typography text={description} color="textSecondary" />
            </div>
            <div style={{ marginRight: theme.spacing(2), minWidth: "150px" }}>
                <PropertyTypeDropdown  
                    aria-label="Property"
                    placeholder="(propertyName)"                    
                    allowQuery={true}
                    objectTypeObject={objectTypeObject}
                    onChanged={onSelectProperty}
                    value={propertyName}
                />
            </div>
            <div style={{ marginRight: theme.spacing(2), minWidth: "150px" }}>
                <Dropdown 
                    aria-label="Operator"
                    required={true}
                    selectedKey={operation}
                    options={Object.values(rqlOperations).map(v => ({ ...v, key: v.name }))}                    
                    onChanged={onSelectOperator}
                />
            </div>
            <div style={{ flex: 1 }}>
                <TextField                            
                    aria-label="Value"                        
                    placeholder="value"
                    value={value ? value : ""}
                    onChanged={onSelectValue}                    
                />
            </div>
        </div>
    );
};

/** 
 * Group
 * {
 *     name:"and",
 *     args:[
 *         name:"eq",
 *         args:["foo","bar"]
 *     ]
 * }
 */
const Group = ({ objectTypeObject, parsedQuery, onChanged, prefix, depth }) => {

    const [operator, setOperator] = useState("and");
    const [args, setArgs] = useState();  
    const theme = useTheme();

    useEffect(() => {
        if (parsedQuery) {
            setOperator(parsedQuery.name);
            setArgs(parsedQuery.args);
        }
    }, [parsedQuery]);
    
    const addGroup = () => {         
        if (args)    
            setArgs(args => [ ...args, { name: "and", args: [] } ]);
        else
            setArgs([ { name: "and", args: [] } ]);
    };

    const addCondition = () => {       
        if (args)        
            setArgs(args => [ ...args, { name: "eq", args: [] } ]);
        else
            setArgs([ { name: "eq", args: [] } ]);
    };

    const onArgChanged = (arg, index) => {
        let newArgs = args ? [ ...args ] : [];
        newArgs[index] = arg;
        setArgs(newArgs);
        
        onChanged({
            name: operator,
            args: newArgs,
        });                
    }; 

    const onSetOperator = (op) => {
        setOperator(op);
    
        onChanged({
            name: op,
            args
        });
    };

    const onRemove = (arg, index) => {
        const newArgs = [ ...args ];
        newArgs.splice(index, 1);
        
        const conds = { name: operator, args: newArgs };
    
        setArgs(newArgs);
        onChanged(conds);
    };

    /* eslint-disable jsx-a11y/no-static-element-interactions */
    return (
        <Paper                         
            contains={
                <div 
                    style={{ 
                        marginTop: theme.spacing(1), 
                        backgroundColor: (depth % 2 === 1) ? theme.palette.background.default : theme.palette.background.paper 
                    }}
                >   
                    <div 
                        style={{ width: "100%", display: "flex", justifyContent: "space-between", padding: theme.spacing(1) }}                     
                        onClick={evt => evt.stopPropagation()}                        
                        onKeyDown={evt => evt.stopPropagation()}
                    >   
                        <div style={{ padding: theme.spacing(1) }}>
                            <div style={{ display: "inline-block", marginRight: theme.spacing(1) }}>
                                <Button                     
                                    label="Group"   
                                    icon="add"
                                    size="small"
                                    tooltip="Add a new Query Criteria condition group."
                                    onClick={addGroup}
                                />
                            </div>
                            <div style={{ display: "inline-block" }}>
                                <Button       
                                    size="small"
                                    label="Condition"
                                    icon="add"
                                    tooltip="Add a Query Criteria condition."
                                    onClick={addCondition}
                                />     
                            </div>     
                        </div>    
                        <div>
                            <ToggleButtons 
                                exclusive={true}
                                value={operator}
                                onChanged={onSetOperator}
                                options={[
                                    {
                                        value: "and",
                                        tooltip: "AND Conditions",
                                        label: "And",
                                        "aria-label": "AND Conditions"
                                    },
                                    {
                                        value: "or",
                                        tooltip: "OR Conditions",
                                        label: "Or",
                                        "aria-label": "OR Conditions"
                                    },
                                ]}
                            />                     
                        </div>         
                    </div>     
                    <div style={{ padding: theme.spacing(2) }}>
                        {
                            ((args === undefined) || args.length === 0) ?
                                <Typography text="(No Conditions have been defined)" color="textSecondary" /> :
                                args.map((arg, index) => {
                                    return (
                                        <div key={index} style={{ display: "flex", alignItems: "center" }}>   
                                            <div>                                         
                                                <Button 
                                                    type="icon"
                                                    icon="close"
                                                    style={{ color: "red" }}
                                                    onClick={() => onRemove(arg, index)}
                                                />
                                            </div>
                                            <div style={{ flex: 1 }}>
                                                {
                                                    (arg.name === "and" || arg.name === "or") ?
                                                        <Group 
                                                            key={index}
                                                            prefix={prefix + "-" + index}
                                                            parsedQuery={arg}
                                                            objectTypeObject={objectTypeObject}
                                                            index={index}
                                                            onChanged={cond => onArgChanged(cond, index)}
                                                            depth={depth + 1}
                                                        /> :
                                                        <Condition 
                                                            key={index}
                                                            prefix={prefix + "-" + index}
                                                            objectTypeObject={objectTypeObject}
                                                            index={index}
                                                            total={args.length}
                                                            operator={operator}
                                                            arg={arg}
                                                            onChanged={cond => onArgChanged(cond, index)}
                                                            depth={depth + 1}
                                                        />
                                                }
                                            </div>
                                        </div>
                                    );

                                })
                        }
                    </div>                    
                </div>
            } />        
    );
};


const ObjectsQueryBuilder = ({ queryCriteria, objectTypeObject, onChanged }) => {

    const parsedQuery = useMemo(() => {

        const parse = (str) => {            

            if (str[0] === "(") {
                let operator = "and";
                let conds = [];
                let stack = 1;
                let pos = 1;
                for (let i = 1; i < str.length; i++) {
                    if (str[i] === "(") {
                        pos = i;
                        stack++;
                    } else if (str[i] === ")") {
                        stack--;
                        if (stack === 0) {
                            conds.push( parse(str.substring(pos, i)) );
                        }
                    } else if (str[i] === "|" && stack === 0) {
                        operator = "or";
                    }
                }

                return ({
                    name: operator,
                    args: conds
                });
              
            }

            else {
                let conditions = str.split("&");
                if (conditions.length > 1) {
                    return ({
                        name: "and",
                        args: conditions.map(c => parse(c))                        
                    });
                } else {
                    conditions = str.split("|");
                    if (conditions.length > 1) {
                        return ({
                            name: "or",
                            args: conditions.map(c => parse(c))                            
                        });
                    }

                    else {
                        // prop=value                        
                        for (let operator of Object.values(rqlOperations)) {
                            if (str.indexOf(operator.operation) > 0) {
                                return ({
                                    name: operator.name,
                                    args: [
                                        str.split(operator.operation)[0],
                                        str.split(operator.operation)[1],
                                    ]
                                });
                            }
                        }
                    }
                }
            }
        };

        const parsed = parse(queryCriteria);

        if (parsed && parsed.name !== "and" && parsed.name !== "or") {
            return ({
                name: "and",
                args: [ parsed ]
            });
        }

        return parsed;
    }, [queryCriteria]);    
       
    const conditionToStr = (condition) => {

        const {name, args} = condition;

        if (name === "and" || name === "or") {
            return "(" + args.filter(arg => arg.name && arg.args?.length).map(arg => conditionToStr(arg)).join(rqlOperators[name].operator) + ")";
        } else {
            return args[0] + rqlOperations[name].operation + args[1];
        }
        
    };

    const onGroupChanged = ({ name, args }) => {        

        let queryCriteria;

        queryCriteria = args.filter(arg => arg.name && arg.args?.length).map(arg => conditionToStr(arg)).join(rqlOperators[name].operator);        

        onChanged(queryCriteria);
    };

    return (
        <div>
            <Group 
                parsedQuery={parsedQuery}
                objectTypeObject={objectTypeObject}
                onChanged={onGroupChanged}
                prefix=""
                depth={1}
            />
        </div>        
    );
};

const ObjectsQueryBuilderDialog = ({ open, queryCriteria, objectTypeObject, onDismiss, onApply }) => {

    const [criteria, setCriteria] = useState();

    const theme = useTheme();

    useEffect(() => {
        setCriteria(queryCriteria);
    }, [queryCriteria]);

    const onCancel = () => {
        setCriteria(queryCriteria);
        onDismiss();
    };

    return (
        <Dialog 
            open={open}
            title="Query Builder"
            maxWidth="xl"
            showClose={true}     
            onDismiss={onCancel}       
            contains={
                <div style={{ height: "60vh", overflow: "auto" }}>
                    <ObjectsQueryBuilder                         
                        queryCriteria={criteria}
                        objectTypeObject={objectTypeObject}
                        onChanged={newCriteria => setCriteria(newCriteria)}
                    />
                </div>
            }
            footer={
                <div style={{ width: "100%", display: "flex", justifyContent: "space-between", alignItems: "center" }}>
                    <div style={{ width: "100%", marginLeft: theme.spacing(2) }}>
                        <Typography text={criteria ? criteria : ""} color="textSecondary" />
                    </div>                    
                    <div style={{ display: "flex", justifyContent: "flex-end", width: "100%", padding: theme.spacing(2) }}>                                
                        <div style={{ marginRight: theme.spacing(1) }}>
                            <Button                         
                                label="Apply"
                                color="primary"
                                variant="contained"
                                disabled={!criteria || (criteria === queryCriteria)}
                                onClick={() => onApply(criteria)}
                            />
                        </div>                            
                        <div>
                            <Button 
                                label="Cancel"
                                onClick={onCancel}
                            />
                        </div>
                    </div>                    
                </div>
            }
        />
    );
};

export default ObjectsQueryBuilderDialog;
