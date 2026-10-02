// See the 'COPYING' file in the project root for licensing information.
import {matchPath} from "../matchPath";

/* React Router 5's matchPath semantics, as the model editor uses them */
describe("matchPath", () => {

    test("exact patterns match the whole path, with params", () => {
        expect(matchPath("/Admin/Models/afw/m1/objectTypes/OT", {
            path: "/Admin/Models/:adapterId/:modelId/objectTypes/:objectType", exact: true,
        })).toEqual({
            path: "/Admin/Models/:adapterId/:modelId/objectTypes/:objectType",
            url: "/Admin/Models/afw/m1/objectTypes/OT",
            isExact: true,
            params: { adapterId: "afw", modelId: "m1", objectType: "OT" },
        });

        expect(matchPath("/Admin/Models/afw/m1/objectTypes/OT/custom", {
            path: "/Admin/Models/:adapterId/:modelId/objectTypes/:objectType", exact: true,
        })).toBeNull();
    });

    test("without exact, a pattern matches a prefix at a segment boundary", () => {
        const match = matchPath("/Admin/Models/afw/m1/objectTypes/OT/custom", {
            path: "/Admin/Models/:adapterId/:modelId/objectTypes/:objectType",
        });
        expect(match).toMatchObject({ url: "/Admin/Models/afw/m1/objectTypes/OT", isExact: false });
        expect(match.params.objectType).toBe("OT");

        expect(matchPath("/Admin/Models/afw/m1/objectTypesX", { path: "/Admin/Models/:a/:m/objectTypes" })).toBeNull();
    });

    test("an array matches its first matching pattern", () => {
        const match = matchPath("/Admin/Models/afw/m1/custom", {
            path: ["/Admin/Models/:adapterId/:modelId", "/Admin/Models/:adapterId/:modelId/custom"], exact: true,
        });
        expect(match.path).toBe("/Admin/Models/:adapterId/:modelId/custom");
    });

    test("trailing slashes and case don't matter", () => {
        expect(matchPath("/Admin/Models/", { path: "/Admin/Models", exact: true })).not.toBeNull();
        expect(matchPath("/admin/models", { path: "/Admin/Models/", exact: true })).not.toBeNull();
    });

    test("params come through as given (TanStack's pathname is decoded)", () => {
        expect(matchPath("/Admin/Models/afw/my model", { path: "/Admin/Models/:adapterId/:modelId" }).params.modelId)
            .toBe("my model");
    });

});
