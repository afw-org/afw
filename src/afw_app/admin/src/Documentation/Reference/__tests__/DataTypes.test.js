// See the 'COPYING' file in the project root for licensing information.
import {render, waitFor, mswPostCallback, waitForSpinner} from "../../../test-utils";
import DataTypes from "../DataTypes";

describe("DataTypes Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("DataTypes renders", async () => {

        window.scrollTo = jest.fn();

        render( <DataTypes /> );        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                   
        await waitForSpinner();     

    });
});
