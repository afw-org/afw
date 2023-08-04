import {render} from "@afw/test";


const Test = (wrapper, DateTimePicker) => {

    describe("DateTimePicker tests", () => {

        test("DateTimePicker renders with minimal props", async () => {

            render(
                <DateTimePicker id="DateTimePicker" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads DateTimePicker Test", async () => expect(true).toBe(true));

export default Test;
