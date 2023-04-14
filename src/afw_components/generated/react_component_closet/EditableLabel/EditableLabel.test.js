import {render} from "@afw/test";


const Test = (wrapper, EditableLabel) => {

    describe("EditableLabel tests", () => {

        test("EditableLabel renders with minimal props", async () => {

            render(
                <EditableLabel id="EditableLabel" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads EditableLabel Test", async () => expect(true).toBe(true));

export default Test;
