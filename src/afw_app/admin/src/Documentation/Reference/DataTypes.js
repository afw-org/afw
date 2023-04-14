// See the 'COPYING' file in the project root for licensing information.
import {useRouteMatch} from "react-router";

import {
    Checkbox,
    Link,
    Message,
    Spinner,
    Table,
    Typography,
    useDataTypes, 
    useFunctions,
} from "@afw/react";

import {useTheme} from "../../hooks";

export const DataTypesTable = () => {

    const {dataTypes} = useDataTypes();
    const sortedDataTypes = dataTypes?.sort((a, b) => a.dataType.localeCompare(b.dataType)) || [];

    return (
        <Table 
            columns={[
                {
                    key: "DataType",
                    name: "Data Type",
                    minWidth: 150,
                    maxWidth: 250,
                    isResizable: true,
                    style: { wordWrap: "break-word" },
                    onRender: (d) => {
                        const dataType = d.dataType;
                        return (
                            <Link 
                                url={"/Documentation/Reference/DataTypes/" + encodeURIComponent(dataType)} 
                                text={dataType} 
                            />
                        );
                    }
                },
                {
                    key: "Description",
                    name: "Description",
                    minWidth: 200,
                    isResizable: true,
                    isMultiline: true,
                    style: { wordWrap: "break-word" },
                    onRender: (dataType) => {
                        return (
                            <Typography text={dataType.brief} />
                        );
                    }
                },
            ]}
            rows={sortedDataTypes}
            selectionMode="none"
            compact={true}
        />
    );
};

export const DataType = (props) => {

    const theme = useTheme();
    const {dataTypes} = useDataTypes();
    const {functions} = useFunctions();
    const match = useRouteMatch();

    let dataTypeId = props.dataTypeId;

    if (!dataTypeId) dataTypeId = decodeURIComponent(match.params.dataTypeId);

    let dataType = null;
    if (!dataTypes || !functions) 
        return null;

    dataTypes.forEach((d) => {
        if (d.dataType === dataTypeId) 
            dataType = d;
    });

    if (!dataType)
        return <Typography text={"DataType " + dataTypeId + " not found."} />;

    const {
        description,
        ldapOid,
        jsonPrimitive,
        cType,
        scalar,
        directReturn,
        evaluated,
        xacmlDataType,
        dataTypeParameterType,
        jsonDataTypeNeeded,
        relationalCompares,
    } = dataType;

    return (
        <div
            data-testid={"admin-documentation-reference-dataType-" + dataTypeId}
            id={"admin-documentation-reference-dataType-" + dataTypeId}
        >
            <div>
                <Typography size="8" color="primary" text={dataTypeId} />
            </div>
            <div style={{ padding: theme.spacing(2) }}>
                <div style={{ paddingBottom: theme.spacing(2) }}>
                    <Typography text={description} />
                </div>
                <div style={{ display: "flex" }}>
                    <div>                        
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <div style={{ paddingBottom: theme.spacing(2) }}>
                                <Typography
                                    size="6"
                                    color="primary"
                                    text="Object Identifier (OID)"
                                />
                            </div>
                            <div style={{ paddingLeft: theme.spacing(2) }}>
                                { ldapOid ?
                                    <Typography text={ldapOid} /> : (                                        
                                        <Typography text="None" />                                        
                                    )
                                }
                            </div>
                        </div>                         
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <div style={{ paddingBottom: theme.spacing(2) }}>
                                <Typography
                                    size="6"
                                    color="primary"
                                    text="XACML Data Type"
                                />
                            </div>
                            <div style={{ paddingLeft: theme.spacing(2) }}>
                                {xacmlDataType ? (
                                    <Link
                                        url={xacmlDataType}
                                        text={xacmlDataType}
                                    />
                                ) : (
                                    <Typography text="None" />
                                )}
                            </div>
                        </div>
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <div style={{ paddingBottom: theme.spacing(2) }}>
                                <Typography
                                    size="6"
                                    color="primary"
                                    text="JSON Primitive"
                                />
                            </div>
                            <div style={{ paddingLeft: theme.spacing(2) }}>
                                <Typography text={jsonPrimitive} />
                            </div>
                        </div>
                        <div style={{ paddingBottom: theme.spacing(2) }}>
                            <div style={{ paddingBottom: theme.spacing(2) }}>
                                <Typography
                                    size="6"
                                    color="primary"
                                    text="C Type"
                                />
                            </div>
                            <div style={{ paddingLeft: theme.spacing(2) }}>
                                <Typography text={cType} />
                            </div>
                        </div>
                        {dataTypeParameterType && (
                            <div style={{ paddingBottom: theme.spacing(2) }}>
                                <div style={{ paddingBottom: theme.spacing(2) }}>
                                    <Typography
                                        size="6"
                                        color="primary"
                                        text="Data Type Parameter Type"
                                    />
                                </div>
                                <div style={{ paddingLeft: theme.spacing(2) }}>
                                    <Typography text={dataTypeParameterType} />
                                </div>
                            </div>
                        )}
                    </div>
                    <div style={{ marginLeft: theme.spacing(8) }}>
                        <div>
                            <Checkbox
                                label="Scalar"
                                disabled={true}
                                value={scalar}
                            />
                        </div>
                        <div>
                            <Checkbox
                                label="Direct Return"
                                disabled={true}
                                value={directReturn}
                            />
                        </div>
                        <div>
                            <Checkbox
                                label="Evaluated"
                                disabled={true}
                                value={evaluated}
                            />
                        </div>
                        <div>
                            <Checkbox
                                label="Relational Compares"
                                disabled={true}
                                value={relationalCompares}
                            />
                        </div>
                        <div>
                            <Checkbox
                                label="JSON Data Type Needed"
                                disabled={true}
                                value={jsonDataTypeNeeded}
                            />
                        </div>
                    </div>
                </div>
            </div>
            <div style={{ paddingBottom: theme.spacing(2) }}>
                <div style={{ paddingBottom: theme.spacing(2) }}>
                    <Typography
                        size="6"
                        color="primary"
                        text="Related Functions"
                    />
                </div>
                <ul>
                    {functions &&
                        functions.map((f, index) => {
                            const { category, functionId } = f;
                            if (category === dataTypeId) {
                                return (
                                    <li key={index}>
                                        <Link
                                            style={{
                                                fontFamily: "roboto mono",
                                            }}
                                            url={
                                                "/Documentation/Reference/Functions/" +
                                                encodeURIComponent(dataTypeId) +
                                                "/" +
                                                encodeURIComponent(functionId)
                                            }
                                            text={functionId}
                                        />
                                    </li>
                                );
                            }

                            return null;
                        })}
                </ul>
            </div>
        </div>
    );
};

export const DataTypes = () => {

    const {dataTypes} = useDataTypes();
    const theme = useTheme();

    if (!dataTypes)
        return <Spinner text="Loading DataTypes.." size="large" fullScreen />;

    return (
        <>
            <Message 
                message="Adaptive Framework provides Data Types, which can be referenced by properties,
                functions, and expressions.  These data types govern how values are represented in various content forms
                and converted from one data type to another."
            />
            <div style={{ height: theme.spacing(3) }} />
            <DataTypesTable />                             
        </>
    );
};

export default DataTypes;
