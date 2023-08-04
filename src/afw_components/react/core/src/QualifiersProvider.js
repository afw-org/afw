// See the 'COPYING' file in the project root for licensing information.
import {useMemo} from "react";

import {useContextTypes} from "@afw/react";
import {QualifiersContext} from "@afw/react";


/**
 * QualifiersProvider
 * 
 * This component provides Qualifiers based on the requested 
 * contextTypes.
 */
export const QualifiersProvider = ({ contextTypes, children }) => {
    const {contextTypes: allContextTypes} = useContextTypes();

    const qualifiers = useMemo(() => {
        /* filter our list by the contextTypes that we're interested in */
        let qualifiers = [];
        if (allContextTypes && contextTypes) { 
            allContextTypes.forEach(({ qualifierDefinitions, contextTypeId }) => {                
                if (!contextTypes.includes(contextTypeId))
                    return;
                    
                if (qualifierDefinitions) {                
                    Object.entries(qualifierDefinitions).forEach(([qualifierName, qualifierDef]) => {   
                        if (qualifierName !== "_meta_")
                            qualifiers.push({ qualifierName, qualifierDef });
                    });
                }
            });
            
            return qualifiers;
        }
    }, [allContextTypes, contextTypes]);

    return (
        <QualifiersContext.Provider value={qualifiers}>
            { children }
        </QualifiersContext.Provider>
    );
};

export default QualifiersProvider;
