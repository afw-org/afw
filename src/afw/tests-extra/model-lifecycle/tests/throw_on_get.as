replace_object("backend", "_AdaptiveModel_", "Hostile", {
    "modelId": "Hostile",
    "objectTypes": {
        "Demo": {
            "mappedObjectType": "Demo",
            "onGetObject": "throw \"hook-boom\";",
            "onAddObject": "return current::useDefaultProcessing;",
            "propertyTypes": {
                "msg": {
                    "dataType": "string",
                    "mappedPropertyName": "msg"
                }
            }
        }
    }
});
service_stop("adapter-model");
service_restart("adapter-backend");
const started = service_start("adapter-model");
assert(started.status === "running", started.statusMessage);
assert(
    safe_evaluate(get_object("model", "Demo", "seed"), "error") === "error",
    "onGetObject throw should fail get");
const f = get_object("backend", "Demo", "seed");
assert(f.msg === "seed", "backend still serves when hook throws");
return true;
