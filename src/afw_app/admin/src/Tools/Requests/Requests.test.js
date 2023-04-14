// See the 'COPYING' file in the project root for licensing information.
import {render, waitFor, within, fireEvent, screen, waitForSpinner, server, rest, mswPostCallback, mswGetCallback} from "../../test-utils";
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
            rest.get("/afw/_AdaptiveObjectType_/_AdaptiveObjectType_", async (req, res, ctx) => {
                mswGetCallback("/afw/_AdaptiveObjectType_/_AdaptiveObjectType_", req, res, ctx);                
                return res(
                    ctx.status(200),
                    ctx.json({})
                );
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
        fireEvent.mouseDown(methodDropdown.getByRole("button"));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        let opts = screen.getAllByRole("option");
        opts[1].click();
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));

        fireEvent.change(screen.getByLabelText("URI"), { target: { value: "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_" } });        
        
        // select JSON
        const contentTypeDropdown = within(screen.getByTestId("requests-accept"));
        fireEvent.mouseDown(contentTypeDropdown.getByRole("button"));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        opts = screen.getAllByRole("option");
        opts[0].click();
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));

        fireEvent.click(screen.getByLabelText("Send"));
        
        await waitFor(() => expect(mswGetCallback).toHaveBeenCalledWith(
            "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_",
            expect.objectContaining({
                method: "GET"
            }),
            expect.anything(),
            expect.anything()
        ));     
        await waitForSpinner();  

        expect(await screen.findByText("{}")).toBeInTheDocument();

    });

    test("Make GET request to /afw/_AdaptiveObjectType_/_AdaptiveObjectType_ with XML accept", async () => {

        server.use(
            rest.get("/afw/_AdaptiveObjectType_/_AdaptiveObjectType_", async (req, res, ctx) => {
                mswGetCallback("/afw/_AdaptiveObjectType_/_AdaptiveObjectType_", req, res, ctx);                
                return res(
                    ctx.status(200),
                    ctx.xml("<xml />")
                );
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
        fireEvent.mouseDown(methodDropdown.getByRole("button"));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        let opts = screen.getAllByRole("option");
        opts[1].click();
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));

        fireEvent.change(screen.getByLabelText("URI"), { target: { value: "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_" } });        
        
        // select XML
        const contentTypeDropdown = within(screen.getByTestId("requests-accept"));
        fireEvent.mouseDown(contentTypeDropdown.getByRole("button"));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        opts = screen.getAllByRole("option");
        opts[1].click();
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));

        fireEvent.click(screen.getByLabelText("Send"));
        
        await waitFor(() => expect(mswGetCallback).toHaveBeenCalledWith(
            "/afw/_AdaptiveObjectType_/_AdaptiveObjectType_",
            expect.objectContaining({
                method: "GET"
            }),
            expect.anything(),
            expect.anything()
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
        fireEvent.mouseDown(contentTypeDropdown.getByRole("button"));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        const opts = screen.getAllByRole("option");
        opts[0].click();
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0));
        
        fireEvent.click(screen.getByLabelText("Send"));
        
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWith(
            "/afw",
            expect.anything(),
            expect.anything(),
            expect.anything()
        )); 
        await waitForSpinner();

    });
});
