// See the 'COPYING' file in the project root for licensing information.
import {render, waitFor, within, fireEvent, screen, waitForSpinner, server, http, HttpResponse, mswPostCallback, mswGetCallback} from "../../test-utils";
import Requests from "./Requests";

describe("Requests Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();       
        mswGetCallback.mockClear();         
    });    

    test("Requests renders", async () => {

        render( <Requests /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());  
        await waitForSpinner();                      
        await waitFor(() => expect(screen.getByTestId("admin-tools-requests")).toBeInTheDocument());

    });

    test("Make GET request to /afw/_AdaptiveObjectType_/_AdaptiveObjectType_ with JSON accept", async () => {

        server.use(
            http.get("/afw/_AdaptiveObjectType_/_AdaptiveObjectType_", ({request}) => {
                mswGetCallback("/afw/_AdaptiveObjectType_/_AdaptiveObjectType_", {method: request.method, url: request.url, headers: request.headers});
                return HttpResponse.json({});
            })
        );

        render( <Requests /> );
   
        await waitForSpinner();      
        
        await waitFor(() => expect(screen.getByLabelText("Method")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("URI")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Content-Type")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Send")).toBeInTheDocument());

        // select GET
        const methodDropdown = within(screen.getByTestId("requests-method"));
        fireEvent.mouseDown(methodDropdown.getByRole("combobox"));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        let opts = screen.getAllByRole("option");
        opts[1].click();
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));

        fireEvent.change(screen.getByLabelText("URI"), { target: { value: "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_" } });        
        
        // select JSON
        const contentTypeDropdown = within(screen.getByTestId("requests-accept"));
        fireEvent.mouseDown(contentTypeDropdown.getByRole("combobox"));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        opts = screen.getAllByRole("option");
        opts[0].click();
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));

        fireEvent.click(screen.getByLabelText("Send"));
        
        await waitFor(() => expect(mswGetCallback).toHaveBeenCalledWith(
            "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_",
            expect.objectContaining({
                method: "GET"
            })
        ));
        await waitForSpinner();

        expect(await screen.findByText("{}")).toBeInTheDocument();

    });

    test("Make GET request to /afw/_AdaptiveObjectType_/_AdaptiveObjectType_ with XML accept", async () => {

        server.use(
            http.get("/afw/_AdaptiveObjectType_/_AdaptiveObjectType_", ({request}) => {
                mswGetCallback("/afw/_AdaptiveObjectType_/_AdaptiveObjectType_", {method: request.method, url: request.url, headers: request.headers});
                return HttpResponse.xml("<xml />");
            })
        );

        render( <Requests /> );
 
        await waitForSpinner();      
        
        await waitFor(() => expect(screen.getByLabelText("Method")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("URI")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Content-Type")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Send")).toBeInTheDocument());

        // select GET
        const methodDropdown = within(screen.getByTestId("requests-method"));
        fireEvent.mouseDown(methodDropdown.getByRole("combobox"));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        let opts = screen.getAllByRole("option");
        opts[1].click();
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));

        fireEvent.change(screen.getByLabelText("URI"), { target: { value: "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_" } });        
        
        // select XML
        const contentTypeDropdown = within(screen.getByTestId("requests-accept"));
        fireEvent.mouseDown(contentTypeDropdown.getByRole("combobox"));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        opts = screen.getAllByRole("option");
        opts[1].click();
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));

        fireEvent.click(screen.getByLabelText("Send"));
        
        await waitFor(() => expect(mswGetCallback).toHaveBeenCalledWith(
            "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_",
            expect.objectContaining({
                method: "GET"
            })
        ));
        await waitForSpinner();

        expect(await screen.findByText("<xml />")).toBeInTheDocument();

    });

    test("Make POST request to /afw", async () => {

        render( <Requests /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());   
        await waitForSpinner();        

        await waitFor(() => expect(screen.getByLabelText("Method")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("URI")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Content-Type")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Send")).toBeInTheDocument());        

        const value = JSON.stringify({
            "function": "print",
            "arg": "hi"
        });
        
        const request = screen.getByTestId("request");
        fireEvent.change(request, { target: { value } });

        // select JSON
        const contentTypeDropdown = within(screen.getByTestId("requests-accept"));
        fireEvent.mouseDown(contentTypeDropdown.getByRole("combobox"));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        const opts = screen.getAllByRole("option");
        opts[0].click();
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));
        
        fireEvent.click(screen.getByLabelText("Send"));
        
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWith(
            "/afw",
            expect.anything()
        ));
        await waitForSpinner();

    });
});
