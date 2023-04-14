import {render} from "@afw/test";


const Test = (wrapper, ChoiceGroup) => {

    describe("ChoiceGroup tests", () => {

        test("ChoiceGroup renders with minimal props", async () => {

            render(
                <ChoiceGroup id="ChoiceGroup" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads ChoiceGroup Test", async () => expect(true).toBe(true));

export default Test;
