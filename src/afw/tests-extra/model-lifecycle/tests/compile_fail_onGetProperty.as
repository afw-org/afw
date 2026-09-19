service_stop("adapter-model");
replace_object("backend", "_AdaptiveModel_", "Hostile", {
    "modelId": "Hostile",
    "objectTypes": {
        "Demo": {
            "mappedObjectType": "Demo",
            "onGetObject": "return current::useDefaultProcessing;",
            "propertyTypes": {
                "msg": {
                    "dataType": "string",
                    "mappedPropertyName": "msg",
                    "onGetProperty": "function ("
                }
            }
        }
    }
});
service_stop("adapter-backend");
service_start("adapter-backend");
const started = service_start("adapter-model");
assert(started.status === "running", started.statusMessage);
let threw = false;
try {
    get_object("model", "Demo", "seed");
} catch (e) {
    threw = true;
}
assert(threw, "unclosed onGetProperty should fail get");
return true;
