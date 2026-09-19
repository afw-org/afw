service_stop("adapter-model");
replace_object("backend", "_AdaptiveModel_", "Hostile", {
    "modelId": "Hostile",
    "objectTypes": {
        "Demo": {
            "mappedObjectType": "Demo",
            "onGetObject": "return evaluate(compile<script>(script('return { msg: \"from-eval\", tag: \"t\" };')));",
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
service_stop("adapter-backend");
service_start("adapter-backend");
const started = service_start("adapter-model");
assert(started.status === "running", started.statusMessage);
const a = get_object("model", "Demo", "seed");
const b = get_object("model", "Demo", "seed");
assert(a.msg === "from-eval");
assert(b.msg === "from-eval");
assert(a.tag === "t");
a.msg = "mutated";
assert(b.msg === "from-eval", "second get must not share the first managed object");
return true;
