// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect, useRef} from "react";

import {
    Callout
} from "@afw/react";

import Graph from "../../common/Graph";

import ObjectType from "./ObjectType";

export const ModelDiagram = (props) => {

    const [nodeCalloutVisible, setNodeCalloutVisible] = useState();
    const [calloutPoint, setCalloutPoint] = useState();
    const [objectTypeObject, setObjectTypeObject] = useState();

    const objectTypeObjects = props.objectTypeObjects;

    const theGraph = useRef();

    const onDragStart = () => {
        setNodeCalloutVisible(false);
    };

    const onDeselectNode = () => {
        setNodeCalloutVisible(false);
    };

    const onSelectNode = (params) => {        

        const selectedNode = params.nodes[0];        
        const objectTypeObject = objectTypeObjects[selectedNode];

        const calloutPoint = {
            left: params.event.srcEvent.pageX,
            top: params.event.srcEvent.pageY,
        };

        setCalloutPoint(calloutPoint);
        setNodeCalloutVisible(true);        
        setObjectTypeObject(objectTypeObject);        
    };

    const [diagram, setDiagram] = useState({ nodes: [], edges: [] });
    const [options, ] = useState({
        width: "100%",
        height: "100%",
        autoResize: true,
        layout: {
            hierarchical: {
                enabled: true,
                nodeSpacing: 250,
                levelSeparation: 250,
                treeSpacing: 300,
                sortMethod: "directed",
                direction: "DU",
            },
        },
        interaction: {
            hover: true,
            dragNodes: true,
            zoomView: true,
            dragView: true,
        },
        edges: { color: "#000000" },
        nodes: { "shape": "box" }
    });
    const onStabilized = () => {
        if (!theGraph.current)
            return;

        const Network = theGraph.current.Network;
        const nodePositions = Network.getPositions();
        
        if (Object.keys(nodePositions).length > 0 && Network.layoutEngine.options.hierarchical.enabled) {
            options.layout.hierarchical = false;
            Network.setOptions(options);

            theGraph.current.Network.fit();
        }
    };
    const [events,] = useState({
        stabilized: onStabilized,
        selectNode: onSelectNode,
        deselectNode: onDeselectNode,
        dragStart: onDragStart,
    });    

    useEffect(() => {
        if (objectTypeObjects) {
            const nodeHash = {};
            const diagram = {
                nodes: [],
                edges: [],
            };
    
            if (objectTypeObjects) {
                // eslint-disable-next-line 
                for (let [path, objectTypeObject] of Object.entries(objectTypeObjects)) {
                    let node = {
                        id: path,
                        font: {
                            multi: "html",
                            face: "Segoe UI WestEuropean,Segoe UI,-apple-system,BlinkMacSystemFont,Roboto,Helvetica Neue,sans-serif",
                            
                        },
                        heightConstraint: {
                            minimum: 100,
                            valign: "top",
                        },
                        widthConstraint: {
                            maximum: 200,
                        },
                        shadow: true,
                        borderWidth: 1,
                        color: {
                            border: "#004578",
                            background: "white"
                        },
                        physics: false,
                        label: "<b>" + objectTypeObject.getObjectId() + "</b>\n\n" + objectTypeObject.getPropertyValue("description"),                    
                    };
    
                    nodeHash[path] = node;
                    diagram.nodes.push(node);
                }
    
                // eslint-disable-next-line 
                for (let [path, objectTypeObject] of Object.entries(objectTypeObjects)) {
                    let parentPaths = objectTypeObject.getResolvedParentPaths();
                    parentPaths && objectTypeObject.getResolvedParentPaths().forEach((parentPath) => {
                        if (nodeHash[parentPath]) {
                            diagram.edges.push({
                                from: objectTypeObject.getPath(),
                                to: parentPath,
                                smooth: true,
                                shadow: true,
                            });
                        }
                    });
                }
            }
    
            setDiagram(diagram);
        }

    }, [objectTypeObjects]);
    
    return (
        <>
            <Graph 
                ref={theGraph}
                style={{ height: "calc(100vh - 250px)" }}
                graph={diagram}
                options={options}
                events={events}
            />
            {
                nodeCalloutVisible &&
                    <Callout 
                        open={nodeCalloutVisible}
                        gapSpace={10}
                        isBeakVisible={true}
                        beakWidth={16}
                        targetPosition={calloutPoint}
                        setInitialFocus={true}
                        onDismiss={() => setNodeCalloutVisible(false)}
                        contains={
                            <div style={{ padding: "20px", width: "600px", height: "800px", overflow: "auto" }}>
                                <ObjectType 
                                    {...props}
                                    objectTypeObjects={Object.values(objectTypeObjects)}
                                    objectTypeObject={objectTypeObject}                                
                                />
                            </div>
                        } 
                    />
            }                
        </>
    );    
};

export default ModelDiagram;
