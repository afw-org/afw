// See the 'COPYING' file in the project root for licensing information.
import {defineConfig} from "vite";
import react from "@vitejs/plugin-react-swc";
import path from "path";
import {transform as esbuildTransform} from "esbuild";
import monacoEditorPlugin from "vite-plugin-monaco-editor";
import {visualizer} from "rollup-plugin-visualizer";

// Sibling workspace packages (@afw/react, @afw/client, etc.) are consumed
// as source, not a prebuilt dist - the dev server needs permission to read
// outside this package's own directory.
const monorepoRoot = path.resolve(__dirname, "../../..");

// `afwdev build` sets PUBLIC_URL to the app's real install path
// (/apps/<afwPackageId>/admin) before invoking this build - see
// src/afw_dev/_afwdev/build/js.py. Falls back to a relative base for
// standalone builds (build_app.sh, local `npm run build`/`npm start`).
const publicUrl = process.env.PUBLIC_URL;
const base = publicUrl ? (publicUrl.endsWith("/") ? publicUrl : publicUrl + "/") : "./";

// These packages are workspace source (plain .js files containing JSX), not
// prebuilt dependencies - excluding them from esbuild's dependency-scan/
// pre-bundle step avoids "JSX syntax extension is not enabled" errors there
// (that scanner doesn't go through @vitejs/plugin-react-swc's transform).
// They still get transformed normally as part of the regular module graph.
const workspaceSourcePackages = [
    "@afw/client",
    "@afw/react",
    "@afw/react-material-ui",
    "@afw/react-monaco"
];

// The codebase uses JSX in plain .js files (a CRA/babel convention, instead
// of .jsx). @vitejs/plugin-react-swc's `parserConfig` option (used below)
// only takes effect in dev - in production builds (without custom swc
// plugins configured) it delegates entirely to Vite's own esbuild transform,
// which infers a loader from the file extension and never enables JSX for
// plain .js. Vite's esbuild config only accepts a single loader for every
// matched file, and this codebase's .ts files (e.g. @afw/client) use
// old-style `<Type>value` angle-bracket casts that are ambiguous with JSX -
// so a single blanket loader can't correctly cover both. This plugin runs
// only for the production build and strips JSX from .js files specifically,
// before Vite's own esbuild transform (which handles every other extension
// exactly as it already did) ever sees them.
function jsxInJsForBuild() {
    return {
        name: "jsx-in-js-build",
        apply: "build",
        enforce: "pre",
        async transform(code, id) {
            if (!id.endsWith(".js")) return;
            const result = await esbuildTransform(code, {
                loader: "jsx",
                jsx: "automatic",
                jsxImportSource: "react",
                sourcefile: id,
                sourcemap: true
            });
            return {code: result.code, map: result.map};
        }
    };
}

export default defineConfig({
    base,
    // afwdev's build orchestration (src/afw_dev/_afwdev/build/js.py) and the
    // "analyze"/"analyze:stats" scripts expect the app's output at build/,
    // not Vite's dist/ default.
    build: {
        outDir: "build"
    },
    plugins: [
        jsxInJsForBuild(),
        // Handles Monaco's web worker bundling (language services, e.g. the
        // CodeEditor's JSON schema validation) - our custom languages are
        // Monarch-only, so only the base editor worker and JSON are needed.
        monacoEditorPlugin({languageWorkers: ["editorWorkerService", "json"]}),
        // The codebase uses JSX in plain .js files (a CRA/babel convention)
        // instead of .jsx - the plugin only parses .jsx/.tsx/.ts/.mts/.mdx as
        // JSX-aware by default, so .js/.mjs need to be added explicitly.
        // (Dev only - see jsxInJsForBuild() above for the build-mode path.)
        react({
            parserConfig(id) {
                if (id.endsWith(".tsx"))
                    return {syntax: "typescript", tsx: true};
                if (id.endsWith(".ts") || id.endsWith(".mts"))
                    return {syntax: "typescript", tsx: false};
                if (id.endsWith(".jsx") || id.endsWith(".js") || id.endsWith(".mjs"))
                    return {syntax: "ecmascript", jsx: true};
            }
        }),
        // "npm run analyze" (ANALYZE=true vite build) - Rollup-native
        // replacement for the old webpack-bundle-analyzer/source-map-explorer
        // pair, which expected webpack's output shape (build/static/js/,
        // bundle-stats.json) and no longer applies under Vite/Rollup output.
        process.env.ANALYZE && visualizer({
            filename: "build/stats.html",
            open: true,
            gzipSize: true,
            brotliSize: true
        })
    ].filter(Boolean),
    optimizeDeps: {
        exclude: workspaceSourcePackages,
        esbuildOptions: {
            loader: {".js": "jsx"}
        }
    },
    // Workspace packages (excluded from optimizeDeps above) and the app
    // itself must resolve to the exact same react/react-dom instance, or
    // hooks break ("Invalid hook call") across the package boundary.
    resolve: {
        dedupe: ["react", "react-dom", "@emotion/react", "@emotion/styled"]
    },
    server: {
        port: 3000,
        proxy: {
            "/afw": "http://localhost:8080"
        },
        fs: {
            allow: [monorepoRoot]
        }
    }
});
