import {render} from "@afw/test";


const Test = (wrapper, DiffEditor) => {

    describe("DiffEditor tests", () => {

        test("DiffEditor renders with minimal props", async () => {

            render(
                <DiffEditor id="DiffEditor" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads DiffEditor Test", async () => expect(true).toBe(true));

export default Test;
