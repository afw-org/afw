#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: object_options.as
//? customPurpose: Part of object category tests
//? description: Make calls using object options on afw adapter objects (no verification)
//? sourceType: script
//?
//? test: checkRequired
//? description: Tests the checkRequired property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", { checkRequired: true });
return meta(obj).dataType;

//? test: composite
//? description: Tests the composite property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", { composite: true });
return meta(obj).dataType;

//? test: debug
//? description: Tests the debug property 
//? expect: string("object")
//? skip: true
//? source: ...

// fixme not sure this was implemented
const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", { debug: true });
return meta(obj).dataType;

//? test: includeDefaultValues
//? description: Tests the includeDefaultValues property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", { includeDefaultValues: true });
return meta(obj).dataType;

//? test: includeDescendentObjectTypes
//? description: Tests the includeDescendentObjectTypes property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", { includeDescendentObjectTypes: true });
return meta(obj).dataType;

//? test: inheritedFrom
//? description: Tests the inheritedFrom property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", { inheritedFrom: true });
return meta(obj).dataType;

//? test: integersAsString
//? description: Tests the integersAsString property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { integersAsString: true });
return meta(obj).dataType;

//? test: largeIntegersAsString
//? description: Tests the largeIntegersAsString property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { largeIntegersAsString: true });
return meta(obj).dataType;

//? test: metaFull
//? description: Tests the metaFull property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { metaFull: true });
return meta(obj).dataType;

//? test: metaLimited
//? description: Tests the metaLimited property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { metaLimited: true });
return meta(obj).dataType;

//? test: normalize
//? description: Tests the normalize property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { normalize: true });
return meta(obj).dataType;

//? test: objectId
//? description: Tests the objectId property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { objectId: true });
return meta(obj).dataType;

//? test: objectType
//? description: Tests the objectType property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { objectType: true });
return meta(obj).dataType;

//? test: objectTypes
//? description: Tests the objectTypes property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { objectTypes: true });
return meta(obj).dataType;

//? test: path
//? description: Tests the path property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { path: true });
return meta(obj).dataType;

//? test: pathEmbedded
//? description: Tests the pathEmbedded property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { pathEmbedded: true });
return meta(obj).dataType;

//? test: reconcilable
//? description: Tests the reconcilable property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { reconcilable: true });
return meta(obj).dataType;

//? test: resolvedParentPaths
//? description: Tests the resolvedParentPaths property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { resolvedParentPaths: true });
return meta(obj).dataType;

//? test: typedValues
//? description: Tests the typedValues property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { typedValues: true });
return meta(obj).dataType;

//? test: whitespace
//? description: Tests the whitespace property 
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveFunction_", "add", { whitespace: true });
return meta(obj).dataType;

//? test: everything everywhere all at once
//? description: Tests all the options together
//? expect: error:Object immutable
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", { 
    checkRequired: true,
    composite: true,
    includeDefaultValues: true,
    includeDescendentObjectTypes: true,
    inheritedFrom: true,
    integersAsString: true,
    largeIntegersAsString: true,
    metaFull: true,
    metaLimited: true,
    normalize: true,
    objectId: true,
    objectType: true,
    objectTypes: true,
    path: true,
    pathEmbedded: true,
    reconcilable: true,
    resolvedParentPaths: true,
    typedValues: true,    
    whitespace: true
});
return meta(obj).dataType;

//? test: no checkRequired
//? description: Tests all the options except checkRequired
//? expect: error:Object immutable
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", {     
    composite: true,
    includeDefaultValues: true,
    includeDescendentObjectTypes: true,
    inheritedFrom: true,
    integersAsString: true,
    largeIntegersAsString: true,
    metaFull: true,
    metaLimited: true,
    normalize: true,
    objectId: true,
    objectType: true,
    objectTypes: true,
    path: true,
    pathEmbedded: true,
    reconcilable: true,
    resolvedParentPaths: true,
    typedValues: true,    
    whitespace: true
});
return meta(obj).dataType;

//? test: no composite
//? description: Tests all the options except composite
//? expect: error:Object immutable
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", {     
    checkRequired: true,
    includeDefaultValues: true,
    includeDescendentObjectTypes: true,
    inheritedFrom: true,
    integersAsString: true,
    largeIntegersAsString: true,
    metaFull: true,
    metaLimited: true,
    normalize: true,
    objectId: true,
    objectType: true,
    objectTypes: true,
    path: true,
    pathEmbedded: true,
    reconcilable: true,
    resolvedParentPaths: true,
    typedValues: true,    
    whitespace: true
});
return meta(obj).dataType;

//? test: no includeDefaultValues
//? description: Tests all the options except includeDefaultValues
//? expect: error:Object must have a pool
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", {     
    checkRequired: true,
    composite: true,
    includeDescendentObjectTypes: true,
    inheritedFrom: true,
    integersAsString: true,
    largeIntegersAsString: true,
    metaFull: true,
    metaLimited: true,
    normalize: true,
    objectId: true,
    objectType: true,
    objectTypes: true,
    path: true,
    pathEmbedded: true,
    reconcilable: true,
    resolvedParentPaths: true,
    typedValues: true,    
    whitespace: true
});
return meta(obj).dataType;

//? test: no includeDescendentObjectTypes
//? description: Tests all the options except includeDescendentObjectTypes
//? expect: error:Object immutable
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", {     
    checkRequired: true,
    composite: true,
    includeDefaultValues: true,
    inheritedFrom: true,
    integersAsString: true,
    largeIntegersAsString: true,
    metaFull: true,
    metaLimited: true,
    normalize: true,
    objectId: true,
    objectType: true,
    objectTypes: true,
    path: true,
    pathEmbedded: true,
    reconcilable: true,
    resolvedParentPaths: true,
    typedValues: true,    
    whitespace: true
});
return meta(obj).dataType;

//? test: no inheritedFrom
//? description: Tests all the options except inheritedFrom
//? expect: error:Object immutable
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", {     
    checkRequired: true,
    composite: true,
    includeDefaultValues: true,
    includeDescendentObjectTypes: true,
    integersAsString: true,
    largeIntegersAsString: true,
    metaFull: true,
    metaLimited: true,
    normalize: true,
    objectId: true,
    objectType: true,
    objectTypes: true,
    path: true,
    pathEmbedded: true,
    reconcilable: true,
    resolvedParentPaths: true,
    typedValues: true,    
    whitespace: true
});
return meta(obj).dataType;

//? test: composite options
//? description: Tests all the options except inheritedFrom
//? expect: string("object")
//? source: ...

const obj = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveObjectType_", {         
    composite: true,
    inheritedFrom: true,
    resolvedParentPaths: true
});
return meta(obj).dataType;

//? test: composite options on inherited object type
//? description: Tests all the options except inheritedFrom
//? expect: string("object")
//? source: ...

const obj = get_object("file", "_AdaptiveObjectType_", "TestObjectType2", {         
    composite: true,
    inheritedFrom: true,
    resolvedParentPaths: true
});
return meta(obj).dataType;