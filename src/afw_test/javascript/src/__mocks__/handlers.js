// See the 'COPYING' file in the project root for licensing information.
import { http, HttpResponse } from "msw";

export const mswPostCallback = jest.fn();
export const mswGetCallback = jest.fn();

const streamFromJSON = (json) => {
    let data = "";

    let sequence = 1;
    json.forEach(obj => {
        const payload = JSON.stringify({ intermediate: true, result: obj });
        data += sequence + " " + payload.length + " response\n" + payload;
        sequence++;
    });

    const payload = (JSON.stringify({result:null,status:"success"}) + "\n");
    data += sequence + " " + payload.length + " response\n" + payload;

    sequence++;
    data += sequence + " 0 end";

    return data;
};

// Absolute URLs (as a host-agnostic RegExp, not a plain string), not relative
// paths: msw's Node-side setupServer matches relative patterns against
// `window.location`, which is a browser convenience this test environment
// doesn't reliably provide. Different test packages construct AfwClient with
// either no url (defaults to the literal "http://localhost/afw") or a
// relative "/afw" (resolved against jsdom's default test URL,
// "http://localhost:3000/") - the port needs to stay optional to match both.
export const handlers = [

    http.post(/^http:\/\/localhost(:\d+)?\/afw$/, async ({request}) => {
        // .clone() before reading the body: per-test server.use() overrides
        // registered ahead of this base handler may have already read (and
        // conditionally fallen through past) the same underlying stream -
        // reading it a second time without cloning throws "body stream
        // already read".
        const body = await request.clone().json();
        mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers, body});

        //const accept = req.headers.get("accept");
        let {function: func, uri, adapterId, objectType, objectId} = body;

        switch (func) {
        case "get_object_with_uri":
            ([,adapterId, objectType, objectId] = uri.split("/"));
        // eslint-disable-next-line no-fallthrough
        case "get_object":

            switch (adapterId) {
            case "afw":
                switch (objectType) {
                case "_AdaptiveObjectType_":
                    if (objectId === "_AdaptiveServer_")
                    {
                        const obj = await import("../../../../../afw_server_fcgi/generated/objects/_AdaptiveObjectType_/" + objectId);

                        return HttpResponse.json({ result: obj });
                    }

                    else
                    {
                        /* first try local mock data */
                        let obj;
                        try {
                            obj = await import("./get_object/afw/_AdaptiveObjectType_/" + objectId);
                            if (obj)
                                return HttpResponse.json(obj);
                        } catch (e) {
                            // fall through and use generated
                        }

                        /* then try generated mock data */
                        try {
                            obj = await import("../../../../../afw/generated/objects/_AdaptiveObjectType_/" + objectId);

                            return HttpResponse.json({ result: obj });
                        } catch (e) {
                            console.log(e);
                        }
                    }
                    break;

                // default case for objectType
                default: {
                    let obj;
                    try {
                        obj = await import("./get_object/" + adapterId + "/" + objectType + "/" + objectId);
                        if (obj)
                            return HttpResponse.json(obj);
                    } catch (e) {
                        console.log(e);
                    }
                    break;
                }}
                break;

            // default case for adapterId is to use local mocks
            default: {
                let obj;
                try {
                    obj = await import("./get_object/" + adapterId + "/" + objectType + "/" + objectId);
                    if (obj)
                        return HttpResponse.json(obj);
                } catch (e) {
                    console.log(e);
                }
                break;
            }}
            break;

        case "retrieve_objects": {

            let data;
            try {
                data = await import("./retrieve_objects/" + adapterId + "/" + objectType);
            } catch (e) {
                console.log(e);
            }

            return HttpResponse.json(data);
        }

        case "retrieve_objects_to_response": {
            let data;
            try {
                data = await import("./retrieve_objects/" + adapterId + "/" + objectType);
            } catch (e) {
                console.log(e);
            }

            const stream = streamFromJSON(data.result);
            return new HttpResponse(stream, {
                status: 200,
                headers: { "Content-Type": "application/x-afw" }
            });

        }

        case "reconcile_object":
            return HttpResponse.json({ result: {}, status: "success" });

        case "extension_load":
            return HttpResponse.json({ result: true, status: "success" });

        case "extension_load_by_module_path":
            return HttpResponse.json({ result: true, status: "success" });

        case "service_start":
            return HttpResponse.json({ result: true, status: "success" });

        case "service_restart":
            return HttpResponse.json({ result: true, status: "success" });

        case "service_stop":
            return HttpResponse.json({ result: true, status: "success" });

        case "script::eval":
        case "eval_script":
            return HttpResponse.json({ result: 2, status: "success" });

        case "compile_script":
        case "script::compile":
            return HttpResponse.json({ result: "#!\n#n#\nresult\n", status: "success" });

        default:
            return HttpResponse.json({ result: {} });
        }
    }),

    // msw v2 does not extract named RegExp capture groups into `params`
    // (confirmed against its own matchRequestUrl test suite - a RegExp path
    // always yields params: {}), so adapterId/objectTypeId/objectId are
    // parsed from the pathname directly rather than relying on named groups.
    http.get(/^http:\/\/localhost(:\d+)?\/[^/]+\/[^/]+\/[^/]+$/, async ({request}) => {
        const {pathname} = new URL(request.url);
        const [, adapterId, objectTypeId, objectId] = pathname.split("/");

        mswGetCallback("/" + adapterId + "/" + objectTypeId + "/" + objectId, {method: request.method, url: request.url, headers: request.headers});

        let data;
        try {
            data = await import("./get_object/" + adapterId + "/" + objectTypeId + "/" + objectId);
        } catch (e) {
            console.log(e);
        }

        return HttpResponse.json(data);
    }),

];
