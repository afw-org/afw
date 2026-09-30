// See the 'COPYING' file in the project root for licensing information.
// @vitest-environment node

/*
 * @afw/react-monaco assembles its monaco namespace from monaco-editor's
 * tree-shakeable entry points (monaco-editor/editor, .../features/
 * register.all, the JSON language service, ...) - see its src/monaco.js.
 * Those only load in a real browser: Vitest's jsdom config aliases every
 * monaco-editor import to a stub (see vitest.config.js), so nothing else in
 * this suite exercises them.
 *
 * A monaco-editor release that moves an entry point or changes its
 * package.json "exports" map (as 0.56 reorganized them) would break them
 * silently in every jsdom/unit test and surface only when a real browser
 * opens an editor - including the editor worker's `?worker` import. This
 * resolves each specifier, as written in that source (less any `?query`),
 * against the real installed package with Node's resolver (which the alias
 * doesn't touch).
 */
import {readFileSync} from "node:fs";
import {createRequire} from "node:module";
import path from "node:path";

const monacoPath = path.resolve(import.meta.dirname,
    "../../../../afw_components/react/monaco/src/monaco.js");

/* resolve from the importing file, as Vite does - monaco-editor is installed
   under @afw/react-monaco, not hoisted to the workspace root */
const require = createRequire(monacoPath);

const specifiers = [...readFileSync(monacoPath, "utf8")
    .matchAll(/(?:import|from) "(monaco-editor\/[^"?]+)(?:\?[^"]*)?"/g)]
    .map(([, specifier]) => specifier);

describe("Monaco entry point resolution (@afw/react-monaco)", () => {

    test("finds the editor, features, definitions, worker and JSON imports", () => {
        expect(specifiers).toHaveLength(5);
    });

    test.each(specifiers)(
        "resolves %s to a real file on disk",
        (specifier) => {
            const resolved = require.resolve(specifier);
            expect(resolved).toMatch(/\.js$/);
            expect(resolved).not.toMatch(/esm[/\\]vs[/\\].*esm[/\\]vs/);
        }
    );

});
