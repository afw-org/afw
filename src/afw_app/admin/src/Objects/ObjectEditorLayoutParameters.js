// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {
    Button,
    Dialog,
    ObjectResponsive, 
    useModel,
    useOperational,
    OperationalContext,
    OperationalMode
} from "@afw/react";

import {useAppCore, useTheme} from "../hooks";


/*
 * The default layout parameters for viewing objects.
 */
export const defaultLayoutParameters = (editMode, addMode, isMobile) => ({
    filterOptions: {
        filterValuedProperties: (addMode ? undefined : true),
    },
    sortOptions: {
        sortByRequired: (addMode ? true : undefined),        
        sortByPropertyName: true, 
    },    
    showMeta: false,   
    selectable: addMode ? false : true,
    showLabels: true,
    showDescriptions: true,
    readOnlyOrientation: isMobile ? "vertical" : undefined,
    preferredComponents : (editMode ? 
        [
            {
                componentProps: {
                    tooltip: false,
                    breakpoints: {
                        small: 12,
                        medium: 9,
                        large: 6,
                        xl: 5,
                        xxxl: 4,
                    }
                }
            },
            {
                dataType: "object",
                componentProps: {
                    tooltip: false,
                    breakpoints: {
                        small: 12,                                
                    }
                }
            },      
            {
                dataType: "array",
                componentProps: {
                    breakpoints: {
                        small: 12,                                
                    }
                }
            },                           
        ] : 
        [            
            {
                componentProps: {
                    tooltip: true
                }
            }
        ])                      
});

/*
 * ObjectEditorLayoutParameters
 *
 * This small component provides the user with a popup of layout view options, derived from 
 * the ObjectResponsive component.  When saved, the layout parameters are turned into JSON and used
 * as props to the actual rendering of the ObjectResponsive component.
 * 
 * Some default props are used here, based on whether the object is new or existing and some
 * useful sorting options and breakpoint values.
 */
export const ObjectEditorLayoutParameters = (props) => {

    const [layoutParametersSavable, setLayoutParametersSavable] = useState(false);
    const [layoutParameters, setLayoutParameters] = useState(props.layoutParameters);          

    const theme = useTheme();
    const {application} = useAppCore();
    const model = useModel();
    const {editable} = useOperational();

    /*
     * onLayoutParametersSavable(savable)
     *
     * Registered callback when the object we are editing becomes "savable".  This enables the
     * Save button, which was previously disabled.
     */
    const onLayoutParametersSavable = (savable) => {
        if (savable !== layoutParametersSavable)
            setLayoutParametersSavable(savable);            
    };
    
    useEffect(() => {
        const setTempLayoutParameters = async () => {       
            
            if (application) {                        
                let layoutParameters = model.newObject({
                    adaptorId: "afw", 
                    objectTypeId: "_AdaptiveLayout_ObjectResponsive", 
                    object: props.layoutParameters.toJSON()
                });
                await layoutParameters.initialize();                            
                
                layoutParameters.addEventListener("onSavable", onLayoutParametersSavable);
                setLayoutParameters(layoutParameters);            
            }
        };

        if (!props.layoutParameters)
            setTempLayoutParameters();
        else 
            layoutParameters.addEventListener("onSavable", onLayoutParametersSavable);
            
        // eslint-disable-next-line
    }, [model, props.layoutParameters, editable, application]);       
       

    /*
     * onSaveLayoutParameters()
     *
     * When the user changes the layout parameters and clicks "Save", this triggers us to set the
     * layoutParameters JSON representation and notify the parent.
     */
    const onSaveLayoutParameters = () => {        
        setLayoutParametersSavable(false);

        props.onSave(layoutParameters);
    };
        
    if (!layoutParameters)
        return null;

    return (
        <Dialog 
            open={props.open}
            isBlocking={false}
            showClose={true}
            onDismiss={props.onDismiss}     
            title="Layout Parameters"
            maxWidth="xl"
            contains={
                <div style={{ padding: theme.spacing(2) }}>
                    <OperationalContext.Provider value={OperationalMode.Editable}>
                        <ObjectResponsive 
                            filterOptions={{
                                filterExcludeProperties: [
                                    "object"
                                ]
                            }}                                                        
                            object={layoutParameters}
                        />
                    </OperationalContext.Provider>
                </div>                       
            }
            footer={
                <div style={{ display: "flex", justifyContent: "flex-end", padding: theme.spacing(2) }}>                            
                    <div style={{ marginRight: theme.spacing(1) }}>
                        <Button 
                            label="Ok"
                            color="primary"
                            variant="contained"
                            onClick={onSaveLayoutParameters}
                            disabled={!layoutParametersSavable}
                        />
                    </div>
                    <div>
                        <Button 
                            label="Cancel"                                    
                            onClick={() => {
                                layoutParameters.discardChanges();
                                props.onDismiss();
                            }}
                        />
                    </div>
                </div>
            }
        />  
    );
};

export default ObjectEditorLayoutParameters;
