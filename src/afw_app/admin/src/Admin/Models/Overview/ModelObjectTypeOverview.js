// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    CustomVariables,
    Link,
    ObjectResponsive,
    Table,
    Tabs,
    Tooltip,
    Typography,
    useOperational
} from "@afw/react";

import ModelPropertyTypesTable from "./ModelPropertyTypesTable";
import InheritanceTree from "../../../common/InheritanceTree";
import {useTheme} from "../../../hooks";

const generalProperties = [
    "description", 
    "collectionURIs", 
    "originURI",
    "objectIdPropertyName", 
    "descriptionPropertyName",
    "tags",
];

const constraintsProperties = [
    "allowAdd",
    "allowChange",
    "allowDelete",
    "allowEntity",
];

const computedProperties = [
    "onGetInitialObjectId", 
    "mappedObjectType",
    "onAddObject", 
    "onDeleteObject", 
    "onGetObject", 
    "onModifyObject", 
    "onReplaceObject", 
    "onRetrieveObjects",
];

const ModelCustomVariablesReadonly = ({ custom }) => {

    const theme = useTheme();

    if (custom) {
        return ( 
            <>
                <div style={{ height: theme.spacing(5) }} />
                <Typography 
                    size="6"
                    text="Custom Variables"
                />
                <div style={{ height: theme.spacing(2) }} />
                <Table 
                    aria-label="Custom Variables"
                    compact={true}
                    rows={custom.getProperties().map(property => property.getName())}
                    columns={[
                        {
                            key: "Variable",
                            name: "Variable",
                            onRender: (variable) => (
                                <Tooltip 
                                    content={custom.getPropertyValue(variable)}
                                    delay={500}
                                    target={
                                        <div>
                                            <Typography text={variable} />
                                        </div>
                                    }
                                />                                            
                            ),
                            minWidth: 100,
                            maxWidth: 200,
                            isResizable: true,
                            width: "20%"
                        }
                    ]}
                    selectionMode="none"
                />
            </>
        );
    } else return null;
};

export const ModelObjectTypeOverviewEditable = (props) => {

    const [selectedTab, setSelectedTab] = useState(0);   
    const theme = useTheme();

    const {objectTypeObject} = props;
    const {propertyTypes} = objectTypeObject.getPropertyValues();
    const custom = objectTypeObject.getProperty("custom");

    return (
        <Tabs 
            selectedTab={selectedTab}
            onTabSwitch={(tab, selectedTab) => setSelectedTab(selectedTab)}
            tabs={[
                {
                    key: "General",
                    text: "General",
                    index: 0,
                    contains:
                        <div style={{ paddingTop: theme.spacing(2) }}>
                            <ObjectResponsive                                 
                                object={objectTypeObject}
                                filterOptions={{
                                    filterProperties: generalProperties
                                }}
                                breakpoints={{
                                    small: 12,
                                    large: 9,
                                    xl: 6
                                }}
                            />
                        </div>
                },
                {
                    key: "Properties",
                    text: "Properties",
                    index: 1,
                    badge: propertyTypes?.getProperties() ? propertyTypes.getProperties().length : 0,
                    contains:                             
                        <ModelPropertyTypesTable 
                            {...props}
                            objectTypeObject={objectTypeObject}
                            propertyTypes={propertyTypes}
                        />                            
                },
                {
                    key: "Constraints",
                    text: "Constraints",
                    index: 2,
                    contains: 
                        <div style={{ paddingTop: theme.spacing(2) }}>
                            <ObjectResponsive 
                                object={objectTypeObject}
                                filterOptions={{
                                    filterProperties: constraintsProperties
                                }}
                                breakpoints={{
                                    small: 12,
                                    large: 6
                                }}
                            />
                        </div>
                },
                {
                    key: "Computed",
                    text: "Computed",
                    index: 3,
                    contains: 
                        <div style={{ paddingTop: theme.spacing(2) }}>
                            <ObjectResponsive 
                                object={objectTypeObject}
                                filterOptions={{
                                    filterProperties: computedProperties
                                }}
                                breakpoints={{
                                    small: 12,
                                    large: 9,
                                    xl: 6
                                }}
                            />
                        </div>
                },                                       
                {
                    key: "Custom",
                    text: "Custom",
                    index: 4,
                    contains: 
                        <CustomVariables 
                            custom={custom}
                        />
                }
            ]}
        />
    );
};

