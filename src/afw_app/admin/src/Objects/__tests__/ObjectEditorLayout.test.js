// See the 'COPYING' file in the project root for licensing information.
import {createMemoryHistory} from "history";
import {Router} from "react-router-dom";
import {editor as monacoEditorMock} from "monaco-editor";

import {render, waitFor, within, fireEvent, screen, waitForSpinner, mswPostCallback} from "../../test-utils";

import objectTypeObject from "@afw/test/build/cjs/__mocks__/get_object/files/_AdaptiveObjectType_/_AdaptiveObjectType_.json";
import Objects from "../Objects";

/*
 * jsdom can't drive Monaco's real DOM/canvas editing surface, so the "View
 * object source" CodeEditor is backed by src/afw_test/javascript/src/__mocks__/monaco-editor.js.
 * This grabs the most recently created mock editor instance (the one behind
 * whichever CodeEditor is currently on screen) so a test can simulate typing
 * via its __setValueAndFireChange() test helper.
 */
const getLatestMonacoEditorInstance = () => {
    const results = monacoEditorMock.create.mock.results;
    return results[results.length - 1].value;
};


describe("ObjectsEditorLayout Tests", () => { 

    test("View individual object", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();
        
    });

    test("Verify non-editable object", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();
        
        let objectPropertiesChecked = 0, arrayPropertiesChecked = 0, scalarPropertiesChecked = 0;

        for (const [propertyName, propertyType] of Object.entries(objectTypeObject.result.propertyTypes)) {
            if (propertyName === "_meta_")
                continue;

            const {dataType, label} = propertyType;
            const value = objectTypeObject.result[propertyName];

            if (value && dataType === "object") {
                objectPropertiesChecked++;
                let propertyButton;
                // eslint-disable-next-line jest/no-conditional-expect -- guarded against vacuousness below (objectPropertiesChecked)
                await waitFor(() => expect(propertyButton = screen.getByRole("button", { name: label })).toBeInTheDocument());

                // click on the propertyType to expand the embedded object
                fireEvent.click(propertyButton);
            }

            else if (value && dataType === "array") {
                arrayPropertiesChecked++;
                let list;
                // eslint-disable-next-line jest/no-conditional-expect -- guarded against vacuousness below (arrayPropertiesChecked)
                await waitFor(() => expect(list = screen.getByRole("list", { name: label })).toBeInTheDocument());

                for (const v of value) {
                    // eslint-disable-next-line jest/no-conditional-expect -- guarded against vacuousness below (arrayPropertiesChecked)
                    expect(within(list).getByText(v)).toBeInTheDocument();
                }
            }

            else if (value) {
                scalarPropertiesChecked++;
                // eslint-disable-next-line jest/no-conditional-expect -- guarded against vacuousness below (scalarPropertiesChecked)
                await waitFor(() => expect(screen.getByLabelText(label)).toHaveTextContent(value));
            }
        }

        // guard against the fixture changing in a way that silently makes one of the branches above vacuous
        expect(objectPropertiesChecked).toBeGreaterThan(0);
        expect(arrayPropertiesChecked).toBeGreaterThan(0);
        expect(scalarPropertiesChecked).toBeGreaterThan(0);

    });

    test("View non-editable object in different perspectives", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();
        
        let toolbar;
        await waitFor(() => expect(toolbar = screen.getByTestId("ObjectEditorLayout-Toolbar")).toBeInTheDocument());       
        
        const responsiveButton = within(toolbar).getByRole("button", { name: "View object in Responsive View" });        
        //fireEvent.click(button);
        
        const flattenedButton = within(toolbar).getByRole("button", { name: "View object in Flattened View" });        
        //fireEvent.click(button);

        const treeButton = within(toolbar).getByRole("button", { name: "View object in Tree View" });        
        //fireEvent.click(button); 

        const sourceButton = within(toolbar).getByRole("button", { name: "View object source" });        
        //fireEvent.click(button);  

        fireEvent.click(responsiveButton);
        fireEvent.click(flattenedButton);
        fireEvent.click(treeButton);
        fireEvent.click(sourceButton);

    });

    test("Verify editable object", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Edit Object"));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 

        await waitForSpinner();
        
        let objectPropertiesChecked = 0, booleanPropertiesChecked = 0, scalarPropertiesChecked = 0;

        for (const [propertyName, propertyType] of Object.entries(objectTypeObject.result.propertyTypes)) {
            if (propertyName === "_meta_")
                continue;

            const {dataType, label} = propertyType;
            const value = objectTypeObject.result[propertyName];

            if (value && dataType === "object") {
                objectPropertiesChecked++;
                let propertyButton;
                // eslint-disable-next-line jest/no-conditional-expect -- guarded against vacuousness below (objectPropertiesChecked)
                await waitFor(() => expect(propertyButton = screen.getByRole("button", { name: label })).toBeInTheDocument());

                // click on the propertyType to expand the embedded object
                fireEvent.click(propertyButton);
            }

            else if (value && dataType === "array") {
                /*
                fixme
                await waitFor(() => expect(screen.getByRole("list", { name: label })).toBeInTheDocument());

                for (const v of value) {
                    expect(within(screen.getByRole("list", { name: label })).getByText(v)).toBeInTheDocument();
                }
                 */
            }

            else if (value !== undefined) {
                if (dataType === "boolean" && value === true) {
                    booleanPropertiesChecked++;
                    // eslint-disable-next-line jest/no-conditional-expect -- guarded against vacuousness below (booleanPropertiesChecked)
                    await waitFor(() => expect(screen.getByLabelText(label)).toBeChecked());
                }
                else {
                    scalarPropertiesChecked++;
                    // eslint-disable-next-line jest/no-conditional-expect -- guarded against vacuousness below (scalarPropertiesChecked)
                    await waitFor(() => expect(screen.getByLabelText(label)).toHaveValue(value));
                }
            }
        }

        // guard against the fixture changing in a way that silently makes one of the branches above vacuous
        expect(objectPropertiesChecked).toBeGreaterThan(0);
        expect(booleanPropertiesChecked).toBeGreaterThan(0);
        expect(scalarPropertiesChecked).toBeGreaterThan(0);
    });

    test("View editable object in different perspectives", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));
        
        await waitForSpinner();
        
        await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Edit Object"));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 

        await waitForSpinner();
        
        let toolbar;
        await waitFor(() => expect(toolbar = screen.getByTestId("ObjectEditorLayout-Toolbar")).toBeInTheDocument());       
        
        const responsiveButton = within(toolbar).getByRole("button", { name: "View object in Responsive View" });        
        //fireEvent.click(button);        
  
        const flattenedButton = within(toolbar).getByRole("button", { name: "View object in Flattened View" });        
        //fireEvent.click(button);        
  
        const treeButton = within(toolbar).getByRole("button", { name: "View object in Tree View" });        
        //fireEvent.click(button);        
  
        const sourceButton = within(toolbar).getByRole("button", { name: "View object source" });        
        //fireEvent.click(button);        

        fireEvent.click(responsiveButton);
        fireEvent.click(flattenedButton);
        fireEvent.click(treeButton);
        fireEvent.click(sourceButton); 
    });

    test("Edit object and save the changes", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Edit Object"));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 

        await waitForSpinner();
        
        await waitFor(() => expect(screen.getByLabelText("Allow Add")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Description")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Object Id Property Name")).toBeInTheDocument());

        await waitFor(() => expect(screen.getByLabelText("Save")).not.toBeEnabled());

        fireEvent.click(screen.getByLabelText("Allow Add"));
        fireEvent.change(screen.getByLabelText("Description"), { target: { value: "A new description" } });
        fireEvent.change(screen.getByLabelText("Object Id Property Name"), { target: { value: "New Object Id Property Name" } });
        
        await waitFor(() => expect(screen.getByLabelText("Save")).toBeEnabled());


        mswPostCallback.mockClear();
        fireEvent.click(screen.getByLabelText("Save"));

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("reconcile_object"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("allowAdd", false));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("description", "A new description"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("objectIdPropertyName", "New Object Id Property Name"));

        await waitForSpinner();


    });

    test("Edit object then cancel the changes", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Edit Object"));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 

        await waitForSpinner();
        
        await waitFor(() => expect(screen.getByLabelText("Allow Add")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Description")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Object Id Property Name")).toBeInTheDocument());

        fireEvent.click(screen.getByLabelText("Allow Add"));
        fireEvent.change(screen.getByLabelText("Description"), { target: { value: "A new description" } });
        fireEvent.change(screen.getByLabelText("Object Id Property Name"), { target: { value: "New Object Id Property Name" } });
        
        await waitFor(() => expect(screen.getByLabelText("Cancel")).toBeEnabled());


        fireEvent.click(screen.getByLabelText("Cancel"));

        // confirm discard
        await waitFor(() => expect(screen.getByLabelText("Yes")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("Yes"));
        
        await waitFor(() => expect(screen.getByLabelText("Description")).toHaveTextContent(objectTypeObject.result.description));
        await waitFor(() => expect(screen.getByLabelText("Object Id Property Name")).toHaveTextContent(objectTypeObject.result.objectIdPropertyName));


    });

    test("Edit object source, then save the changes", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Edit Object"));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 

        await waitForSpinner();

        let toolbar, button;
        await waitFor(() => expect(toolbar = screen.getByTestId("ObjectEditorLayout-Toolbar")).toBeInTheDocument());             
        await waitFor(() => expect(button = within(toolbar).getByRole("button", { name: "View object source" })).toBeInTheDocument());
        fireEvent.click(button);               
        
        await waitFor(() => expect(screen.getByLabelText("Save")).not.toBeEnabled());
        const monacoInstance = getLatestMonacoEditorInstance();
        const source = monacoInstance.getValue();
        const position = source.indexOf("\"allowAdd\": true");

        monacoInstance.__setValueAndFireChange(
            source.slice(0, position + 11) + "false" + source.slice(position + 16)
        );

        await waitFor(() => expect(screen.getByLabelText("Save")).toBeEnabled());


        mswPostCallback.mockClear();
        fireEvent.click(screen.getByLabelText("Save"));

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("reconcile_object"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("allowAdd", false));

        await waitForSpinner();

        
    });

    test("Edit object source, then switch to View Object in Responsive View", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Edit Object"));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 

        await waitForSpinner();

        let button, toolbar;        
        await waitFor(() => expect(toolbar = screen.getByTestId("ObjectEditorLayout-Toolbar")).toBeInTheDocument());     
        await waitFor(() => expect(button = within(toolbar).getByRole("button", { name: "View object source" })).toBeInTheDocument());
        fireEvent.click(button);               
        
        await waitFor(() => expect(screen.getByLabelText("Save")).not.toBeEnabled());
        const monacoInstance = getLatestMonacoEditorInstance();
        const source = monacoInstance.getValue();
        const position = source.indexOf("\"allowAdd\": true");

        monacoInstance.__setValueAndFireChange(
            source.slice(0, position + 11) + "false" + source.slice(position + 16)
        );

        await waitFor(() => expect(screen.getByLabelText("Save")).toBeEnabled());

        fireEvent.click(screen.getByRole("button", { name: "View object in Responsive View" }));

        expect(screen.getByLabelText("Allow Add")).not.toBeChecked();
        
    });

    test("Edit object source, then cancel the changes", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Edit Object"));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 

        await waitForSpinner();

        let button, toolbar;        
        await waitFor(() => expect(toolbar = screen.getByTestId("ObjectEditorLayout-Toolbar")).toBeInTheDocument());     
        await waitFor(() => expect(button = within(toolbar).getByRole("button", { name: "View object source" })).toBeInTheDocument());
        fireEvent.click(button);               
        
        await waitFor(() => expect(screen.getByLabelText("Save")).not.toBeEnabled());
        const monacoInstance = getLatestMonacoEditorInstance();
        const source = monacoInstance.getValue();
        const position = source.indexOf("\"allowAdd\": true");

        monacoInstance.__setValueAndFireChange(
            source.slice(0, position + 11) + "false" + source.slice(position + 16)
        );

        await waitFor(() => expect(screen.getByLabelText("Save")).toBeEnabled());

        fireEvent.click(screen.getByLabelText("Cancel"));

        // confirm discard
        await waitFor(() => expect(screen.getByLabelText("Yes")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("Yes"));

        await waitFor(() => expect(monacoInstance.getValue().indexOf("\"allowAdd\": true")).toBe(position));

    });

    test("Edit object then cancel the changes, but don't confirm discarding changes", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Edit Object"));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 

        await waitForSpinner();
        
        await waitFor(() => expect(screen.getByLabelText("Allow Add")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Description")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByLabelText("Object Id Property Name")).toBeInTheDocument());

        fireEvent.click(screen.getByLabelText("Allow Add"));
        fireEvent.change(screen.getByLabelText("Description"), { target: { value: "A new description" } });
        fireEvent.change(screen.getByLabelText("Object Id Property Name"), { target: { value: "New Object Id Property Name" } });
        
        await waitFor(() => expect(screen.getByLabelText("Cancel")).toBeEnabled());


        fireEvent.click(screen.getByLabelText("Cancel"));

        // confirm discard
        await waitFor(() => expect(screen.getByLabelText("No")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("No"));
        
        await waitFor(() => expect(screen.getByLabelText("Description")).toHaveValue("A new description"));
        await waitFor(() => expect(screen.getByLabelText("Object Id Property Name")).toHaveValue("New Object Id Property Name"));


    });

    test("Edit object then add a new property and save", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 
        
        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Edit Object"));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 

        await waitForSpinner();
        
        await waitFor(() => expect(screen.getByLabelText("Add New Property")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("Add New Property"));

        const originURI = await screen.findByRole("menuitem", { name: "Origin URI" });
        fireEvent.click(originURI);

        await waitFor(() => expect(screen.getByLabelText("Ok")).toBeInTheDocument());
        fireEvent.change(screen.getByLabelText("Origin URI"), { target: { value: "http://some.origin.org" } });
        
        fireEvent.click(screen.getByLabelText("Ok"));


        await waitFor(() => expect(screen.getByLabelText("Save")).toBeEnabled());
        fireEvent.click(screen.getByLabelText("Save"));

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("reconcile_object"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("originURI", "http://some.origin.org")); 

        await waitForSpinner();


    });

    test("Edit object then remove a new property and save", async () => {

        const history = createMemoryHistory();
        history.push("/Objects/files/_AdaptiveObjectType_/_AdaptiveObjectType_");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_"));  

        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Edit Object")).toBeInTheDocument());

        mswPostCallback.mockClear();

        fireEvent.click(screen.getByLabelText("Edit Object"));
        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("get_object_with_uri"));
        await waitFor(() => expect(mswPostCallback).toHaveBeenCalledWithObjectContainingDeep("uri", "/files/_AdaptiveObjectType_/_AdaptiveObjectType_")); 

        await waitForSpinner();
        
        await waitFor(() => expect(screen.getByLabelText("Select Property description")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("Select Property description"));

        await waitFor(() => expect(screen.getByLabelText("Remove Property")).toBeInTheDocument());
        fireEvent.click(screen.getByLabelText("Remove Property"));


        await waitFor(() => expect(screen.getByLabelText("Save")).toBeEnabled());
        fireEvent.click(screen.getByLabelText("Save"));

        mswPostCallback.mockClear();

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("reconcile_object"));  
        await waitFor(() => expect(mswPostCallback).not.toHaveBeenCalledWithObjectContainingDeep("description", objectTypeObject.result.description)); 

        await waitForSpinner();


    });    
});
