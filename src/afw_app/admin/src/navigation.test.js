// See the 'COPYING' file in the project root for licensing information.
import {createMemoryHistory, createRootRoute, createRoute, createRouter, RouterProvider} from "@tanstack/react-router";
import {act, render, screen, waitFor} from "@testing-library/react";
import userEvent from "@testing-library/user-event";
import {vi} from "vitest";

import {appNavigation} from "./navigation";
import {parseSearch, stringifySearch} from "./router/router";

/*
 * The admin app's adapter for @afw/react's navigation contract (see
 * navigation.js), on a router of its own: every path renders `ui`.
 */

const {Link, useNavigate, NavigationBlocker} = appNavigation;

const renderAt = (path, ui, options = {}) => {
    const rootRoute = createRootRoute({ component: () => ui });
    const router = createRouter({
        routeTree: rootRoute.addChildren([
            createRoute({ getParentRoute: () => rootRoute, path: "$" }),
        ]),
        history: createMemoryHistory({ initialEntries: [path] }),
        parseSearch,
        stringifySearch,
        ...options,
    });

    render(<RouterProvider router={router} />);

    return router;
};

/* start a navigation without awaiting it: a blocked one never resolves */
const navigate = (router, href) => act(() => { router.navigate({ href }); });

/* an object id with a space and a slash, RQL search and a view hash */
const objectHref = "/Objects/files/a%20b%2Fc?eq(x,1)&sort(+y)#tree";

describe("appNavigation", () => {

    test("Link keeps an encoded href, its search and hash", async () => {
        const user = userEvent.setup();
        const router = renderAt("/Start", <Link to={objectHref}>object</Link>);

        const link = await screen.findByRole("link", { name: "object" });
        expect(link).toHaveAttribute("href", objectHref);

        await user.click(link);

        await waitFor(() => expect(router.state.location.href).toBe(objectHref));
        expect(router.state.location.searchStr).toBe("?eq(x,1)&sort(+y)");
        expect(router.state.location.hash).toBe("tree");
    });

    test("Link adds the basepath to its href", async () => {
        renderAt("/apps/afw/admin/Start", <Link to="/Admin">admin</Link>, { basepath: "/apps/afw/admin" });

        expect(await screen.findByRole("link", { name: "admin" })).toHaveAttribute("href", "/apps/afw/admin/Admin");
    });

    test("Only the current page's link is aria-current, with no active class", async () => {
        renderAt("/Admin/Services",
            <>
                <Link to="/Admin">admin</Link>
                <Link to="/Admin/Services">services</Link>
            </>
        );

        const admin = await screen.findByRole("link", { name: "admin" });
        const services = screen.getByRole("link", { name: "services" });

        expect(admin).not.toHaveAttribute("aria-current");
        expect(services).toHaveAttribute("aria-current", "page");
        expect(admin).not.toHaveClass("active");
        expect(services).not.toHaveClass("active");
    });

    test("useNavigate goes to an href", async () => {
        const user = userEvent.setup();
        const Go = () => {
            const navigateTo = useNavigate();
            return <button onClick={() => navigateTo(objectHref)}>go</button>;
        };
        const router = renderAt("/Start", <Go />);

        await user.click(await screen.findByRole("button", { name: "go" }));

        await waitFor(() => expect(router.state.location.href).toBe(objectHref));
    });

    describe("NavigationBlocker", () => {

        let confirm;

        beforeEach(() => {
            confirm = vi.spyOn(window, "confirm");
        });

        afterEach(() => {
            confirm.mockRestore();
        });

        test("While `when`, leaving asks first; Cancel stays, OK goes", async () => {
            const router = renderAt("/Start",
                <>
                    <span>page</span>
                    <NavigationBlocker when={true} message={() => "Leave?"} />
                </>
            );
            await screen.findByText("page");

            confirm.mockReturnValueOnce(false);
            navigate(router, "/Elsewhere");

            await waitFor(() => expect(confirm).toHaveBeenCalledWith("Leave?"));
            expect(router.state.location.pathname).toBe("/Start");

            confirm.mockReturnValueOnce(true);
            navigate(router, "/Elsewhere");

            await waitFor(() => expect(router.state.location.pathname).toBe("/Elsewhere"));
            expect(confirm).toHaveBeenCalledTimes(2);
        });

        test("Without `when`, leaving does not ask", async () => {
            const router = renderAt("/Start",
                <>
                    <span>page</span>
                    <NavigationBlocker when={false} message="Leave?" />
                </>
            );
            await screen.findByText("page");

            navigate(router, "/Elsewhere");

            await waitFor(() => expect(router.state.location.pathname).toBe("/Elsewhere"));
            expect(confirm).not.toHaveBeenCalled();
        });

    });

});
