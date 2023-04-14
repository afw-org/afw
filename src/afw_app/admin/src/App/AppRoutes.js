// See the 'COPYING' file in the project root for licensing information.
import {lazy, Suspense} from "react";
import {Route, Switch} from "react-router";

import Loading from "../common/Loading";
import Versions from "../Admin/Versions";
import NoRoute from "../common/NoRoute";

const Loadable = Component => (props) => 
    <Suspense fallback={<Loading />}>
        <Component {...props} />
    </Suspense>;

/* create Loadable (async) components for code-splitting on routes */
const LoadableHome = Loadable(lazy(() =>
    import("../Home/Home"  /* webpackChunkName: "home" */)
));

const LoadableAdmin = Loadable(lazy(() =>
    import("../Admin/Admin"  /* webpackChunkName: "admin" */)
));

const LoadableDocumentation = Loadable(lazy(() =>
    import("../Documentation/Documentation"  /* webpackChunkName: "documentation" */)
));

const LoadableObjects = Loadable(lazy(() =>
    import("../Objects/Objects"  /* webpackChunkName: "objects" */)
));

const LoadableTools = Loadable(lazy(() =>
    import("../Tools/Tools"  /* webpackChunkName: "tools" */)
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
            <Route path="/Admin">
                <LoadableAdmin />
            </Route>
            <Route path="/Documentation">
                <LoadableDocumentation />
            </Route>
            <Route path="/Objects">
                <LoadableObjects />
            </Route>
            <Route path="/Tools">
                <LoadableTools />
            </Route>
            <Route path="/Versions">
                <Versions />
            </Route>
            <Route component={NoRoute} />
        </Switch>
    );    
};

export default AppRoutes;
