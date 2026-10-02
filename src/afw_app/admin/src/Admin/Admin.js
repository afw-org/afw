// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";
import {Outlet} from "@tanstack/react-router";

import Container from "../common/Container";

import {useModel, useValues} from "@afw/react";
import {useApplication, useAppCore} from "../hooks";
import {Typography, Spinner, RouteBasePathContext} from "@afw/react";
import { ConfigContext } from "../context";

export const Admin = ({ children }) => {
            
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState();

    const model = useModel();
    const {notification, marginHeight} = useApplication();
    const {application, error: appError} = useAppCore();
    const {confAdapterId} = useValues(application);


    useEffect(() => {
        const loadConfigObjectTypes = async () => {
            try {
                setLoading(true);
    
                /* first load all object types from the config adapter */
                await model.loadObjectTypes({ adapterId: confAdapterId });
    
                setLoading(false);
            } catch (error) {
                setLoading(false);
                setError(error);
                notification({ message: error, type: "error" });
            }
        };

        if (confAdapterId)
            loadConfigObjectTypes();
        
    }, [confAdapterId, model, notification]);
        
    if (error || appError) {
        return (
            <div>
                <div style={{ height: "10vh" }} />
                <div style={{ textAlign: "center" }}>
                    <Typography color="error" size="7" text="Error Loading Data" />
                    <Typography color="error" text="Unable to load required Services and Object Type definitions." />
                </div>
            </div>
        );
    }

    if (loading) {
        return (
            <Spinner
                fullScreen={true}
                size="large"
                label="Loading configuration data..."
            />
        );
    }

    return (
        <ConfigContext.Provider value={{ loading }}>
            <Container maxWidth="xl" style={{ height: "calc(100vh - " + marginHeight + ")", overflow: "auto" }}>
                { children }
            </Container>        
        </ConfigContext.Provider>
    );
};

/*
 * AdminLayout
 *
 * The /Admin layout route's component (see routes.js): what every admin
 * page shares, around the matched child route.
 */
export const AdminLayout = () =>
    <Admin>
        <RouteBasePathContext.Provider value="/Objects">
            <Outlet />
        </RouteBasePathContext.Provider>
    </Admin>;

export default AdminLayout;
