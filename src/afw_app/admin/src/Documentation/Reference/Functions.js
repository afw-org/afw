// See the 'COPYING' file in the project root for licensing information.
import React, {useMemo, useState} from "react";
import {Switch, Route, useRouteMatch, useHistory} from "react-router";

import {    
    Autocomplete,
    Button,
    Link,
    Message,        
    Tabs,
    Table,
    Spinner,
    Typography,
    useFunctions,
    useFunctionCategories,
} from "@afw/react";

import {useAppCore, useTheme} from "../../hooks";

const Section = ({ title, children }) => {
    const theme = useTheme();

    if (!children)
        return null;

    return (
        <div style={{ marginBottom: theme.spacing(3) }}>
            { title &&
                <Typography 
                    style={{ marginBottom: theme.spacing(1) }} 
                    size="6" color="primary" 
                    text={title}
                />
            }
            {children}
        </div>
    );
};

const buildRequestSyntax = (functionId, parameters) => {
    let requestSyntax = {
        adapterId: "afw",
        function: functionId,
    };

    parameters.forEach(parameter => {
        const param = parameter;
        const { allowSingle, dataType, name } = param;
        
        if (allowSingle) {
            if (dataType === "object")
                requestSyntax[name] = {};
            else
                requestSyntax[name] = dataType ? dataType : "";
        } else {
            requestSyntax[name] = [];
        }
    });

    return requestSyntax;
};

const buildResponseSyntax = (functionId, returns) => {
    let responseSyntax = {            
        status: "success"
    };

    const {allowSingle, dataType} = returns;

    if (allowSingle) {
        if (dataType === "object")
            responseSyntax["result"] = {};
        else
            responseSyntax["result"] = dataType ? dataType : "";
    } else {
        responseSyntax["result"] = [];            
    }

    return responseSyntax;
};

const FunctionParameters = ({ parameters }) => {
         
    const theme = useTheme();       

    return (            
        <div style={{ marginLeft: theme.spacing(4), marginBottom: theme.spacing(1) }}>                
            {
                parameters.map((parameter, index) => {
                    const param = parameter;
                    const {name, description} = param;                   
                   
                    return (
                        <div key={index} style={{ marginBottom: theme.spacing(4) }}>
                            <Typography size="5" color="textSecondary" text={name} style={{ display: "inline-block" }} />                            
                            <div style={{ marginTop: theme.spacing(1), marginLeft: theme.spacing(2) }}>
                                <Typography 
                                    style={{ marginBottom: theme.spacing(1), whiteSpace: "pre-line" }}
                                    text={(description || "")}
                                />
                            </div>
                        </div>
                    );
                })
            }
        </div>
    );
};


// eslint-disable-next-line no-unused-vars
const FunctionExamples = ({ functionId, parameters, returns }) => {
    const theme = useTheme();

    const requestSyntax = buildRequestSyntax(functionId, parameters);
    const responseSyntax = buildResponseSyntax(functionId, returns);

    return (
        <>
            <Typography size="6" color="primary" text="Examples" />
            <div style={{ height: theme.spacing(2) }} />
            <Tabs
                gapSpace={20}
                tabs={[
                    {
                        text: "Expression",
                        contains: 
                            <div style={{ padding: theme.spacing(2), backgroundColor: theme.palette.background.paper }}>                                            
                                <Typography color="inherit" size="5" style={{ whiteSpace: "pre", fontFamily: "Roboto mono" }} text={functionId + "()"} />                                                                                 
                            </div>
                    },
                    {
                        text: "HTTP",
                        contains: 
                            <div>
                                <div style={{ paddingBottom: theme.spacing(3) }}>
                                    <Typography size="6" color="primary" text="Request Syntax" />
                                    <div style={{ height: theme.spacing(1) }} />
                                    <div style={{ padding: theme.spacing(2), backgroundColor: theme.palette.background.paper }}>                                                 
                                        <Typography 
                                            color="inherit"
                                            size="5"
                                            style={{ whiteSpace: "pre", fontFamily: "Roboto mono" }} 
                                            text={JSON.stringify(requestSyntax, null, 4)} 
                                        />                                                    
                                    </div>
                                </div>
                                <div style={{ paddingBottom: theme.spacing(3) }}>
                                    <Typography size="6" color="primary" text="Response Syntax" />
                                    <div style={{ height: theme.spacing(1) }} />
                                    <div style={{ padding: theme.spacing(2), backgroundColor: theme.palette.background.paper }}>
                                        <Typography 
                                            color="inherit"
                                            size="5"
                                            style={{ whiteSpace: "pre", fontFamily: "Roboto mono" }}
                                            text={JSON.stringify(responseSyntax, null, 4)} 
                                        />                                                
                                    </div>
                                </div>
                            </div>
                    }
                ]}
            />
        </>
    );
};

