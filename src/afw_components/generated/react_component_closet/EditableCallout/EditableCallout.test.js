import {render} from "@afw/test";


const Test = (wrapper, EditableCallout) => {

    describe("EditableCallout tests", () => {

        test("EditableCallout renders with minimal props", async () => {

            render(
                <EditableCallout id="EditableCallout" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads EditableCallout Test", async () => expect(true).toBe(true));

export default Test;
