// See the 'COPYING' file in the project root for licensing information.
import ExpressionTupleOperationObject from "./ExpressionTupleOperationObject";
import ExpressionTupleOperationList from "./ExpressionTupleOperationList";
import ExpressionTupleOperationExpression from "./ExpressionTupleOperationExpression";
import ExpressionTupleOperationLambda from "./ExpressionTupleOperationLambda";
import ExpressionTupleOperationVariable from "./ExpressionTupleOperationVariable";
import ExpressionTupleOperationCall from "./ExpressionTupleOperationCall";
import ExpressionTupleOperationCallLambda from "./ExpressionTupleOperationCallLambda";
import ExpressionTupleOperationCallAdaptive from "./ExpressionTupleOperationCallAdaptive";
import ExpressionTupleOperationProperty from "./ExpressionTupleOperationProperty";
import ExpressionTupleOperationRawExpression from "./ExpressionTupleOperationRawExpression";
import ExpressionTupleOperationTemplate from "./ExpressionTupleOperationTemplate";
import ExpressionTupleOperationScript from "./ExpressionTupleOperationScript";

/**
 * Represents the ExpressionTuple Grammar in a JSON-friendly way for easier consumption via Javascript.
 * 
 */
export const ExpressionTupleOperations = {
   
    "var": {
        label: "Variable Reference()",
        description: "",
        component: ExpressionTupleOperationVariable,        
    },

    "prop": {
        label: "Get Object Property()",
        description: "This function retrieves the value of the Property on the specified Object.",
        component: ExpressionTupleOperationProperty,        
    },

    "tmpl": {
        label: "Template()",
        description: "",
        component: ExpressionTupleOperationTemplate,        
    },

    "exprT": {
        label: "Build Expression()",
        description: "This allows you create variables, then execute an expression body.",
        component: ExpressionTupleOperationExpression,        
    },

    "lambda": {
        label: "Build Lambda Function()",
        description: "This function allows you to design a Lambda function by defining its Return type, Parameters and an Expression body.",
        component: ExpressionTupleOperationLambda,        
    },

    "call": {
        label: "Call Function()",
        description: "This function allows you to Call a Lambda or Adaptive Function that has been stored in a variable definition.  Parameters can be added to be passed along during execution.",
        component: ExpressionTupleOperationCall,        
    },

    "callL": {
        label: "Call Lambda Function()",
        description: "This function allows you to Call a Lambda function that has been stored in a variable definition or declared by an Expression.  Parameters to the lambda can be added and be passed along during execution.",
        component: ExpressionTupleOperationCallLambda,        
    },
    
    "callA": {
        label: "Call Adaptive Function()",
        description: "",
        component: ExpressionTupleOperationCallAdaptive,        
    },

    "expr": {
        label: "Raw Expression()",
        description: "",
        component: ExpressionTupleOperationRawExpression,        
    },

    "obj": {
        label: "Build Object()",
        description: "This component allows you to build an object programmatically, by defining each property.",
        component: ExpressionTupleOperationObject,        
    },

    "array": {
        label: "Build Array()",
        description: "This component allows you to build an array programmatically, by defining each item.",
        component: ExpressionTupleOperationList,
    },

    "script": {
        label: "Run Script()",
        description: "This component allows you to write an inline Adaptive Script to be executed.",
        component: ExpressionTupleOperationScript,
    },

};
