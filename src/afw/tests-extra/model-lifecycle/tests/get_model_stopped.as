assert(
    safe_evaluate(get_object("model", "Demo", "seed"), "error") === "error",
    "get_object on stopped model should fail");
return true;
