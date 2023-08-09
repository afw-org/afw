// See the 'COPYING' file in the project root for licensing information.
/**
 * registry.js
 * 
 * This provides an anchor for built-in registered component layouts and allows
 * us to map objects and properties to the appropriate layouts found in the registry.
 */

import {isArray, isFunction, isString} from "../utils";
import {OperationalMode} from "../context";

/* dataTypes */
import {
    AnyURI,
    Base64Binary,
    _Boolean,
    _Date,
    DateTime,
    DayTimeDuration,
    DnsName,
    Double,
    Expression,
    _Function,
    HexBinary,
    Hybrid,
    Ia5String,
    Integer,
    IpAddress,
    List,
    Null,
    _Object,
    ObjectId,
    ObjectPath,
    Password,
    Rfc822Name,
    Script,
    _String,
    Template,
    Time,
    Undefined,
    X500Name,
    XpathExpression,
    YearMonthDuration
} from "./dataTypes";

/* propertyTypes */
import {
    AdaptorDropdown,
    CustomVariables,
    DataTypeDropdown,
    ObjectTypeDropdown,
    PropertyTypeDropdown,
    FlagsSelector,
    DataTypeParameterDropdown,
    QualifiedVariables,
    AllowableDropdown,
    PossibleAutocomplete,
} from "./propertyTypes";

/* objectTypes */
import {
    AdaptiveDataType,
    AdaptiveFunction,
    AdaptiveFunctionGenerate,
    AdaptiveFunctionParameter,
    AdaptiveMeta,
    AdaptiveObjectType,    
    AdaptiveQualifierDefinitions,
    AdaptiveValueMeta,    
    ObjectResponsive,
    AdaptiveHybridPropertiesObjects,
} from "./objectTypes";

/* components that can be derived by their dataType only */
export const layoutByDataType = {    
    anyURI:                     AnyURI,
    base64Binary:               Base64Binary,
    boolean:                    _Boolean,
    date:                       _Date,    
    dateTime:                   DateTime,
    dayTimeDuration:            DayTimeDuration,
    dnsName:                    DnsName,
    double:                     Double,    
    expression:                 Expression,
    function:                   _Function,
    hexBinary:                  HexBinary,
    hybrid:                     Hybrid,
    ia5String:                  Ia5String,
    integer:                    Integer,  
    ipAddress:                  IpAddress,      
    array:                      List,
    null:                       Null,
    object:                     _Object,
    password:                   Password,    
    objectId:                   ObjectId,
    objectPath:                 ObjectPath,    
    rfc822Name:                 Rfc822Name,
    script:                     Script,
    string:                     _String,    
    template:                   Template,  
    time:                       Time,  
    undefined:                  Undefined,        
    x500Name:                   X500Name,
    xpathExpression:            XpathExpression,
    yearMonthDuration:          YearMonthDuration,
};

/* computed data type properties are those whose dataTypes are determined by their embedded object's "dataType" property */
const isComputedDataType = (propertyName) => [
    "defaultValue",
    "minValue",
    "maxValue",
    "skeleton",
].includes(propertyName);


const isAdaptiveObjectType = (objectTypeId) => {
    return objectTypeId?.startsWith("_Adaptive");
};

/**
 * propertyLayoutRegistry
 * 
 * This is the default built-in registry for layouts.  It contains all compiled in 
 * metadata-handlers that will render basic layouts for primitive dataTypes, some 
 * common propertyTypes and some objectTypes.
 * 
 * Each entry may contain the component or componentFunc to render the layout, and 
 * optional matching criteria to help the layout registry selection process determine
 * which layout to render for a particular object or property.
 */
