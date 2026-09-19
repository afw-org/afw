service_stop("adapter-model");
replace_object("backend", "_AdaptiveModel_", "Hostile", {
    "modelId": "Hostile",
    "objectTypes": {
        "Demo": {
            "mappedObjectType": "Demo",
            "onGetObject": "return current::useDefaultProcessing;",
            "onAddObject": "return current::useDefaultProcessing;",
            "onReplaceObject": "return current::useDefaultProcessing;",
            "propertyTypes": {
                "msg": {
                    "dataType": "string",
                    "mappedPropertyName": "msg"
                }
            }
        }
    }
});
service_restart("adapter-backend");
const started = service_start("adapter-model");
assert(started.status === "running", started.statusMessage);
service_stop("adapter-backend");
assert(
    safe_evaluate(get_object("model", "Demo", "seed"), "error") === "error",
    "model get should fail when mapped adapter is stopped");
const back = service_start("adapter-backend");
assert(back.status === "running", back.statusMessage);
const o = get_object("model", "Demo", "seed");
assert(o.msg === "seed", "model get after mapped restart");
return true;
