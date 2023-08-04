// See the 'COPYING' file in the project root for licensing information.
import {render, AllTheProviders} from "../test-utils";
import {Admin} from "./Admin";
   
const AdminProvider = ({ children }) => 
    <AllTheProviders>
        <Admin>
            { children }
        </Admin>
    </AllTheProviders>;    

const customRender = (ui, options) =>
    render(ui, { wrapper: AdminProvider, ...options });

// re-export everything
export * from "../test-utils";
export { customRender as render, AdminProvider };
