// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";

import {propTypes, defaultProps} from "./ObjectResponsive.propTypes";

import {
    Button,
    Dialog,
    TextField,
    Tooltip,
    Typography
} from "@afw/react";

import {
    useOperational, 
    useAdaptiveLayout,
    useTheme,
} from "@afw/react";

import {ObjectResponsiveToolbar} from "./ObjectResponsiveToolbar";

/**
 * ObjectResponsiveHeader
 *
 * This component renders the Header portion of an ObjectResponsive, which presents itself above the Object.
 * This Header may contain the property name (if embedded), and a toolbar for adding/removing properties
 * from the object inside the form.
 */
export const ObjectResponsiveHeader = (props) => {

    const {tooltip, description, onRenderHeader} = props;
    const [showEditName, setShowEditName] = useState(false);
    const theme = useTheme();
    const {editable} = useOperational(props);
    const {data: { object, property}, layoutParameters} = useAdaptiveLayout(props, propTypes, defaultProps);
    const [propertyName, setPropertyName] = useState( property ? property.getName() : "");
    
    const label = props.label || property?.getLabel();

    if (showEditName) {
        return (
            <Dialog
                isBlocking={false}
                open={Boolean(showEditName)}
                showClose={true}
                title="Set Property Name"
                maxWidth="sm"
                contains={
                    <div 
                        role="button"
                        tabIndex={-1}
                        onClick={event => event.stopPropagation()}
                        onKeyPress={event => event.stopPropagation()}
                    >
                        <TextField
                            // eslint-disable-next-line jsx-a11y/no-autofocus
                            autoFocus={true}
                            label="Property Name"
                            description="Set the property name for this object."
                            value={propertyName}
                            onChanged={newName => setPropertyName(newName)}
                        />
                        <div style={{ height: theme.spacing(1) }} />
                        <div style={{ display: "flex", justifyContent: "flex-end" }}>
                            <Button
                                style={{ marginRight: theme.spacing(0.5) }}
                                label="Cancel"
                                onClick={() => {
                                    setShowEditName(false);
                                }}
                            />
                            <Button
                                label="Save"
                                color="primary"
                                variant="contained"
                                onClick={() => {
                                    property.setName(propertyName);
                                    setShowEditName(false);
                                }}
                            />                            
                        </div>
                    </div>
                }
                onDismiss={() => setShowEditName(false)}
            />
        );
    }

    let labelComponent;
    if (property && editable && property.isOtherPropertyType()) {
        labelComponent = (
            <div style={{ display: "flex", alignItems: "center", marginRight: theme.spacing(1) }}>
                <Typography
                    style={{ marginRight: theme.spacing(1) }}
                    text={label ? label : propertyName}
                    size="6"
                    component="span"
                />
                <Button
                    type="icon"
                    icon="edit"
                    label="Edit Property Name"
                    tooltip="Edit Property Name"
                    onClick={event => {
                        event.stopPropagation();
                        setShowEditName(true);
                    }}
                />
            </div>
        );
    } else {
        labelComponent = <Typography text={label ? label : propertyName} size="6" component="span" color="primary" />;
    }

    /* create a header above the object properties that contains the label and description */
    let renderedHeader = (propertyName || label || description || editable) ? (
        <div>
            <div style={{ display: "flex", alignItems: "center", paddingRight: theme.spacing(3) }}>                
                { labelComponent }                
                <div>
                    <ObjectResponsiveToolbar
                        object={object}
                        canDelete={props.canDeleteProperties}
                        onAdd={props.onAddProperty}
                        onDelete={props.onDeleteSelectedProperties}
                    />
                </div>
            </div>
            <div>
                <Typography
                    size="1"
                    text={description || ""}
                />
            </div>
        </div>
    ) : <div />;

    if (onRenderHeader) {              
        renderedHeader = onRenderHeader({ ...layoutParameters, ...props }, renderedHeader);
    } else {
        /* check the header props and nestingDepth to determine if we should display the header or not */
        if (!props.isHeaderVisible)
            return null;
    }

    if (tooltip && property) {
        let tooltipContent = property.getName() + ": " + property.getDataType();
        if (property.getDataTypeParameter())
            tooltipContent += " " + property.getDataTypeParameter();

        const brief = property.getBrief();
        const description = property.getDescription();

        renderedHeader = (
            <Tooltip
                contains={
                    <>
                        <Typography size="2" text={tooltipContent} />
                        <div style={{ height: theme.spacing(1) }} />
                        <Typography size="1" text={brief} />
                        <div style={{ height: theme.spacing(1) }} />
                        <Typography size="1" text={description} />
                    </>
                }
                delay={2000}
                target={renderedHeader}
            />
        );
    }

    return renderedHeader;
};
