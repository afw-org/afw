// See the 'COPYING' file in the project root for licensing information.
import {useEffect, useState} from "react";

import {
    Button,   
    ObjectResponsive,
    CodeEditor,    
    Tabs,
    Typography,
    useModel
} from "@afw/react";

const useErrorObject = ({ error }) => {

    const [errorObject, setErrorObject] = useState();
    const model = useModel();

    useEffect(() => {
        const setError = async() => {
            const obj = model.newObject({
                adapterId: "afw",
                objectTypeId: "_AdaptiveError_",
                object: error,
            });

            await obj.initialize();
            setErrorObject(obj);
        };

        if (model && error)
            setError();
        else {
            setErrorObject();        
        }

    }, [model, error]);

    return { errorObject };
};

export const TestScriptOutputTabs = (props) => {

    const {object, stdout, stderr, error, source} = props;    

    const {errorObject} = useErrorObject(props);

    return (
        <Tabs 
            style={{ height: "100%", minHeight: 0 }}
            gapSpace={20}            
            tabs={[
                ...(object ? [{
                    style: { height: "100%", overflow: "auto" },
                    text: "Response",
                    key: "Response",
                    contains:     
                        <ObjectResponsive 
                            object={object}
                            filterOptions={{
                                filterValuedProperties: true,
                                filterExcludeProperties: [
                                    "source",   
                                    "error",                                     
                                ]
                            }}
                            spacing="5px"
                        />
                }] : []),
                ...((error && errorObject) ? [{
                    style: { height: "100%", overflow: "auto" },
                    text: "Error",
                    key: "Error",
                    contains:                                            
                        <ObjectResponsive 
                            object={errorObject}
                            filterOptions={{
                                filterValuedProperties: true,
                                filterExcludeProperties: [
                                    "error",
                                    "backtrace",
                                    "backtraceEvaluation"
                                ]
                            }}
                            spacing="5px"
                        />
                }] : []),
                ...(stdout ? [{
                    style: { height: "100%", overflow: "auto" },
                    text: "Stdout",
                    key: "Stdout",
                    contains:
                        <CodeEditor 
                            language="text/plain"                    
                            source={stdout}
                            readOnly={true}
                            wordWrap={true}
                            showMinimap={true}
                        />
                }] : []),
                ...(stderr ? [{
                    style: { height: "100%", overflow: "auto" },
                    text: "Stderr",
                    key: "Stderr",
                    contains:
                        <CodeEditor 
                            language="text/plain"                    
                            source={stderr}
                            readOnly={true}
                            wordWrap={true}
                            showMinimap={true}
                        />
                }] : []),
                ...(source ? [{
                    style: { height: "100%", overflow: "auto" },
                    text: "Source",
                    key: "Source",
                    contains:
                        <CodeEditor 
                            language="afw"                    
                            source={source}
                            showLineNumbers={true}
                            errorOffset={error?.parserCursor ? error?.parserCursor - 1 : error?.offset}
                            errorHoverMessage={error?.message}
                            glyphMargin={true}
                            readOnly={true}
                            wordWrap={true}
                            showMinimap={false}
                        />
                }] : []),
                ...(error?.backtraceEvaluation ? [{
                    style: { height: "100%", overflow: "auto" },
                    text: "Evaluation Backtrace",
                    key: "EvaluationBacktrace",
                    contains:
                        <CodeEditor 
                            language="afw_evaluation_backtrace"                    
                            source={error["backtraceEvaluation"]}
                            readOnly={true}
                            wordWrap={true}
                            showMinimap={false}
                        />
                }] : []),
                ...(error?.backtrace ? [{
                    style: { height: "100%", overflow: "auto" },
                    text: "Backtrace",
                    key: "Backtrace",
                    contains:
                        <CodeEditor 
                            language="afw_backtrace"                    
                            source={error?.backtrace}
                            readOnly={true}
                            wordWrap={true}
                            showMinimap={false}
                        />
                }] : []),
            ]}
        />
    );
};

const TestScriptOutput = ({ tests, size, stdout, stderr }) => {

    if (!tests)
        return null;
   
    return (        
        <Tabs 
            gapSpace={20}
            style={{ height: "100%", minHeight: 0 }}
            tabs={
                tests.map((test, index) => ({
                    style: { height: "100%", minHeight: 0 },
                    text: test.test,
                    badge: (test.passed === false) ? "!" : undefined,   
                    key: test.test + "-" + index,
                    contains:
                        <TestScriptOutputTabs                                 
                            size={size}
                            remainingSize={100}
                            object={test}
                            stdout={stdout} 
                            stderr={stderr} 
                            error={test.error} 
                            source={test.source}
                        /> 
                }))
            }
        />        
    );
};

