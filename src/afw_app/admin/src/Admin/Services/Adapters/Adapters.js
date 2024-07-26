// See the 'COPYING' file in the project root for licensing information.
/* eslint-disable no-use-before-define */
import {useState, useEffect, useRef} from "react";
import {Route, Switch, useRouteMatch} from "react-router";

import AdapterDetails from "./AdapterDetails";
import AdapterDetailsLmdb from "./AdapterDetailsLmdb";
import NoRoute from "../../../common/NoRoute";

import {useAppCore, useTheme} from "../../../hooks";

import {
    Callout,
    Link,
    Message,
    Table,
    Tabs
} from "@afw/react";

import Graph from "../../../common/Graph";

const AdaptersDiagram = () => {

    const theme = useTheme();
    const [nodeCalloutVisible, setNodeCalloutVisible] = useState();
    const [targetPosition, setTargetPosition] = useState();
    const [selectedNode, setSelectedNode] = useState();
    const [diagram, setDiagram] = useState({ 
        nodes: [], edges: []
    });
    const [options,] = useState({
        width: "100%",
        height: "100%",
        autoResize: true,
        layout: {
            hierarchical: {
                enabled: true,
                nodeSpacing: 150,
                levelSeparation: 250,
                treeSpacing: 200,
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
        edges: {
            color: "#004578",
            font: {
                color: "#004578"
            }
        },
        nodes: {
            "shape": "box",
        }
    });
    
    const [nodeHash, setNodeHash] = useState();

    const {adapters} = useAppCore();
    const theGraph = useRef(null);


    const onDragStart = () => {
        setNodeCalloutVisible(false);        
    };

    const onDeselectNode = () => {
        setNodeCalloutVisible(false);        
    };

    const onSelectNode = (params) => {
        const selectedNode = params.nodes[0];        
        
        const calloutPoint = {
            left: params.event.srcEvent.pageX,
            top: params.event.srcEvent.pageY,
        };

        setTargetPosition(calloutPoint);
        setSelectedNode(selectedNode);
        setNodeCalloutVisible(true);       
    };

    const onShowPopup = () => {        
        return null;
    };

    const onStabilized = () => {
        if (!theGraph.current)
            return;

        let Network = theGraph.current.Network;
        let nodePositions = Network.getPositions();
        
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
        showPopup: onShowPopup,
    });
    

    useEffect(() => {
        const databaseImage = () => {
            let image = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"  width=\"64\" height=\"64\" viewBox=\"0 0 24 24\"><path fill=\"#004578\" d=\"M12,3C7.58,3 4,4.79 4,7C4,9.21 7.58,11 12,11C16.42,11 20,9.21 20,7C20,4.79 16.42,3 12,3M4,9V12C4,14.21 7.58,16 12,16C16.42,16 20,14.21 20,12V9C20,11.21 16.42,13 12,13C7.58,13 4,11.21 4,9M4,14V17C4,19.21 7.58,21 12,21C16.42,21 20,19.21 20,17V14C20,16.21 16.42,18 12,18C7.58,18 4,16.21 4,14Z\" /></svg>";
    
            return "data:image/svg+xml;base64," + btoa(image); 
        };
    
        const filesImage = () => {
            let image = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"  width=\"64\" height=\"64\" viewBox=\"0 0 24 24\"><path fill=\"#004578\" d=\"M15,7H20.5L15,1.5V7M8,0H16L22,6V18A2,2 0 0,1 20,20H8C6.89,20 6,19.1 6,18V2A2,2 0 0,1 8,0M4,4V22H20V24H4A2,2 0 0,1 2,22V4H4Z\" /></svg>";
        
            return "data:image/svg+xml;base64," + btoa(image); 
        };
    
        const filetreeImage = () => {
            let image = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"  width=\"64\" height=\"64\" viewBox=\"0 0 24 24\"><path fill=\"#004578\" d=\"M3,3H9V7H3V3M15,10H21V14H15V10M15,17H21V21H15V17M13,13H7V18H13V20H7L5,20V9H7V11H13V13Z\" /></svg>";
    
            return "data:image/svg+xml;base64," + btoa(image); 
        };
    
        const sharevariantImage = () => {
            let image = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"  width=\"64\" height=\"64\" viewBox=\"0 0 24 24\"><path fill=\"#004578\" d=\"M18,16.08C17.24,16.08 16.56,16.38 16.04,16.85L8.91,12.7C8.96,12.47 9,12.24 9,12C9,11.76 8.96,11.53 8.91,11.3L15.96,7.19C16.5,7.69 17.21,8 18,8A3,3 0 0,0 21,5A3,3 0 0,0 18,2A3,3 0 0,0 15,5C15,5.24 15.04,5.47 15.09,5.7L8.04,9.81C7.5,9.31 6.79,9 6,9A3,3 0 0,0 3,12A3,3 0 0,0 6,15C6.79,15 7.5,14.69 8.04,14.19L15.16,18.34C15.11,18.55 15.08,18.77 15.08,19C15.08,20.61 16.39,21.91 18,21.91C19.61,21.91 20.92,20.61 20.92,19A2.92,2.92 0 0,0 18,16.08Z\" /></svg>";
            
            return "data:image/svg+xml;base64," + btoa(image); 
        };
    
        const memoryImage = () => {
            let image = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"><svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" version=\"1.1\"  width=\"64\" height=\"64\" viewBox=\"0 0 24 24\"><path fill=\"#004578\" d=\"M17,17H7V7H17M21,11V9H19V7C19,5.89 18.1,5 17,5H15V3H13V5H11V3H9V5H7C5.89,5 5,5.89 5,7V9H3V11H5V13H3V15H5V17A2,2 0 0,0 7,19H9V21H11V19H13V21H15V19H17A2,2 0 0,0 19,17V15H21V13H19V11M13,13H11V11H13M15,9H9V15H15V9Z\" /></svg>";
    
            return "data:image/svg+xml;base64," + btoa(image); 
        };

        if (adapters) {
            const nodeHash = {};
            const diagram = {
                nodes: [],
                edges: [],
            };

            adapters.forEach(adapter => {
                const {adapterId, properties} = adapter;
                const {adapterType, description} = properties;

                let image = filesImage();
                if (adapterType === "afw_runtime")
                    image = memoryImage();
                if (adapterType === "file")
                    image = filesImage();
                else if (adapterType === "model")
                    image = sharevariantImage();
                else if (adapterType === "lmdb" || adapterType.startsWith("dbd_"))
                    image = databaseImage();
                else if (adapterType === "ldap")
                    image = filetreeImage();

                let node = {
                    id: adapterId,
                    font: {
                        multi: "html",
                        face: "Segoe UI WestEuropean,Segoe UI,-apple-system,BlinkMacSystemFont,Roboto,Helvetica Neue,sans-serif",
                        size: 20,
                        color: "#004578"
                    },
                    heightConstraint: {
                        minimum: 125,
                        valign: "top",
                    },
                    widthConstraint: {
                        minimum: 100,
                        maximum: 200,
                    },
                    shadow: true,
                    borderWidth: 1,
                    color: {
                        border: "#004578",
                        background: "white"
                    },
                    physics: false,
                    shape: "image",                                        
                    image,
                    label: "<b>" + adapterId + " (" + adapterType + ")</b>",   
                    title: description,
                    adapter,
                };

                diagram.nodes.push(node);
                nodeHash[adapterId] = node;
            });

            /* build edge links */
            adapters.forEach(adapter => {
                const {adapterId, properties} = adapter;
                const {journalAdapterId, mappedAdapterId, modelLocationAdapterId} = properties;

                if (mappedAdapterId) {
                    diagram.edges.push({
                        from: adapterId,
                        to: mappedAdapterId,
                        smooth: true,
                        shadow: true,
                        label: "Maps Objects To/From"
                    });
                }

                if (journalAdapterId) {
                    diagram.edges.push({
                        from: adapterId,
                        to: journalAdapterId,
                        smooth: true,
                        shadow: true,
                        label: "Journals To"
                    });
                }

                if (modelLocationAdapterId) {
                    diagram.edges.push({
                        from: adapterId,
                        to: modelLocationAdapterId,
                        smooth: true,
                        shadow: true,
                        dashes: true,
                        label: "Model Location From"
                    });
                }
            });
            
            setDiagram(diagram);
            setNodeHash(nodeHash);            
        }
    }, [adapters]);

    return (
        <>
            <Graph 
                ref={theGraph}
                style={{ height: "100%" }}
                graph={diagram}
                options={options}
                events={events}
            />
            {
                nodeCalloutVisible &&                 
                    <Callout                           
                        targetPosition={targetPosition}    
                        onDismiss={() => setNodeCalloutVisible(false)}
                        open={nodeCalloutVisible ? true : false}
                        isBeakVisible={false}
                        gapSpace={5}
                        contains={
                            <div style={{ padding: theme.spacing(1) }}>
                                <AdapterDetails 
                                    adapter={nodeHash[selectedNode].adapter}
                                />
                            </div>
                        }
                    />
            }
        </>
    );    
};

const Adapters = () => {

    const theme = useTheme();
    const {adapters} = useAppCore();
    const match = useRouteMatch();
    
    if (!adapters)
        return null;

    return (        
        <Switch>
            <Route exact path="/Admin/Adapters/" render={(props) => 
                <div style={{ height: "100%" }}>                            
                    <Message
                        contains={
                            <div>
                                <span>To create a new Adapter, add a new Adapter Type Service </span>
                                <Link style={{ display: "inline-block" }} url="/Admin/Services/" text="here" />
                                <span>.</span>
                            </div>
                        }
                    /> 
                    <div style={{ height: theme.spacing(5) }} />
                    <Tabs                         
                        gapSpace={20}
                        tabs={[
                            {
                                text: "List",
                                contains: 
                                    <Table                                           
                                        rows={adapters}
                                        columns={[
                                            { 
                                                key: "adapterId", name: "Adapter Id", minWidth: 150, maxWidth: 200, isResizable: true,
                                                onRender: (adapter) => {                                            
                                                    let adapterId = adapter.adapterId;                                            
                                                    let url = match.url + "/" + encodeURIComponent(adapterId);
        
                                                    return (
                                                        <Link url={url} text={adapterId} />
                                                    );
                                                }
                                            },
                                            {
                                                key: "adapterType", name: "Adapter Type", minWidth: 125, maxWidth: 175, isResizable: true,
                                                onRender: (adapter) => {
                                                    return <span>{adapter.properties.adapterType}</span>;
                                                }
                                            },
                                            {
                                                key: "description", name: "Description", isResizable: true, isMultiline: true,
                                                onRender: (adapter) => {
                                                    return <span>{adapter.properties.description}</span>;
                                                }
                                            }                                   
                                        ]}                                
                                        selectionMode="none"
                                    />
                            },
                            {
                                text: "Diagram",
                                style: { height: "calc(100% - 140px)" },
                                contains: 
                                    <AdaptersDiagram 
                                        {...props}
                                    />
                            }
                        ]}
                    />    
                </div>
            }/>
            <Route path="/Admin/Adapters/:adapterId" render={(props) => {
                let selectedAdapter;

                if (!adapters)
                    return null;

                adapters.forEach((adapter) => {
                    if (adapter.adapterId === decodeURIComponent(props.match.params.adapterId))
                        selectedAdapter = adapter;
                });

                if (!selectedAdapter)
                    return null;

                return (
                    <div>         
                        {
                            (selectedAdapter.properties.adapterType === "lmdb") ?
                                <AdapterDetailsLmdb adapter={selectedAdapter} />
                                :
                                <AdapterDetails adapter={selectedAdapter} />
                        }                                                       
                    </div>
                );
            }} />
            <Route component={NoRoute} />
        </Switch>        
    );
};

export default Adapters;
