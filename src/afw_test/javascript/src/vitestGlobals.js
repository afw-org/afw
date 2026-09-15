// See the 'COPYING' file in the project root for licensing information.
/**
 * Must run before setupTests.js (and anything it imports, like the MSW
 * handlers) - those still call the Jest-style `jest.fn()`/`jest.spyOn()`/etc.
 * Vitest's `vi` is API-compatible for that common subset, so alias it rather
 * than rewrite every call site. List this file first in each package's
 * vitest.config.js `setupFiles`.
 */
import {vi} from "vitest";

globalThis.jest = vi;
