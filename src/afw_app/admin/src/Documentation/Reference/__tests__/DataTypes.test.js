// See the 'COPYING' file in the project root for licensing information.
import {renderRoute, waitFor, mswPostCallback, waitForSpinner} from "../../../test-utils";

describe("DataTypes Tests", () => {    

    beforeEach(() => {
        mswPostCallback.mockClear();
    });    

    test("DataTypes renders", async () => {

        window.scrollTo = jest.fn();

        renderRoute("/Documentation/Reference/DataTypes");        

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());                   
        await waitForSpinner();     

    });
});
