// See the 'COPYING' file in the project root for licensing information.
import {ObjectTypeDropdown, Providers} from "@afw/test/build/cjs/coreTests";
import componentRegistry from "../..";

describe("Core Layout Tests for [ObjectTypeDropdown]", () => ObjectTypeDropdown(Providers(componentRegistry)));
