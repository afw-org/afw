// See the 'COPYING' file in the project root for licensing information.
import {editor as monacoEditorMock} from "monaco-editor";
import {render, waitFor, within, fireEvent, screen, waitForSpinner, server, http, HttpResponse, mswPostCallback, mswGetCallback} from "../../test-utils";
import Requests from "./Requests";

/*
 * jsdom can't drive Monaco's real DOM/canvas editing surface, so the
 * Headers/Request/Response tabs are backed by
 * src/afw_test/javascript/src/__mocks__/monaco-editor.js. TabbedCodeEditor
 * keeps a single CodeEditor mounted and swaps its "source" prop as tabs are
 * switched, so there's exactly one mock editor instance for the whole
 * component's lifetime - grab it to simulate typing (__setValueAndFireChange)
 * or to read whatever tab is currently displayed (getValue()).
 */
const getLatestMonacoEditorInstance = () => {
    const results = monacoEditorMock.create.mock.results;
    return results[results.length - 1].value;
};

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

        await waitFor(() => expect(getLatestMonacoEditorInstance().getValue()).toBe("{}"));

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

        await waitFor(() => expect(getLatestMonacoEditorInstance().getValue()).toBe("<xml />"));

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

        // "Request" is the default active tab, so this is that tab's editor
        getLatestMonacoEditorInstance().__setValueAndFireChange(value);

        // the mock editor's onDidChangeModelContent handler is CodeEditor.js's
        // real 100ms debounce wrapper around onSourceChanged - give it a
        // moment to fire and land in Requests' tab state before Send reads it
        await new Promise(resolve => setTimeout(resolve, 150));

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
