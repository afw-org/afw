/* Live compile is v2. Replace _AdaptiveModel_ on disk with v3 while
 * the model adapter is running. Requests must keep the live compile.
 * Reloading after an in-use replace is covered by stop + location
 * restart + start (swap_hook_v2). */
replace_object("backend", "_AdaptiveModel_", "Hostile", {
    "modelId": "Hostile",
    "objectTypes": {
        "Demo": {
            "mappedObjectType": "Demo",
            "onGetObject": "return { msg: \"from-hook\", tag: \"v3\" };",
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
assert(stored.objectTypes.Demo.onGetObject ===
    "return { msg: \"from-hook\", tag: \"v3\" };",
    "v3 onGetObject not on disk");
const before = get_object("model", "Demo", "seed");
assert(before.tag === "v2", "live compile must ignore disk until reload");
return true;
