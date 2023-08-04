// See the 'COPYING' file in the project root for licensing information.
import {setupServer} from "msw/node";
import {handlers} from "./handlers";

export const server = setupServer(...handlers);
