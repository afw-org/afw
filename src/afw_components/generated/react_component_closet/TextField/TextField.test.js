import {render} from "@afw/test";


const Test = (wrapper, TextField) => {

    describe("TextField tests", () => {

        test("TextField renders with minimal props", async () => {

            render(
                <TextField id="TextField" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads TextField Test", async () => expect(true).toBe(true));

export default Test;
