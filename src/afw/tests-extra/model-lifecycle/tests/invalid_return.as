service_stop("adapter-model");
replace_object("backend", "_AdaptiveModel_", "Hostile", {
    "modelId": "Hostile",
    "objectTypes": {
        "Demo": {
            "mappedObjectType": "Demo",
            "onGetObject": "return 7;",
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
assert(
    safe_evaluate(get_object("model", "Demo", "seed"), "error") === "error",
    "integer onGetObject should be invalid");
return true;
