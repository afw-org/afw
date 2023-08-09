// See the 'COPYING' file in the project root for licensing information.
import {useMemo, useCallback} from "react";
import PropTypes from "prop-types";

import {
    useClipboard, 
    useFunctions,
    useQualifiers,
} from "@afw/react";

import {Menu} from "@afw/react";

/* Static Menu Options, whose contents will never change */
const staticMenuOptions = {
    "Property": {
        key: "Property", 
        type: "Property",
        label: "Get Object Property",        
        operation: "prop",        
    },

    "ExpressionBuilder": {
        key: "ExpressionBuilder", 
        type: "ExpressionBuilder", 
        label: "Build Expression",
        operation: "exprT",        
    },

    "ListBuilder": {
        key: "ListBuilder",
        type: "ListBuilder",
        label: "Build List",
        operation: "array"
    },
    
    "ObjectBuilder": {
        key: "ObjectBuilder", 
        type: "ObjectBuilder", 
        label: "Build Object",
        operation: "obj",        
    },    

    "LambdaBuilder": {
        key: "LambdaBuilder", 
        type: "LambdaBuilder", 
        label: "Build Lambda Function",
        operation: "lambda",        
    },

    "Call": {
        key: "Call", 
        type: "Call", 
        label: "Call",
        operation: "call",        
    },

    "CallLambda": {
        key: "CallLambda", 
        type: "CallLambda", 
        label: "Call Lambda Function",
        operation: "callL",        
    },

    "RawExpression": {
        key: "RawExpression", 
        type: "RawExpression", 
        label: "Raw Expression",
        operation: "expr",        
    },

    "Template": {
        key: "Template", 
        type: "Template", 
        label: "Template",
        operation: "tmpl",        
    },

    "Script": {
        key: "Script",
        type: "Script",
        label: "Script",
        operation: "script",
    },
};


/*
 * The TupleContextualMenu is a general-purpose component for
 *   displaying a ContextualMenu for the user to choose UI items from.
 *   It can display all options, is requested, or any subset of items
 *   depending on the context the menu may be displayed from, or the
 *   appropriate data types that are needed to fill in a parameter 
 *   value.
 *
 *  Parameters (all boolean):
 *
 *    allowLiterals
 *    allowTuples
 *    dataType
 *    dataTypeParameter
 *
 */
