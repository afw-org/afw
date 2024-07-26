// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {
    Button,
    Checkbox,
    CustomVariables,
    Dialog,
    ObjectResponsive,
    PropertyResponsive,
    Picker,
    Responsive,
    Table,
    Tabs,
    TextField,
    Typography,
    useOperational,
    useForceUpdate,
    layoutByDataType
} from "@afw/react";

import {
    afwIndexList, 
    afwIndexCreate, 
    afwIndexRemove
} from "@afw/core";

import {useApplication, useTheme} from "../../hooks";

import {
    ServiceEditorGeneral, 
    ServiceEditorRuntime
} from "./ServiceEditor";

/**
 * ServiceNewAdapterLmdb
 * 
 * This is a common component, used by the ServiceNew stepper.  It provides a
 * layout for the General configuration for type=adapter, subtype=lmdb services.
 */
export const ServiceNewAdapterLmdb = (props) => {
    
    const confObject = props.confObject;
    if (!confObject)
        return null;             

    return (
        <div>
            <ObjectResponsive 
                object={confObject}
                property={confObject.getProperty("env")}      
                filterOptions={{
                    filterProperties: [
                        "env"
                    ]
                }}                  
            />
        </div>
    );
};


/**
 * ServiceEditorAdapterLmdbIndex
 * 
 * This component displays a modal to gather information for the 
 * creation of a new LMDB Index definition.
 */
export const ServiceEditorAdapterLmdbIndex = (props) => {    

    const [objectTypeObjects, setObjectTypeObjects] = useState();
    const [index, setIndex] = useState();
    const [value, setValue] = useState();
    const [filter, setFilter] = useState();
    const [caseInsensitive, setCaseInsensitive] = useState();
    const [unique, setUnique] = useState();
    const [retroactive, setRetroactive] = useState();
    const [objectType, ] = useState();

    const theme = useTheme();
    const forceUpdate = useForceUpdate();
    const {client} = useApplication();
    const ScriptComponent = layoutByDataType["script"];

    const {open, onDismiss} = props;

    const onAddIndex = async () => {
        const adapterId = props.serviceConf.getPropertyValue([ "conf", "adapterId" ]);

        let objectTypeObjects;
        if (objectType) {
            objectTypeObjects = [];

            objectTypeObjects.forEach((objectType) => {
                objectTypeObjects.push(objectType.key);
            });
        }

        let options;
        if (caseInsensitive || unique) {
            options = [];

            if (caseInsensitive)
                options.push("case-insensitive-string");
            if (unique)
                options.push("unique");
        }        

        // \fixme try/catch
        const {result} = afwIndexCreate(
            client, 
            index, 
            value, 
            objectTypeObjects, 
            adapterId, 
            filter, 
            options, 
            retroactive, 
            false
        );
        await result;

        /* \fixme trigger a refresh */
        forceUpdate();       
    };

    const onIndexValidate = () => {
        /*! \fixme */
        return true;
    };

    return(
        <Dialog 
            open={open}
            isBlocking={true}
            onDismiss={onDismiss}   
            title="Index Editor"
            maxWidth="lg"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <Responsive 
                        items={[                            
                            {
                                breakpoints: {
                                    large: 6,
                                },
                                contains:
                                    <div style={{ paddingBottom: theme.spacing(2) }}>
                                        <TextField 
                                            label="Index Name"           
                                            //eslint-disable-next-line
                                            autoFocus={true}                           
                                            required={true}                         
                                            description="The name of the index, that may be used in search query criteria."
                                            value={index}
                                            onValidate={onIndexValidate}
                                            onChanged={setIndex}
                                        />
                                    </div>
                            },
                            {
                                breakpoints: {},
                                contains:
                                    <Picker
                                        label="Object Types"
                                        description="Choose all applicable Object Types for this index, or omit to allow any Object Type to be applicable." 
                                        items={objectTypeObjects}
                                        onChange={setObjectTypeObjects}
                                    />
                            },                                
                            {
                                breakpoints: {
                                    large: 9,
                                },
                                contains:
                                    <div style={{ paddingBottom: theme.spacing(2) }}>
                                        <ScriptComponent 
                                            label="Filter"                                                                                                  
                                            description="An optional filter condition to determine if an index is applicable to a particular value."
                                            value={filter}                                    
                                            onChanged={setFilter}
                                        />
                                    </div>
                            },                                
                            {
                                breakpoints: {
                                    large: 9,
                                },
                                contains:
                                    <div style={{ paddingBottom: theme.spacing(2) }}>
                                        <ScriptComponent 
                                            label="Value"                                                                                                  
                                            description="An expression to compute the indexed value.  Omit this value to use the property value described by the index name."
                                            value={value}                                    
                                            onChanged={setValue}
                                        />
                                    </div>
                            },                                
                            {
                                breakpoints: {},
                                contains:
                                    <div>
                                        <div style={{ paddingBottom: theme.spacing(1) }}>
                                            <Checkbox
                                                label="Case Insensitive"            
                                                description="Indexed values will be compared case-insensitive."      
                                                onChanged={setCaseInsensitive}    
                                                value={caseInsensitive}              
                                            />
                                        </div>
                                        <div style={{ paddingBottom: theme.spacing(1) }}>
                                            <Checkbox
                                                label="Unique" 
                                                description="Indexed values must be unique for this index."  
                                                onChanged={setUnique}              
                                                value={unique}                                     
                                            />
                                        </div>
                                        <div>
                                            <Checkbox
                                                label="Retroactive" 
                                                description="Retroactively, index all current values in the database."  
                                                onChanged={setRetroactive}             
                                                value={retroactive}                                      
                                            />
                                        </div>
                                    </div> 
                            }
                        ]}
                    />
                </div>   
            }
            footer={
                <div style={{ display: "flex", padding: theme.spacing(2) }}>
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            label="Ok"
                            color="primary"
                            variant="contained"
                            onClick={onAddIndex}
                            disabled={!index}
                        />
                    </div>
                    <div>
                        <Button 
                            label="Cancel"
                            onClick={onDismiss}
                        />
                    </div>                                                
                </div>   
            }
        /> 
    );
};

