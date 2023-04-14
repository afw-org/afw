import {render} from "@afw/test";


const Test = (wrapper, DropdownEditor) => {

    describe("DropdownEditor tests", () => {

        test("DropdownEditor renders with minimal props", async () => {

            render(
                <DropdownEditor id="DropdownEditor" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads DropdownEditor Test", async () => expect(true).toBe(true));

export default Test;
