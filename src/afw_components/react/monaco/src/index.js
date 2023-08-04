// See the 'COPYING' file in the project root for licensing information.
/**
 * index.js
 * 
 * Registers the Monaco Editor AFW Component Implementation package.
 */

/* import every component to be exported via componentRegistry */
import {
    CodeEditor,
    DiffEditor
} from "./components";

export {MonacoProvider} from "./components/MonacoProvider";
export * from "./hooks";

const monacoBaseUrl         = process.env.PUBLIC_URL + "/monaco-editor/min/vs";
const monacoLoaderUrl       = process.env.PUBLIC_URL + "/monaco-editor/min/vs/loader.js";

/*
 * initialize()
 *
 * This method fetches the monaco loader in and establishes 
 * a global window.monaco object, which anchors the language
 * and editor modules.  It should be run once from an application
 * to setup the global object.
 *
 * This is an alternative to getting monaco to run directly from
 * NPM/webpack.  It's not ideal, but a solution until Microsoft
 * or someone can figure out a way to bundle up Monaco without
 * messing with webpack.
 */
export const initialize = async () => {
    
    let finalResolve, finalReject;

    const script = document.createElement("script");   

    script.innerHTML = `
        require.config({ paths: { 'vs': '${document.location.origin + monacoBaseUrl}' }});
        require(['vs/editor/editor.main'], function() {
            document.dispatchEvent(new Event('monaco_init'));
        });
    `;

    const loaderScript = document.createElement("script");
    loaderScript.src = document.location.origin + monacoLoaderUrl;

    loaderScript.onload = () => {
        script.onload = () => {
            // eslint-disable-next-line no-undef
            resolve();
        };

        script.onerror = finalReject;

        document.body.appendChild(script);
    };

    loaderScript.onerror = finalReject;
    document.body.appendChild(loaderScript);

    const initEventListener = () => {
        /* we've initialized everything and should have window.monaco now */
        document.removeEventListener("monaco_init", initEventListener);        
        finalResolve(window.monaco);
    };

    document.addEventListener("monaco_init", initEventListener);   

    return new Promise((resolve, reject) => {
        finalResolve = resolve;
        finalReject = reject;
    });

};

const setTheme = () => {
};

export const componentRegistry = {
    implementationId:   "@afw/react-monaco",
    initialize: () => undefined,
    setTheme,
    components: {
        CodeEditor,
        DiffEditor,
    }
};

export default componentRegistry;
