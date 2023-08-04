// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import vline from "../images/vline.png";
import lastnode from "../images/lastnode.png";
import node from "../images/node.png";

import {useClasses} from "../hooks";

const HierarchyStyles = theme => ({
    hierarchy: {
        listStyleType: "none",  
        background: "url(" + vline + ") repeat-y",
        margin: 0,
        padding: 0,           
        "& ul": {
            marginLeft: theme.spacing(1),                    
        },
        "& li": {
            margin: 0,
            padding: "0 12px",
            lineHeight: "20px",
            background: "url(" + node + ") no-repeat",
            "&.hierarchyLastItem": {
                background: "#fff url(" + lastnode + ") no-repeat",
            }
        },                
    },
    hierarchyLastItem: {
        background: "#fff url(" + lastnode + ") no-repeat !important",
        backgroundColor: "inherit !important"
    } 
});

export const InheritanceTree = (props) => {

    const [ancestors, setAncestors] = useState();
    const [descendants, setDescendants] = useState();
    const classes = useClasses(HierarchyStyles);

    useEffect(() => {
        const buildAncestors = (object) => {
            let ancestors = {};
            const objects = props.objects;
            
            if (object && objects) {            
                const parentPaths = object.getResolvedParentPaths();
    
                if (parentPaths) {
                    parentPaths.forEach(parentPath => {                    
                        objects.forEach(obj => {
                            if (obj.getPath() === parentPath)
                                ancestors[parentPath] = buildAncestors(obj);
                        });
                        if (!ancestors[parentPath])
                            ancestors[parentPath] = {};
                    });
                }
            }
            
            return ancestors;
        };
    
        const buildDescendants = (object) => {
            const objectPath = object.getPath();
            const objects = props.objects;
            let descendants = {};
            
            if (objects) {
                for (const childObj of objects) {
                    const parentPaths = childObj.getResolvedParentPaths();
                    if (parentPaths && parentPaths.indexOf(objectPath) >= 0) {
                        descendants[childObj.getPath()] = buildDescendants(childObj);
                    }
                }
            }
    
            return descendants;
        };
        
        setAncestors(buildAncestors(props.object));
        setDescendants(buildDescendants(props.object));
    }, [props.object, props.objects, props.direction]);
    
    const buildTree = (tree) => {
        if (!tree) 
            return <ul />;

        const onRenderPath = props.onRenderPath;
        const hierarchy = classes.hierarchy;
        const hierarchyLastItem = classes.hierarchyLastItem;
        const count = Object.keys(tree).length;

        return (
            <ul className={count > 1 ? hierarchy : hierarchy + " " + hierarchyLastItem} style={props.style}>
                {
                    Object.keys(tree).map((key, index) => {
                        const child = tree[key];
                        return (
                            <li 
                                key={key}
                                className={index === count - 1 ? hierarchyLastItem : undefined}
                                style={index === count - 1 ? {backgroundColor: "white"} : undefined}                                
                            >
                                {onRenderPath ? onRenderPath(key) : key}
                                {buildTree(child)}
                            </li>
                        );
                    })   
                }
            </ul>
        );
    };
    
    const {object, direction} = props;        

    return (
        (direction === "descendants") ?
            buildTree({ [object.getPath()]: descendants }) :
            buildTree({ [object.getPath()]: ancestors })
    );    
};

export default InheritanceTree;
