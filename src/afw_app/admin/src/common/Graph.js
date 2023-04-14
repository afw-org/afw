// See the 'COPYING' file in the project root for licensing information.

import {forwardRef, useRef, useEffect} from "react";

import "vis-network/dist/dist/vis-network.min.css";
import {Network} from "vis-network";

const defaultOptions = {
    physics: {
        stabilization: false
    },
    autoResize: false,
    edges: {
        smooth: false,
        color: "#000000",
        width: 0.5,
        arrows: {
            to: {
                enabled: true,
                scaleFactor: 0.5
            }
        }
    }
};

export const Graph = forwardRef( (props, ref) => {

    const { id, style, graph, options = defaultOptions, events = {} } = props;

    const containerRef = useRef();    
    const networkRef = useRef();

    useEffect(() => {

        ref.current = {};
        ref.current.Network = networkRef.current = new Network(
            containerRef.current,
            Object.assign({}, graph),
            options
        );

        for (let eventName of Object.keys(events)) {
            networkRef.current.on(eventName, events[eventName]);
        }

        return () => {
            networkRef.current.destroy();
        };

    }, [ref, graph, options, events]);

    return (
        <div id={id} style={style} ref={containerRef} />
    );
});

export default Graph;