const FunctionRelated = ({ category, functions, functionId }) => {

    const theme = useTheme();

    return (
        <div style={{ paddingBottom: theme.spacing(3) }}>
            <Typography size="6" color="primary" text="Related Functions" />
            <div style={{ height: theme.spacing(2) }} />
            <Typography 
                style={{ display: "inline-block", paddingRight: theme.spacing(0.5) }} 
                component="span"
                text="This function belongs to the category" />
            <Link 
                style={{ display: "inline-block", fontFamily: "roboto mono" }}
                url={"/Documentation/Reference/Functions/" + encodeURIComponent(category)} 
                text={category} 
            />
            <ul>
                {
                    functions.map((f, index) => {
                        if (f.category === category && f.functionId !== functionId) {
                            return (
                                <li key={index}>
                                    <Link 
                                        key={index}
                                        style={{ fontFamily: "roboto mono" }} 
                                        url={"/Documentation/Reference/Functions/" + encodeURIComponent(category) + "/" + encodeURIComponent(f.functionId)} 
                                        text={f.functionId} 
                                    />
                                </li>
                            );
                        } else return null;
                    })
                }
            </ul>
        </div>
    );
};

const FunctionPolymorphic = ({ polymorphicDataTypes }) => {
    const theme = useTheme();

    return (
        <div>
            <Typography size="5" text="For this function, &lt;dataType&gt; may be one of the following: " />        
            <ul>    
                {
                    polymorphicDataTypes.map((dataType, index) =>                         
                        <li key={index}>                  
                            <Link 
                                style={{ fontFamily: "roboto mono", marginLeft: theme.spacing(1) }} 
                                url={"/Documentation/Reference/DataTypes/" + encodeURIComponent(dataType)} 
                                text={dataType} 
                            />                                    
                        </li>                                           
                    )
                }
            </ul>
        </div>
    );
};

