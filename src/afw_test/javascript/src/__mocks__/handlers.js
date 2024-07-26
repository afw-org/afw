// See the 'COPYING' file in the project root for licensing information.
import { rest } from "msw";

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

export const handlers = [

    rest.post("/afw", async (req, res, ctx) => {
        mswPostCallback("/afw", req, res, ctx);

        //const accept = req.headers.get("accept");
        let {function: func, uri, adapterId, objectType, objectId} = req.body;

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

                        return res( ctx.status(200), ctx.json({ result: obj }) );
                    } 
                    
                    else 
                    {
                        /* first try local mock data */
                        let obj;                           
                        try {
                            obj = await import("./get_object/afw/_AdaptiveObjectType_/" + objectId);
                            if (obj) 
                                return res( ctx.status(200), ctx.json(obj));
                        } catch (e) {
                            // fall through and use generated
                        }                        

                        /* then try generated mock data */
                        try {                            
                            obj = await import("../../../../../afw/generated/objects/_AdaptiveObjectType_/" + objectId);                                                        

                            return res( ctx.status(200), ctx.json({ result: obj }) );
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
                            return res( ctx.status(200), ctx.json(obj));
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
                        return res( ctx.status(200), ctx.json(obj));                    
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

            return res( ctx.status(200), ctx.json(data));
        }           

        case "retrieve_objects_to_response": {
            let data;
            try {                    
                data = await import("./retrieve_objects/" + adapterId + "/" + objectType);                    
            } catch (e) {
                console.log(e);
            }                
          
            const stream = streamFromJSON(data.result);            
            return res(
                ctx.status(200),
                ctx.set("Content-Type", "application/x-afw"),
                ctx.body(stream)
            );
              
        } 

        case "reconcile_object":
            return res( ctx.status(200), ctx.json({ result: {}, status: "success" }) );

        case "extension_load":
            return res( ctx.status(200), ctx.json({ result: true, status: "success" }) );

        case "extension_load_by_module_path":
            return res( ctx.status(200), ctx.json({ result: true, status: "success" }) );

        case "service_start":
            return res( ctx.status(200), ctx.json({ result: true, status: "success" }) );

        case "service_restart":
            return res( ctx.status(200), ctx.json({ result: true, status: "success" }) );

        case "service_stop":
            return res( ctx.status(200), ctx.json({ result: true, status: "success" }) );

        case "script::evaluate":
        case "evaluate_script":
            return res( ctx.status(200), ctx.json({ result: 2, status: "success" }) );

        case "compile_script":
        case "script::compile":
            return res( ctx.status(200), ctx.json({ result: "#!\n#n#\nresult\n", status: "success" }) );        
        
        default:
            return res( ctx.status(200), ctx.json({ result: {} }) );
        }
    }),

    rest.get("/:adapterId/:objectTypeId/:objectId", async (req, res, ctx) => {
        const {adapterId, objectTypeId, objectId} = req.params;

        mswGetCallback("/" + adapterId + "/" + objectTypeId + "/" + objectId, req, res, ctx);
        
        let data;
        try {
            data = await import("./get_object/" + adapterId + "/" + objectTypeId + "/" + objectId);
        } catch (e) {
            console.log(e);
        }

        return res(
            ctx.status(200),
            ctx.json(data)
        );
    }),

];
