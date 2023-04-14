// See the 'COPYING' file in the project root for licensing information.
import {
    render, 
    waitFor, 
    waitForElementToBeRemoved,
    fireEvent, 
    userEvent, 
    within, 
    screen, 
    mswPostCallback, 
    waitForSpinner
} from "../../test-utils";
import {Router} from "react-router-dom";
import {createMemoryHistory} from "history";
import Models from "../Models";

describe("ModelsNew Tests", () => {

    beforeEach(() => {
        mswPostCallback.mockClear();
    });

    const newModel = async () => {

        const history = createMemoryHistory();
        history.push("/Admin/Models/models");

        render(
            <Router history={history}>
                <Models />
            </Router>
        );

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        await screen.findByRole("table");        

        const newModelBtn = await screen.findByRole("button", { name: "New Model" });
        fireEvent.click(newModelBtn);

        const dialog = await screen.findByRole("dialog", { name: "New Model" });

        const modelIdTextField = await within(dialog).findByRole("textbox", { name: "Model Id" });
        const descriptionTextField = await within(dialog).findByRole("textbox", { name: "Description" });

        /* check all of the footer buttons */
        const cancelBtn = within(dialog).getByRole("button", { name: "Cancel" });
        const nextBtn = within(dialog).getByRole("button", { name: "Next" });
        const createBtn = within(dialog).getByRole("button", { name: "Create" });
        const prevBtn = within(dialog).getByRole("button", { name: "Previous" });

        expect(cancelBtn).toBeEnabled();
        expect(prevBtn).not.toBeEnabled();
        expect(nextBtn).not.toBeEnabled();
        expect(createBtn).not.toBeEnabled();

        return { dialog, modelIdTextField, descriptionTextField, cancelBtn, prevBtn, nextBtn, createBtn };
    };
   
    test("New Model Step1 Cancel", async () => {

        const {modelIdTextField, dialog} = await newModel();

        userEvent.type(modelIdTextField, "test1");
        expect(within(dialog).getByRole("button", { name: "Next" })).toBeEnabled();

        /* Cancel changes */
        const cancelBtn = within(dialog).getByRole("button", { name: "Cancel" });
        fireEvent.click(cancelBtn);

        await waitForElementToBeRemoved(() => screen.queryByRole("dialog", { name: "New Model"}));
    });

    test("New Model Step1 Create", async () => {

        const {modelIdTextField, descriptionTextField, nextBtn, createBtn} = await newModel();

        userEvent.type(modelIdTextField, "test1");
        userEvent.type(descriptionTextField, "this is a test model");

        expect(nextBtn).toBeEnabled();

        mswPostCallback.mockClear();

        /* Create the model */
        fireEvent.click(createBtn);

        await waitForElementToBeRemoved(() => screen.queryByRole("dialog", { name: "New Model"}));

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("add_object_with_uri"));  
        await waitForSpinner(); 
        
    });

    test("New Model Step2 Cancel", async () => {

        const {modelIdTextField, descriptionTextField, nextBtn} = await newModel();

        userEvent.type(modelIdTextField, "test1");
        userEvent.type(descriptionTextField, "this is a test model");

        expect(nextBtn).toBeEnabled();

        mswPostCallback.mockClear();
        
        /* Click next */
        fireEvent.click(nextBtn);

        await screen.findByLabelText("Select Adaptor to Map");

        const dialog = await screen.findByRole("dialog", { name: "New Model" });
        const cancelBtn = within(dialog).getByRole("button", { name: "Cancel" });

        fireEvent.click(cancelBtn);

        await waitForElementToBeRemoved(() => screen.queryByRole("dialog", { name: "New Model"}));

    });

    test("New Model Step2 Create", async () => {

        const {modelIdTextField, descriptionTextField, nextBtn} = await newModel();

        userEvent.type(modelIdTextField, "test1");
        userEvent.type(descriptionTextField, "this is a test model");

        expect(nextBtn).toBeEnabled();

        mswPostCallback.mockClear();
        
        /* Click next */
        fireEvent.click(nextBtn);

        await screen.findByLabelText("Select Adaptor to Map");

        const dialog = await screen.findByRole("dialog", { name: "New Model" });
        const createBtn = within(dialog).getByRole("button", { name: "Create" });

        fireEvent.click(createBtn);

        await waitForElementToBeRemoved(() => screen.queryByRole("dialog", { name: "New Model"}));

        await waitFor(() => expect(mswPostCallback).toHaveCalledAdaptiveFunction("add_object_with_uri"));  
        await waitForSpinner();

    });

    test.skip("New Model Step2 Map", async () => {
        
    });
});
