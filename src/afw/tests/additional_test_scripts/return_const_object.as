#!/usr/bin/env -S afw --syntax script

/* return a const object directly from the script */
const obj = perform({
    "function": "get_object_with_uri",
    "uri": "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_"
});

return obj;