// See the 'COPYING' file in the project root for licensing information.
import {MemoryRouter} from "react-router-dom";
import {render, waitFor, fireEvent, screen, waitForSpinner} from "../test-utils";
import Server from "./Server";

import server from "@afw/test/build/cjs/__mocks__/get_object/afw/_AdaptiveServer_/current.json";
import serverMeta from "@afw/test/build/cjs/__mocks__/get_object/afw/_AdaptiveObjectType_/_AdaptiveServer_.json";
import systemInfo from "@afw/test/build/cjs/__mocks__/retrieve_objects/afw/_AdaptiveSystemInfo_.json";

describe("Server Tests", () => {

    test("Server renders", async () => {

        render(
            <MemoryRouter initialEntries={[ "/Admin/Server" ]}>
                <Server /> 
            </MemoryRouter>
        );
        
        await waitForSpinner();

        const serverInfoText = await screen.findByText("Server Information");
        expect(serverInfoText).toBeInTheDocument();

        const systemInfoText = await screen.findByText("System Information");
        expect(systemInfoText).toBeInTheDocument();

        expect(await screen.findByText("Compiled AFW Version")).toBeInTheDocument();
        
    });

    test("Tabs display server data", async () => {

        render(
            <MemoryRouter initialEntries={[ "/Admin/Server" ]}>
                <Server /> 
            </MemoryRouter>
        );   

        await waitForSpinner();

        const serverInfoText = await screen.findByText("Server Information");
        expect(serverInfoText).toBeInTheDocument();

        const systemInfoText = await screen.findByText("System Information");
        expect(systemInfoText).toBeInTheDocument();                   
        
        for (const [key, value] of Object.entries(server.result)) {
            if (key === "_meta_")
                continue;

            const label = serverMeta.result.propertyTypes[key].label;            
            await waitFor(() => expect(screen.getByLabelText(label)).toHaveTextContent(value));
        }

        for (const info of systemInfo.result) {
            const objectId = info._meta_.objectId;

            fireEvent.click(screen.getByLabelText(objectId));
            for (const [key, value] of Object.entries(info)) {
                if (key === "_meta_")
                    continue;
                
                const label = info._meta_.propertyTypes[key].label ? info._meta_.propertyTypes[key].label : key;               

                await waitFor(() => expect(screen.getByLabelText(label)).toHaveTextContent(value));
            }
        }             
    });

});