export const Result = (props) => {

    const {
        format,
        subformat,
        size,
        result, 
        error,
        stdout, 
        stderr
    } = props;        

    const {errorObject} = useErrorObject(props);

    if (format === "script" && subformat === "test" && result) {
        let tests;

        try {
            tests = JSON.parse(result).tests;
        } catch (e) {
            // \fixme handle error here
        }

        return (
            <TestScriptOutput 
                tests={tests} 
                size={size}
                stdout={stdout}
                stderr={stderr}
            />
        );
    }


    return (
        <Tabs 
            style={{ height: "100%", minHeight: 0 }}
            gapSpace={20}
            tabs={[
                {
                    text: "Result",
                    contains:
                        <CodeEditor 
                            onCreate
                            language="json"                    
                            source={result ? result : "undefined"}
                            readOnly={true}
                            wordWrap={true}
                            showMinimap={true}
                        />
                },
                ...((error && errorObject) ? [{
                    style: { height: "100%", overflow: "auto" },
                    text: "Error",
                    key: "Error",
                    contains:                                            
                        <ObjectResponsive 
                            object={errorObject}
                            filterOptions={{
                                filterValuedProperties: true,
                                filterExcludeProperties: [
                                    "error",
                                    "backtrace",
                                    "backtraceEvaluation"
                                ]
                            }}
                            spacing="5px"
                        />
                }] : []),
                ...(error?.backtraceEvaluation ? [{
                    style: { height: "100%", overflow: "auto" },
                    text: "Evaluation Backtrace",
                    key: "EvaluationBacktrace",
                    contains:
                        <CodeEditor 
                            language="afw_evaluation_backtrace"                    
                            source={error["backtraceEvaluation"]}
                            readOnly={true}
                            wordWrap={true}
                            showMinimap={false}
                        />
                }] : []),
                ...(error?.backtrace ? [{
                    style: { height: "100%", overflow: "auto" },
                    text: "Backtrace",
                    key: "Backtrace",
                    contains:
                        <CodeEditor 
                            language="afw_backtrace"                    
                            source={error?.backtrace}
                            readOnly={true}
                            wordWrap={true}
                            showMinimap={false}
                        />
                }] : []),
                ...(stdout ? [{
                    text: "Stdout",
                    contains:
                        <CodeEditor 
                            language="text/plain"                    
                            source={stdout}
                            readOnly={true}
                            wordWrap={true}
                            showMinimap={true}
                        />
                }] : []),
                ...(stderr ? [{
                    text: "Stderr",
                    contains:
                        <CodeEditor 
                            language="text/plain"                    
                            source={stderr}
                            readOnly={true}
                            wordWrap={true}
                            showMinimap={true}
                        />
                }] : []),
            ]}
        />
    );
};

export const Listing = (props) => {

    const {listing} = props;

    return (
        <>
            <Typography size="6" text="Listing" />
            <div style={{ height: "100%", minHeight: 0 }}> 
                <CodeEditor 
                    style={{ height: "100%" }}
                    language="afw_listing"                    
                    source={listing.trim()}
                    readOnly={true}
                    wordWrap={true}
                    showMinimap={true}
                    foldingStrategy="indentation"
                />
            </div>
        </>
    );
};


/**
 * The bottom panel that displays the output from any of the Fiddle commands.
 */
export const EvaluationOutput = (props) => {

    const {
        format,
        subformat,
        listing, 
        result, 
        stdout, 
        stderr, 
        error, 
        onClear, 
        size,
        outputPanelSize,
    } = props;

    if (!result && !error && !listing)
        return <div />;

    return (
        <div 
            role="region" 
            aria-label="Evaluation Output" 
            style={{ display: "flex", flexDirection: "column", height: outputPanelSize }}
        >
            <div style={{ textAlign: "right", marginRight: "4px", marginTop: "4px" }}>
                <Button 
                    type="icon" 
                    label="Close Output"
                    tooltip="Close Output"
                    size="small"
                    icon="close" 
                    onClick={onClear}
                />
            </div>           
            <div style={{ flex: "1 1 auto", height: "100%", minHeight: 0 }}>
                {                     
                    (result || error) ?    
                        <Result 
                            format={format}                            
                            subformat={subformat}
                            size={size}
                            error={error}
                            result={result} 
                            resultDataType={result}
                            stdout={stdout} 
                            stderr={stderr} 
                        />
                        :
                        listing ?
                            <Listing 
                                listing={listing} 
                                error={error}
                            /> : null                            
                }
            </div>            
        </div>
    );
};


