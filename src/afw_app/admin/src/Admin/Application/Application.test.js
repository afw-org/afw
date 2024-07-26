// See the 'COPYING' file in the project root for licensing information.
import {render, waitFor, fireEvent, mswPostCallback, screen, waitForSpinner} from "../test-utils";
import Application from "./Application";

import applicationConf from "@afw/test/build/cjs/__mocks__/get_object/conf/_AdaptiveConf_application/AFWDev.json";

describe("Application Tests", () => {

    const {
        defaultFlags, 
        extensions,
        qualifiedVariables,
        layoutsAdapterId,
        defaultModelAdapterId,
        defaultAdapterId,
        description,
        title
    } = applicationConf.result;

    test("Tabs display application configuration data", async () => {

        render( <Application /> );        

        expect(await screen.findByLabelText("Edit Object")).toBeInTheDocument();            
        expect(await screen.findByLabelText("Show Object in Layout")).toBeInTheDocument();   
        expect(await screen.findByLabelText("Show Object Source")).toBeInTheDocument();   
        expect(await screen.findByLabelText("Title")).toBeInTheDocument();
        
        expect(await screen.findByLabelText("Title")).toHaveTextContent(title);
        expect(await screen.findByLabelText("Description")).toHaveTextContent(description);
        expect(await screen.findByLabelText("Default Adapter")).toHaveTextContent(defaultAdapterId);
        expect(await screen.findByLabelText("Default Model Adapter")).toHaveTextContent(defaultModelAdapterId);
        expect(await screen.findByLabelText("Layouts Adapter")).toHaveTextContent(layoutsAdapterId);
        
        fireEvent.click(screen.getByLabelText("Startup"));        
        for (const ext of extensions) {
            expect(await screen.findByText(ext)).toBeInTheDocument();            
        }

        fireEvent.click(screen.getByLabelText("Flags"));
        for (const flag of defaultFlags) {
            expect(await screen.findByText(flag)).toBeInTheDocument();                        
        }

        fireEvent.click(screen.getByLabelText("Variables"));
        for (const [qualifier, variables] of Object.entries(qualifiedVariables)) {
            if (qualifier === "_meta_")
                continue;

            expect(await screen.findByText(qualifier)).toBeInTheDocument();                        

            for (const [variable, value] of Object.entries(variables)) {
                if (variable === "_meta_")
                    continue;
                
                expect(await screen.findByText(variable)).toBeInTheDocument();            
                expect(await screen.findByText(value)).toBeInTheDocument();            
            }
        }     
    });

    test("Switch between source and layout", async () => {

        render( <Application /> );        
        
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        expect(await screen.findByLabelText("Show Object Source")).toBeInTheDocument();
        fireEvent.click(screen.getByLabelText("Show Object Source"));    

        fireEvent.click(screen.getByLabelText("Show Object in Layout"));
    });

    test("Edit mode", async () => {

        render( <Application /> );        

        expect(await screen.findByLabelText("Edit Object")).toBeInTheDocument();
        fireEvent.click(screen.getByLabelText("Edit Object"));

        /* make sure we have save/cancel buttons */
        expect(await screen.findByLabelText("Save")).toBeInTheDocument();            
        expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();            
        
        fireEvent.click(screen.getByLabelText("Startup"));        
        for (const ext of extensions) {
            expect(await screen.findByText(ext)).toBeInTheDocument();            
        }

        fireEvent.click(screen.getByLabelText("Flags"));
        for (const flag of defaultFlags) {
            expect(await screen.findByText(flag)).toBeInTheDocument();            
        }

        fireEvent.click(screen.getByLabelText("Variables"));
        for (const [qualifier, variables] of Object.entries(qualifiedVariables)) {
            if (qualifier === "_meta_")
                continue;

            expect(await screen.findByText(qualifier)).toBeInTheDocument();            

            for (const [variable, value] of Object.entries(variables)) {
                if (variable === "_meta_")
                    continue;
                
                expect(await screen.findByLabelText(variable)).toHaveValue(value);                      
            }
        }  
    });

    test("Change Title and make sure it saves properly", async () => {

        render( <Application /> );        

        expect(await screen.findByLabelText("Edit Object")).toBeInTheDocument();

        fireEvent.click(screen.getByLabelText("Edit Object"));

        /* make sure we have save/cancel buttons */
        expect(await screen.findByLabelText("Save")).toBeInTheDocument();            
        expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();      

        let title = screen.getByLabelText("Title");
        const saveBtn = screen.getByLabelText("Save");

        fireEvent.change(title, { target: { value: "New Title" }});
        fireEvent.click(saveBtn);

        await waitForSpinner();

        title = await screen.findByLabelText("Title");
        expect(title).toHaveTextContent("New Title");                             
    });

    test("Change General fields, make sure they all save properly", async () => {
        
        render( <Application /> );        

        expect(await screen.findByLabelText("Edit Object")).toBeInTheDocument();

        fireEvent.click(screen.getByLabelText("Edit Object"));

        /* make sure we have save/cancel buttons */
        expect(await screen.findByLabelText("Save")).toBeInTheDocument();            
        expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();  
        
        const saveBtn = screen.getByLabelText("Save");

        const title = screen.getByLabelText("Title");
        fireEvent.change(title, { target: { value: "New Title" }});

        const description = screen.getByLabelText("Description");
        fireEvent.change(description, { target: { value: "New Description" }});

        const defaultAdapterId = screen.getByLabelText("Default Adapter");
        fireEvent.change(defaultAdapterId, { target: { value: "newAdapterId" }});

        const defaultModelAdapterId = screen.getByLabelText("Default Model Adapter");
        fireEvent.change(defaultModelAdapterId, { target: { value: "newAdapterId" }});

        const layoutsAdapterId = screen.getByLabelText("Layouts Adapter");
        fireEvent.change(layoutsAdapterId, { target: { value: "newAdapterId" }});

        fireEvent.click(saveBtn);        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("title", "New Title"));                        
        expect(await screen.findByLabelText("Title")).toHaveTextContent("New Title");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("description", "New Description"));        
        expect(await screen.findByLabelText("Description")).toHaveTextContent("New Description");    

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("defaultAdapterId", "newAdapterId"));        
        expect(await screen.findByLabelText("Default Adapter")).toHaveTextContent("newAdapterId");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("defaultModelAdapterId", "newAdapterId"));                
        expect(await screen.findByLabelText("Default Model Adapter")).toHaveTextContent("newAdapterId");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("layoutsAdapterId", "newAdapterId"));        
        expect(await screen.findByLabelText("Layouts Adapter")).toHaveTextContent("newAdapterId");

        await waitForSpinner();
 
    });

    test("Change Extension fields, make sure they all save properly", async () => {
        
        render( <Application /> );        

        expect(await screen.findByLabelText("Edit Object")).toBeInTheDocument();

        fireEvent.click(screen.getByLabelText("Edit Object"));

        /* make sure we have save/cancel buttons */
        expect(await screen.findByLabelText("Save")).toBeInTheDocument();            
        expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();  
        
        const saveBtn = screen.getByLabelText("Save");

        fireEvent.click(screen.getByLabelText("Startup"));        

        fireEvent.click(screen.getByLabelText("Add List Item"));                
        const input = await screen.findByLabelText("Extensions", { selector: "input" });     
        
        // add a new extension
        fireEvent.change(input, { target: { value: "my_ext" } });
        fireEvent.click(screen.getByLabelText("Ok"));        

        // remove the first extension
        fireEvent.click(screen.getByText(extensions[0]));
        fireEvent.click(screen.getByText("remove"));
        
        fireEvent.click(saveBtn);        

        await waitFor(() => expect(mswPostCallback).toHaveBeenLastCalledWithObjectContainingDeep(undefined, "my_ext"));        
        await waitFor(() => expect(mswPostCallback).not.toHaveBeenLastCalledWithObjectContainingDeep(undefined, extensions[0]));

        await waitForSpinner();
         
    });

    test("Change Flag fields, make sure they all save properly", async () => {
        
        render( <Application /> );        

        expect(await screen.findByLabelText("Edit Object")).toBeInTheDocument();

        fireEvent.click(screen.getByLabelText("Edit Object"));

        /* make sure we have save/cancel buttons */
        expect(await screen.findByLabelText("Save")).toBeInTheDocument();            
        expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();  
        
        fireEvent.click(screen.getByLabelText("Flags"));        

        fireEvent.click(screen.getByLabelText("Add List Item"));                
        const input = await screen.findByLabelText("Default Flags", { selector: "input" });     
        
        // add a new flag
        fireEvent.change(input, { target: { value: "my_flag" } });
        fireEvent.click(screen.getByLabelText("Ok"));        

        // remove the second flag
        fireEvent.click(screen.getByText(defaultFlags[1]));
        fireEvent.click(screen.getByText("remove"));
        
        const saveBtn = screen.getByLabelText("Save");
        fireEvent.click(saveBtn);

        await waitFor(() => expect(mswPostCallback).toHaveBeenLastCalledWithObjectContainingDeep(undefined, "my_flag"));        
        await waitFor(() => expect(mswPostCallback).not.toHaveBeenLastCalledWithObjectContainingDeep(undefined, defaultFlags[1])); 
        
        await waitForSpinner();
            
    });

    test("Change Authorization fields, make sure they all save properly", async () => {
        
        render( <Application /> );        

        expect(await screen.findByLabelText("Edit Object")).toBeInTheDocument();

        fireEvent.click(screen.getByLabelText("Edit Object"));

        /* make sure we have save/cancel buttons */
        expect(await screen.findByLabelText("Save")).toBeInTheDocument();            
        expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();  
                
        fireEvent.click(screen.getByLabelText("Authorization")); 
        expect(await screen.findByLabelText("Check Intermediate")).toBeInTheDocument();            
        expect(await screen.findByLabelText("Deny If Not Applicable")).toBeInTheDocument();  

        fireEvent.click(screen.getByLabelText("Check Intermediate"));        
        fireEvent.click(screen.getByLabelText("Deny If Not Applicable"));        
            
        //const coreCheckInput = await screen.findByLabelText("Core Check", { selector: "input" }); 
        const coreCheckInput = await screen.findByRole("textbox", { name: "Core Check" });    
        fireEvent.change(coreCheckInput, { target: { value: "return true;" } });        

        //const initialCheckInput = await screen.findByLabelText("Initial Check", { selector: "input" });  
        const initialCheckInput = await screen.findByRole("textbox", { name: "Initial Check" });     
        fireEvent.change(initialCheckInput, { target: { value: "return false;" } });        
        
        const saveBtn = screen.getByLabelText("Save");
        fireEvent.click(saveBtn);

        await waitFor(() => expect(mswPostCallback).toHaveBeenLastCalledWithObjectContainingDeep(undefined, "return true;"));        
        await waitFor(() => expect(mswPostCallback).toHaveBeenLastCalledWithObjectContainingDeep(undefined, "return false;"));
        
        await waitForSpinner();
               
    });

    test("Change Variables fields, make sure they all save properly", async () => {
        
        render( <Application /> );        

        expect(await screen.findByLabelText("Edit Object")).toBeInTheDocument();

        fireEvent.click(screen.getByLabelText("Edit Object"));

        /* make sure we have save/cancel buttons */
        expect(await screen.findByLabelText("Save")).toBeInTheDocument();            
        expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();  
                
        fireEvent.click(screen.getByLabelText("Variables"));   
        expect(await screen.findByText("Qualified Variables")).toBeInTheDocument();         

        const [/*qualifier*/, variables] = Object.entries(qualifiedVariables)[1];
        const [variable, value] = Object.entries(variables)[1];        

        //const initialCheckInput = await screen.findByLabelText(variable, { selector: "input" });     
        const initialCheckInput = await screen.findByRole("textbox", { name: variable });
        fireEvent.change(initialCheckInput, { target: { value: "new qualified variable value" } });        
        
        const saveBtn = screen.getByLabelText("Save");
        fireEvent.click(saveBtn);

        await waitFor(() => expect(mswPostCallback).toHaveBeenLastCalledWithObjectContainingDeep(variable, "new qualified variable value"));                
        await waitFor(() => expect(mswPostCallback).not.toHaveBeenLastCalledWithObjectContainingDeep(variable, value));        
        
        await waitForSpinner();
                 
    });
});
