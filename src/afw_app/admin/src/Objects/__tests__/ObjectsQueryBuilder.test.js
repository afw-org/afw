// See the 'COPYING' file in the project root for licensing information.
import {createMemoryHistory} from "history";
import {Router} from "react-router-dom";

import {render, waitFor, within, fireEvent, screen, waitForSpinner, mswPostCallback} from "../../test-utils";

import Objects from "../Objects";


describe("ObjectsQueryBuilder Tests", () => {    

    test("Build Query Criteria, renders", async () => {

        const history = createMemoryHistory();
        history.push("/Objects");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitForSpinner();

        await waitFor(() => expect(screen.getByLabelText("Object Type")).not.toBeDisabled());
        await waitFor(() => expect(screen.getByLabelText("Build Query Criteria")).toBeInTheDocument()); 

        fireEvent.click(screen.getByLabelText("Build Query Criteria"));

        await waitFor(() => expect(screen.getByLabelText("Apply")).toBeInTheDocument()); 
        await waitFor(() => expect(screen.getByLabelText("Cancel")).toBeInTheDocument()); 

        await waitFor(() => expect(screen.getByLabelText("Adaptor")).toHaveTextContent("afw"));


    });

    test("Build Query Criteria, build simple condition", async () => {

        const history = createMemoryHistory();
        history.push("/Objects");

        render(
            <Router history={history}>
                <Objects /> 
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());    
        await waitForSpinner();
        
        await waitFor(() => expect(screen.getByLabelText("Build Query Criteria")).toBeInTheDocument());         

        await waitFor(() => expect(screen.getByLabelText("Adaptor")).toHaveTextContent("afw"));
        await waitFor(() => expect(screen.getByLabelText("Object Type")).not.toBeDisabled());

        const textbox = screen.getByRole("combobox", { name: "Object Type" });
        fireEvent.mouseDown(textbox);
        fireEvent.click(textbox);

        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        const typeOpt = screen.getByRole("option", { name: "_AdaptiveAdaptor_" });        
        // select '_AdaptiveAdaptor_'
        typeOpt.click();
        // wait options to disappear
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0)); 

        expect(await screen.findByLabelText("Object Type")).toHaveValue("_AdaptiveAdaptor_");
        fireEvent.click(screen.getByLabelText("Build Query Criteria"));

        await waitFor(() => expect(screen.getByLabelText("Apply")).toBeInTheDocument()); 
        await waitFor(() => expect(screen.getByLabelText("Cancel")).toBeInTheDocument()); 
        await waitFor(() => expect(screen.getByLabelText("Condition")).toBeInTheDocument()); 

        fireEvent.click(screen.getByLabelText("Condition"));
        await waitFor(() => expect(within(screen.getByLabelText("Property")).getByRole("combobox")).toBeInTheDocument());
        await waitFor(() => expect(screen.getByRole("button", { name: /equals/i })).toBeInTheDocument());
        await waitFor(() => expect(screen.getByRole("textbox", { name: "Value" })).toBeInTheDocument());

        // select a property        
        const propertyTextbox = within(screen.getByLabelText("Property")).getByRole("combobox");
        fireEvent.mouseDown(propertyTextbox);
        fireEvent.click(textbox);

        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        const propertyOpt = screen.getByRole("option", { name: "Service Id" });        
        // select 'Service Id'
        propertyOpt.click();
        // wait options to disappear
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0)); 

        // select a comparison operator
        fireEvent.mouseDown(screen.getByRole("button", { name: /equals/i }));
        await waitFor(() => expect(screen.queryAllByRole("option")).not.toHaveLength(0));
        const operatorOpts = screen.getAllByRole("option");
        operatorOpts[0].click();        
        // wait options to disappear
        await waitFor(() => expect(screen.queryAllByRole("option")).toHaveLength(0)); 

        const value = screen.getByRole("textbox", { name: "Value" });
        fireEvent.change(value, { target: { value: "abc" } } );

        expect(await screen.findByText("serviceId=abc")).toBeInTheDocument();        

        // apply our changes
        fireEvent.click(screen.getByLabelText("Apply"));

        await waitFor(() => expect(screen.queryByLabelText("Apply")).not.toBeInTheDocument()); 
        await waitFor(() => expect(screen.getByPlaceholderText("property=value")).toHaveValue("serviceId=abc"));


    });
    
});
