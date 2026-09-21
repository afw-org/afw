/* Isolated swap-model: stop, persist a new onGetObject, restart the
 * location adapter so the compile reloads, start, get, restore v1.
 * Own adapters so firehose model_get on `model` is not disturbed. */
function soak_swap_model(tag, msg) {
    return {
        "modelId": "SoakSwap",
        "objectTypes": {
            "Demo": {
                "mappedObjectType": "Demo",
                "onGetObject":
                    "return { msg: \"" + msg + "\", tag: \"" + tag + "\" };",
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
    };
}

function reload_swap(tag, msg) {
    service_stop("adapter-swap-model");
    replace_object("swap-backend", "_AdaptiveModel_", "SoakSwap",
        soak_swap_model(tag, msg));
    const stored = get_object("swap-backend", "_AdaptiveModel_", "SoakSwap");
    assert(index_of(stored.objectTypes.Demo.onGetObject, tag) >= 0,
        "onGetObject missing tag " + tag);
    service_stop("adapter-swap-backend");
    const b = service_start("adapter-swap-backend");
    assert(b.status === "running", b.statusMessage);
    const started = service_start("adapter-swap-model");
    assert(started.status === "running", started.statusMessage);
    const o = get_object("swap-model", "Demo", "seed");
    assert(o.tag === tag, "expected tag " + tag + " got " + string(o.tag));
    assert(o.msg === msg, "expected msg " + msg);
    return true;
}

/* One reload per request: a second compile in the same xctx still
 * serves the first hook. Unique tag so every firehose hit is new. */
const tag = generate_uuid();
reload_swap(tag, "from-hook");
return true;
