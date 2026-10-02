// See the 'COPYING' file in the project root for licensing information.
import {vi} from "vitest";
import {createMemoryHistory} from "@tanstack/react-router";
import {createLegacyHistory} from "../legacyHistory";

/* blocked push/replace go through an async blocker check */
const settle = () => new Promise(resolve => setTimeout(resolve, 0));

const setup = (entry = "/apps/afw/admin/Admin/Models", basepath = "/apps/afw/admin") => {
    const history = createMemoryHistory({ initialEntries: [entry] });
    return { history, legacy: createLegacyHistory(history, basepath) };
};

describe("createLegacyHistory (React Router 5 over TanStack history)", () => {

    test("locations drop the basepath", () => {
        const {legacy} = setup("/apps/afw/admin/Admin/Models?x=1#tree");

        expect(legacy.location).toMatchObject({
            pathname: "/Admin/Models",
            search: "?x=1",
            hash: "#tree",
        });
    });

    test("the basepath itself is the root", () => {
        const {legacy} = setup("/apps/afw/admin");
        expect(legacy.location.pathname).toBe("/");
    });

    test("push and replace add the basepath back", () => {
        const {history, legacy} = setup();

        legacy.push("/Objects");
        expect(history.location.pathname).toBe("/apps/afw/admin/Objects");
        expect(legacy.location.pathname).toBe("/Objects");

        legacy.replace("/Tools");
        expect(history.location.pathname).toBe("/apps/afw/admin/Tools");
        expect(history.length).toBe(2);
    });

    test("push takes a location object", () => {
        const {legacy} = setup();

        legacy.push({ pathname: "/Objects", search: "a=1", hash: "tree" });
        expect(legacy.location).toMatchObject({ pathname: "/Objects", search: "?a=1", hash: "#tree" });
    });

    test("relative paths resolve against the current location", () => {
        const {legacy} = setup("/apps/afw/admin/Admin/Models?x=1");

        legacy.push("#tree");
        expect(legacy.location).toMatchObject({ pathname: "/Admin/Models", search: "?x=1", hash: "#tree" });

        legacy.push("?y=2");
        expect(legacy.location).toMatchObject({ pathname: "/Admin/Models", search: "?y=2" });

        legacy.push("Services");
        expect(legacy.location.pathname).toBe("/Admin/Services");
    });

    test("with a root basepath, paths pass through", () => {
        const {history, legacy} = setup("/Admin", "/");

        legacy.push("/Objects");
        expect(history.location.pathname).toBe("/Objects");
        expect(legacy.location.pathname).toBe("/Objects");
    });

    test("createHref adds the basepath", () => {
        const {legacy} = setup();
        expect(legacy.createHref({ pathname: "/Objects", hash: "#tree" })).toBe("/apps/afw/admin/Objects#tree");
    });

    test("listen reports React Router 5 actions", () => {
        const {legacy} = setup();
        const listener = vi.fn();
        const unlisten = legacy.listen(listener);

        legacy.push("/Objects");
        expect(listener).toHaveBeenLastCalledWith(expect.objectContaining({ pathname: "/Objects" }), "PUSH");
        expect(legacy.action).toBe("PUSH");

        legacy.replace("/Tools");
        expect(listener).toHaveBeenLastCalledWith(expect.objectContaining({ pathname: "/Tools" }), "REPLACE");

        legacy.goBack();
        expect(listener).toHaveBeenLastCalledWith(expect.objectContaining({ pathname: "/Admin/Models" }), "POP");
        expect(legacy.action).toBe("POP");

        unlisten();
        legacy.push("/Home");
        expect(listener).toHaveBeenCalledTimes(3);
    });

    test("a string prompt asks window.confirm", async () => {
        const {legacy} = setup();
        const confirm = vi.spyOn(window, "confirm");
        legacy.block("Discard your changes?");

        confirm.mockReturnValueOnce(false);
        legacy.push("/Objects");
        await settle();
        expect(confirm).toHaveBeenLastCalledWith("Discard your changes?");
        expect(legacy.location.pathname).toBe("/Admin/Models");

        confirm.mockReturnValueOnce(true);
        legacy.push("/Objects");
        await settle();
        expect(legacy.location.pathname).toBe("/Objects");

        confirm.mockRestore();
    });

    test("a function prompt sees the next location and action", async () => {
        const {legacy} = setup();
        const prompt = vi.fn(() => true);
        legacy.block(prompt);

        legacy.push("/Objects");
        await settle();
        expect(prompt).toHaveBeenCalledWith(expect.objectContaining({ pathname: "/Objects" }), "PUSH");
        expect(legacy.location.pathname).toBe("/Objects");
    });

    test("prompt false blocks; unblock lifts it", async () => {
        const {legacy} = setup();
        const unblock = legacy.block(() => false);

        legacy.push("/Objects");
        await settle();
        expect(legacy.location.pathname).toBe("/Admin/Models");

        unblock();
        legacy.push("/Objects");
        await settle();
        expect(legacy.location.pathname).toBe("/Objects");
    });

});
