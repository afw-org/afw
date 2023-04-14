import {render} from "@afw/test";


const Test = (wrapper, Icon) => {

    describe("Icon tests", () => {

        test("Icon renders with minimal props", async () => {

            render(
                <Icon id="Icon" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Icon Test", async () => expect(true).toBe(true));

export default Test;
