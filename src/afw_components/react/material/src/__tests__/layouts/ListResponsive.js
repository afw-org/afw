// See the 'COPYING' file in the project root for licensing information.
import {ListResponsive, Providers} from "@afw/test/build/cjs/coreTests";
import componentRegistry from "../..";

describe("Core Layout Tests for [ListResponsive]", () => ListResponsive(Providers(componentRegistry)));
