// See the 'COPYING' file in the project root for licensing information.
import {Spreadsheet} from "../../../common/Spreadsheet";

export const ModelSpreadsheet = ({ model }) => {

    const {objectTypes} = model.getPropertyValues();    
    if (!objectTypes)
        return <div data-testid="admin-admin-models-spreadsheet" />;

    const grid = objectTypes.getProperties()?.reduce( (acc, cur) => {
        const value = cur.getValue();
        if (!value)
            return [];

        return acc.concat(
            [                 
                {
                    objectType: cur.getName(),
                    required: "",
                    dataType: "",
                    propertyName: "",
                    description: value.getDescription(),                    
                },                
            ]
        ).concat(
            cur.getValue().getPropertyValue("propertyTypes").getProperties().filter(p => !p.getInheritedFrom()).map(p => 
                ({
                    objectType: "",
                    required: p.getValue()?.getPropertyValue("required") ? "true" : "",
                    propertyName: p.getName(),
                    dataType: p.getValue()?.getPropertyValue("dataType"),
                    description: p.getValue()?.getPropertyValue("description")
                })
            )
        ).concat(
            []
        );
    }, []);

    let meta = {};

    let row = 1;
    for (const ot of objectTypes.getProperties()) {
        if (ot.getValue() !== undefined) {                
            meta["A" + row] = { objectType: ot, name: "objectType" };
            row++;

            const propertyTypes = ot.getValue()?.getPropertyValue("propertyTypes");
            if (propertyTypes) {
                for (const pt of propertyTypes.getProperties().filter(p => !p.getInheritedFrom())) {
                    const m = { objectType: ot, propertyType: pt };

                    meta["B" + row] = { ...m, name: "required" };
                    meta["C" + row] = { ...m, name: "propertyName" };
                    meta["D" + row] = { ...m, name: "dataType" };
                    meta["E" + row] = { ...m, name: "description" };

                    row++;
                }
            }
        }
    }

    return (
        <div data-testid="admin-admin-models-spreadsheet">
            <Spreadsheet 
                columns={[
                    { title: "Object Type", name: "objectType", width: "15%" },
                    { title: "Required", name: "required", width: 100 },
                    { title: "Property Name", name: "propertyName", width: "15%" },
                    { title: "Data Type", name: "dataType", width: 100 },
                    { title: "Description", name: "description", width: "50%" },
                ]}
                data={grid}
                meta={meta}
                onCellChanged={(instance, cell, x, y, value, oldValue, meta) => {     
                    const {objectType, propertyType, name} = meta;

                    if (name === "objectType") {
                        objectType.setName(value);
                    } else if (name === "propertyName") {
                        propertyType.getValue().setName(value);
                    } else {
                        propertyType.getValue().setPropertyValue(name, value);
                    }
                }}
                allowInsertRow={false}
                allowManualInsertRow={false}
                allowInsertColumn={false}
                allowManualInsertColumn={false}
                allowDeleteRow={false}
                allowDeleteColumn={false}
                allowRenameColumn={false}
                columnSorting={false}            
                wordWrap
            />  
        </div>
    );
};

export default ModelSpreadsheet;
