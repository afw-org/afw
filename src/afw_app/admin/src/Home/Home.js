// See the 'COPYING' file in the project root for licensing information.
import {useHistory} from "react-router";
import Container from "../common/Container";

import {
    Icon,
    Link,
    Responsive,
    Typography,
    Button,
    Divider
} from "@afw/react";

import {useTheme} from "../hooks";

import logo from "../images/logo.png";

const HomeHeader = () => {
    const theme = useTheme();

    return (
        <div style={{
            display: "flex",
            alignItems: "center",
            justifyContent: "center",
            minHeight: "40vh",
        }}>
            <div style={{
                display: "flex",
                alignItems: "center",
                flexDirection: "column",
                justifyContent: "center",
            }}>
                <img style={{ width: "70%" }} src={logo} alt="logo" />
                <h1 style={{
                    color: theme.palette.primary.main,
                    letterSpacing: "0.3rem",
                    fontWeight: "300",
                    whiteSpace: "nowrap",
                }}>adaptive framework</h1>
                <h2 style={{
                    color: theme.palette.primary.main,
                    maxWidth: "375px",
                    textAlign: "center",
                    paddingLeft: "32px",
                    paddingRight: "32px",
                    fontSize: "1.2rem",
                    fontWeight: "400",
                    lineHeight: "1.5em",
                }}>Interfaces and Tools for designing Object-based Applications in an Adaptive way.</h2>
            </div>
        </div>
    );
};

const HomeSection = ({ title, iconName, description, linkText, linkUrl }) => {

    const theme = useTheme();
    const history = useHistory();

    return (
        <div style={{ backgroundColor: theme.palette.background.paper, padding: theme.spacing(2.5) }}>
            <div style={{ minHeight: "200px" }}>
                <div style={{ display: "flex" }}>
                    <Icon color="primary" style={{ marginRight: theme.spacing(1) }} iconName={iconName} />
                    <h2 style={{ fontSize: "1.2125rem", margin: "0" }}>{title}</h2>
                </div>
                <h3 style={{ fontSize: "1em", lineHeight: "1.5em", fontWeight: "400" }}>
                    { description }
                </h3>
            </div>
            <Divider />
            <Button 
                color="primary"
                label={linkText}
                variant="text" 
                onClick={() => history.push(linkUrl)} 
                endIcon="chevron_right"
            />
        </div>
    );
};

const Home = () => {    

    return (
        <Container id="admin-home" data-testid="admin-home" maxWidth="xl" style={{ height: "100%", overflow: "auto" }}>            
            <HomeHeader />
            <Responsive
                rows={[
                    {
                        spacing: 1,
                        columns: [
                            {
                                breakpoints: {
                                    small: 12,
                                    medium: 6,
                                    xl: 3,
                                },
                                contains:
                                    <HomeSection 
                                        title="Objects"
                                        description="Objects are at the core of data access.  Manage the objects that are available from this instance of Adaptive Framework, in a general-purpose way."
                                        iconName="edit"
                                        linkText="Browse Objects"
                                        linkUrl="/Objects"
                                    />                                    
                            },
                            {
                                breakpoints: {
                                    small: 12,
                                    medium: 6,
                                    xl: 3,
                                },
                                contains:
                                    <HomeSection 
                                        title="Administration"
                                        description="Customize this instance of Adaptive Framework server by adding extensions to provide adapter, log or authorization services.  Create and manage data models and authorization policies."
                                        iconName="settings"
                                        linkText="Administration"
                                        linkUrl="/Admin"
                                    />
                            },
                            {
                                breakpoints: {
                                    small: 12,
                                    medium: 6,
                                    xl: 3,
                                },
                                contains:
                                    <HomeSection 
                                        title="Documentation"
                                        description="The Documentation provides a living reference and user guide, generated by this instance of Adaptive Framework.  View your live metadata through API reference and visual diagrams."
                                        iconName="bookmarks"
                                        linkText="Documentation"
                                        linkUrl="/Documentation"
                                    />
                            },
                            {
                                breakpoints: {
                                    small: 12,
                                    medium: 6,
                                    xl: 3,
                                },
                                contains:
                                    <HomeSection 
                                        title="Tools"
                                        description="Access Tools for both developers and end-users to interact with the Adaptive interfaces and objects.  Simulate expressions and functions, or design simple object layouts."
                                        iconName="build"
                                        linkText="Tools"
                                        linkUrl="/Tools"
                                    />
                            },
                        ]
                    }
                ]}
            />
            <div style={{ padding: "80px 0 0 10px" }}>
                <div>
                    <Typography
                        style={{ display: "inline-block", paddingRight: "5px" }}
                        text="For more information, please visit "
                    />
                    <Link
                        style={{ display: "inline-block" }}
                        url="http://adaptiveframework.org" text="http://adaptiveframework.org"
                    />
                </div>
            </div>
        </Container>
    );
};

export default Home;
