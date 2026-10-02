// See the 'COPYING' file in the project root for licensing information.
import {waitForSpinner, mswPostCallback, mswGetCallback, screen, server, http, HttpResponse} from "../../test-utils";

import {renderRoute, waitFor} from "../../test-utils";
import userEvent from "@testing-library/user-event";

describe("Home Tests", () => {

    beforeEach(() => {
        mswPostCallback.mockClear();
    });

    test("Home renders", async () => {

        renderRoute("/");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        expect(await screen.findByTestId("admin-home")).toBeInTheDocument();
    });

    test("Home page shows appropriate content", async () => {

        renderRoute("/Home");

        await waitFor(() => expect(mswPostCallback).toHaveBeenCalled());
        await waitForSpinner();

        expect(await screen.findByTestId("admin-home")).toBeInTheDocument();
    });

    test("A section's button navigates to it", async () => {
        const user = userEvent.setup();

        const {router} = renderRoute("/Home");
        await waitForSpinner();

        await user.click(await screen.findByRole("button", { name: "Tools" }));

        await waitFor(() => expect(router.state.location.pathname).toBe("/Tools"));
    });

    test("Home page error when unable to fetch data", async () => {

        server.use(
            http.post("/afw", ({request}) => {
                mswPostCallback("/afw", {method: request.method, url: request.url, headers: request.headers});
                return HttpResponse.text("Bad Gateway", {status: 502, statusText: "Bad Gateway"});
            }),
            http.get("/*", ({request}) => {
                mswGetCallback("/afw", {method: request.method, url: request.url, headers: request.headers});
                return HttpResponse.text("Bad Gateway", {status: 502, statusText: "Bad Gateway"});
            })
        );

        renderRoute("/Home");

        await waitForSpinner();

        await waitFor(() => expect(screen.getByText("Error Loading Application Data")).toBeInTheDocument());
    });
});
