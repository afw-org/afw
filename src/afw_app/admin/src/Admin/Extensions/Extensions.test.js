// See the 'COPYING' file in the project root for licensing information.
import {server, rest, render, waitFor, within, screen, fireEvent, mswPostCallback, waitForSpinner} from "../test-utils";
import Extensions from "./Extensions";

import environmentRegistry from "@afw/test/build/cjs/__mocks__/get_object/afw/_AdaptiveEnvironmentRegistry_/current.json";
import manifest from "@afw/test/build/cjs/__mocks__/retrieve_objects/afw/_AdaptiveManifest_.json";


describe("Extensions Tests", () => {    

    const extensions = environmentRegistry.result.extension;

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("Extensions renders", async () => {

        render( <Extensions /> );                  

        expect(await screen.findByText("Extension Id")).toBeInTheDocument();        
        expect(await screen.findByLabelText("Load Extension")).toBeInTheDocument();
        expect(await screen.findByLabelText("Refresh")).toBeInTheDocument();
        expect(await screen.findByLabelText("Details")).toBeInTheDocument();

        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();
        
    });

    test("Table lists all extensions", async () => {

        render( <Extensions /> );        

        expect(await screen.findByText("Extension Id")).toBeInTheDocument();        
        expect(await screen.findByLabelText("Load Extension")).toBeInTheDocument();
        expect(await screen.findByLabelText("Refresh")).toBeInTheDocument();
        expect(await screen.findByLabelText("Details")).toBeInTheDocument();  

        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();

        /* look for all loaded extensions */
        for (const [, value] of Object.entries(extensions)) {
            const {afwCompiledVersion, extensionId, extensionVersion} = value;

            await waitFor(() => expect(screen.getByText(extensionId)).toBeInTheDocument());
            const row = screen.getByText(extensionId).closest("tr");
            const utils = within(row);

            expect(utils.getByText(extensionVersion + " " + afwCompiledVersion)).toBeInTheDocument();            

            // find the matching manifest entry
            for (const manifest_entry of manifest.result) {
                if (manifest_entry.extensionId === extensionId) {
                    expect(utils.getByText(manifest_entry.brief)).toBeInTheDocument();
                }
            }
        }        

        /* now look for all manifest entries */
        for (const manifest_entry of manifest.result) {
            const {extensionId, brief} = manifest_entry;

            const row = screen.getByText(extensionId).closest("tr");
            const utils = within(row);

            expect(utils.getByText(extensionId)).toBeInTheDocument();
            expect(utils.getByText(brief)).toBeInTheDocument();
        }
    });

    test("View details of each extension", async () => {

        render( <Extensions /> );        

        expect(await screen.findByText("Extension Id")).toBeInTheDocument();        
        expect(await screen.findByLabelText("Load Extension")).toBeInTheDocument();
        expect(await screen.findByLabelText("Refresh")).toBeInTheDocument();
        expect(await screen.findByLabelText("Details")).toBeInTheDocument();

        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();

        for (const manifest_entry of manifest.result) {
            const {extensionId, brief, description, modulePath, registers} = manifest_entry;

            const row = screen.getByText(extensionId).closest("tr");           
            const detailsBtn = screen.getByLabelText("Details");

            fireEvent.click(row);
            fireEvent.click(detailsBtn);

            await waitFor(() => expect(screen.getByLabelText("Brief")).toHaveTextContent(brief));
            await waitFor(() => expect(screen.getByLabelText("Description")).toHaveTextContent(description, { normalizeSpaces: true }));
            await waitFor(() => expect(screen.getByLabelText("Extension")).toHaveTextContent(extensionId));
            await waitFor(() => expect(screen.getByLabelText("Module Path")).toHaveTextContent(modulePath));
            
            const registersList = screen.getByLabelText("Registers");
            for (const r of registers) {
                const utils = within(registersList);
                expect(utils.getByText(r)).toBeInTheDocument();
            }

            // close the dialog
            const closeBtn = screen.getByLabelText("Close Dialog");

            // un-check the row
            fireEvent.click(closeBtn);
            fireEvent.click(row);
        }

    });

    test("Refresh extensions", async () => {

        render( <Extensions /> );        

        expect(await screen.findByText("Extension Id")).toBeInTheDocument();        
        expect(await screen.findByLabelText("Load Extension")).toBeInTheDocument();
        expect(await screen.findByLabelText("Refresh")).toBeInTheDocument();
        expect(await screen.findByLabelText("Details")).toBeInTheDocument();

        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();

        const refreshBtn = screen.getByLabelText("Refresh");

        fireEvent.click(refreshBtn);

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());   

        /* wait for manifest reload to finish */
        await waitForSpinner();
    });

    test("Loads an extension", async () => {        

        render( <Extensions /> );        

        expect(await screen.findByText("Extension Id")).toBeInTheDocument();        
        expect(await screen.findByLabelText("Load Extension")).toBeInTheDocument();
        expect(await screen.findByLabelText("Refresh")).toBeInTheDocument();
        expect(await screen.findByLabelText("Details")).toBeInTheDocument();

        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();

        let loaded = false;
        for (const manifest_entry of manifest.result) {
            if (loaded)
                break;

            const {extensionId} = manifest_entry;

            let found = false;
            for (const [, value] of Object.entries(extensions)) {
                const {extensionId: id} = value;
                if (extensionId === id)
                    found = true;
            }

            if (!found) {
                const loadBtn = screen.getByLabelText("Load Extension");                                
                await waitFor(() => expect(screen.getByText(extensionId)).toBeInTheDocument());

                // clear requests and start over from here
                mswPostCallback.mockClear();

                /* extension not loaded */
                const row = screen.getByText(extensionId).closest("tr");                       

                /* select the fow */
                fireEvent.click(row);

                await waitFor(() => expect(loadBtn).toBeEnabled());

                /* load the extension */
                fireEvent.click(loadBtn);
                
                await waitFor(() => expect(screen.getByLabelText("Yes")).toBeInTheDocument());                
                
                const yesBtn = screen.getByLabelText("Yes");      
                await waitFor(() => expect(yesBtn).toBeEnabled());

                fireEvent.click(yesBtn);                

                await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("extension_load"));
                await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("extension_id", extensionId));                     

                /* The extension should not be selected now, and "Load Extension" disabled */
                await waitFor(() => expect(screen.getByLabelText("Load Extension")).toBeDisabled());    

                /* wait for manifest reload to finish */
                await waitForSpinner();

                loaded = true;
            }
        }
    });

    test("Loads an extension and adds it to startup", async () => {

        render( <Extensions /> );        

        expect(await screen.findByText("Extension Id")).toBeInTheDocument();        
        expect(await screen.findByLabelText("Load Extension")).toBeInTheDocument();
        expect(await screen.findByLabelText("Refresh")).toBeInTheDocument();
        expect(await screen.findByLabelText("Details")).toBeInTheDocument();

        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();

        let unloadedExtensionId;
        for (const manifest_entry of manifest.result) {
            const {extensionId} = manifest_entry;

            if (unloadedExtensionId)
                break;

            let found = false;
            for (const [, value] of Object.entries(extensions)) {
                const {extensionId: id} = value;
                if (extensionId === id) {
                    found = true;                    
                    break;
                }
            }   
            
            if (!found)
                unloadedExtensionId = extensionId;
        }        

        expect(await screen.findByText(unloadedExtensionId)).toBeInTheDocument();        

        /* find row for extension not loaded yet */
        let row = screen.getByText(unloadedExtensionId).closest("tr");                       

        /* select the fow */
        fireEvent.click(row);

        /* load the extension */
        const loadBtn = screen.getByLabelText("Load Extension");
        expect(loadBtn).toBeEnabled();

        fireEvent.click(loadBtn);
        
        await waitFor(() => expect(screen.getByLabelText("Load on startup")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Yes")).toBeInTheDocument());                
        
        // clear requests and start from here
        mswPostCallback.mockClear();
        
        fireEvent.click(screen.getByLabelText("Load on startup"));
        fireEvent.click(screen.getByLabelText("Yes"));
                
        /* wait for extension_load request to occur */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("extension_load"));            
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("extension_id", unloadedExtensionId));

        /* also wait for the reconcile_object call to update the startup configuration */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("reconcile_object"));     
        
        /* The extension should not be selected now, and "Load Extension" disabled */
        await waitFor(() => expect(screen.getByLabelText("Load Extension")).toBeDisabled());        

        /* wait for manifest reload to finish */
        await waitForSpinner();
    });

    test("Loads multiple extensions at once", async () => {
        render( <Extensions /> );        

        expect(await screen.findByText("Extension Id")).toBeInTheDocument();        
        expect(await screen.findByLabelText("Load Extension")).toBeInTheDocument();
        expect(await screen.findByLabelText("Refresh")).toBeInTheDocument();
        expect(await screen.findByLabelText("Details")).toBeInTheDocument();      
        
        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();

        let unloadedExtensions = [];
        for (const manifest_entry of manifest.result) {
            const {extensionId} = manifest_entry;

            let found = false;
            for (const [, value] of Object.entries(extensions)) {
                const {extensionId: id} = value;
                if (extensionId === id) {
                    found = true;                    
                    break;
                }
            }   
            
            if (!found)
                unloadedExtensions.push(extensionId);
        }        

        for (const unloadedExtensionId of unloadedExtensions) {

            expect(await screen.findByText(unloadedExtensionId)).toBeInTheDocument();            

            /* find row for extension not loaded yet */
            const row = screen.getByText(unloadedExtensionId).closest("tr");                       

            /* select the fow */
            fireEvent.click(row);
        }

        /* load the extensions */
        const loadBtn = screen.getByLabelText("Load Extension");
        expect(loadBtn).toBeEnabled();
        fireEvent.click(loadBtn);
        
        await waitFor(() => expect(screen.getByLabelText("Load on startup")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Yes")).toBeInTheDocument());                
        
        // clear requests and start from here
        mswPostCallback.mockClear();
        
        fireEvent.click(screen.getByLabelText("Load on startup"));
        fireEvent.click(screen.getByLabelText("Yes"));
        
        await waitFor(() => expect(mswPostCallback.mock.calls.length > 15 ));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("extension_load"));

        for (const unloadedExtensionId of unloadedExtensions) {            
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("extension_id", unloadedExtensionId));                                   
        }

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("reconcile_object"));     
        
        /* The extension should not be selected now, and "Load Extension" disabled */
        await waitFor(() => expect(screen.getByLabelText("Load Extension")).toBeDisabled());        

        /* wait for manifest reload to finish */
        await waitForSpinner();     
    });

    test("Add an Extension", async () => {

        render( <Extensions /> );        
        
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId} = req.body;

                if (functionId === "extension_load_by_module_path") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "success",
                            result: "my_ext"
                        })
                    );
                }
            })
        );        
        
        expect(await screen.findByText("Extension Id")).toBeInTheDocument();        
        expect(await screen.findByLabelText("Load Extension")).toBeInTheDocument();
        expect(await screen.findByLabelText("Refresh")).toBeInTheDocument();
        expect(await screen.findByLabelText("Details")).toBeInTheDocument();      
        
        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();

        /* load the extensions */
        const addBtn = screen.getByRole("button", { name : "Add Extension" });
        expect(addBtn).toBeEnabled();
        fireEvent.click(addBtn);

        await waitFor(() => expect(screen.getByRole("textbox", { name: "Module Path" })).toBeInTheDocument());
        expect(screen.getByRole("button", { name: "Add" })).not.toBeEnabled();       

        fireEvent.change(screen.getByRole("textbox", { name: "Module Path" }), { target: { value: "libmyext" }});

        const addConfirmBtn = screen.getByRole("button", { name: "Add" });
        expect(addConfirmBtn).toBeEnabled();
        fireEvent.click(addConfirmBtn);

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("extension_load_by_module_path"));    

        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();      
    });

    test("Add an Extension", async () => {

        render( <Extensions /> );        
        
        server.use(
            rest.post("/afw", (req, res, ctx) => {
                const {function: functionId} = req.body;

                if (functionId === "extension_load_by_module_path") { 
                    mswPostCallback("/afw", req, res, ctx);

                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "success",
                            result: "my_ext"
                        })
                    );
                }
            })
        );        
        
        expect(await screen.findByText("Extension Id")).toBeInTheDocument();        
        expect(await screen.findByLabelText("Load Extension")).toBeInTheDocument();
        expect(await screen.findByLabelText("Refresh")).toBeInTheDocument();
        expect(await screen.findByLabelText("Details")).toBeInTheDocument();      
        
        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();

        /* load the extensions */
        const addBtn = screen.getByRole("button", { name : "Add Extension" });
        expect(addBtn).toBeEnabled();
        fireEvent.click(addBtn);

        await waitFor(() => expect(screen.getByRole("textbox", { name: "Module Path" })).toBeInTheDocument());
        expect(screen.getByRole("button", { name: "Add" })).not.toBeEnabled();       

        fireEvent.change(screen.getByRole("textbox", { name: "Module Path" }), { target: { value: "libmyext" }});
        expect(screen.getByRole("button", { name: "Add" })).toBeEnabled();

        fireEvent.click(screen.getByRole("button", { name: "Add" }));

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("extension_load_by_module_path"));    

        /* wait for manifest, extensions, etc to finish */
        await waitForSpinner();
   
    });
});
