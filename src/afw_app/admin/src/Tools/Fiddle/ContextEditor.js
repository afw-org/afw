// See the 'COPYING' file in the project root for licensing information.

import {useState} from "react";

import {
    Button,
    CodeEditor,
    Dialog,    
    EditableLabel,
    HybridEditor,
    Menu,
    Table,    
    Typography,
    OperationalMode,
    OperationalContext,
    layoutByDataType
} from "@afw/react";

import {useTheme} from "../../hooks";

const ContextSourceEditor = (/*{ contextVariables }*/) => {
    
    return (
        <CodeEditor 

        />        
    );
};

export const ContextEditor = (props) => {

    const [contextVariables, setContextVariables] = useState(props.contextVariables ? props.contextVariables: []);
    const [selected, setSelected] = useState([]);
    const [menuTarget, setMenuTarget] = useState();
    const [selectedContexts, setSelectedContexts] = useState([]);
    const [selectedContextTypes, setSelectedContextTypes] = useState([]);
    const [editSource, setEditorSource] = useState(false);

    const theme = useTheme();
    const {contextTypes, open, onSave} = props;

    const onDismiss = () => {
        setContextVariables([]);
        props.onDismiss();
    };

    const onSelectContext = (key, { contextTypeId, qualifierDefinitions }) => {
        /* now update variables */
        const qualifiers = Object.entries(qualifierDefinitions);

        if (!selectedContexts.includes(key)) {
            setSelectedContexts([...selectedContexts, key]);
            setSelectedContextTypes([...selectedContextTypes, contextTypeId]);

            let vars = [];
            qualifiers.forEach(([qualifierName, qualifier]) => {
                if (qualifierName === "_meta_")
                    return;

                const variables = Object.entries(qualifier);
                variables.forEach(([varName, {dataType, dataTypeParameter}]) => {     
                    if (varName === "_meta_")               
                        return;
                        
                    vars.push({ 
                        variable: qualifierName + "::" + varName, 
                        dataType, 
                        dataTypeParameter 
                    });                
                });                
            }); 

            setContextVariables([ ...vars, ...contextVariables ]);           
        } else {
            /* remove the variables */
            setSelectedContexts(selectedContexts.filter(c => c !== key));
            setSelectedContextTypes(selectedContextTypes.filter(c => c !== contextTypeId));

            setContextVariables(
                contextVariables.filter(contextVariable => {
                    if (contextVariable.value)
                        return true;

                    for (const {qualifierName, qualifierDef} of Object.entries(qualifiers)) {                        
                        for (const [varName,] of Object.entries(qualifierDef)) {
                            if (contextVariable.variable === (qualifierName + "::" + varName))
                                return false;
                        }
                    }

                    return true;
                })
            );
        }      
        
        setMenuTarget();
    };

    return (
        <Dialog 
            open={open}
            onDismiss={onDismiss}
            showClose={true}
            isBlocking={false}
            title="Context Editor"
            maxWidth="lg"
            contains={ 
                <OperationalContext.Provider value={OperationalMode.Editable}>     
                    {
                        editSource ?
                            <div style={{ height: "70vh", overflow: "auto" }}>    
                                <ContextSourceEditor 
                                    contextVariables={contextVariables}
                                />
                            </div> :                               
                            <div style={{ height: "70vh", overflow: "auto" }}>    
                                <div style={{ display: "flex", alignItems: "center" }}>
                                    <Typography 
                                        size="1" 
                                        text="Edit the contextual variables available during evaluation." 
                                    />
                                    <Button 
                                        label="Add Context Variable"
                                        tooltip="Add Context Variable"
                                        type="icon"
                                        icon="add"
                                        color="primary"
                                        onClick={() => {
                                            let vars = [...contextVariables];
                                            vars.push({ variable: "", value: "" });
                                            setContextVariables(vars);
                                        }}
                                    />
                                    <Button 
                                        type="icon"
                                        label="Remove Context Variable"
                                        tooltip="Remove Context Variable"
                                        icon="remove"
                                        color="primary"
                                        disabled={selected.length === 0}
                                        onClick={() => {
                                            setContextVariables(
                                                contextVariables.filter(contextVariable => {
                                                    let found = false;
                                                    selected.forEach(s => {                                        
                                                        if (contextVariable.variable === s.variable)
                                                            found = true;
                                                    });

                                                    return !found;
                                                })
                                            );
                                        }}
                                    />       
                                </div>        
                                <Table 
                                    compact={true}
                                    rows={contextVariables}
                                    columns={[
                                        {
                                            key: "Variable",
                                            name: "Variable",
                                            minWidth: 100,
                                            maxWidth: 200,
                                            isResizable: true,
                                            onRender: (contextVar) => (
                                                // eslint-disable-next-line 
                                                <div onClick={e => e.stopPropagation()}>
                                                    <EditableLabel 
                                                        placeholder="[Name]"
                                                        value={contextVar.variable} 
                                                        onChanged={(variable) => {
                                                            contextVar.variable = variable;
                                                            setContextVariables([...contextVariables]);
                                                        }}
                                                    />
                                                </div>
                                            )
                                        },
                                        {
                                            key: "Value",
                                            name: "Value",
                                            width: "60%",
                                            minWidth: 100,
                                            maxWidth: 400,
                                            isResizable: true,
                                            onRender: (contextVar) => {                                        
                                                if (contextVar.dataType) {
                                                    const Component = layoutByDataType[contextVar.dataType];
                                                    if (Component) {
                                                        return (
                                                            // eslint-disable-next-line
                                                            <div onClick={e => e.stopPropagation()}>
                                                                <Component 
                                                                    value={contextVar.value}
                                                                    valueMeta={{
                                                                        dataType: contextVar.dataType,
                                                                        dataTypeParameter: contextVar.dataTypeParameter
                                                                    }}
                                                                    onChanged={value => {
                                                                        contextVar.value = value;
                                                                        setContextVariables([ ...contextVariables ]);
                                                                    }}
                                                                />
                                                            </div>
                                                        );
                                                    }
                                                }

                                                return (
                                                    // eslint-disable-next-line
                                                    <div onClick={e => e.stopPropagation()}>
                                                        <HybridEditor 
                                                            placeholder="[Value]"
                                                            value={contextVar.value} 
                                                            onChanged={(value) => {
                                                                contextVar.value = value;
                                                                setContextVariables([ ...contextVariables ]);
                                                            }}
                                                        />
                                                    </div>
                                                );
                                            }                                    
                                        },
                                    ]}
                                    onSelectionChanged={(selection) => {                            
                                        setSelected([...selection]);
                                    }}
                                />
                            </div>  
                    }
                </OperationalContext.Provider>
            }
            footer={
                <div style={{ display: "flex", justifyContent: "space-between", padding: theme.spacing(2), width: "100%" }}>
                    <div>
                        <Button                                                 
                            label="Context Types"
                            onClick={(event) => setMenuTarget(event.target)}                        
                            variant="text"                                              
                            icon="add"
                        />
                        {
                            editSource ?
                                <Button 
                                    label="Table"
                                    onClick={() => setEditorSource(false)}
                                    variant="text"
                                    icon="table"
                                /> :
                                <Button                                                 
                                    label="Source"  
                                    onClick={() => setEditorSource(true)}               
                                    variant="text"                                              
                                    icon="code"
                                />
                        }
                        <Menu      
                            open={Boolean(menuTarget)}                       
                            target={menuTarget}
                            onClose={() => setMenuTarget()}
                            items={
                                contextTypes ? Object.values(contextTypes).map((contextType, key) => {                                         
                                    const {contextTypeId, description} = contextType;                                    
                                    return ({
                                        key,
                                        label: contextTypeId,
                                        description,
                                        canCheck: true,  
                                        checked: (selectedContexts && selectedContexts.includes(key)),
                                        onClick: () => onSelectContext(key, contextType)
                                    });
                                }) : []
                            }
                        />
                    </div>
                    <div style={{ display: "flex" }}>
                        <div style={{ marginRight: theme.spacing(1) }}>
                            <Button 
                                color="primary"
                                label="Apply"                            
                                onClick={() => onSave(contextVariables, selectedContextTypes)}
                            />
                        </div>
                        <div style={{ marginRight: theme.spacing(1) }}>
                            <Button 
                                label="Cancel"
                                onClick={onDismiss}
                            />
                        </div>
                    </div>                    
                </div>
            }
        />
    );
};

export default ContextEditor;
