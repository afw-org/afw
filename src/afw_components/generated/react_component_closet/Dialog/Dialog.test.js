import {render} from "@afw/test";


const Test = (wrapper, Dialog) => {

    describe("Dialog tests", () => {

        test("Dialog renders with minimal props", async () => {

            render(
                <Dialog id="Dialog" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Dialog Test", async () => expect(true).toBe(true));

export default Test;
