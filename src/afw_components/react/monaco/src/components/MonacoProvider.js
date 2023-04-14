// See the 'COPYING' file in the project root for licensing information.
import {useState, useEffect} from "react";

import {
    useFunctions,
} from "@afw/react";

import {MonacoContext} from "../context";
import {initialize} from "..";

/* import Adaptive Script language support */
import {
    constructLanguageConfiguration as afwScriptLanguageConfiguration,
    monarchLanguage as afwScriptMonarchLanguage,
    functionCompletionItemProvider,
    polymorphicMethodCompletionItemProvider,
    hoverProvider as afwScriptHoverProvider,
    signatureHelpProvider,
    signatures
} from "../language/AdaptiveScript";

import {
    constructLanguageConfiguration as afwListingLanguageConfiguration,
    monarchLanguage as afwListingMonarchLanguage,
} from "../language/AdaptiveListing";    

import {
    constructLanguageConfiguration as afwEvaluationBacktraceLanguageConfiguration,
    monarchLanguage as afwEvaluationBacktraceMonarchLanguage,
} from "../language/AdaptiveEvaluationBacktrace";    

import {
    constructLanguageConfiguration as afwBacktraceLanguageConfiguration,
    monarchLanguage as afwBacktraceMonarchLanguage,
} from "../language/AdaptiveBacktrace";    


/**
 * MonacoProvider
 *
 * This component loads the monaco environment, then passes
 * it along to other components via React Context.
 */
export const MonacoProvider = ({ theme, children }) => {

    const [monaco, setMonaco] = useState();
    const {functions} = useFunctions();    

    useEffect(() => {
        let isMounted = true;

        const initializeMonaco = async () => {
            const monaco = await initialize();
            
            if (isMounted)
                setMonaco(monaco);

            /* register a new language with id 'afw' */
            monaco.languages.register({ id: "afw" });
            monaco.languages.register({ id: "afw_listing" });
            monaco.languages.register({ id: "afw_evaluation_backtrace" });
            monaco.languages.register({ id: "afw_backtrace" });

            /* setup the configuration for the 'afw' language */
            const afwScriptLC = monaco.languages.setLanguageConfiguration("afw",
                afwScriptLanguageConfiguration(monaco)
            );

            const afwListingLC = monaco.languages.setLanguageConfiguration("afw_listing",
                afwListingLanguageConfiguration(monaco)
            );

            const afwEvaluationBacktraceLC = monaco.languages.setLanguageConfiguration("afw_evaluation_backtrace",
                afwEvaluationBacktraceLanguageConfiguration(monaco)
            );

            const afwBacktraceLC = monaco.languages.setLanguageConfiguration("afw_backtrace",
                afwBacktraceLanguageConfiguration(monaco)
            );

            const afwScriptML = monaco.languages.setMonarchTokensProvider("afw", 
                afwScriptMonarchLanguage);

            const afwListingML = monaco.languages.setMonarchTokensProvider("afw_listing", 
                afwListingMonarchLanguage);

            const afwEvaluationBacktraceML = monaco.languages.setMonarchTokensProvider("afw_evaluation_backtrace", 
                afwEvaluationBacktraceMonarchLanguage);

            const afwBacktraceML = monaco.languages.setMonarchTokensProvider("afw_backtrace", 
                afwBacktraceMonarchLanguage);

            /* define a custom theme */
            monaco.editor.defineTheme("custom-dark", {
                base: "vs-dark",
                inherit: true,
                rules: [],
                colors: {
                    "editor.background": "#151515",                    
                }
            });

            return () => {
                afwScriptLC.dispose();
                afwListingLC.dispose();
                afwEvaluationBacktraceLC.dispose();
                afwBacktraceLC.dispose();
                afwScriptML.dispose();
                afwListingML.dispose();
                afwEvaluationBacktraceML.dispose();
                afwBacktraceML.dispose();

                isMounted = false;
            };
        };

        initializeMonaco();
    }, []);

    useEffect(() => {

        if (monaco && functions) {
            /* 
                Once monaco and functions are available, we can register 
                autocomplete helpers and signature providers.
             */

            /* register these autocomplete helpers on the "afw" languageId */
            const itemProvider = monaco.languages.registerCompletionItemProvider("afw",
                functionCompletionItemProvider(monaco, functions)
            );
            const methodProvider = monaco.languages.registerCompletionItemProvider("afw",
                polymorphicMethodCompletionItemProvider(monaco, functions)
            );

            const sigs = signatures(functions);
            const signatureProvider = monaco.languages.registerSignatureHelpProvider("afw",
                signatureHelpProvider(monaco, functions, sigs)
            );

            const hoverProvider = monaco.languages.registerHoverProvider("afw",
                afwScriptHoverProvider(monaco, functions)
            );

            return () => {
                itemProvider.dispose();
                signatureProvider.dispose();
                hoverProvider.dispose();
                methodProvider.dispose();
            };
        }
    }, [monaco, functions]);

    return (
        <MonacoContext.Provider value={{ monaco, theme }}>
            { children }
        </MonacoContext.Provider>
    );
};

export default MonacoProvider;
