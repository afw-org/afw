// See the 'COPYING' file in the project root for licensing information.
import {ObjectResponsive, Providers} from "@afw/test/build/cjs/coreTests";
import componentRegistry from "../..";

describe("Core Layout Tests for [ObjectResponsive] Basic", () => ObjectResponsive.basic(Providers(componentRegistry)));
describe("Core Layout Tests for [ObjectResponsive] Add Properties", () => ObjectResponsive.addProperties(Providers(componentRegistry)));
describe("Core Layout Tests for [ObjectResponsive] Edit Properties", () => ObjectResponsive.editProperties(Providers(componentRegistry)));
describe("Core Layout Tests for [ObjectResponsive] Remove Properties", () => ObjectResponsive.removeProperties(Providers(componentRegistry)));
