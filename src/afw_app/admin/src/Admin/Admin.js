// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";
import {Route} from "react-router";

import Container from "../common/Container";
import Application from "./Application/Application";
import Services from "./Services/Services";
import Server from "./Server/Server";
import Status from "./Status";
import Provisioning from "./Provisioning/Provisioning";
import Authorization from "./Authorization/Authorization";
import Adapters from "./Services/Adapters/Adapters";
import AuthorizationHandlers from "./Services/AuthorizationHandlers/AuthorizationHandlers";
import RequestHandlers from "./RequestHandlers/RequestHandlers";
import Logs from "./Services/Logs/Logs";
import Models from "./Models/Models";
import Schema from "./Schema/Schema";
import Extensions from "./Extensions/Extensions";

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

export default () => 
    <Admin>
        <RouteBasePathContext.Provider value="/Objects">        
            <Route exact path="/Admin/Status" component={Status} />
            <Route path="/Admin/Server" component={Server} />
            <Route path="/Admin/Application" component={Application} />
            <Route path="/Admin/Extensions" component={Extensions} />
            <Route path="/Admin/Models" component={Models} />
            <Route path="/Admin/Schema" component={Schema} />
            <Route path="/Admin/Provisioning" component={Provisioning} />
            <Route path="/Admin/Authorization" component={Authorization} />
            <Route path="/Admin/Services" component={Services} />
            <Route path="/Admin/RequestHandlers" component={RequestHandlers} />
            <Route path="/Admin/Adapters" component={Adapters} />
            <Route path="/Admin/Logs" component={Logs} />
            <Route path="/Admin/AuthHandlers" component={AuthorizationHandlers} />
            <Route exact path="/Admin" component={Status} />
        </RouteBasePathContext.Provider>
    </Admin>;