const ModelPropertyTypesTableReadonly = (props) => {

    const { 
        adaptorId, 
        modelId, 
        objectType,
        propertyTypes,
        propertyTypesSorted
    } = props;

    const theme = useTheme();

    return (
        <Table 
            aria-label="Property Types"
            rows={propertyTypesSorted.filter(() => {                        
                //if (inherited === false && propertyTypes.getPropertyValue(prop).getInheritedFrom())
                //    return false;                        

                return true;
            })}
            columns={[
                {
                    key: "PropertyType",
                    name: "Property Type",
                    minWidth: 100,
                    maxWidth: 200,
                    isResizable: true,
                    onRender: (propertyType) => 
                        <Link 
                            url={"/Admin/Models/" + adaptorId + "/" + modelId + "/objectTypes/" + objectType + "/propertyTypes/" + propertyType + "#overview"} 
                            text={propertyType} 
                        />                            
                },
                {
                    key: "DataType",
                    name: "Data Type",
                    minWidth: 100,
                    maxWidth: 200,
                    isResizable: true,
                    onRender: (propertyType) => {
                        const dataType = propertyTypes.getPropertyValue([propertyType, "dataType"]);
                        const dataTypeParameter = propertyTypes.getPropertyValue([propertyType, "dataTypeParameter"]);

                        if (dataType === "object" && dataTypeParameter) {
                            return (
                                <Link 
                                    url={"/Admin/Models/" + adaptorId + "/" + modelId + "/objectTypes/" + dataTypeParameter + "#overview"}
                                    text={dataTypeParameter}
                                />
                            );
                        } else if (dataType === "objectId" && dataTypeParameter) {
                            return (
                                <div style={{ display: "flex" }}>
                                    <Link 
                                        style={{ marginRight: theme.spacing(1) }}
                                        url={"/Documentation/Reference/DataTypes/" + dataType}
                                        text={dataType}
                                    />
                                    <Link 
                                        url={"/Admin/Models/" + adaptorId + "/" + modelId + "/objectTypes/" + dataTypeParameter + "#overview"}
                                        text={" (" + dataTypeParameter + ")"}
                                    />
                                </div>
                            );                                
                        } else if (dataTypeParameter) {
                            return (
                                <div style={{ display: "flex" }}>
                                    <Link 
                                        style={{ marginRight: theme.spacing(1) }}
                                        url={"/Documentation/Reference/DataTypes/" + dataType}
                                        text={dataType}
                                    />
                                    <Link 
                                        url={"/Documentation/Reference/DataTypes/" + dataTypeParameter}
                                        text={" (" + dataTypeParameter + ")"}
                                    />
                                </div>
                            );
                        } else {
                            return (
                                <Link 
                                    url={"/Documentation/Reference/DataTypes/" + dataType}
                                    text={dataType}
                                />
                            );                                
                        }
                    }                                
                },                        
                {
                    key: "Description",
                    name: "Description",
                    minWidth: 200,
                    maxWidth: 600,
                    isResizable: true,
                    isMultiline: true,
                    onRender: (propertyType) => 
                        <Typography 
                            text={propertyTypes.getPropertyValue([propertyType, "description"]) || ""}
                        />
                } 
            ]}
            selectionMode="none"
        />
    );
};

export const ModelObjectTypeOverviewReadonly = (props) => {

    const {adaptorId, model, objectType, objectTypeObject} = props;
    const {editable} = useOperational(props);
    const theme = useTheme();

    /* turn into an array */
    const {modelId, objectTypes} = model.getPropertyValues();
    
    const propertyTypes = objectTypeObject.getPropertyValue("propertyTypes");
    const propertyTypesSorted = propertyTypes ? propertyTypes.getProperties().map(p => p.getName()).sort((A, B) => {
        return (A.toLowerCase().localeCompare(B.toLowerCase()));
    }) : [];

    const custom = objectTypeObject.getPropertyValue("custom");

    /* determine if this objectType has any computed properties */
    let hasComputed = objectTypeObject ? 
        computedProperties.some(p => 
            objectTypeObject.getPropertyValue(p)
        ) : false;

    return (
        <div>
            <div style={{ height: theme.spacing(2) }} />
            <Typography 
                size="8"
                text={objectType}
            />
            <div style={{ height: theme.spacing(2) }} />
            <Typography 
                text={objectTypeObject.getPropertyValue("description")}
                size="3"
                color="textSecondary"
            />
            <div style={{ height: theme.spacing(2) }} />
            <Typography 
                size="6"
                text="Inheritance"
            />
            <div style={{ height: theme.spacing(1) }} />
            <InheritanceTree 
                style={{ marginLeft: theme.spacing(2) }}
                object={objectTypeObject}
                objects={Object.values(objectTypes.getPropertyValues())}
                onRenderPath={(path) => {
                    const paths = path.split("/");

                    if (path === objectTypeObject.getPath())
                        return (
                            <Typography text={paths[paths.length -1]} color="textSecondary" />
                        );
                    else
                        return (
                            <Link 
                                url={"/Admin/Models/" + adaptorId + "/" + modelId + "/objectTypes/" + paths[paths.length - 1] + "#overview"} 
                                text={paths[paths.length - 1]} 
                            />
                        );
                }}
            />
            <div style={{ height: theme.spacing(5) }} />
            <Typography 
                size="6"
                text="Property Types"
            />
            <ModelPropertyTypesTableReadonly 
                adaptorId={adaptorId}
                modelId={modelId}
                objectType={objectType}
                propertyTypes={propertyTypes}
                propertyTypesSorted={propertyTypesSorted}
            />
            <ModelCustomVariablesReadonly custom={custom} /> 
            <>
                <div style={{ height: theme.spacing(5) }} />
                <Typography 
                    size="6"
                    text="Computed"
                />
                <div style={{ height: theme.spacing(2) }} />   
                {
                    (hasComputed === false) &&
                            <Typography 
                                text="This Object Type has no computed values."
                                size="3"
                                color="textSecondary"
                            />
                }   
                <ObjectResponsive 
                    object={objectTypeObject}
                    filterOptions={{
                        filterValuedProperties: (editable ? false : true),
                        filterProperties: computedProperties
                    }}
                />
            </>
            <div style={{ height: theme.spacing(5) }} />
        </div>
    );
};

export const ModelObjectTypeOverview = (props) => {

    const {editable} = useOperational(props);

    if (editable)
        return <ModelObjectTypeOverviewEditable {...props} />;
    else 
        return <ModelObjectTypeOverviewReadonly {...props} />;
};

export default ModelObjectTypeOverview;
