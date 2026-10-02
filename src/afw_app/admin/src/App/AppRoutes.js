// See the 'COPYING' file in the project root for licensing information.
import {lazy, Suspense} from "react";
import {Route, Switch} from "react-router";

import Loading from "../common/Loading";
import Versions from "../Admin/Versions";
import NoRoute from "../common/NoRoute";

const Loadable = Component => function Loadable(props) {
    return (
        <Suspense fallback={<Loading />}>
            <Component {...props} />
        </Suspense>
    );
};

/* create Loadable (async) components for code-splitting on routes */
const LoadableHome = Loadable(lazy(() =>
    import("../Home/Home")
));


const LoadableDocumentation = Loadable(lazy(() =>
    import("../Documentation/Documentation")
));




/*
 * AppRoutes
 *
 * This simple component defines all of the Routes for this App 
 * forwards them to the appropriate component target, using
 * React Router.
 */
export const AppRoutes = () => {
    return (
        <Switch>
            <Route exact path="/">
                <LoadableHome />
            </Route>
            <Route path="/Home">
                <LoadableHome />
            </Route>
            <Route path="/Documentation">
                <LoadableDocumentation />
            </Route>
            <Route path="/Versions">
                <Versions />
            </Route>
            <Route component={NoRoute} />
        </Switch>
    );    
};

export default AppRoutes;