export const FunctionCategoriesTable = () => {

    const {categories} = useFunctionCategories();         

    const sortedCategories = useMemo(() => {
        if (categories) {
            return categories.sort((cat1, cat2) =>
                cat1.category.toLowerCase().localeCompare(cat2.category.toLowerCase())
            );
        } else return [];
    }, [categories]);

    return (
        <Table 
            columns={[
                {
                    key: "Category",
                    name: "Category",
                    minWidth: 100,
                    maxWidth: 200,
                    isResizable: true,
                    style: { wordWrap: "break-word" },
                    onRender: (c) => {
                        const category = c.category;
                        return (
                            <Link 
                                url={"/Documentation/Reference/Functions/" + encodeURIComponent(category)} 
                                text={category} 
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
                    onRender: (c) => {
                        const brief = c.brief;
                        return (
                            <Typography text={brief ? brief : ""} />
                        );
                    }
                },
            ]}
            rows={sortedCategories.filter(category => category.dataTypeCategory !== true)}
            selectionMode="none"
            compact={true}
        /> 
    );
};

export const Function = (props) => {    

    const {functions} = useAppCore();
    const theme = useTheme();
    const match = useRouteMatch();

    let functionId = props.functionId;    

    if (!functions)
        return null;

    if (!functionId)
        functionId = decodeURIComponent(match.params.functionId);
    
    let func;
    functions.forEach(f => {
        if (f.functionId === functionId)
            func = f;
    });
    
    if (!func)
        return <Typography size="6" text={"Function " + functionId + " not found."} />;

    const {
        description, category, deprecated, op,
        polymorphic, polymorphicDataTypes, returns,
        parameters, sideEffects, pure, 
        functionDeclaration, errorsThrown
    } = func;    

    return (
        <div id={"admin-documentation-reference-function-" + functionId} data-testid={"admin-documentation-reference-function-" + functionId}>
            <Section>
                <Typography size="8" component="h2" text={functionId} />
            </Section>
            <Section>
                <Typography text={description} />
            </Section>
            <Section>
                <div style={{ padding: theme.spacing(2), backgroundColor: theme.palette.background.paper }}>
                    <Typography 
                        style={{ whiteSpace: "pre-wrap", fontFamily: "roboto mono" }} 
                        text={functionDeclaration} 
                    />
                </div>
            </Section>
            <Section>
                {
                    (polymorphic && polymorphicDataTypes) &&
                    <FunctionPolymorphic 
                        polymorphicDataTypes={polymorphicDataTypes}
                    />
                }    
            </Section>
            <Section>
                {
                    deprecated &&
                        <Typography size="5" color="error" text="This function has been deprecated." />
                }
            </Section>                       
            <Section title="Parameters">                
                <FunctionParameters parameters={parameters} />
            </Section>                
            <Section title="Returns">                
                <Typography 
                    size="5" 
                    style={{ paddingTop: theme.spacing(1), whiteSpace: "pre-line" }} 
                    text={(returns.description || "(None)")}                            
                />
            </Section>  
            <Section title="Operator">
                {
                    op && 
                        <Typography text={op} />
                }
            </Section>                  
            <Section title="Side Effects">
                {
                    sideEffects ? (
                        sideEffects.map((effect, index) => <Typography key={index} size="5" text={effect} />)
                    ) : <Typography size="5" text="This function has no side effects." />
                }
            </Section>            
            <Section title="Errors Thrown">                        
                <Typography 
                    size="5" style={{ whiteSpace: "pre-wrap" }}
                    text={
                        errorsThrown ? JSON.stringify(errorsThrown, null, 4) : "(None)"
                    } 
                />
            </Section>                            
            <Section title="Pure">
                {
                    pure ? 
                        <Typography size="5" text="This is a pure function." /> :
                        <Typography size="5" text="This is not a pure function." />
                }
            </Section>                 
            <Section>
                { /* \fixme decide how to handle this later 
                <FunctionExamples 
                    functionId={functionId} 
                    parameters={parameters} 
                    returns={returns} 
                />
                */
                }
            </Section>
            <Section>
                <FunctionRelated 
                    category={category}
                    functions={functions}
                    functionId={functionId}
                />
            </Section>            
        </div>
    );

};

const FunctionCategory = (props) => {

    let category = decodeURIComponent(props.match.params.category);

    const {categories} = useFunctionCategories();
    const {functions} = useFunctions();

    if (!categories)
        return <Spinner text="Loading Functions and Categories.." size="large" fullScreen />;

    let description;
    for (const cat of categories) {
        if (cat.category === category) {
            description = cat.description;
            break;
        }
    }

    return (
        <div>   
            <Typography size="7" text={"Function Category " + category} />
            <p>{description}</p>
            <Table 
                columns={[
                    {
                        key: "FunctionId",
                        name: "FunctionId",
                        minWidth: 200,
                        maxWidth: 250,
                        isResizable: true,
                        style: { wordWrap: "break-word" },
                        onRender: (func) => {
                            const {functionId} = func;
                            return (
                                <Link 
                                    uriComponents={[ "Documentation", "Reference", "Functions", category, functionId ]} 
                                    text={functionId} 
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
                        onRender: (func) => {
                            const {brief} = func;
                            return (
                                <Typography text={brief ? brief : ""} />
                            );
                        }
                    },
                ]}
                rows={functions.filter(func => func.category === category)}
                selectionMode="none"
                compact={true}
            />
        </div>
    );  
};

export const Functions = () => {

    const theme = useTheme();

    const history = useHistory();
    const {functions} = useFunctions();
    const {categories} = useFunctionCategories();    
    const [selectedFunction, setSelectedFunction] = useState();    

    const functionOptions = useMemo(() => {
        if (functions) {
            const functionOptions = functions.map((func) => {
                const {functionId, category} = func;
                return ({
                    key: functionId,
                    text: functionId,
                    category,
                    func,                    
                });
            });
            
            return functionOptions;
        } else return [];
    }, [functions]);

    const sortedCategories = useMemo(() => {
        if (categories) {
            return categories.sort((cat1, cat2) =>
                cat1.category.toLowerCase().localeCompare(cat2.category.toLowerCase())
            );
        } else return [];
    }, [categories]);

    if (!categories || !functions)
        return <Spinner text="Loading Functions and Categories.." size="large" fullScreen />;

    return (
        <Switch>
            <Route exact path="/Documentation/Reference/Functions" render={() =>
                <>                                
                    <div style={{ display: "flex", justifyContent: "flex-end" }}>
                        <div style={{ minWidth: "225px", alignItems: "center", marginRight: theme.spacing(1) }}>
                            <Autocomplete 
                                placeholder="Find Function..."
                                noOptionsText="No Function"                                        
                                options={functionOptions}
                                onChanged={setSelectedFunction}
                            />
                        </div>
                        <div>
                            <Button                                  
                                type="icon"
                                label="Goto Function"
                                tooltip="Goto Function"
                                icon="keyboard_arrow_right"
                                disabled={!selectedFunction}
                                onClick={() => history.push("/Documentation/Reference/Functions/" + selectedFunction.category + "/" + selectedFunction.text)}
                            />
                        </div>
                    </div>
                    <div style={{ height: theme.spacing(3) }} />                            
                    <Typography size="7" text="Function Categories" />                            
                    <div style={{ height: theme.spacing(3) }} />
                    <Message 
                        message="Function Categories organize Adaptive Functions together in groups, according to their functionality."
                    />
                    <FunctionCategoriesTable />
                    <div style={{ height: theme.spacing(3) }} />                                  
                    <Typography size="7" text="Data Type Categories" />                            
                    <div style={{ height: theme.spacing(3) }} />
                    <Message 
                        message="Each Data Type also has Adaptive Functions which can perform operations on their data."
                    />
                    <Table 
                        columns={[
                            {
                                key: "Category",
                                name: "Category",
                                minWidth: 150,
                                maxWidth: 200,
                                isResizable: true,
                                style: { wordWrap: "break-word" },
                                onRender: (c) => {
                                    const {category} = c;
                                    return (
                                        <Link 
                                            url={"/Documentation/Reference/Functions/" + encodeURIComponent(category)} 
                                            text={category} 
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
                                onRender: (category) => {
                                    return (
                                        <Typography text={category.brief} />
                                    );
                                }
                            },
                        ]}
                        rows={sortedCategories.filter(category => category.dataTypeCategory === true)}
                        selectionMode="none"
                        compact={true}
                    />                              
                </>
            } />
            <Route exact path="/Documentation/Reference/Functions/:category" component={FunctionCategory} />    
            <Route exact path="/Documentation/Reference/Functions/:category/:functionId" component={Function} />                        
        </Switch>
    );
};

export default Functions;
