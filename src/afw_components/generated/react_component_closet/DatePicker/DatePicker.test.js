import {render} from "@afw/test";


const Test = (wrapper, DatePicker) => {

    describe("DatePicker tests", () => {

        test("DatePicker renders with minimal props", async () => {

            render(
                <DatePicker id="DatePicker" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads DatePicker Test", async () => expect(true).toBe(true));

export default Test;
