// See the 'COPYING' file in the project root for licensing information.

import {
    Table,
    useRetrieveObjects,
    modelOptions_noAdaptiveObject
} from "@afw/react";

export const InterfaceTable = () => {

    const {objects : interfaces = []} = useRetrieveObjects({ 
        adaptorId: "afw", 
        objectTypeId: "_AdaptiveInterface_",
        modelOptions: modelOptions_noAdaptiveObject,
    });

    return (
        <Table
            columns={[
                {
                    name: "Name",                    
                    key: "name",
                    minWidth: 150,
                    maxWidth: 250,
                    isResizable: true,
                    fieldName: "name",
                },
                {
                    name: "Description",
                    key: "description",
                    isResizable: true,
                    fieldName: "description",
                }
            ]}
            rows={interfaces}
            selectionMode="none"
        />
    );
};

export default InterfaceTable;
