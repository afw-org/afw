// See the 'COPYING' file in the project root for licensing information.
import {forwardRef, useCallback} from "react";
import {Link as RouterLink, useBlocker, useNavigate as useRouterNavigate} from "@tanstack/react-router";

import {parseSearch} from "./router/router";

/**
 * appNavigation
 *
 * The admin app's adapter for @afw/react's navigation contract (see
 * navigation.js in @afw/react), passed to AdaptiveProvider. It is the only
 * place Adaptive Components meet the app's router (TanStack Router):
 * changing routers means changing this file, not the component libraries.
 *
 * Every member runs where it is used, inside <RouterProvider> - App.js
 * mounts AdaptiveProvider outside it.
 */

/*
 * Split an app href ("/Objects/a%20b?raw#tree") into TanStack Link options.
 * A relative `to` passes through for TanStack to resolve.
 */
const linkOptions = (to) => {
    if (typeof to !== "string" || !to.startsWith("/"))
        return { to };

    const url = new URL(to, "http://localhost");

    return {
        to: url.pathname,
        search: parseSearch(url.search),
        hash: url.hash.slice(1),
    };
};

/* no "active" class: React Router 5's <Link> added none */
const noActiveProps = {};

/* aria-current="page" only on the current page's own link, not its parents' */
const activeOptions = { exact: true };

const Link = forwardRef(({ to, ...rest }, ref) =>
    <RouterLink
        ref={ref}
        {...rest}
        {...linkOptions(to)}
        activeProps={noActiveProps}
        activeOptions={activeOptions}
    />
);
Link.displayName = "Link";

const useNavigate = () => {
    const navigate = useRouterNavigate();

    return useCallback((to) => { navigate({ href: to }); }, [navigate]);
};

/* message: a string, or a function returning one */
const NavigationBlocker = ({ when, message }) => {
    useBlocker({
        disabled: !when,
        shouldBlockFn: () =>
            !window.confirm((typeof message === "function") ? message() : message),
    });

    return null;
};

export const appNavigation = {
    Link,
    useNavigate,
    NavigationBlocker,
};

export default appNavigation;
