// See the 'COPYING' file in the project root for licensing information.
import {render, screen} from "../test-utils";
import {MemoryRouter} from "react-router-dom";
import Authorization from "./Authorization";

describe("Authorization Tests", () => {    

    test("Authorization renders", async () => {

        render(
            <MemoryRouter initialEntries={[ "/Admin/Authorization" ]}>
                <Authorization />
            </MemoryRouter>
        );

        expect(await screen.findByText("Admin")).toBeInTheDocument();
        expect(await screen.findByLabelText("Help")).toBeInTheDocument();
    });
});
