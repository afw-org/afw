// See the 'COPYING' file in the project root for licensing information.
/*
 * These common routines can be used throughout the application to retrieve 
 *   objects or lists that may be used by other components.  Once fetched,
 *   they are added to the store for container components to distribute
 *   them as they need.
 */


/*
 * This routine will load an external Javascript module under a Promise that will be 
 *   resolved when the script has been loaded.  Functions defined by the module can be
 *   called through the "window" object.
 * 
 * Another way is to use RequireJS, by loading it from index.html:
 * 
 *   <script src="https://cdnjs.cloudflare.com/ajax/libs/require.js/2.3.1/require.min.js" />
 * 
 * Then calling its requirejs function:
 * 
 *      window.requirejs(["mylib"], function(myLib) {
 *          myLib.hello();
 *      });
 * 
 */ 
export function loadExternalJS(url) {
    const script = document.createElement("script");
    script.src = url;  /* /path/file.js */

    return new Promise((resolve, reject) => {
        script.onload = () => {
            /* now you can invoke:  window.myFunc(); defined inside file */
            resolve();
        };

        script.onerror = () => {
            reject();
        };

        document.getElementsByTagName("head")[0].appendChild(script);
    });
}
