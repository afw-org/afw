// See the 'COPYING' file in the project root for licensing information.
/**
 * copyToClipboard(content)
 *
 */
export async function copyToClipboard(content) {
    let string = content;

    if (typeof(content) === "object") {
        /* stringify it */
        string = JSON.stringify(content, null, 4);
    }

    /* first try the Async Clipboard API, if available */
    if (navigator && navigator.clipboard) {
        try {
            await navigator.clipboard.writeText(string);
        } catch (error) {
            console.log(error);
        }
    }

    /* fallback to using "copy" command */
    else {        
        const textArea = document.createElement("textarea");
        textArea.value = string;
        document.body.appendChild(textArea);
        textArea.focus();
        textArea.select();

        try {
            document.execCommand("copy");
        } catch (error) {
            /* fixme */
            console.log(error);
        }

        document.body.removeChild(textArea);
    }
}
