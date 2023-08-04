// See the 'COPYING' file in the project root for licensing information.
import {HybridBuilder, Providers} from "@afw/test/build/cjs/coreTests";
import componentRegistry from "../..";

describe("Core Layout Tests for [HybridBuilder]", () => HybridBuilder(Providers(componentRegistry)));
