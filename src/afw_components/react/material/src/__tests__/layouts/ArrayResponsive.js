// See the 'COPYING' file in the project root for licensing information.
import {ArrayResponsive, Providers} from "@afw/test/build/esm/coreTests";
import componentRegistry from "../..";

describe("Core Layout Tests for [ArrayResponsive]", () => ArrayResponsive(Providers(componentRegistry)));
