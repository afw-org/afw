// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable no-redeclare */
/* eslint-disable @typescript-eslint/no-use-before-define */
/**
 * Some utility functions for all of @afw/react to use. 
 */

export function isObject(value) {
    return (value && typeof value === "object" && value.constructor !== Array);
}

export function isArray(value) {
    return (value && typeof value === "object" && value.constructor === Array);
}

export function isInteger(value) {
    return (typeof value === "number");
}

export function isBoolean(value) {
    return (typeof value === "boolean");
}

export function isString(value) {
    return (typeof value === "string");
}

export function isFunction(value) {
    return (value && typeof value === "function");
}

export function isHybridScript(hybrid) {
    if (!hybrid || typeof hybrid !== "string")
        return false;

    const lines = hybrid.split("\n");
    if (lines && lines.length > 0) {
        if (lines[0].startsWith("#!"))
            return true;
    }

    return false;
}

export function isHybridTemplate(hybrid) {
    if (!hybrid || typeof hybrid !== "string")
        return false;

    if (hybrid.indexOf("${") >= 0)
        return true;

    return false;
}

export function isHybridLiteral(hybrid) {

    if (isObject(hybrid))
        return true;

    else if (isArray(hybrid))
        return true;

    else if (typeof(hybrid) === "number")
        return true;

    else if (typeof(hybrid) === "boolean")
        return true;

    else if (typeof(hybrid) === "string") {
        /* if it's a string, it could be a template or script */
        return !(isHybridScript(hybrid) || isHybridTemplate(hybrid));
    }
}

export function ctx(props, componentType) {
    return [props["data-component-type"], componentType].join(" ").trim();
}

export function cls(props, className) {
    return [props["className"], className].join(" ").trim();
}

/*
 * From https://github.com/substack/json-stable-stringify (MIT licensed)
 *
 * This will stringify() an object in deterministic fashion.
 */
export function stable_stringify(obj, opts) {
    if (!opts) opts = {};
    if (typeof opts === "function") opts = { cmp: opts };
    var space = opts.space || "";
    if (typeof space === "number") space = Array(space+1).join(" ");
    var cycles = (typeof opts.cycles === "boolean") ? opts.cycles : false;
    var replacer = opts.replacer || function(key, value) { return value; };

    var cmp = opts.cmp && (function (f) {
        return function (node) {
            return function (a, b) {
                var aobj = { key: a, value: node[a] };
                var bobj = { key: b, value: node[b] };
                return f(aobj, bobj);
            };
        };
    })(opts.cmp);

    var seen = [];
    return (function stringify (parent, key, node, level) {
        var indent = space ? ("\n" + new Array(level + 1).join(space)) : "";
        var colonSeparator = space ? ": " : ":";

        if (node && node.toJSON && typeof node.toJSON === "function") {
            node = node.toJSON();
        }

        node = replacer.call(parent, key, node);

        if (node === undefined) {
            return;
        }
        if (typeof node !== "object" || node === null) {
            return JSON.stringify(node);
        }
        if (isArray(node)) {
            var out = [];
            for (var i = 0; i < node.length; i++) {
                var item = stringify(node, i, node[i], level+1) || JSON.stringify(null);
                out.push(indent + space + item);
            }
            return "[" + out.join(",") + indent + "]";
        }
        else {
            if (seen.indexOf(node) !== -1) {
                if (cycles) return JSON.stringify("__cycle__");
                throw new TypeError("Converting circular structure to JSON");
            }
            else seen.push(node);

            var keys = objectKeys(node).sort(cmp && cmp(node));
            var out = [];
            for (var i = 0; i < keys.length; i++) {
                var key = keys[i];
                var value = stringify(node, key, node[key], level+1);

                if(!value) continue;

                var keyValue = JSON.stringify(key)
                    + colonSeparator
                    + value;
                
                out.push(indent + space + keyValue);
            }
            seen.splice(seen.indexOf(node), 1);
            return "{" + out.join(",") + indent + "}";
        }
    })({ "": obj }, "", obj, 0);
}

var objectKeys = Object.keys || function (obj) {
    var has = Object.prototype.hasOwnProperty || function () { return true; };
    var keys = [];
    for (var key in obj) {
        if (has.call(obj, key)) keys.push(key);
    }
    return keys;
};
