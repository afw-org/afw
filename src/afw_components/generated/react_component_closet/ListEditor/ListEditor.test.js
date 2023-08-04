import {render} from "@afw/test";


const Test = (wrapper, ListEditor) => {

    describe("ListEditor tests", () => {

        test("ListEditor renders with minimal props", async () => {

            render(
                <ListEditor id="ListEditor" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads ListEditor Test", async () => expect(true).toBe(true));

export default Test;
