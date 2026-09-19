service_stop("adapter-model");
replace_object("backend", "_AdaptiveModel_", "Hostile", {
    "modelId": "Hostile",
    "objectTypes": {
        "Demo": {
            "mappedObjectType": "Demo",
            "onGetObject": "return evaluate(compile<script>(script('return { msg: \"from-eval\", tag: \"eval\" };')));",
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
assert(o.msg === "from-eval", "nested evaluate in onGetObject");
assert(o.tag === "eval");
return true;
