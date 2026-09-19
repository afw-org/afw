service_stop("adapter-model");
replace_object("backend", "_AdaptiveModel_", "Hostile", {
    "modelId": "Hostile",
    "objectTypes": {
        "Demo": {
            "mappedObjectType": "Demo",
            "onGetObject": "return current::useDefaultProcessing;",
            "onAddObject": "return current::useDefaultProcessing;",
            "onReplaceObject": "return current::useDefaultProcessing;",
            "onModifyObject": "return current::useDefaultProcessing;",
            "onDeleteObject": "return current::useDefaultProcessing;",
            "propertyTypes": {
                "msg": {
                    "dataType": "string",
                    "mappedPropertyName": "msg"
                },
                "tag": {
                    "dataType": "string"
                }
            }
        }
    }
});
service_restart("adapter-backend");
const started = service_start("adapter-model");
assert(started.status === "running", started.statusMessage);
const o = get_object("model", "Demo", "seed");
assert(o.msg === "seed");
return o.msg;