const propertyLayoutRegistry = [

    /* properties whose value references an adaptorId */
    {
        component: AdaptorDropdown,
        objectTypeId: isAdaptiveObjectType,
        //propertyName: [ "adaptorId", "layoutsAdaptorId", "defaultModelAdaptorId", "defaultAdaptorId" ],
        propertyName: "adaptorId",
        operational: OperationalMode.Editable,
        dataType: "string"
    },

    /* properties whose values use "custom" variables */
    {
        component: CustomVariables,
        matches: () => false
    },

    /* properties that reference an _AdaptiveFlag_ value */
    {
        component: FlagsSelector,
        matches: () => false
    },

    /* properties that reference a list of dataType names */
    {
        component: DataTypeDropdown,
        objectTypeId: isAdaptiveObjectType,
        propertyName: "dataType",
        operational: OperationalMode.Editable,
        dataType: "string"
    },

    /* properties that reference a list of dataTypeParameter names */
    {
        component: DataTypeParameterDropdown,
        objectTypeId: isAdaptiveObjectType,
        propertyName: "dataTypeParameter",
        operational: OperationalMode.Editable
    },

    /* properties that reference a list of objectType names */
    {
        component: ObjectTypeDropdown,
        objectTypeId: (objectTypeId) => isAdaptiveObjectType(objectTypeId) && objectTypeId !== "_AdaptiveObjectType_",
        propertyName: "objectType",
        operational: OperationalMode.Editable,
        dataType: "string"
    },

    /* properties that reference a list of propertyType names */
    {
        component: PropertyTypeDropdown,
        objectTypeId: isAdaptiveObjectType,
        propertyName: "propertyType",
        operational: OperationalMode.Editable,
        dataType: "string"
    },
    
    /* _AdaptiveHybridPropertiesObjects_ property */
    {
        component: QualifiedVariables,
        objectTypeId: isAdaptiveObjectType,
        propertyName: "qualifiedVariables",
        operational: OperationalMode.NotEditable,
        dataType: "object",
        dataTypeParameter: "_AdaptiveHybridPropertiesObjects_"
    },

    /* properties with allowedValues */
    {
        component: AllowableDropdown,
        operational: OperationalMode.Editable,
        matches: ({ property }) => property.getAllowedValues()
    },

    /* properties with possibleValues */
    {
        component: PossibleAutocomplete,
        operational: OperationalMode.Editable,
        matches: ({ property }) => property.getPossibleValues()
    },

    /* properties with property values whose dataTypes are computed */
    {
        componentFunc: ({ property }) => {
            const embeddingObject = property.getEmbeddingObject();
            if (embeddingObject)
                return layoutByDataType[ embeddingObject.getPropertyValue("dataType") ];
        },
        objectTypeId: isAdaptiveObjectType,
        matches: ({ property }) => isComputedDataType( property.getName() )
    },

    /* default component to use is determined by the dataType */
    {
        componentFunc: ({ property }) => layoutByDataType[ property.getDataType() ],
        matches: () => true
    }
];

/**
 * objectLayoutRegistry
 * 
 * This table determines the appropriate layout for an object using the 
 * objectTypeId as a filter.  A custom "matches" and any operational 
 * properties can also be used to match a layout.
 */
const objectLayoutRegistry = [

    {
        component: AdaptiveDataType,
        objectTypeId: "_AdaptiveDataType_"
    },

    {
        component: AdaptiveFunction,
        objectTypeId: "_AdaptiveFunction_"
    },

    {
        component: AdaptiveFunctionGenerate,
        objectTypeId: "_AdaptiveFunctionGenerate_"
    },

    {
        component: AdaptiveFunctionParameter,
        objectTypeId: "_AdaptiveFunctionParameter_"
    },

    {
        component: AdaptiveMeta,
        objectTypeId: "_AdaptiveMeta_"
    },

    {
        component: AdaptiveObjectType,
        objectTypeId: "_AdaptiveObjectType_"
    },

    {
        component: AdaptiveQualifierDefinitions,
        objectTypeId: "_AdaptiveQualifierDefinitions_"
    },

    {
        component: AdaptiveValueMeta,
        objectTypeId: "_AdaptiveValueMeta_"
    },

    {
        component: AdaptiveHybridPropertiesObjects,
        objectTypeId: "_AdaptiveHybridPropertiesObjects_"
    },

    /* default layout for any object */
    {
        component: ObjectResponsive,        
    }
];

