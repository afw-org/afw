// See the 'COPYING' file in the project root for licensing information.
import {useCallback} from "react";
import {Link, Prompt, useHistory} from "react-router-dom";

/**
 * reactRouterNavigation
 *
 * The admin app's adapter for @afw/react's navigation contract (see
 * navigation.js in @afw/react), passed to AdaptiveProvider. It is the only
 * place Adaptive Components meet the app's router: changing routers means
 * changing this file, not the component libraries.
 *
 * Every member runs where it is used, inside <BrowserRouter> - App.js mounts
 * AdaptiveProvider outside it.
 */
const useNavigate = () => {
    const history = useHistory();

    return useCallback((to) => history.push(to), [history]);
};

const NavigationBlocker = ({ when, message }) =>
    <Prompt when={when} message={message} />;

export const reactRouterNavigation = {
    Link,
    useNavigate,
    NavigationBlocker,
};

export default reactRouterNavigation;
