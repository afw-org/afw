// See the 'COPYING' file in the project root for licensing information.
import { useState } from "react";

import {
    CustomVariables,
    Link,
    ObjectResponsive,
    Table,    
    Tabs,
    Typography,    
    useValues,
    useOperational
} from "@afw/react";

import {useTheme} from "../../../hooks";
import InheritanceTree from "../../../common/InheritanceTree";

import ModelObjectTypesTable from "./ModelObjectTypesTable";
import ModelPropertyTypesTable from "./ModelPropertyTypesTable";

export const ModelOverviewEditable = (props) => {
    
    const theme = useTheme();

    const [selectedTab, setSelectedTab] = useState(0);
    const {model} = props;

    const {objectTypes, propertyTypes} = model.getPropertyValues();
    const custom = model.getProperty("custom");

    return (
        <div data-testid="admin-admin-models-overview">
            <Tabs                 
                selectedTab={selectedTab}
                onTabSwitch={(tab, selectedTab) => setSelectedTab(selectedTab)}
                tabs={[                    
                    {
                        key: "general",
                        text: "General",
                        contains: 
                            <div style={{ paddingTop: theme.spacing(2) }}>
                                <ObjectResponsive 
                                    object={model}
                                    filterOptions={{
                                        filterProperties: [
                                            "modelId", "description", "collectionURIs", "originURI"
                                        ]
                                    }}
                                    breakpoints={{
                                        small: 12,
                                        large: 6
                                    }}
                                />
                            </div>
                    },
                    {
                        key: "objectTypes",
                        text: "Object Types",
                        badge: objectTypes.getProperties() ? objectTypes.getProperties().length : 0,
                        contains:                                          
                            <ModelObjectTypesTable 
                                {...props}
                                objectTypes={objectTypes}
                            />
                    },
                    {
                        key: "propertyTypes",
                        text: "Property Types",
                        contains:                             
                            <ModelPropertyTypesTable 
                                {...props}
                                propertyTypes={propertyTypes}
                            />
                    },                                      
                    {
                        key: "custom",
                        text: "Custom",
                        contains: 
                            <CustomVariables                                     
                                custom={custom}    
                            />
                    }
                ]}
            />
        </div>
    );
};

/**
 * ModelOverview
 * 
 * This component displays a read-only summary of a Model, including
 * its Description, Object Type definitions, and Custom Variables.
 */
export const ModelOverviewReadonly = (props) => {
    
    const {adapterId, model, models} = props;

    const theme = useTheme();
    const {custom, modelId, objectTypes, description} = useValues(model);
    const {editable} = useOperational();

    const objectTypesSorted = objectTypes ? objectTypes.getProperties().map(property => property.getName()).sort((A, B) => {
        return (A.toLowerCase().localeCompare(B.toLowerCase()));
    }) : [];

    return (
        <div data-testid="admin-admin-models-overview"> 
            <div style={{ height: theme.spacing(2) }} />
            <Typography 
                size="8"
                text={modelId ? modelId : ""}                
            />              
            <div style={{ height: theme.spacing(2) }} /> 
            <Typography 
                size="3"
                color="textSecondary"
                text={description ? description : ""}                                    
            />
            <div style={{ height: theme.spacing(2) }} />
            <Typography 
                size="6"
                text="Inheritance"                                    
            />
            <div style={{ height: theme.spacing(1) }} />
            <InheritanceTree 
                style={{ marginLeft: theme.spacing(1) }}
                object={model}
                objects={models}
                onRenderPath={(path) => {
                    const paths = path.split("/");

                    if (path === model.getPath())
                        return (
                            <Typography color="textSecondary" text={decodeURIComponent(paths[paths.length -1])} />
                        );
                    else
                        return (
                            <Link 
                                url={"/Admin/Models/" + model.getAdapterId() + "/" + paths[paths.length - 1]} 
                                text={decodeURIComponent(paths[paths.length - 1])} 
                            />
                        );
                }}
            />
            <div style={{ height: theme.spacing(3) }} />
            <ObjectResponsive 
                object={model}
                filterOptions={{
                    filterProperties: [
                        "originURI",
                        "collectionURIs"
                    ],
                    filterValuedProperties: (editable !== true)
                }}
            />
            <div style={{ height: theme.spacing(3) }} />
            {
                (objectTypesSorted && objectTypesSorted.length) > 0 ?
                    <>
                        <Typography 
                            size="6"
                            text="Object Types"
                        />             
                        <Table 
                            aria-label="Object Types"
                            rows={objectTypesSorted}
                            columns={[
                                {
                                    key: "ObjectType",
                                    name: "Object Type",
                                    minWidth: 100,
                                    maxWidth: 200,
                                    isResizable: true,
                                    onRender: (objectType) => 
                                        <Link url={"/Admin/Models/" + adapterId + "/" + modelId + "/objectTypes/" + objectType + "#overview"} text={objectType} />                            
                                },
                                {
                                    key: "Description",
                                    name: "Description",
                                    minWidth: 200,
                                    isResizable: true,
                                    isMultiline: true,
                                    onRender: (objectType) => 
                                        <Typography 
                                            text={objectTypes.getPropertyValue([objectType, "description"])}
                                        />
                                }                        
                            ]}
                            selectionMode="none"
                        />
                    </> :
                    <Typography
                        size="6"
                        text="No Object Types Defined"
                    />
            }                
            <div style={{ height: theme.spacing(5) }} />
            {
                (custom && custom.getProperties().length > 0) ?
                    <>
                        <Typography 
                            size="6"
                            text="Custom Variables"                    
                        />
                        <Table 
                            aria-label="Custom Variables"
                            compact={true}
                            rows={custom.getProperties().map(property => property.getName())}
                            columns={[
                                {
                                    key: "Variable",
                                    name: "Variable",
                                    onRender: (variable) => <Typography text={variable} />,     
                                    minWidth: 150,
                                    maxWidth: 200,
                                    isResizable: true,
                                    width: "20%"                       
                                }
                            ]}
                            selectionMode="none"
                        />
                    </> :
                    <Typography 
                        size="6"
                        text="No Custom Variables Defined"
                    />
            }         
            <div style={{ height: theme.spacing(3) }} />
        </div>
    );
};

export const ModelOverview = (props) => {

    const {editable} = useOperational(props);

    if (editable)
        return <ModelOverviewEditable {...props} />;
    else 
        return <ModelOverviewReadonly {...props} />;
};

export default ModelOverview;