/**
 * ServiceEditorAdapterLmdb
 * 
 * Main component for displaying the editor for a LMDB Service.
 */
export const ServiceEditorAdapterLmdb = (props) => {

    const [showModal, setShowModal] = useState(false);
    const [indexes, setIndexes] = useState();

    const theme = useTheme();
    const {client} = useApplication();
    const {editable} = useOperational(props);

    const {service, serviceConf, serviceUriRelated, confTypes} = props;

    useEffect(() => {
        const loadIndexes = async () => {
            const adapterId = serviceConf.getPropertyValue([ "conf", "adapterId" ]);
            
            const indexDefinitions = await afwIndexList(client, adapterId).result();            
            const indexes = [];
            for (let [key, def] of Object.entries(indexDefinitions)) {
                indexes.push({...def, key});
            }

            setIndexes(indexes);         
        };

        if (serviceConf)
            loadIndexes();

    }, [serviceConf, client]);

   
    const loadIndexes = async () => {
        if (serviceConf) {
            const adapterId = serviceConf.getPropertyValue([ "conf", "adapterId" ]);
            
            const indexDefinitions = await afwIndexList(client, adapterId).result();            
            const indexes = [];
            for (let [key, def] of Object.entries(indexDefinitions)) {
                indexes.push({...def, key});
            }

            setIndexes(indexes);
        }
    };

    const removeIndex = async (key) => {
        const adapterId = serviceConf.getPropertyValue([ "conf", "adapterId" ]);
        await afwIndexRemove(client, key, adapterId).result();        
        
        await loadIndexes();
    };      
       
    if (!service || !serviceConf || !confTypes)
        return null;

    const conf = serviceConf.getPropertyValue("conf");
    if (!conf)
        return null;

    const {type, environment, limits} = conf.getPropertyValues();
    const {idPropertyName, subtypePropertyName} = confTypes[type];

    const runtime = editable ? [] :
        ([{
            text: "Runtime",
            contains: 
                <ServiceEditorRuntime 
                    service={service}
                    serviceUriRelated={serviceUriRelated}
                />
        }]);

    return (            
        <div>
            <Tabs 
                gapSpace={20}
                tabs={[
                    {
                        text: "General",
                        contains: 
                            <ServiceEditorGeneral 
                                service={service}                                    
                                serviceConf={serviceConf}
                                idPropertyName={idPropertyName}
                                subtypePropertyName={subtypePropertyName}
                            />
                    },
                    {
                        text: "Configuration",                                
                        contains:
                            <div style={{ padding: theme.spacing(2) }}>                                    
                                <Typography size="7" text="General" />
                                <Responsive 
                                    spacing={2}
                                    items={[                                        
                                        {
                                            breakpoints: {
                                                small: 12,
                                            },
                                            contains:
                                                <PropertyResponsive 
                                                    property={conf.getProperty("description")}                                  
                                                /> 
                                        },                                          
                                        {
                                            breakpoints: {
                                                small: 12,
                                            },
                                            contains:
                                                <PropertyResponsive 
                                                    property={conf.getProperty("authorizationHandlerId")}                                                                
                                                /> 
                                        },
                                        {
                                            breakpoints: {
                                                small: 12,
                                            },
                                            contains:
                                                <PropertyResponsive 
                                                    property={conf.getProperty("journalAdapterId")}                                                             
                                                />
                                        }                            
                                    ]}
                                /> 
                                <div style={{ height: theme.spacing(5) }} />
                                <Typography size="7" text="Environment" />
                                <Responsive                                     
                                    items={[                                        
                                        {
                                            breakpoints: {
                                                small: 12,                                                
                                            },
                                            contains:
                                                <ObjectResponsive 
                                                    object={environment}
                                                    filterOptions={{
                                                        filterProperties: [
                                                            "path", 
                                                            "mode", 
                                                            "maxreaders", 
                                                            "maxdbs", 
                                                            "mapsize"
                                                        ]
                                                    }}                                                                                                                                                                                                               
                                                />
                                        },  
                                    ]}
                                />   
                                <div style={{ height: theme.spacing(5) }} />
                                <Typography size="7" text="Limits" />
                                <Responsive                                     
                                    items={[                                        
                                        {
                                            breakpoints: {
                                                small: 12,
                                            },
                                            contains:
                                                <ObjectResponsive 
                                                    object={limits} 
                                                    filterOptions={{
                                                        filterProperties: [
                                                            "size", 
                                                            "time",                                                                         
                                                        ],
                                                        recurse: false
                                                    }}                                                                                                                                                                                                                
                                                />
                                        }, 
                                    ]}
                                />                         
                            </div>
                    },                        
                    {
                        text: "Indexes",
                        contains: 
                            <Responsive                                     
                                items={[                                   
                                    {
                                        breakpoints: {
                                            small: 12,                                                        
                                        },
                                        contains:
                                            indexes ? 
                                                <div>
                                                    <div style={{ maxHeight: "50vh", overflow: "auto" }}>
                                                        <Table 
                                                            rows={indexes}
                                                            columns={[
                                                                { key: "key", name: "Key", fieldName: "key", isResizable: true },
                                                                { key: "value", name: "Value", fieldName: "value", isResizable: true },
                                                                { key: "objectType", name: "ObjectTypes", fieldName: "objectType", isResizable: true },
                                                                { key: "filter", name: "Filter", fieldName: "filter", isResizable: true, minWidth: 250 },
                                                                { key: "options", name: "Options", fieldName: "options", isResizable: true, minWidth: 200 },
                                                            ]}
                                                            selectionMode="multiple"
                                                        />
                                                    </div>
                                                    <div style={{ display: "flex", paddingTop: theme.spacing(2) }}>
                                                        <div style={{ marginRight: theme.spacing(1) }}>
                                                            <Button                                                             
                                                                label="Create"      
                                                                size="small"                                                                                                                      
                                                                icon="add"
                                                                onClick={() => setShowModal(true)}
                                                            />
                                                        </div>
                                                        <div>
                                                            <Button 
                                                                label="Remove"
                                                                size="small"                                                                
                                                                icon="delete"
                                                                disabled={true}
                                                                onClick={() => removeIndex()}
                                                            />
                                                        </div>
                                                    </div>
                                                </div>
                                                :
                                                null
                                    },
                                ]}
                            />
                    },
                    ...runtime,
                    {
                        ...(
                            conf.getProperty("custom") ? 
                                {
                                    text: "Custom",
                                    contains:
                                        <CustomVariables 
                                            custom={conf.getProperty("custom")}
                                        />
                                } : {}
                        )
                    }
                ]}
            />                
            <ServiceEditorAdapterLmdbIndex 
                {...props}
                open={showModal}
                onDismiss={() => setShowModal(false)}
            />
        </div>                            
    );
};

export default ServiceEditorAdapterLmdb;
