// See the 'COPYING' file in the project root for licensing information.
import {   
    Typography, 
    Table
} from "@afw/react";

import {useOperational} from "@afw/react";

const ReadonlyQualifiedVariables = (props) => {
    const {property} = props;

    if (property.getValue() === undefined)
        return <Typography text="No Qualified Variables defined." />;

    const qualifiedVariables = property.getValue().toJSON();

    return (
        <div>
            <Typography color="primary" size="7" text="Qualified Variables" />
            <div style={{ marginLeft: "32px", marginTop: "8px" }}>
                {
                    Object.entries(qualifiedVariables).map(([qualifier, variables]) =>
                        <div key={qualifier} style={{ marginTop: "8px" }}>
                            <Typography color="primary" size="6" text={qualifier} />
                            <div style={{ marginLeft: "32px", marginTop: "8px" }}>
                                <Table 
                                    selectionMode="none"
                                    compact={true}
                                    columns={[
                                        {
                                            key: "variable",
                                            style: { wordWrap: "break-word" },
                                            name: "Variable",
                                            onRender: ([_var]) => <Typography color="textSecondary" text={_var} />,
                                        },
                                        {
                                            key: "value",
                                            style: { wordWrap: "break-word" },
                                            name: "Value",
                                            onRender: ([, val]) => <Typography text={val} />
                                        }
                                    ]}
                                    rows={Object.entries(variables)}                                   
                                />                                                             
                            </div>
                        </div>                        
                    )
                }
            </div>
        </div>
    );
};

export const QualifiedVariables = (props) => {

    const {property} = props;
    const {editable} = useOperational(props);
    const qualifiedVariables = property?.getValue()?.toJSON() || [];

    if (editable !== true)
        return <ReadonlyQualifiedVariables {...props} />;        

    return (
        <div>
            <Typography color="primary" size="7" text="Qualified Variables" />
            <div style={{ marginLeft: "32px", marginTop: "8px" }}>
                {
                    Object.entries(qualifiedVariables).map(([qualifier, variables]) =>
                        <div key={qualifier}>
                            <Typography color="primary" size="6" text={qualifier} />
                            <div style={{ marginLeft: "32px", marginTop: "8px" }}>
                                <Table 
                                    selectionMode="none"
                                    compact={true}
                                    columns={[
                                        {
                                            key: "variable",
                                            name: "Variable",
                                            onRender: ([_var]) => <Typography text={_var} />
                                        },
                                        {
                                            key: "value",
                                            name: "Value",
                                            onRender: ([, val]) => <Typography text={val} />
                                        }
                                    ]}
                                    rows={Object.entries(variables)}                                   
                                />                                                             
                            </div>
                        </div>                        
                    )
                }
            </div>
        </div>
    );
};

export default QualifiedVariables;
