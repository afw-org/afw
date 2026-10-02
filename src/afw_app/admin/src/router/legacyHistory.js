// See the 'COPYING' file in the project root for licensing information.

/**
 * createLegacyHistory(history, basepath)
 *
 * The bridge for migrating the admin app from React Router 5 to TanStack
 * Router one section at a time (see designs/tanstack-router-migration.md).
 * TanStack Router owns the browser history; this presents that history
 * (an @tanstack/history RouterHistory) in the `history` v4 shape React
 * Router 5's <Router history={...}> expects, so the not-yet-migrated parts
 * of the app keep working on the same locations.
 *
 *   - Paths: TanStack's history sees full paths, including the router's
 *     basepath (/apps/afw/admin); React Router 5's routes don't. Locations
 *     drop the basepath, and push/replace/createHref add it back.
 *   - Actions: TanStack reports PUSH / REPLACE / BACK / FORWARD / GO;
 *     React Router 5 knows PUSH / REPLACE / POP.
 *   - block(): React Router 5's <Prompt> blocks through history.block(),
 *     confirmed with window.confirm (what BrowserRouter's
 *     getUserConfirmation did).
 */
const toAction = (type) =>
    (type === "PUSH" || type === "REPLACE") ? type : "POP";

export const createLegacyHistory = (history, basepath = "/") => {

    const base = basepath.replace(/\/+$/, "");

    const stripBase = (pathname) =>
        (base && (pathname === base || pathname.startsWith(base + "/"))) ?
            (pathname.slice(base.length) || "/") : pathname;

    const toLocation = (location) => ({
        pathname: stripBase(location.pathname),
        search: location.search,
        hash: location.hash,
        state: location.state,
        key: location.state?.__TSR_key ?? location.state?.key,
    });

    /*
     * history v4 takes a path string or a {pathname, search, hash} object,
     * and resolves a relative one ("?q", "#tree", "child") against the
     * current location - as a browser resolves a relative URL.
     */
    const toPath = (to) => {
        let path = to;
        if (typeof to !== "string") {
            const {pathname = "", search = "", hash = ""} = to;
            path = pathname +
                ((search && !search.startsWith("?")) ? "?" + search : search) +
                ((hash && !hash.startsWith("#")) ? "#" + hash : hash);
        }

        if (path.startsWith("/"))
            return path;

        const current = toLocation(history.location);
        const url = new URL(path, "http://localhost" + current.pathname + current.search);
        return url.pathname + url.search + url.hash;
    };

    const toFullPath = (to) => base + toPath(to);

    const stateOf = (to, state) =>
        (state !== undefined) ? state : ((typeof to === "object") ? to.state : undefined);

    let action = "POP";

    return {
        get action() {
            return action;
        },
        get length() {
            return history.length;
        },
        get location() {
            return toLocation(history.location);
        },
        push: (to, state) => history.push(toFullPath(to), stateOf(to, state)),
        replace: (to, state) => history.replace(toFullPath(to), stateOf(to, state)),
        go: (n) => history.go(n),
        goBack: () => history.back(),
        goForward: () => history.forward(),
        createHref: (to) => history.createHref(toFullPath(to)),
        listen: (listener) => history.subscribe(({location, action: {type}}) => {
            action = toAction(type);
            listener(toLocation(location), action);
        }),
        /* returns the unblock function, as history v4 does */
        block: (prompt = false) => history.block({
            blockerFn: ({nextLocation, action: type}) => {
                const result = (typeof prompt === "function") ?
                    prompt(toLocation(nextLocation), toAction(type)) : prompt;

                /* a string asks the user; true allows; false blocks */
                if (typeof result === "string")
                    return !window.confirm(result);
                return result === false;
            }
        }),
    };
};

export default createLegacyHistory;
