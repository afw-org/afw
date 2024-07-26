// See the 'COPYING' file in the project root for licensing information.

import AfwObject from "./AfwObject";

export interface IObjectParam {
    adapterId?:     string;
    objectTypeId?:  string;
    path?:          string;
    object?:        AfwObject|object;    
}

export interface IMetaObject {
    objectType?:    string;
    objectId?:      string;
    path?:          string;
    [prop: string]: any;
}

export interface IAnyObject {
    _meta_?:        IMetaObject;
    [prop: string]: any;
}

export interface IPropertyType {
    dataType?:          string;
    dataTypeParameter?: string;
    label?:             string;
    brief?:             string;
    description?:       string;    
    errors?:            any;
    [prop: string]:     any;
}

export interface IPropertyTypesObject {
    [prop: string]:     IPropertyType;
}

export interface IObjectTypeObject {
    _meta_?:            IMetaObject;
    propertyTypes?:     IPropertyTypesObject;
    otherProperties?:   IPropertyType;
    [prop: string]:     any;
}

export type IJSONPrimitive = void | Date | null | boolean | number | string;
export interface IJSONObject { 
    [prop: string]: IJSONPrimitive | IJSONPrimitive[] | IJSONObject;
}

export type IJSON = IJSONPrimitive | IJSONObject | IJSONPrimitive[] | IJSONObject[];

export interface IPerformPayload {    
    [prop: string]: IJSONPrimitive | IJSONPrimitive[] | IJSONObject | IJSONObject[];    
    actions?:       IJSONObject[];
}

export interface IJSONSchema {
    type?:                  string;
    required?:              string[];
    properties:             {
        [prop: string]:     any;
    };
    additionalProperties?:  {
        type?:              string;
        [prop: string]:     any;
    } | boolean;
}

export interface IObjectOptions {
    objectId?:      boolean;
    path?:          boolean;
    objectType?:    boolean;
    normalize?:     boolean;
    composite?:     boolean;
}

export interface IModelOptions {
    adaptiveObject?:    boolean;
    initialize?:        boolean;
}


export type AfwOrObject = AfwObject | IAnyObject;
export type PromisableObject = Promise<AfwOrObject>;