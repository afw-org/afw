// See the 'COPYING' file in the project root for licensing information.

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

export function objectGet(object, selector) {
    if (isArray(selector)) {
        let value = object ? object[ selector[0] ] : undefined;
        for (let i = 1; (i <  selector.length) && value; i++)
            value = value[ selector[i] ];
        return value;
    } else {
        return object ? object[selector] : undefined;
    }
}

/* 
 * Returns a function, that, as long as it continues to be invoked, will not
 * be triggered. The function will be called after it stops being called for
 * N milliseconds. If `immediate` is passed, trigger the function on the
 * leading edge, instead of the trailing.
 */
export function debounce(func, wait, immediate) {
    var timeout;
    return function() {
        var context = this, args = arguments;
        var later = function() {
            timeout = null;
            if (!immediate) func.apply(context, args);
        };
        var callNow = immediate && !timeout;
        clearTimeout(timeout);
        timeout = setTimeout(later, wait);
        if (callNow) func.apply(context, args);
    };
}
