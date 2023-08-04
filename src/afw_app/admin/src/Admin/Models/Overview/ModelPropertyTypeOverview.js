// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {
    CustomVariables,
    Divider,
    ObjectResponsive,
    Tabs,
    Typography,
    useOperational,
    useValues
} from "@afw/react";

import {useTheme} from "../../../hooks";

const generalProperties = [
    "label",
    "description",
    "dataType",
    "dataTypeParameter",
    "testDataParameter",
    "contextType",
    "tags",
    "hints",
];

const computedProperties = [
    "onGetInitialValue",
    "defaultValue",
    "mappedPropertyName",
    "onGetProperty",
    "onSetProperty",
    "method",
];

const constraintsProperties = [
    "allowRead", 
    "allowWrite", 
    "allowQuery", 
    "required", 
    "unique", 
    "transitory",
    "allowedValues", 
    "possibleValues",
    "minValue", 
    "maxValue", 
    "minLength",
    "maxLength", 
    "maxNormalLength", 
    "additionalConstraints"
];

export const ModelPropertyTypeOverviewEditable = (props) => {

    const [selectedTab, setSelectedTab] = useState();   
    const theme = useTheme();

    const {propertyTypeObject} = props;
    const custom = propertyTypeObject.getProperty("custom");

    return (
        <Tabs 
            selectedTab={selectedTab}
            onTabSwitch={(tab, selectedTab) => setSelectedTab(selectedTab)}
            tabs={[
                {
                    text: "General",
                    contains:
                        <div style={{ paddingTop: theme.spacing(2) }}>
                            <ObjectResponsive 
                                object={propertyTypeObject}
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
                    text: "Computed",
                    contains: 
                        <div style={{ paddingTop: theme.spacing(2) }}>
                            <ObjectResponsive 
                                object={propertyTypeObject}
                                filterOptions={{
                                    filterProperties: computedProperties
                                }}
                                breakpoints={{
                                    small: 12,
                                    large: 6
                                }}
                            />
                        </div>
                },
                {
                    text: "Constraints",
                    contains: 
                    <div style={{ paddingTop: theme.spacing(2) }}>
                        <ObjectResponsive 
                            object={propertyTypeObject}
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
                    text: "Custom",
                    contains: 
                        <CustomVariables custom={custom} />
                }
            ]}
        />
    );
};

export const ModelPropertyTypeOverviewReadonly = (props) => {
        
    const {propertyType, propertyTypeObject, filterExcludeProperties = []} = props;
    const theme = useTheme();
    const {editable} = useOperational(props);
    const {description} = useValues(propertyTypeObject);

    /* determine if this propertyType has any computed properties */
    let hasComputed = false;
    if (propertyTypeObject) {
        for (const p of computedProperties)
            if (propertyTypeObject.getPropertyValue(p))
                hasComputed = true;
    }

    return (
        <div>
            <div style={{ height: theme.spacing(2) }} />
            <Typography 
                size="8"
                text={propertyType}
            />
            <div style={{ height: theme.spacing(2) }} /> 
            <Typography 
                text={description}
                color="textSecondary"
                size="3"
            />
            <div style={{ height: theme.spacing(5) }} />     
            <Typography 
                size="6"
                text="Type Information"
            />
            <div style={{ height: theme.spacing(2) }} />
            <ObjectResponsive 
                spacing={theme.spacing(1)}
                object={propertyTypeObject}
                filterOptions={{
                    filterValuedProperties: editable ? false : true,                    
                    filterProperties: [
                        "dataType", "dataTypeParameter", "testDataParameter",
                        "hints", "tags"
                    ].filter(e => !filterExcludeProperties.includes(e))
                }}
            /> 
            <div style={{ height: theme.spacing(1) }} />               
            <Divider />
            <div style={{ height: theme.spacing(1) }} />
            <Typography 
                size="6"
                text="Restrictions and Constraints"
            />
            <div style={{ height: theme.spacing(2) }} />
            <ObjectResponsive 
                spacing={theme.spacing(1)}
                object={propertyTypeObject}
                filterOptions={{
                    filterValuedProperties: editable ? false : true,
                    filterProperties: constraintsProperties.filter(e => !filterExcludeProperties.includes(e))
                }}
            />     
            <div style={{ height: theme.spacing(1) }} />
            <Divider />                 
            <div style={{ height: theme.spacing(1) }} />
            <Typography 
                size="6"
                text="Computed Values"
            />
            <div style={{ height: theme.spacing(2) }} />
            {
                (hasComputed === false) &&
                    <Typography 
                        text="This Property Type has no computed values."
                        size="3"
                        color="textSecondary"
                    />
            }
            <ObjectResponsive 
                spacing={theme.spacing(1)}
                object={propertyTypeObject}
                filterOptions={{
                    filterValuedProperties: editable ? false : true,
                    filterProperties: computedProperties.filter(e => !filterExcludeProperties.includes(e))
                }}
            />                                   
        </div>
    );
};

export const ModelPropertyTypeOverview = (props) => {

    const {editable} = useOperational(props);

    if (editable)
        return <ModelPropertyTypeOverviewEditable {...props} />;
    else 
        return <ModelPropertyTypeOverviewReadonly {...props} />;
};

export default ModelPropertyTypeOverview;