export const TupleContextualMenu = (props) => {    

    const {functions} = useFunctions();
    const {clipboard} = useClipboard();
    const qualifiers = useQualifiers();   

    const {allowTuples, allowLiterals, dataType, onChanged} = props;

    /* when the user selects a menu item */
    const onSelectMenuItem = useCallback((event, item) => {
        let hybrid;        

        if (props.onSelectMenuItem) {
            props.onSelectMenuItem(item);            
            return;
        }
        
        if (item.type === "function") 
        {
            let func = item.func;
            let functionId = item.id;

            hybrid = functionId;            
            let parameters = func.parameters;

            hybrid = [null, "callA"];
            hybrid.push(functionId);

            for (const param of parameters) {
                let minArgs = param.minArgs;
                if (minArgs > 0) {
                    for (let j = 0; j < minArgs; j++)
                        hybrid.push(undefined);
                } else {
                    hybrid.push(undefined);
                }
            }
        }        

        else if (item.type === "dataType")
        {            
            let dataType = item.key;
            if (dataType === "string")
                hybrid = String("");
            else if (dataType === "integer")
                hybrid = 0;
            else if (dataType === "true")
                hybrid = true;
            else if (dataType === "false")
                hybrid = false;
            else if (dataType === "null")
                hybrid = null;
            else if (dataType === "object")
                hybrid = {};
        }

        else if (item.type === "Variable")
            hybrid = [ undefined, "var", item.key ];

        else if (item.type === "Paste") {            
            hybrid = clipboard[ clipboard.length - 1 ];
            if (allowLiterals === false && typeof(hybrid) === "string") {
                /* need to try and convert this to a tuple */
                try {
                    hybrid = JSON.parse(hybrid);
                } catch (error) {
                    return;
                }
            }
        }

        else
            hybrid = [ undefined, item.operation ];      
      
        onChanged(hybrid);
    }, [clipboard, allowLiterals, onChanged, props]);

    const buildFunctionContextualMenu = useCallback(() => {
        let dataType = props.dataType;        
        let dataTypeParameter = props.dataTypeParameter;
        let functionContextualMenu = [];
        let categoryContextualMenu;
        let category;

        /* make sure we have functions loaded first */
        if (!functions)
            return functionContextualMenu;

        functions.forEach(func => {
            let cat = func.category;
            let id = func.functionId;

            /* filter our menu based on the dataType/dataTypeParameter */
            let returnDataType = func.returns.dataType;
            let returnDataTypeParameter = func.returns.dataTypeParameter;

            if (dataType && returnDataType && (dataType !== returnDataType))   
                return;
            
            if (dataTypeParameter && returnDataTypeParameter && (dataTypeParameter !== returnDataTypeParameter))                  
                return;
            
            let name = id;
            if (id.indexOf("::") > 0)
                name = id.split("::")[1];

            if (cat !== category) {
                /* new category */
                categoryContextualMenu = {
                    key: cat,
                    label: cat,
                    name: cat,
                    subMenu: {
                        items: [],
                    }
                };

                functionContextualMenu.push(categoryContextualMenu);
                category = cat;
            }

            categoryContextualMenu.subMenu.items.push({
                key: cat + "_" + id,
                id,
                type: "function",
                label: name,
                name,
                func: func,
                onClick: onSelectMenuItem,
            });
        });

        return functionContextualMenu;
    }, [functions, onSelectMenuItem, props.dataType, props.dataTypeParameter]);    

    const contextualMenu = useMemo(() => {
        /* the workhorse of this component, which figures out which items go into the menus */
        let contextualMenu = [];        
        
        if (!dataType) {
            contextualMenu.push(Object.assign({}, staticMenuOptions.ExpressionBuilder));                
            contextualMenu.push(Object.assign({}, staticMenuOptions.LambdaBuilder));
        }

        /* Build Object */
        if (!dataType || dataType === "object")
            contextualMenu.push(Object.assign({}, staticMenuOptions.ObjectBuilder)); 

        if (!dataType || dataType === "array")
            contextualMenu.push(Object.assign({}, staticMenuOptions.ListBuilder));

        /* Call, Call Adaptive, Call Method, Call Lambda */
        contextualMenu.push({
            key: "Call",
            label: "Call",
            subMenu: {
                items: [
                    {
                        key: "CallAdaptive", 
                        label: "Adaptive Function",
                        operation: "callA",        
                        subMenu: {
                            items: buildFunctionContextualMenu()
                        }
                    },
                    {
                        key: "CallLambda",
                        label: "Lambda Function",
                        operation: "callL",
                        disabled: true,
                        subMenu: {
                            items: []
                        }
                    },
                    {
                        key: "CallOther",
                        label: "Other",
                        operation: "call",
                        onClick: onSelectMenuItem
                    }
                ]
            }
        });
        
        /*! \fixme go through variables, determining which are lambda functions */
            
        /* Get Object Property */
        contextualMenu.push(Object.assign({}, staticMenuOptions.Property));

        /* Literal */
        if (allowLiterals !== false) {
            let dataTypeMenu = [];

            if (!dataType || dataType === "string")
                dataTypeMenu.push({ key: "string", name: "String", label: "String", type: "dataType", onClick: onSelectMenuItem });

            if (!dataType || (dataType === "number" || dataType === "integer" || dataType === "double"))
                dataTypeMenu.push({ key: "integer", name: "Integer", label: "Integer", type: "dataType", onClick: onSelectMenuItem });
            
            if (!dataType || dataType === "boolean") {
                dataTypeMenu.push({ key: "true", name: "True", label: "True", type: "dataType", onClick: onSelectMenuItem });
                dataTypeMenu.push({ key: "false", name: "False", label: "False", type: "dataType", onClick: onSelectMenuItem });
            }

            if (!dataType || dataType === "object")
                dataTypeMenu.push({ key: "object", name: "Object", label: "Object", type: "dataType", onClick: onSelectMenuItem });

            contextualMenu.push({
                key: "Literal",
                label: "Literal",
                subMenu: {
                    items: dataTypeMenu
                }
            });
        }

        /* Raw Expression */
        contextualMenu.push(Object.assign({}, staticMenuOptions.RawExpression));

        /* Template */
        contextualMenu.push(Object.assign({}, staticMenuOptions.Template));

        /* Script */
        contextualMenu.push(Object.assign({}, staticMenuOptions.Script));

        /* Variables */
        if (allowTuples !== false) {
            let variablesContextualMenu = [];
            if (qualifiers) {
                for (const {qualifierName, qualifierDef} of qualifiers) {                    

                    /* get and sort the variable names */                    
                    const variables = qualifierDef;    
                    if (!variables)
                        continue;

                    let qualifierMenuOption = {
                        key: qualifierName,
                        label: qualifierName,
                        subMenu: {
                            items: []
                        }
                    };
    
                    const sortedVariables = Object.entries(variables).map(([varName,]) => 
                        qualifierName + "::" + varName
                    ).sort((a, b) => a.toLowerCase().localeCompare(b.toLowerCase()));
    
                    sortedVariables.forEach(variable => {
                        qualifierMenuOption.subMenu.items.push({
                            key: variable,
                            label: variable,
                            type: "Variable",
                            onClick: onSelectMenuItem,
                        });
                    });

                    variablesContextualMenu.push(qualifierMenuOption);
                }         
            }

            variablesContextualMenu.push({
                key: "variable",
                label: "Other Variable",
                type: "Variable",
                onClick: onSelectMenuItem,
            });

            contextualMenu.push({
                key: "Variables",
                label: "Variables",
                subMenu: {
                    items: variablesContextualMenu,
                },
            });
        }

        contextualMenu.push({
            key: "Divider",
            type: "divider"
        });

        contextualMenu.push({
            key: "Paste",
            type: "Paste",
            label: "Paste Expression",
            disabled: (clipboard === undefined || clipboard.length === 0),
        });

        /* set onClick handlers */
        contextualMenu.forEach((menuItem) => {
            menuItem.onClick = onSelectMenuItem;
        });

        return contextualMenu;
        
    }, [dataType, buildFunctionContextualMenu, onSelectMenuItem, allowLiterals, allowTuples, clipboard, qualifiers]);
    

    return (
        <Menu 
            target={props.target}
            items={contextualMenu}
            open={props.visible}
            onClose={props.onDismiss}
            onClick={onSelectMenuItem}
        />
    );        
};

TupleContextualMenu.propTypes = {
    allowSingle:                PropTypes.bool,
    allowTuples:                PropTypes.bool,
    dataType:                   PropTypes.string,    
    dataTypeParameter:          PropTypes.string,
    visible:                    PropTypes.bool,
    onSelectMenuItem:           PropTypes.func,
};

export default TupleContextualMenu;
