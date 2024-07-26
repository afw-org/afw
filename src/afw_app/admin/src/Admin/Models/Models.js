// See the 'COPYING' file in the project root for licensing information.
import {useReducer, useState, useEffect} from "react";
import {Route, Switch, useLocation} from "react-router";

import {
    Breadcrumb,
    Spinner,
    Typography,
    useGetObject,
    useRetrieveObjects,
} from "@afw/react";

import {useTheme, useAppCore} from "../../hooks";
import {ContextualHelpButton, ContextualHelp} from "../../common/ContextualHelp";
import NoRoute from "../../common/NoRoute";

import {ContextualHelpRoutes} from "./ContextualHelp";
import ModelsTable from "./ModelsTable";
import ModelEditor from "./ModelEditor";


const initialState = {};

const reducer = (state, action) => {

    switch (action.type) {

    case "LOCATION":
        return {
            ...state,
            adapterId: action.adapterId,
            modelId: action.modelId,
            objectType: action.objectType,
            propertyType: action.propertyType,
            hash: action.hash,
        };

    case "SELECT_ADAPTER":
        return {
            ...state,
            adapterId: action.adapterId,
        };

    default:
        return state;
    }
};

/**
 * Header shows the Breadcrumbs and the Contextual Help button.
 */
const Header = ({ adapterId, modelId, objectType, propertyType, hash }) => {

    const [showHelp, setShowHelp] = useState(false);
    const theme = useTheme();    


    /* Compute the breadcrumbs from the current route */
    let breadcrumbItems = [
        { text: "Admin",    key: "Admin",   link: "/Admin"          },
        { text: "Models",   key: "Models",  link: "/Admin/Models"   },
    ];

    let link = "/Admin/Models";
    let text, key;

    if (adapterId) {
        link += "/" + adapterId;
        text = key = adapterId;
        breadcrumbItems.push({ text, key, link });

        if (modelId) {
            link += "/" + modelId;
            text = key = modelId;
            breadcrumbItems.push({ text, key, link: link + hash });

            if (objectType) {
                link += "/objectTypes/" + objectType;
                text = key = objectType;
                breadcrumbItems.push({ text, key, link: link + hash });

                if (propertyType) {
                    link += "/propertyTypes/" + propertyType;
                    text = key = propertyType;
                    breadcrumbItems.push({ text, key, link: link + hash });
                }
            }
        }
    }

    return (
        <div style={{ display: "flex", alignItems: "center", paddingBottom: theme.spacing(2)}}>
            <div style={{ flex: 1 }}>
                <Breadcrumb items={breadcrumbItems} />  
            </div>                 
            <ContextualHelpButton showHelp={setShowHelp} />     
            <ContextualHelp 
                open={showHelp}
                onClose={() => setShowHelp(false)}
                routes={ContextualHelpRoutes}
            />           
        </div>
    );
};

const modelsObjectOptions = {
    objectId: true, 
    path: true, 
    pathEmbedded: true, 
    inheritedFrom: true
};

const modelObjectOptions = {
    objectId: true,
    objectType: true,
    path: true,
    resolvedParentPaths: true,
    composite: true,
    pathEmbedded: true,
    reconcilable: true,
    inheritedFrom: true,
};

/* displays an Error on the screen */
const Error = ({ message }) => 
    <div>
        <div style={{ height: "10vh" }} />
        <div style={{ textAlign: "center" }}>
            <Typography text={message} />
        </div>
    </div>;

/* displays a Spinner indicating we are waiting on data */
const Loading = ({ message }) =>
    <Spinner 
        label={message}
        size="large"        
        fullScreen
    />;

/**
 * Models
 * 
 * This component is used under the Admin tab to display Adaptive Models
 * (_AdaptiveModel_ objects).  You can view and edit Adaptive Models in a 
 * variety of ways and deploy them to services.
 */
export const Models = () => {

    const [state, dispatch] = useReducer(reducer, initialState);
    const {pathname, hash} = useLocation();
    const {application} = useAppCore();    

    const {
        objects: models = [],
        isLoading: isLoadingModels,
        onRefresh,
        error: errorModels,
    } = useRetrieveObjects({
        adapterId: state.adapterId,
        objectTypeId: "_AdaptiveModel_",
        objectOptions: modelsObjectOptions,
    });

    const {
        object: model,
        isLoading: isLoadingModel,
        error: errorModel,
    } = useGetObject({
        adapterId: state.adapterId,
        objectTypeId: "_AdaptiveModel_",
        objectId: state.modelId,
        objectOptions: modelObjectOptions,
    });

    useEffect(() => {
        if (application) {
            let [, adapterId, modelId,, objectType,, propertyType] = pathname.split("/").splice(2);

            if (!adapterId)
                adapterId = application.getPropertyValue("defaultModelAdapterId");

            dispatch({ type: "LOCATION", adapterId, modelId, objectType, propertyType, hash });
        }
    }, [pathname, hash, application]);

    /* When multiple adapters are used to store models, the user can select one */
    const onSelectAdapterId = (adapterId) => dispatch({ type: "SELECT_ADAPTER", adapterId });   

    return (
        <div 
            id="admin-admin-models" 
            data-testid="admin-admin-models"
            style={{ display: "flex", flexDirection: "column", height: "100%" }}
        >
            <Header {...state} />
            <div style={{ flex: 1, overflow: "auto" }}>
                { isLoadingModels && <Loading message="Loading Models..." /> }
                { isLoadingModel && <Loading message={"Loading " + state.modelId} /> }
                { errorModels && <Error message="Error occurred while loading models." /> }
                { errorModel && <Error message={"Error occurred while loading the model " + state.modelId} /> }
                <Switch>
                    <Route exact path="/Admin/Models/" render={props =>
                        (!errorModels && !isLoadingModels) ? <ModelsTable 
                            {...props} 
                            {...state}                             
                            models={models} 
                            onSelectAdapterId={onSelectAdapterId}
                            reloadModels={onRefresh} 
                        /> : null
                    } />
                    <Route exact path="/Admin/Models/:adapterId" render={props =>
                        (!errorModels && !isLoadingModels) ? <ModelsTable 
                            {...props} 
                            {...state}                             
                            models={models} 
                            onSelectAdapterId={onSelectAdapterId}
                            reloadModels={onRefresh} 
                        /> : null
                    } />
                    <Route path="/Admin/Models/:adapterId/:modelId" render={props =>                    
                        (!isLoadingModel && model) ? 
                            <ModelEditor 
                                {...props} 
                                {...state} 
                                models={models} 
                                model={model} 
                                reloadModels={onRefresh}  
                            /> : null
                    } />
                    <Route component={NoRoute} />
                </Switch>
            </div>
        </div>
    );
};

export default Models;
