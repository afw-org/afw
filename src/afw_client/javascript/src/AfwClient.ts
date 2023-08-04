// See the 'COPYING' file in the project root for licensing information.
import {AfwStreams} from "./AfwStreams";
import {IPerformPayload, IJSONObject, IJSON} from "./model/types";

import {TextEncoder} from "util";

type AfwClientConfig = {
    url?                : string;           /* The default url for all requests (overridden by individual functions) */
    contentType?        : string;           /* Content-Type for the client request */
    accept?             : string;           /* Accept preferred content-type for the client */
    timeout?            : number;           /* default timeout for requests */
    headers?            : IJSONObject;      /* Specific HTTP headers to pass along to the request */
    fetchOptions?       : IJSONObject;      /* Additional options to pass along to fetch() */
}

export type AfwClientPerformResult = {
    httpResponse            : Promise<Response>             /* The http response from fetch */
    response                : () => Promise<IJSONObject>,   /* The Adaptive Response object */
    result                  : () => Promise<any>,           /* The result returned by AFW perform operations */
    resultDataType          : () => Promise<any>,           /* The resultDataType returned by AFW perform operations */
    streams                 : () => Promise<IJSONObject>    /* The streams object from underlying connection */
    streamReader            : () => Promise<AfwStreams>     /* Access to the raw AfwStreams object for reading directly */
    controller              : AbortController               /* The AbortController instance for aborting requests */
};

type onCallback = (arg : any) => any;
type onCallbacks = {
    [eventId : string] : onCallback[]
};

const streamFromText : (text : string) => ReadableStream = (text : string) => {

    const encoder = new TextEncoder();
    const stream : ReadableStream = new ReadableStream({
        start(controller) {
            controller.enqueue(
                encoder.encode(text)
            );
            controller.close();
        }
    });

    return stream;
};

/**
 * AfwClient
 * 
 * This class can be used to instantiate a client for Adaptive Framework
 * requests.  It's intended to wrap a variety of ways to access Adaptive
 * Framework, including RESTful, Perform (POSTy) and in a variety of 
 * content-types including XML, JSON or Streams.
 * 
 * It also works in stride with the rest of the "model" code to present 
 * objects, lists, and values in a way that makes it easy for users to
 * retrieve, interact with and update back to a server.
 * 
 */
export class AfwClient {

    private url                 : string;
    private contentType         : string;
    private accept              : string;
    private timeout?            : number;
    private fetchOptions        : IJSONObject;    
    private headers             : IJSONObject;
    private actionFlags?        : string[];
    private onCallbacks         : onCallbacks;

    constructor({ 
        url             = "http://localhost/afw",               
        contentType     = "application/json",   
        accept          = "application/json",
        timeout,                    
        headers         = {},            
        fetchOptions    = {},    
    } : AfwClientConfig = {}) {
        this.url                = url;
        this.contentType        = contentType;
        this.accept             = accept;
        this.timeout            = timeout;        
        this.headers            = headers;
        this.fetchOptions       = fetchOptions;
        this.onCallbacks        = {
            "onRequest":        [],
            "onResponse":       [],
            "onResponseError":  [],
        };
    }

    /**
     * Adds a callback routine for an event.
     * 
     * @param eventId 
     * @param callback 
     */
    addOn(eventId : string, callback : onCallback) {
        this.onCallbacks[eventId] = [ ...this.onCallbacks[eventId],  callback];
        return callback;
    }

    /**
     * Removes a callback routine for an event.
     * 
     * @param eventId 
     * @param callback 
     */
    removeOn(eventId : string, callback : onCallback) {
        this.onCallbacks[eventId] = this.onCallbacks[eventId].filter(cb => cb !== callback);
        return callback;
    }

    /**
     * Sets the _flags_ to be used for all perform actions executed by this
     * client instance.
     */
    setActionFlags(flags : string[]) {
        this.actionFlags = flags;
    }

    /**
     * Sets the timeout duration to be used for all requests executed by 
     * this client instance.
     */
    setTimeout(timeout : number) {
        this.timeout = timeout;
    }

    /**
     * Internal fetch routine, which calls the real fetch implementation
     * underneath.
     */
    _fetch({ url, method, body } : { url? : string, method : string, body? : string }) {

        const controller = new AbortController();
        const signal = controller.signal;
        
        if (this.timeout) {
            setTimeout(() => controller.abort(), this.timeout);
        }               

        const request = { 
            signal,
            method,
            headers: {
                "Content-Type": this.contentType,
                "Accept": this.accept,
            },
            ...this.fetchOptions,
            body
        };
        const _response = fetch(url ? url : this.url, request);
    
        /* 
           This wraps the response from fetch so we can intercept errors for 
           user callbacks.  This is really useful, if your app needs to
           check for errors such as authorization problems due to expired
           authenticated sessions.
        */
        const response = new Promise((resolve, reject) => {
            this.onCallbacks["onRequest"].forEach(cb => cb(request));

            _response.then(res => {                
                this.onCallbacks["onResponse"].forEach(cb => cb(res));
                
                if (res.status < 200 || res.status >= 300) {                                        
                    this.onCallbacks["onResponseError"].forEach(cb => cb(res));
                }
                
                resolve(res);                
            }).catch(err => {                
                reject(err);
            });
        });

        return { response };
    }

