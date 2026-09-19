service_stop("adapter-model");
replace_object("backend", "_AdaptiveModel_", "Hostile", {
    "modelId": "Hostile",
    "objectTypes": {
        "Demo": {
            "mappedObjectType": "Demo",
            "onGetObject": "return (",
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
const stored = get_object("backend", "_AdaptiveModel_", "Hostile");
assert(stored.objectTypes.Demo.onGetObject === "return (",
    stored.objectTypes.Demo.onGetObject);
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
assert(threw, "bad onGetObject should fail get (compile/eval)");
const f = get_object("backend", "Demo", "seed");
assert(f.msg === "seed", "backend still serves after model compile fail");
return true;
