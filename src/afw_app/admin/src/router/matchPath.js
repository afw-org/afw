// See the 'COPYING' file in the project root for licensing information.

/**
 * matchPath(pathname, { path, exact })
 *
 * React Router 5's matchPath, for code whose URL grammar is a list of
 * patterns (the model editor's views, its context menu) rather than routes
 * of its own (see designs/tanstack-router-migration.md). Same semantics:
 *
 *   - `path` is a pattern or an array of them; ":name" matches one
 *     segment; matching ignores case and a trailing slash.
 *   - Without `exact`, a pattern matches a prefix ending at a segment
 *     boundary.
 *
 * Returns { path, url, isExact, params } for the first pattern that
 * matches, or null. Give it TanStack's pathname, which is decoded, so the
 * params are decoded too.
 */
const escape = (text) => text.replace(/[.*+?^${}()|[\]\\]/g, "\\$&");

const compile = (pattern, exact) => {
    const keys = [];
    const source = pattern.replace(/\/+$/, "").split("/").map(segment => {
        if (segment.startsWith(":")) {
            keys.push(segment.slice(1));
            return "([^/]+)";
        }
        return escape(segment);
    }).join("/");

    return { keys, regexp: new RegExp("^" + source + (exact ? "/?$" : "(?:/|$)"), "i") };
};

export const matchPath = (pathname, { path, exact = false } = {}) => {
    for (const pattern of [].concat(path)) {
        const {keys, regexp} = compile(pattern, exact);
        const match = regexp.exec(pathname);
        if (!match)
            continue;

        const url = match[0].replace(/\/$/, "") || "/";
        const params = {};
        keys.forEach((key, index) => { params[key] = match[index + 1]; });

        return {
            path: pattern,
            url,
            isExact: pathname.replace(/\/$/, "") === url.replace(/\/$/, ""),
            params,
        };
    }

    return null;
};

export default matchPath;
