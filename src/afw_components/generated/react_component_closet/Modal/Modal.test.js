import {render} from "@afw/test";


const Test = (wrapper, Modal) => {

    describe("Modal tests", () => {

        test("Modal renders with minimal props", async () => {

            render(
                <Modal id="Modal" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Modal Test", async () => expect(true).toBe(true));

export default Test;
