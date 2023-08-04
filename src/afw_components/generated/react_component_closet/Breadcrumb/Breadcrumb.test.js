import {render} from "@afw/test";


const Test = (wrapper, Breadcrumb) => {

    describe("Breadcrumb tests", () => {

        test("Breadcrumb renders with minimal props", async () => {

            render(
                <Breadcrumb id="Breadcrumb" />, 
                { wrapper }
            );

        });
    });
};

/* When called directly via core testing, Jest expects at least on test per file, so just make a true assertion to pass */
test("Loads Breadcrumb Test", async () => expect(true).toBe(true));

export default Test;