    /**
     * 
     * @returns 
     */
    get(url : string | undefined) {        
        return this._fetch({ url, method: "GET" });
    }

    /**
     * 
     * @param body 
     * @returns 
     */
    async post(url : string | undefined, body : string) {
        return this._fetch({ url, method: "POST", body });
    }

    async put() {}
    async patch() {}
    async delete() {}
    
    /**
     * Executes a perform action request.
     * 
     * @param payload 
     * @returns 
     */
    perform(payload: IPerformPayload) : AfwClientPerformResult {

        if (!payload)
            throw new Error("The perform() function requires an Action payload.");
        
        const controller = new AbortController();
        const signal = controller.signal;
        
        /* create our action body to be sent over, using _flags_, if specified */
        let body;
        if (this.actionFlags)
            body = JSON.stringify({ ...payload, _flags_: this.actionFlags });
        else 
            body = JSON.stringify(payload);

        /* if a timeout is configured, use the abort() to break the connection */
        if (this.timeout) {
            setTimeout(() => controller.abort(), this.timeout);
        }  

        /* Perform a fetch() for the perform request */
        const request = {
            signal,
            method: "POST",
            headers: {
                "Content-Type": "application/json",
                "Accept": this.accept,
                ...this.headers,
            },
            body,
            ...this.fetchOptions,
        };
        const httpResponse = fetch(this.url, request);

        /* Create an AdaptiveResponse promise */
        const response : () => Promise<IJSONObject> = () => new Promise((resolve, reject) => {
            this.onCallbacks["onRequest"].forEach(cb => cb(request));

            httpResponse.then(res => {
                this.onCallbacks["onResponse"].forEach(cb => cb(res));

                const responseContentType = res.headers.get("content-type");                                      
                if (responseContentType === "application/json") {
                    return (
                        res.json().then(json => 
                            resolve(json)).catch(e => reject(e)
                        )
                    );
                } else if (responseContentType === "application/x-afw") {
                    /* need to read the streams */                    
                    if (res.body) {                                                
                        /* we're running with fetch compatible with ReadableStream */
                        new AfwStreams(res.body).readStreams().then(json => 
                            resolve(json["response"])
                        ).catch(e => reject(e));                        
                    } else {
                        /* fetch is not returning a ReadableStream, so create one */
                        res.text().then(t => 
                            new AfwStreams(streamFromText(t)).readStreams().then(json =>
                                resolve(json["response"])
                            ).catch(e => reject(e))
                        ).catch(e => reject(e));                                                
                    }                    
                } else {
                    /*! \fixme assume text/plain or text/html? */
                    res.text().then(text => reject(text)).catch(e => reject(e));                    
                }
            }).catch(e => {
                this.onCallbacks["onResponseError"].forEach(cb => cb(e));
                reject(e);
            });
        }
        );

        /* Create a "result" Promise, which uses the AdaptiveResponse promise */
        const result : () => Promise<IJSON> = () => new Promise((resolve, reject) =>            
            response().then((json : IJSONObject) => {                               
                if (json.status === "error")
                    reject(json.error);
                resolve(json["result"]);                                        
            }).catch(e => reject(e))
        );       

        /* Create a "resultDataType" Promise, which uses the AdaptiveResponse promise */
        const resultDataType : () => Promise<IJSON> = () => new Promise((resolve, reject) =>            
            response().then((json : IJSONObject) => {                               
                if (json.status === "error")
                    reject(json.error);
                resolve(json["resultDataType"]);                                        
            }).catch(e => reject(e))
        );           

        /* Function to return a raw stream reader (AfwStreams instance) */
        const streamReader : () => Promise<AfwStreams> = () => new Promise((resolve, reject) =>
            httpResponse.then(res => {                
                if (res.headers.get("content-type") === "application/x-afw") {                    
                    if (res.body) 
                        resolve(new AfwStreams(res.body));       
                    else {
                        res.text().then(t =>
                            resolve(new AfwStreams(streamFromText(t)))
                        ).catch(e => reject(e));
                    }                        
                } else 
                    reject("AfwStreams requires content-type of application/x-afw");
            }).catch(e => reject(e))
        );
        
        /* Function that create a "streams" Promise, which gets to the actual streams, already parsed */
        const streams : () => Promise<IJSONObject> = () => new Promise((resolve, reject) => 
            streamReader().then(streamReader => {                
                streamReader.readStreams().then(json => 
                    resolve(json)
                ).catch(e => reject(e));                
            }).catch(e => reject(e))
        );

        return { 
            httpResponse, 
            response, 
            result, 
            resultDataType, 
            streams, 
            streamReader, 
            controller 
        };
    }
}

export default AfwClient;