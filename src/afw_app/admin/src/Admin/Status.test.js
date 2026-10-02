// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, screen, waitForSpinner} from "./test-utils";

//import server from "@afw/test/build/cjs/__mocks__/get_object/afw/_AdaptiveServer_/current.json";
import systemInfo from "@afw/test/build/cjs/__mocks__/retrieve_objects/afw/_AdaptiveSystemInfo_.json";

describe("Status Tests", () => {    

    //const uptimeDate = new Date(server.result.startTime);
    //const uptime = uptimeDate.toLocaleDateString() + " " + uptimeDate.toLocaleTimeString();
    let system;    

    systemInfo.result.forEach(r => {
        if (r._meta_.objectId === "general") {
            system = r.sysname + " " + r.release + " (" + r.machine + ")";
        }
    });

    test("Show Status", async () => {

        renderRoute("/Admin/Status");        
        
        await waitForSpinner();

        await screen.findByText("Administration");
        await screen.findByText(system);        
        //screen.getByText(uptime);                
    });

});