/**
 * evalLayoutCondition
 * 
 * This will take a layout condition property, determine if it's a function, 
 * array or string and make the appropriate comparison to the expected value.
 * 
 * For functions, it will call it with the value, expecting a boolean to be 
 * returned.
 * 
 * For arrays, it will determine if the array includes the value.
 * 
 * For strings, it will expect the string to match the value.
 */
const evalLayoutCondition = (condition, value) => {

    if (condition !== undefined) {

        if (isFunction(condition))
            return condition(value);

        else if (isString(condition))
            return condition === value;

        else if (isArray(condition))
            return condition.includes(value);
    }
};


/**
 * layoutByProperty(property)
 *
 * Given an AfwProperty (property), this routine 
 * uses the property type information to determine the 
 * appropriate layout to render.
 */
export const layoutByProperty = ({ property, operational }) => {
    
    if (!property)
        return;

    const dataType = property.getDataType();
    const dataTypeParameter = property.getDataTypeParameter();
    const propertyName = property.getName();
    const embeddingObject = property.getEmbeddingObject();
    const objectTypeId = embeddingObject?.getObjectTypeId();    

    /* iterate over our propertyLayoutRegistry to find the appropriate matching layout */
    for (const layout of propertyLayoutRegistry) {

        /* check for a custom match function */
        if (layout.matches !== undefined) {
            if (!layout.matches({ property }))
                continue;
        }

        /* if operational properties are part of the conditions, check those */
        if (layout.operational !== undefined) {
            if (layout.operational.editable === true && operational.editable !== true)
                continue;
            else if (layout.operational.editable === false && operational.editable !== false)
                continue;           
            else if (layout.operational.add === true && operational.add !== true) 
                continue;
            else if (layout.operational.add === false && operational.add !== false) 
                continue;
        }             

        /* check objectTypeId conditions */
        if (evalLayoutCondition(layout.objectTypeId, objectTypeId) === false)
            continue;

        /* check propertyName conditions */
        if (evalLayoutCondition(layout.propertyName, propertyName) === false)
            continue;
      
        /* check dataType conditions */
        if (evalLayoutCondition(layout.dataType, dataType) === false)
            continue;

        /* check dataTypeParameter conditions */
        if (evalLayoutCondition(layout.dataTypeParameter, dataTypeParameter) === false)
            continue;
           

        if (layout.componentFunc !== undefined)
            return layout.componentFunc({ property });
        else 
            return layout.component;
    }    

};

/**
 * layoutByObject
 * 
 * Given an AfwObject (object), this routine uses the object type information
 * to determine the appropriate layout to render.
 */
export const layoutByObject = ({ object, operational }) => {

    if (!object)
        return;

    const objectTypeId = object.getObjectTypeId();

    for (const layout of objectLayoutRegistry) {

        if (layout.operational !== undefined) {
            if (layout.operational.editable === true && operational.editable !== true)
                continue;
            else if (layout.operational.editable === false && operational.editable !== false)
                continue;           
            else if (layout.operational.add === true && operational.add !== true) 
                continue;
            else if (layout.operational.add === false && operational.add !== false) 
                continue;
        }

        /* check objectTypeId conditions */
        if (evalLayoutCondition(layout.objectTypeId, objectTypeId) === false)
            continue;

        if (layout.componentFunc !== undefined)
            return layout.componentFunc({ object });
        else 
            return layout.component;
    }
};


/**
 * AdaptiveLayoutRegistry
 * 
 * The class to create a global registry for object and property 
 * layouts.
 */
export class AdaptiveLayoutRegistry {

    registerLayoutComponents(components) {
        console.log("registerLayoutComponents", components);
    }

    registerObjectLayouts(layouts) {
        console.log("registerObjectLayouts", layouts);
    }

    registerPropertyLayouts(layouts) {
        console.log("registerPropertyLayouts", layouts);
    }
}
