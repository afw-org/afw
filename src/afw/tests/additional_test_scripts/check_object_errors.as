#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: check_object_errors.as
//? customPurpose: Part of core function tests
//? description: Checks for "hasErrors" reported by object type definitions
//? sourceType: script
//?
//? test: check_object_errors
//? description: Checks for "hasErrors" reported by object type definitions
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const objectTypeObjects: object = retrieve_objects("afw", "_AdaptiveObjectType_");
foreach const objectTypeObject: object of objectTypeObjects {        
    const {objectType: objectTypeId, allowEntity = false} = objectTypeObject;        
    if (allowEntity != false) {                
        const objects: (list of object) = retrieve_objects("afw", objectTypeId,, { normalize: true, composite: true, objectId: true, path: true });        
        foreach const object: object of objects {      
            assert(
                meta(object)->property_get("hasErrors", false) == false, 
                "Object " + meta(object).path + " has errors"
            );            
        }
    }
}


const objectTypeObjectsDev: (list of object) = retrieve_objects("dev", "_AdaptiveObjectType_");
foreach const objectTypeObjectDev: object of objectTypeObjectsDev {
    const {objectType: objectTypeId, allowEntity = false} = objectTypeObjectDev;    
    if (allowEntity != false) {
        const objectsDev: (list of object) = retrieve_objects("dev", objectTypeId, undefined, { normalize: true, composite: true, objectId: true, path: true });
        foreach const objectDev: object of objectsDev {        
            assert(
                meta(objectDev)->property_get("hasErrors", false) == false, 
                "Object " + meta(objectDev).path + " has errors"
            );            
        }
    }
}


return 0;
