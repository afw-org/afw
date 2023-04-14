// See the 'COPYING' file in the project root for licensing information.
import {useClasses} from "../../hooks";

import {Typography} from "@afw/react";

const ObjectSelectorStyles = theme => ({    
    gridListTileBar: {
        backgroundColor: theme.palette.primary.dark,
        padding: theme.spacing(1),
        borderBottomRightRadius: theme.shape.borderRadius,
        borderBottomLeftRadius: theme.shape.borderRadius,
        textAlign: "center",
        color: theme.palette.primary.contrastText,
    },
    card: {
        height: "160px",
        padding: theme.spacing(1),
        borderColor: theme.palette.primary.dark,       
        borderTopRightRadius: theme.shape.borderRadius,
        borderTopLeftRadius: theme.shape.borderRadius, 
        borderStyle: "solid",
        cursor: "pointer"
    },
    cardSelected: {
        height: "160px",
        padding: theme.spacing(1),
        borderColor: theme.palette.primary.dark,
        borderStyle: "solid",
        backgroundColor: "rgba(0, 0, 0, 0.1)",
        borderTopRightRadius: theme.shape.borderRadius,
        borderTopLeftRadius: theme.shape.borderRadius,
        cursor: "pointer"
    },
    gridListTile: {
        padding: "16px",
        borderRadius: theme.shape.borderRadius,
    },
    gridListTileSelected: {
        backgroundColor: "rgba(0, 0, 0, 0.1)",
        borderRadius: theme.shape.borderRadius,
        padding: "16px",
    },
    gridContainer: {
        width: "100%",
        display: "inline-grid",
        gridGap: theme.spacing(4),
        gridTemplateColumns: props => "repeat(" + props.cols + ", 1fr)",
        padding: theme.spacing(1),
    },            
});


export const ObjectSelector = (props) => {

    const {id, className, style, "data-testid": dataTestId, title, objects, titleProperty, descriptionProperty, selectedIndexes} = props;
    const Styles = useClasses(ObjectSelectorStyles);

    const onSelect = (object, index) => {
        if (props.onSelect)
            props.onSelect(object, index);
    };

    return (
        <div id={id} className={className} style={style} data-testid={dataTestId} data-component-type="ObjectSelector">
            <div style={{ marginTop: "16px", marginBottom: "16px" }}>
                {
                    title && <Typography text={title} component="span" />                
                }
            </div>            
            <div className={Styles.gridContainer}>            
                {
                    objects.map((object, index) =>
                        <div
                            role="button" 
                            key={index} 
                            onClick={() => onSelect(object, index)} 
                            onKeyDown={() => onSelect(object, index)}
                            tabIndex="-1"                            
                            className={(selectedIndexes && selectedIndexes.includes(index)) ? Styles.gridListTileSelected : Styles.gridListTile}
                            aria-label={object.getPropertyValue(titleProperty)}
                        >
                            <div className={(selectedIndexes && selectedIndexes.includes(index)) ? Styles.cardSelected : Styles.card}>
                                <Typography 
                                    size="3" 
                                    text={object.getPropertyValue(descriptionProperty)} 
                                />  
                            </div>                              
                            <div className={Styles.gridListTileBar}>
                                <Typography text={object.getPropertyValue(titleProperty)} component="span" />
                            </div>
                        </div>                    
                    )                        
                }
            </div>
        </div>
    );    
};

export default ObjectSelector;
