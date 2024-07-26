// See the 'COPYING' file in the project root for licensing information.
import {createContext} from "react";

/* create a global "App" context for accessing properties provided by the Application */
export const AppContext = createContext();

/* create a "Core" context for accessing common core objects */
export const AppCoreContext = createContext();

/* create a "Configuration" context for fetching objects out of the Config adapter */
export const ConfigContext = createContext();

/* create a "Notifications" context for accessing props related to notifications */
export const NotificationContext = createContext();
