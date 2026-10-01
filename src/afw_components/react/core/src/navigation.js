// See the 'COPYING' file in the project root for licensing information.
import {forwardRef, useCallback, useEffect} from "react";

/**
 * Navigation contract
 *
 * Adaptive Components never import a router. Anything that links,
 * navigates or guards against leaving a page goes through this contract,
 * which the application supplies to AdaptiveProvider as its `navigation`
 * prop (read back with useNavigation()), adapting whichever router it uses:
 *
 *   Link              - forwardRef component taking {to, ...props}; usable
 *                       as a MUI `component=`.
 *   useNavigate       - hook returning navigate(to). A hook, not a function,
 *                       because it is called where it is used: an app may
 *                       mount AdaptiveProvider outside its router.
 *   NavigationBlocker - component {when, message}: while `when` is true,
 *                       asks the user to confirm before leaving the page.
 *
 * defaultNavigation below needs no router at all: plain anchors, full page
 * loads, and a beforeunload guard.
 */

const DefaultLink = forwardRef(({ to, children, ...rest }, ref) =>
    <a ref={ref} {...rest} href={to}>{children}</a>
);
DefaultLink.displayName = "DefaultLink";

const useDefaultNavigate = () =>
    useCallback((to) => window.location.assign(to), []);

const DefaultNavigationBlocker = ({ when }) => {

    useEffect(() => {
        if (!when)
            return;

        /* browsers show their own text; a custom message is not supported */
        const onBeforeUnload = (event) => {
            event.preventDefault();
            event.returnValue = "";
        };

        window.addEventListener("beforeunload", onBeforeUnload);
        return () => window.removeEventListener("beforeunload", onBeforeUnload);
    }, [when]);

    return null;
};

export const defaultNavigation = {
    Link: DefaultLink,
    useNavigate: useDefaultNavigate,
    NavigationBlocker: DefaultNavigationBlocker,
};
