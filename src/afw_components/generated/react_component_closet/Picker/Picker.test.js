import {render} from "@afw/test";


const Test = (wrapper, Picker) => {

    describe("Picker tests", () => {

        test("Picker renders with minimal props", async () => {

            render(
                <Picker id="Picker" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Picker Test", async () => expect(true).toBe(true));

export default Test;
