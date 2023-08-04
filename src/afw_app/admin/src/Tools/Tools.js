// See the 'COPYING' file in the project root for licensing information.
import {useState} from "react";
import {Route, useHistory} from "react-router";
import Container from "../common/Container";

import {
    Breadcrumb,
    Button,
    Divider,
    Icon,
    Responsive,
    Typography
} from "@afw/react";

import {useBreadcrumbs, useApplication, useTheme} from "../hooks";

import {Fiddle} from "./Fiddle";
import {Layouts} from "./Layouts";
import {Requests} from "./Requests";

import {ContextualHelpRoutes} from "./ContextualHelp";
import {ContextualHelp, ContextualHelpButton} from "../common/ContextualHelp";

const root = { text: "Tools", link: "/Tools" };

export const Tools = () => {
    const [showHelp, setShowHelp] = useState(false);    

    const theme = useTheme();
    const history = useHistory();
    const breadcrumbItems = useBreadcrumbs(root);
    const {marginHeight} = useApplication();

    return (
        <Container maxWidth="xl" style={{ height: "100%", overflow: "auto" }}>    
            <div style={{ display: "flex", flexDirection: "column", height: "calc(100vh - " + marginHeight + ")" }}>
                <div style={{ display: "flex", alignItems: "center", paddingBottom: theme.spacing(2) }}>
                    <div style={{ flex: 1 }}>
                        <Breadcrumb items={breadcrumbItems} />  
                    </div>                 
                    <ContextualHelpButton showHelp={setShowHelp} />                
                </div>
                <div style={{ flex: 1, overflow: "auto" }}>
                    <Route exact path="/Tools" render={() => {
                        return (
                            <div style={{ padding: theme.spacing(1) }}>
                                <Typography size="10" text="Tools" />
                                <div style={{ height: theme.spacing(5) }} />
                                <Typography text="Use the following tools to help you develop or explore Adaptive Framework." />
                                <div style={{ height: theme.spacing(5) }} />
                                <Divider />
                                <div style={{ height: theme.spacing(5) }} />
                                <Responsive
                                    rows={[
                                        {
                                            spacing: 1,
                                            justify: "center",
                                            columns: [
                                                {
                                                    breakpoints: {
                                                        small: 12,
                                                        large: 6,
                                                    },
                                                    contains:
                                                            <div style={{ padding: theme.spacing(2), backgroundColor: theme.palette.background.paper }}>
                                                                <div style={{ display: "flex" }}>
                                                                    <Icon style={{ marginRight: theme.spacing(1) }} color="primary" iconName="web" />
                                                                    <Typography size="7" text="Layouts" />
                                                                </div>
                                                                <div style={{ height: "250px", paddingTop: theme.spacing(2) }}>
                                                                    <Typography
                                                                        text="Layouts allow you to design custom UI components to arrange
                                                                                objects and their properties in ways that make sense for your
                                                                                data."
                                                                    />
                                                                </div>
                                                                <Divider />
                                                                <Button 
                                                                    color="primary"
                                                                    variant="text" 
                                                                    label="Create Layouts" 
                                                                    onClick={() => history.push("/Tools/Layouts")} 
                                                                    endIcon="chevron_right"
                                                                />
                                                            </div>
                                                },
                                                {
                                                    breakpoints: {
                                                        small: 12,
                                                        large: 6,
                                                    },
                                                    contains:
                                                            <div style={{ padding: theme.spacing(2), backgroundColor: theme.palette.background.paper }}>
                                                                <div style={{ display: "flex" }}>
                                                                    <Icon style={{ marginRight: theme.spacing(1) }} color="primary" iconName="code" />
                                                                    <Typography size="7" text="Fiddle" />
                                                                </div>
                                                                <div style={{ minHeight: "250px", paddingTop: theme.spacing(2) }}>
                                                                    <Typography
                                                                        text="Fiddle is an advanced tool that allow developers to execute or simulate
                                                                                functions or expressions, in order to design or troubleshoot policy.  You
                                                                                can enter raw source or build up expressions and evaluate them under user-
                                                                                defined contexts for testing."
                                                                    />
                                                                </div>
                                                                <Divider />
                                                                <Button 
                                                                    color="primary"
                                                                    endIcon="chevron_right"
                                                                    variant="text" 
                                                                    label="Try Fiddle" 
                                                                    onClick={() => history.push("/Tools/Fiddle")} 
                                                                />
                                                            </div>
                                                },
                                            ]
                                        }
                                    ]}
                                />
                            </div>
                        );
                    }} />
                    <Route path="/Tools/Fiddle" render={(props) => <Fiddle {...props} />} />
                    <Route path="/Tools/Layouts" render={(props) => <Layouts {...props} /> }/>
                    <Route path="/Tools/Requests" render={(props) => <Requests {...props} /> }/>
                </div>
                <ContextualHelp 
                    open={showHelp}
                    onClose={() => setShowHelp(false)}
                    routes={ContextualHelpRoutes}
                /> 
            </div>
        </Container>
    );
};

export default Tools;
