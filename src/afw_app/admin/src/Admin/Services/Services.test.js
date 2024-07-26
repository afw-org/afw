// See the 'COPYING' file in the project root for licensing information.
import {MemoryRouter} from "react-router-dom";
import {render, waitFor, within, fireEvent, screen, waitForElementToBeRemoved, mswPostCallback, waitForSpinner, server, rest} from "../test-utils";
import Services from "./Services";

import serviceMeta from "@afw/test/build/cjs/__mocks__/get_object/afw/_AdaptiveObjectType_/_AdaptiveService_.json";
import services from "@afw/test/build/cjs/__mocks__/retrieve_objects/afw/_AdaptiveService_.json";

describe("Services Tests", () => {       

    let fileService, modelService, syslogService, authHandlerService;
    for (const service of services.result) {
        if (service.serviceId === "adapter-files")
            fileService = service;
        else if (service.serviceId == "adapter-models")
            modelService = service;
        else if (service.serviceId == "log-syslog")
            syslogService = service;
        else if (service.serviceId == "authorizationHandler-auth-script")
            authHandlerService = service;
    }

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    const waitForServiceConfigs = async () => {
        return waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingSparse({            
            "function": "retrieve_objects",
            "objectType": "_AdaptiveServiceConf_"            
        })
        );
    };

    test("Services renders", async () => {

        render( 
            <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                <Services /> 
            </MemoryRouter>
        );  

        await waitForServiceConfigs();        
        await screen.findByTestId("admin-admin-services");
        await waitForSpinner();                                  
    });

    test("Services displays error when unable to retrieve services", async () => {

        server.use(
            rest.post("/afw", (req, res, ctx) => {                  
                mswPostCallback("/afw", req, res, ctx);

                const {function: functionId, objectType} = req.body;                               

                if (functionId === "retrieve_objects" && objectType === "_AdaptiveServiceConf_") {                                             
                    return res(
                        ctx.status(200),
                        ctx.json({
                            status: "error",
                            error: {
                                message: "An error occurred"
                            }
                        })
                    );
                }
            })
        );

        render( 
            <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                <Services checkError /> 
            </MemoryRouter>
        );  

        await screen.findByTestId("admin-admin-services");
        await waitForSpinner();      

        await waitFor(() => expect(screen.getByRole("alert")).toBeInTheDocument());       
    });

    test("Table lists all services", async () => {

        render( 
            <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                <Services /> 
            </MemoryRouter>
        );        

        await waitForServiceConfigs();        
        await screen.findByTestId("admin-admin-services");
        await waitForSpinner();                      
        
        for (const [, value] of Object.entries(services.result)) {
            const {confId, serviceType, confSubtype, status} = value;

            await waitFor(() => expect(screen.getByText(confId)).toBeInTheDocument());
            const row = screen.getByText(confId).closest("tr");
            const utils = within(row);

            expect(utils.getByText(serviceType + "/" + confSubtype)).toBeInTheDocument();            
            expect(utils.getByText(status)).toBeInTheDocument();
        }           
    });

    test("Start a service", async () => {

        render( 
            <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                <Services /> 
            </MemoryRouter>
        );        

        await waitForServiceConfigs();        
        await screen.findByTestId("admin-admin-services");
        await waitForSpinner();                   
        await waitFor(() => expect(screen.getByText("test1")).toBeInTheDocument());

        mswPostCallback.mockClear();

        const row = screen.getByText("test1").closest("tr");
        fireEvent.click(row);

        await waitFor(() => expect(screen.getByLabelText("Start")));
        fireEvent.click(screen.getByLabelText("Start"));
      
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("service_start"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("serviceId", "adapter-test1"));

        /* wait for reloading services */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("retrieve_objects"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("objectType", "_AdaptiveService_"));
        
        /* wait for reloading adapters */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/afw/_AdaptiveEnvironmentRegistry_/current"));

        await waitForServiceConfigs();        
        await waitForSpinner();    
    });

    test("Stop a running service", async () => {

        render( 
            <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                <Services /> 
            </MemoryRouter>
        );        

        await waitForServiceConfigs();        
        await screen.findByTestId("admin-admin-services");

        await waitFor(() => expect(screen.getByText("syslog")).toBeInTheDocument());

        mswPostCallback.mockClear();

        const row = screen.getByText("syslog").closest("tr");
        fireEvent.click(row);

        await waitFor(() => expect(screen.getByLabelText("Stop")));
        fireEvent.click(screen.getByLabelText("Stop"));

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("service_stop"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("serviceId", "log-syslog"));

        /* wait for reloading services */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("retrieve_objects"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("objectType", "_AdaptiveService_"));

        /* wait for reloading adapters */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/afw/_AdaptiveEnvironmentRegistry_/current"));

        await waitForServiceConfigs();        
        await waitForSpinner();    
    });

    test("Restart a running service", async () => {

        render( 
            <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                <Services /> 
            </MemoryRouter>
        );        

        await waitForServiceConfigs();        
        await waitForSpinner();         
        await screen.findByTestId("admin-admin-services");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitFor(() => expect(screen.getByText("syslog")).toBeInTheDocument());

        mswPostCallback.mockClear();

        const row = screen.getByText("syslog").closest("tr");
        fireEvent.click(row);

        await waitFor(() => expect(screen.getByLabelText("Restart")));
        fireEvent.click(screen.getByLabelText("Restart"));

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("service_restart"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("serviceId", "log-syslog"));

        /* wait for reloading services */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("retrieve_objects"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("objectType", "_AdaptiveService_"));

        /* wait for reloading adapters */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/afw/_AdaptiveEnvironmentRegistry_/current"));

        await waitForServiceConfigs();        
        await waitForSpinner(); 
    });

    test("Start, then stop a service", async () => {
        
        render( 
            <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                <Services /> 
            </MemoryRouter>
        );        

        await waitForServiceConfigs();        
        await waitForSpinner();         
        await screen.findByTestId("admin-admin-services");

        await waitFor(() => expect(screen.getByText("test1")).toBeInTheDocument());

        mswPostCallback.mockClear();

        let row = screen.getByText("test1").closest("tr");
        fireEvent.click(row);

        await waitFor(() => expect(screen.getByLabelText("Start")));
        fireEvent.click(screen.getByLabelText("Start"));

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("service_start"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("serviceId", "adapter-test1"));

        /* wait for reloading services */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("retrieve_objects"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("objectType", "_AdaptiveService_"));

        /* wait for reloading adapters */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/afw/_AdaptiveEnvironmentRegistry_/current"));

        await waitForServiceConfigs();        
        await waitForSpinner(); 

        await waitFor(() => expect(screen.getByText("test1")).toBeInTheDocument());

        /* Can't do this yet, until we can change mswPostCallback to return a started service now 

        // now stop it 
        mswPostCallback.mockClear();

        row = screen.getByText("test1").closest("tr");
        fireEvent.click(row);

        await waitFor(() => expect(screen.getByLabelText("Stop")));
        fireEvent.click(screen.getByLabelText("Stop"));

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWith(
            "/afw",
            expect.stringContaining("\"function\":\"service_stop\""),
            expect.anything()
        ));

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWith(
            "/afw",
            expect.stringContaining("\"serviceId\":\"adapter-test1\""),
            expect.anything()
        ));

         */
    });

    test("Delete a service", async () => {
        
        render( 
            <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                <Services /> 
            </MemoryRouter>
        );        
        
        await waitForServiceConfigs();        
        await waitForSpinner();         
        await screen.findByTestId("admin-admin-services");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitFor(() => expect(screen.getByText("test1")).toBeInTheDocument());

        mswPostCallback.mockClear();

        const row = screen.getByText("test1").closest("tr");
        fireEvent.click(row);

        await waitFor(() => expect(screen.getByLabelText("Delete")).not.toBeDisabled());
        fireEvent.click(screen.getByLabelText("Delete"));

        // confirm Delete
        await waitFor(() => expect(screen.getByLabelText("Confirm Delete")));
        fireEvent.click(screen.getByLabelText("Confirm Delete"));       

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("delete_object_with_uri"));      
        
        /* wait for reloading services */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("retrieve_objects"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("objectType", "_AdaptiveService_"));

        /* wait for reloading adapters */
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/afw/_AdaptiveEnvironmentRegistry_/current"));

        await waitForServiceConfigs();        
        await waitForSpinner(); 
    });

    describe("Creating new services", () => {

        test("Create a new adapter/file service", async () => {

            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitForSpinner(); 
            
            await screen.findByTestId("admin-admin-services");
            const newBtn = await screen.findByLabelText("New");
            fireEvent.click(newBtn);
    
            expect(await screen.findByLabelText("Next")).toBeInTheDocument();      
            expect(await screen.findByLabelText("Previous")).toBeInTheDocument();            
            expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();                                                         
            
            const fileAdapterBtn = await screen.findByLabelText("file");

            await waitFor(() => expect(screen.getByLabelText("Next")).toHaveAttribute("disabled"));
            fireEvent.click(fileAdapterBtn);
            await waitFor(() => expect(screen.getByLabelText("Next")).not.toHaveAttribute("disabled"));

            fireEvent.click(screen.getByLabelText("Next"));

            await waitFor(() => expect(screen.getByLabelText("Adapter *")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Description")).toBeInTheDocument());

            fireEvent.change(screen.getByLabelText("Adapter *"), { target: { value: "test2" }});
            fireEvent.change(screen.getByLabelText("Description"), { target: { value: "New file adapter service test2." }});

            const radioGroup = screen.getByLabelText("Startup Condition");                                  
            await waitFor(() => expect(within(radioGroup).getByLabelText("Manual")).toHaveAttribute("checked"));

            fireEvent.click(within(radioGroup).getByLabelText("Immediate"));

            fireEvent.click(screen.getByLabelText("Next"));

            await waitFor(() => expect(screen.getByLabelText("Authorization Handler")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Journal Adapter")).toBeInTheDocument());              
            
            const authHandler = screen.getByRole("button", { name: /Authorization Handler */i });
            let authHandlerOpts;
            fireEvent.mouseDown(authHandler);                                   
            await waitFor(() => expect(authHandlerOpts = screen.queryAllByRole("option")).not.toHaveLength(0));            
            authHandlerOpts[0].click();

            fireEvent.click(screen.getByLabelText("Next"));

            await waitFor(() => expect(screen.getByLabelText("Content Type *")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Is Development Input")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Is Model Location")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Is Policy Location")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Filename Suffix")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Root *")).toBeInTheDocument());

            const contentType = screen.getByRole("button", { name: /Content Type */i });
            fireEvent.mouseDown(contentType);   
            let contentTypeOpts;                                
            await waitFor(() => expect(contentTypeOpts = screen.queryAllByRole("option")).not.toHaveLength(0));                    
            contentTypeOpts[0].click();

            // wait options to disappear
            await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));                        

            fireEvent.click(screen.getByLabelText("Is Development Input"));
            fireEvent.click(screen.getByLabelText("Is Model Location"));
            fireEvent.click(screen.getByLabelText("Is Policy Location"));

            fireEvent.change(screen.getByLabelText("Filename Suffix"), { target: { value: ".json" }});
            fireEvent.change(screen.getByLabelText("Root *"), { target: { value: "/some/path/test2" }});                          

            await waitFor(() => expect(screen.getByLabelText("Create")).not.toHaveAttribute("disabled"));
            mswPostCallback.mockClear();
            fireEvent.click(screen.getByLabelText("Create"));

            await waitForElementToBeRemoved(() => screen.queryByTestId("service-new"));

            await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("add_object_with_uri"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("contentType", "json"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("isDevelopmentInput", true));                

            await waitForSpinner();                        

            await waitFor(() => expect(screen.getByText("Start service now?")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Yes")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("No")).toBeInTheDocument());              
        });

        test("Create a new adapter/model service", async () => {

            let utils;

            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services debug /> 
                </MemoryRouter>
            );        
                
            await waitForServiceConfigs();                    
            await waitForSpinner(); 
            
            await screen.findByTestId("admin-admin-services");
            const newBtn = await screen.findByLabelText("New");
            fireEvent.click(newBtn);
    
            expect(await screen.findByLabelText("Next")).toBeInTheDocument();      
            expect(await screen.findByLabelText("Previous")).toBeInTheDocument();            
            expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();                            
    
            // render the baseElement (body) to also include the New Service dialog            

            const modelAdapterBtn = await screen.findByLabelText("model");
            await waitFor(() => expect(screen.getByLabelText("Next")).toHaveAttribute("disabled"));
            fireEvent.click(modelAdapterBtn);
            await waitFor(() => expect(screen.getByLabelText("Next")).not.toHaveAttribute("disabled"));

            fireEvent.click(screen.getByLabelText("Next"));

            await waitFor(() => expect(screen.getByLabelText("Adapter *")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Description")).toBeInTheDocument());

            fireEvent.change(screen.getByLabelText("Adapter *"), { target: { value: "test3" }});
            fireEvent.change(screen.getByLabelText("Description"), { target: { value: "New model adapter service test3." }});

            const radioGroup = screen.getByLabelText("Startup Condition");
            utils = within(radioGroup);                        
            await waitFor(() => expect(utils.getByLabelText("Manual")).toHaveAttribute("checked"));

            fireEvent.click(utils.getByLabelText("Immediate"));         
            fireEvent.click(screen.getByLabelText("Next"));

            await waitFor(() => expect(screen.getByLabelText("Authorization Handler")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Journal Adapter")).toBeInTheDocument());               
            
            const authHandler = screen.getByRole("button", { name: /Authorization Handler */i });
            fireEvent.mouseDown(authHandler);           
            let authHandlerOpts;                        
            await waitFor(() => expect(authHandlerOpts = screen.queryAllByRole("option")).not.toHaveLength(0));            
            authHandlerOpts[0].click();

            const nextBtn = screen.getByLabelText("Next");
            fireEvent.click(nextBtn);

            await waitFor(() => expect(screen.getByLabelText("Model Adapter *")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Model *")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Mapped Adapter *")).toBeInTheDocument());

            // select the model adapterId                        
            const modelAdapter = within(screen.getByTestId("model-location-adapter-dropdown"));
            fireEvent.mouseDown(modelAdapter.getByRole("button"));   
            let modelAdapterOpts;                  
            await waitFor(() => expect(modelAdapterOpts = screen.queryAllByRole("option")).not.toHaveLength(0));            
            modelAdapterOpts[0].click();
            // wait options to disappear
            await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));                        

            // async calls to fetch models adapter
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
            await waitForSpinner(); 
            
            // select the model
            const models = within(screen.getByTestId("model-dropdown"));
            fireEvent.mouseDown(models.getByRole("button"));                                               
            let modelOpts;
            await waitFor(() => expect(modelOpts = screen.queryAllByRole("option")).not.toHaveLength(0));            
            modelOpts[0].click();
            // wait options to disappear
            await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));    
            
            // select he mapped adapter
            const mappedAdapter = within(screen.getByTestId("mapped-adapter-dropdown"));
            fireEvent.mouseDown(mappedAdapter.getByRole("button"));  
            let mappedAdapterOpts;                                                          
            await waitFor(() => expect(mappedAdapterOpts = screen.queryAllByRole("option")).not.toHaveLength(0));                      
            mappedAdapterOpts[7].click();
            // wait options to disappear
            await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));  

            let createBtn;
            await waitFor(() => expect(createBtn = screen.getByLabelText("Create")).not.toHaveAttribute("disabled"));
            mswPostCallback.mockClear();
            fireEvent.click(createBtn);

            await waitForElementToBeRemoved(() => screen.queryByTestId("service-new"));

            await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("add_object_with_uri"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("modelLocationAdapterId", "models"));
              
            await waitForSpinner();

            await waitFor(() => expect(screen.getByText("Start service now?")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Yes")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("No")).toBeInTheDocument());
        });

        test("Create a new log/syslog service", async () => {

            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitForSpinner(); 
            
            await screen.findByTestId("admin-admin-services");
            const newBtn = await screen.findByLabelText("New");
            fireEvent.click(newBtn);
    
            expect(await screen.findByLabelText("Next")).toBeInTheDocument();      
            expect(await screen.findByLabelText("Previous")).toBeInTheDocument();            
            expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();                                     
    
            // render the baseElement (body) to also include the New Service dialog
            await waitFor(() => expect(screen.getByLabelText("syslog")));

            await waitFor(() => expect(screen.getByLabelText("Next")).toHaveAttribute("disabled"));
            fireEvent.click(screen.getByLabelText("syslog"));
            await waitFor(() => expect(screen.getByLabelText("Next")).not.toHaveAttribute("disabled"));

            fireEvent.click(screen.getByLabelText("Next"));

            await waitFor(() => expect(screen.getByLabelText("Log *")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Description")).toBeInTheDocument());

            fireEvent.change(screen.getByLabelText("Log *"), { target: { value: "test4" }});
            fireEvent.change(screen.getByLabelText("Description"), { target: { value: "New syslog service test4." }});

            const radioGroup = screen.getByLabelText("Startup Condition");                                    
            await waitFor(() => expect(within(radioGroup).getByLabelText("Manual")).toHaveAttribute("checked"));

            fireEvent.click(within(radioGroup).getByLabelText("Immediate"));
            
            fireEvent.click(screen.getByLabelText("Next"));            

            await waitFor(() => expect(screen.getByLabelText("Critical")).toBeInTheDocument());

            fireEvent.click(screen.getByLabelText("Critical"));
            fireEvent.click(screen.getByLabelText("Emergency"));
            fireEvent.click(screen.getByLabelText("Error"));
            fireEvent.click(screen.getByLabelText("Warning"));
            fireEvent.click(screen.getByLabelText("Notice"));
            fireEvent.click(screen.getByLabelText("Informational"));
            fireEvent.click(screen.getByLabelText("Debug"));
            fireEvent.click(screen.getByLabelText("Trace1"));            

            fireEvent.change(screen.getByLabelText("Format"), { target: { value: "Log Format ${message}" }});
            fireEvent.change(screen.getByLabelText("Filter"), { target: { value: "true" }});                    
            
            fireEvent.click(screen.getByLabelText("Next"));  

            await waitFor(() => expect(screen.getByLabelText("Facility")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Identifier")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Write to Console")).toBeInTheDocument());

            fireEvent.change(screen.getByLabelText("Facility"), { target: { value: "facility" }});
            fireEvent.change(screen.getByLabelText("Identifier"), { target: { value: "identifier" }});

            fireEvent.click(screen.getByLabelText("Write to Console"));        
            fireEvent.click(screen.getByLabelText("Connect Immediately"));        
            fireEvent.click(screen.getByLabelText("No Wait"));        
            fireEvent.click(screen.getByLabelText("Delay Connection"));        
            fireEvent.click(screen.getByLabelText("Standard Error"));        
            fireEvent.click(screen.getByLabelText("Log PID"));        

            await waitFor(() => expect(screen.getByLabelText("Create")).not.toHaveAttribute("disabled"));
            mswPostCallback.mockClear();
            fireEvent.click(screen.getByLabelText("Create"));

            await waitForElementToBeRemoved(() => screen.queryByTestId("service-new"));

            await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("add_object_with_uri"));            
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("crit", true));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("emerg", true));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("err", true));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("warning", true));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("notice", true));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("info", true));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("debug", true));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("trace1", true));  

            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("format", "Log Format ${message}"));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("filter", "true"));  

            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("facility", "facility"));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("ident", "identifier"));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("optionLogCons", true));  
               
            await waitForSpinner();

            await waitFor(() => expect(screen.getByText("Start service now?")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Yes")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("No")).toBeInTheDocument());

        });

        test("Create a new authorizationHandler/script service", async () => {

            let utils;

            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitForSpinner(); 
            
            await screen.findByTestId("admin-admin-services");
            const newBtn = await screen.findByLabelText("New");
            fireEvent.click(newBtn);
    
            expect(await screen.findByLabelText("Next")).toBeInTheDocument();      
            expect(await screen.findByLabelText("Previous")).toBeInTheDocument();            
            expect(await screen.findByLabelText("Cancel")).toBeInTheDocument();                                     
            
    
            // render the baseElement (body) to also include the New Service dialog
            await waitFor(() => expect(screen.getByLabelText("script")));

            await waitFor(() => expect(screen.getByLabelText("Next")).toHaveAttribute("disabled"));
            fireEvent.click(screen.getByLabelText("script"));
            await waitFor(() => expect(screen.getByLabelText("Next")).not.toHaveAttribute("disabled"));

            fireEvent.click(screen.getByLabelText("Next"));

            await waitFor(() => expect(screen.getByLabelText("Authorization Id *")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Description")).toBeInTheDocument());

            fireEvent.change(screen.getByLabelText("Authorization Id *"), { target: { value: "test5" }});
            fireEvent.change(screen.getByLabelText("Description"), { target: { value: "New authHandler/script service test5." }});

            const radioGroup = screen.getByLabelText("Startup Condition");
            utils = within(radioGroup);                        
            await waitFor(() => expect(utils.getByLabelText("Manual")).toHaveAttribute("checked"));

            fireEvent.click(utils.getByLabelText("Immediate"));
            
            fireEvent.click(screen.getByLabelText("Next"));            

            await waitFor(() => expect(screen.getByLabelText("Allow Deny Override")).toBeInTheDocument());

            fireEvent.click(screen.getByLabelText("Allow Deny Override"));
            fireEvent.click(screen.getByLabelText("Allow Permit Override"));
            fireEvent.click(screen.getByLabelText("Required"));                      

            fireEvent.change(screen.getByLabelText("Priority"), { target: { value: "10" }});            
            
            
            fireEvent.click(screen.getByLabelText("Next"));  

            const authCheck = await screen.findByRole("textbox", { name: "Authorization Check" });

            fireEvent.change(authCheck, { target: { value: "return { \"decisionId\": \"permit\" };" }});            

            await waitFor(() => expect(screen.getByLabelText("Create")).not.toHaveAttribute("disabled"));
            mswPostCallback.mockClear();
            fireEvent.click(screen.getByLabelText("Create"));

            await waitForElementToBeRemoved(() => screen.queryByTestId("service-new"));

            await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("add_object_with_uri"));  
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("required", true));  
            
            await waitForSpinner();

            await waitFor(() => expect(screen.getByText("Start service now?")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Yes")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("No")).toBeInTheDocument());

        });
    });       

    describe("Editing services", () => {      

        test("View adapter/file service tabs", async () => {
           
            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitForSpinner(); 

            await waitFor(() => expect(screen.getByText("files")).toBeInTheDocument());

            const row = screen.getByText("files").closest("tr");
            fireEvent.click(row);

            await waitFor(() => expect(screen.getByLabelText("Edit")));
            fireEvent.click(screen.getByLabelText("Edit"));

            await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());       
            await waitForSpinner();  
            
            // examine the General tab
            fireEvent.click(screen.getByLabelText("General"));
            for (const property of Object.keys(serviceMeta.result.propertyTypes)) {
                const label = serviceMeta.result.propertyTypes[property].label;
                const value = fileService[property];

                if (value) {
                    await waitFor(() => expect(screen.getByLabelText(label)).toHaveTextContent(value));                    
                }
            }    
            
            // examine the Configuration tab
            fireEvent.click(screen.getByLabelText("Configuration"));
            await waitFor(() => expect(screen.getByLabelText("Description")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Content Type")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Is Development Input")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Filename Suffix")).toBeInTheDocument());
            await waitFor(() => expect(screen.getByLabelText("Root")).toBeInTheDocument());
            // examine the Runtime tab
            fireEvent.click(screen.getByLabelText("Runtime"));
            
        });

        test("Edit an adapter/file service", async () => {

            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitForSpinner(); 

            await waitFor(() => expect(screen.getByText("files")).toBeInTheDocument());

            const row = screen.getByText("files").closest("tr");
            fireEvent.click(row);

            await waitFor(() => expect(screen.getByLabelText("Edit")));
            fireEvent.click(screen.getByLabelText("Edit"));

            await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());        

            await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());
            fireEvent.click(screen.getByLabelText("Edit Object"));
            
            await waitForSpinner(); 

            // examine the General tab
            fireEvent.click(screen.getByLabelText("General"));            
            await waitFor(() => expect(screen.getByLabelText("Service Id")).toHaveValue("adapter-files"));                
            const radioGroup = screen.getByLabelText("Startup Condition");
            const utils = within(radioGroup);            
            
            await waitFor(() => expect(utils.getByLabelText("Immediate")).toHaveAttribute("checked"));
            // change the serviceId
            fireEvent.change(screen.getByLabelText("Service Id"), { target: { value: "adapter-filesXYZ" } });      

            // examine the Configuration tab
            fireEvent.click(screen.getByLabelText("Configuration"));            
            // change the description
            fireEvent.change(screen.getByLabelText("Description"), { target: { value: "This is a new description." } });  

            mswPostCallback.mockClear();
               
            // click Save
            fireEvent.click(screen.getByLabelText("Save"));

            await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("reconcile_object"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("serviceId", "adapter-filesXYZ"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("description", "This is a new description."));
             
            await waitForSpinner();

        });

        test("View adapter/model service tabs", async () => {

            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitFor(() => expect(screen.getByText("models")).toBeInTheDocument());

            const row = screen.getByText("models").closest("tr");
            fireEvent.click(row);

            await waitFor(() => expect(screen.getByLabelText("Edit")));
            fireEvent.click(screen.getByLabelText("Edit"));

            await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());      
            await waitForSpinner(); 
            
            // examine the General tab
            fireEvent.click(screen.getByLabelText("General"));
            for (const property of Object.keys(serviceMeta.result.propertyTypes)) {
                const label = serviceMeta.result.propertyTypes[property].label;
                const value = modelService[property];

                if (value) {
                    await waitFor(() => expect(screen.getByLabelText(label)).toHaveTextContent(value));
                }
            }                   
            
            // examine the Configuration tab
            fireEvent.click(screen.getByLabelText("Configuration"));            
            // examine the Runtime tab
            fireEvent.click(screen.getByLabelText("Runtime"));
        });
    
        test("Edit a adapter/model service", async () => {

            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitForSpinner(); 

            await waitFor(() => expect(screen.getByText("tier")).toBeInTheDocument());

            const row = screen.getByText("tier").closest("tr");
            fireEvent.click(row);

            await waitFor(() => expect(screen.getByLabelText("Edit")));
            fireEvent.click(screen.getByLabelText("Edit"));

            await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());
            fireEvent.click(screen.getByLabelText("Edit Object"));

            await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());        
            await waitFor(() => expect(screen.getByLabelText("Service Id")).toHaveValue("adapter-tier"));                
            const radioGroup = screen.getByLabelText("Startup Condition");
            const utils = within(radioGroup);            
            
            await waitFor(() => expect(utils.getByLabelText("Immediate")).toHaveAttribute("checked"));
            // change the serviceId
            fireEvent.change(screen.getByLabelText("Service Id"), { target: { value: "adapter-tierXYZ" } });      

            // examine the Configuration tab
            fireEvent.click(screen.getByLabelText("Configuration"));            
            // change the description
            fireEvent.change(screen.getByLabelText("Description"), { target: { value: "This is a new description." } });  

            mswPostCallback.mockClear();
               
            // click Save
            fireEvent.click(screen.getByLabelText("Save"));

            await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("reconcile_object"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("serviceId", "adapter-tierXYZ"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("description", "This is a new description."));
                    
            await waitForSpinner();

        });

        test("View log/syslog service tabs", async () => {

            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitForSpinner(); 

            await waitFor(() => expect(screen.getByText("syslog")).toBeInTheDocument());

            const row = screen.getByText("syslog").closest("tr");
            fireEvent.click(row);

            await waitFor(() => expect(screen.getByLabelText("Edit")));
            fireEvent.click(screen.getByLabelText("Edit"));

            await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());  
            await waitForSpinner();     
            
            // examine the General tab
            fireEvent.click(screen.getByLabelText("General"));
            for (const property of Object.keys(serviceMeta.result.propertyTypes)) {
                const label = serviceMeta.result.propertyTypes[property].label;
                const value = syslogService[property];

                if (value) {
                    await waitFor(() => expect(screen.getByLabelText(label)).toHaveTextContent(value));
                }
            }                   
            
            // examine the Configuration tab
            fireEvent.click(screen.getByLabelText("Configuration"));                    
            
            // examine the Runtime tab
            fireEvent.click(screen.getByLabelText("Runtime"));
        });

        test("Edit a log/syslog service", async () => {
            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitForSpinner(); 

            await waitFor(() => expect(screen.getByText("syslog")).toBeInTheDocument());

            const row = screen.getByText("syslog").closest("tr");
            fireEvent.click(row);

            await waitFor(() => expect(screen.getByLabelText("Edit")));
            fireEvent.click(screen.getByLabelText("Edit"));

            await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());
            fireEvent.click(screen.getByLabelText("Edit Object"));

            await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());     
            await waitForSpinner();    

            // change the serviceId
            fireEvent.change(screen.getByLabelText("Service Id"), { target: { value: "log-syslogXYZ" } });      

            // examine the Configuration tab
            fireEvent.click(screen.getByLabelText("Configuration"));            
            // change the description
            fireEvent.change(screen.getByLabelText("Description"), { target: { value: "This is a new description." } });  

            mswPostCallback.mockClear();
               
            // click Save
            fireEvent.click(screen.getByLabelText("Save"));

            await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("reconcile_object"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("serviceId", "log-syslogXYZ"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("description", "This is a new description."));
                 
            await waitForSpinner();

        });

        test("View authorizationHandler/script service tabs", async () => {

            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitForSpinner(); 

            await waitFor(() => expect(screen.getByText("auth-script")).toBeInTheDocument());

            const row = screen.getByText("auth-script").closest("tr");
            fireEvent.click(row);

            await waitFor(() => expect(screen.getByLabelText("Edit")));
            fireEvent.click(screen.getByLabelText("Edit"));

            await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());      
            await waitForSpinner(); 
            
            // examine the General tab
            fireEvent.click(screen.getByLabelText("General"));
            for (const property of Object.keys(serviceMeta.result.propertyTypes)) {
                const label = serviceMeta.result.propertyTypes[property].label;
                const value = authHandlerService[property];

                if (value) {
                    await waitFor(() => expect(screen.getByLabelText(label)).toHaveTextContent(value));
                }
            }                   
            
            // examine the Configuration tab
            fireEvent.click(screen.getByLabelText("Configuration"));            
            // examine the Runtime tab
            fireEvent.click(screen.getByLabelText("Runtime"));
        });

        test("Edit a authorizationHandler/script service", async () => {

            render( 
                <MemoryRouter initialEntries={[ "/Admin/Services" ]}>
                    <Services /> 
                </MemoryRouter>
            );        
    
            await waitForServiceConfigs();        
            await waitForSpinner(); 

            await waitFor(() => expect(screen.getByText("auth-script")).toBeInTheDocument());

            const row = screen.getByText("auth-script").closest("tr");
            fireEvent.click(row);

            await waitFor(() => expect(screen.getByLabelText("Edit")));
            fireEvent.click(screen.getByLabelText("Edit"));

            await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());
            fireEvent.click(screen.getByLabelText("Edit Object"));

            await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
            await waitForSpinner();     

            // change the serviceId
            fireEvent.change(screen.getByLabelText("Service Id"), { target: { value: "authorizationHandler-auth-scriptXYZ" } });      

            // examine the Configuration tab
            fireEvent.click(screen.getByLabelText("Configuration"));            
            expect(await screen.findByLabelText("Add New Qualified Variables Property")).toBeInTheDocument();
            // change the description
            fireEvent.change(screen.getByLabelText("Description"), { target: { value: "This is a new description." } });  

            mswPostCallback.mockClear();
               
            // click Save
            fireEvent.click(screen.getByLabelText("Save"));

            await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("reconcile_object"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("serviceId", "authorizationHandler-auth-scriptXYZ"));
            await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("description", "This is a new description."));
             
            await waitForSpinner();

        });
    });    

});
