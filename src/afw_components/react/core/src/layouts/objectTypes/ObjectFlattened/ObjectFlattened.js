// See the 'COPYING' file in the project root for licensing information.
import {memo, useMemo} from "react";

import {Table, Typography, TextField} from "@afw/react";
import {useAdaptiveLayout, useOperational, useForceUpdate} from "@afw/react";


const flatten = (flattened, property, indent, index) => {
    const name = property.getName();
    const value = property.getValue();
    const label = property.getLabel();
    
    if (property.getDataType() === "object") {
        flattened.push({ name, indent, property });

        value.getProperties().filter(
            (property) => property.getValue() !== undefined
        ).forEach((property) => {
            flatten(flattened, property, indent+1);
        });
    }

    else if (property.getDataType() === "list") {
        flattened.push({ name, indent, property });

        let i = 0;
        for (const val of value) {
            if (val)
                flatten(flattened, val, indent+1, String(i));
            i++;
        }
    }

    else
        flattened.push({ name: index ? index : name, indent, label, property });
};

const ObjectFlattenedValue = ({ property }) => {
    const {editable} = useOperational();
    const forceUpdate = useForceUpdate();

    const name = property.getName();
    const dataType = property.getDataType();
    const value = property.getValue();

    if (dataType === "object" || dataType === "list")
        return null;

    if (editable) {
        /* in this view, use a TextField to edit each value */
        return (
            <TextField 
                id={"ObjectFlattenedValue-" + name}                
                value={(value !== undefined) ? String(value) : ""}
                onChanged={newValue => {                                        
                    property.setValue(newValue);
                    forceUpdate();
                }}
            />
        );
    } else {
        return (
            <Typography 
                style={{ "whiteSpace": "pre-line" }} 
                text={(value !== undefined) ? String(value) : ""}
            />
        );
    }
};

export const ObjectFlattened = memo((props) => {
    
    const { data: { object }} = useAdaptiveLayout(props);        

    const flattened = useMemo(() => {
        let flattened = [];
        if (object) {
            const properties = object.getProperties().filter(
                (property) => property.getValue() !== undefined
            ).sort( (a, b) => {
                return (a.getName().toLowerCase().localeCompare(b.getName().toLowerCase()));
            });

            /* now flatten all embedded objects and lists */            
            properties.forEach(property => {
                flatten(flattened, property, 0);
            });
        }
        return flattened;
    }, [object]);

    return (
        <Table 
            rows={flattened}
            columns={[
                {
                    key: "Property",
                    name: "Property",
                    minWidth: 50,                    
                    isResizable: true,
                    onRender: (property) => {
                        let spacer = "";
                        for (let i = 0; i < property.indent; i++)
                            spacer += "  ";

                        return (
                            <div key={property.name}>                                    
                                <Typography 
                                    color="textSecondary" 
                                    size="3"
                                    style={{ fontFamily: "roboto mono", whiteSpace: "pre-wrap" }}
                                    text={spacer ? spacer + "." + property.name : property.name} />
                            </div>
                        );
                    }
                },
                {
                    key: "Value",
                    name: "Value",
                    minWidth: 350,
                    isResizable: true,
                    isMultiline: true,
                    onRender: (property) => 
                        <ObjectFlattenedValue property={property.property} />
                }
            ]}
            compact={true}
            selectionMode="none"
        />
    );
});


export default ObjectFlattened;
