replace_object("backend", "_AdaptiveModel_", "Hostile", {
    "modelId": "Hostile",
    "objectTypes": {
        "Demo": {
            "mappedObjectType": "Demo",
            "onGetObject": "return { msg: \"from-hook\", tag: \"v2\" };",
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
const stored = get_object("backend", "_AdaptiveModel_", "Hostile");
assert(stored.objectTypes.Demo.onGetObject !==
    "return current::useDefaultProcessing;",
    "replace_object did not persist onGetObject");
/* Compiled model lives on the location adapter, not the model service. */
service_restart("adapter-backend");
const started = service_start("adapter-model");
assert(started.status === "running", started.statusMessage);
const o = get_object("model", "Demo", "seed");
assert(o.tag === "v2", "expected new compile tag v2");
assert(o.msg === "from-hook", "expected new compile, not mapped seed");
return o.msg;
