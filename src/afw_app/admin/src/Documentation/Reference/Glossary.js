// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useState} from "react";

import {
    Table,
    Typography
} from "@afw/react";

import {useTheme} from "../../hooks";

const getChildElementsByName = (element, nodeName) => {
    let nodes = [];

    element.childNodes.forEach(node => {
        if (!nodeName || node.nodeName === nodeName)
            nodes.push(node);
    });

    return nodes;
};

const getFirstChildElementByName = (element, nodeName) => {
    const nodes = getChildElementsByName(element, nodeName);
    
    return (nodes.length > 0) ? nodes[0] : null;
};

const getChildValue = (element, defaultValue = null) => {    
    if (!element)
        return defaultValue;

    return Array.from(element.childNodes).map((e) => {        
        if (e.nodeType === e.TEXT_NODE)
            return (e.nodeValue);
        else 
            return defaultValue;
    });
};

export const Glossary = () => {
    
    const theme = useTheme();
    const [glossary, setGlossary] = useState();
    const [terms, setTerms] = useState([]);
    
    useEffect(() => {
        const controller = new AbortController();

        const fetchGlossary = async () => {
            const response = await fetch("/doc/Glossary.xml", { signal: controller.signal });
            const data = await response.text();

            if (!controller.signal.aborted) {
                const doc = new DOMParser().parseFromString(data, "application/xml");
                const glossary = doc.documentElement;
                
                setGlossary(glossary);
            }
        };

        fetchGlossary();

        return () => {
            controller.abort();
        };
    }, []);

    useEffect(() => {
        if (glossary) {
            let terms = getChildElementsByName(glossary, "Term");

            terms = terms.sort((a, b) => {
                let aIdentifier = getFirstChildElementByName(a, "Identifier").childNodes[0].nodeValue;
                let bIdentifier = getFirstChildElementByName(b, "Identifier").childNodes[0].nodeValue;

                return (aIdentifier.toLowerCase().localeCompare(bIdentifier.toLowerCase()));
            });

            setTerms(terms);
        }
    }, [glossary]);

    return (
        <div style={{ padding: theme.spacing(1) }}>
            <Typography size="9" color="primary" text="Glossary" />
            <div style={{ height: theme.spacing(2) }} />
            <Table 
                compact={true}
                columns={[
                    {
                        key: "Term",
                        name: "Term",
                        width: "20%",
                        minWidth: 200,
                        maxWidth: 250,
                        isResizable: true,
                        onRender: (term) => {
                            const identifier = getFirstChildElementByName(term, "Identifier");
                            const identifierText = getChildValue(identifier);
                            return (
                                <Typography size="5" text={identifierText ? identifierText[0] : ""} />
                            );
                        }
                    },
                    {
                        key: "Definition",
                        name: "Definition",
                        minWidth: 200,
                        maxWidth: 400,
                        isResizable: true,
                        isMultiline: true,
                        onRender: (term) => {
                            const definition = getFirstChildElementByName(term, "Definition");
                            const definitionText = getChildValue(definition);

                            return (
                                <Typography size="3" text={definitionText ? definitionText[0] : ""} color="textSecondary" />
                            );
                        }
                    }
                ]}
                rows={terms}
                selectionMode="none"
            />
        </div>
    );
};

export default Glossary;
