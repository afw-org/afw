// See the 'COPYING' file in the project root for licensing information.
import {useHistory} from "react-router";
import LinearProgress from "@mui/material/LinearProgress";

import Database from "mdi-material-ui/Database";
import Cogs from "mdi-material-ui/Cogs";

import {
    Button,
    Divider,
    Icon,
    Link,
    Paper,
    Responsive,
    Typography
} from "@afw/react";

import {useSystemInfo, useServer} from "@afw/react";
import {useAppCore, useTheme} from "../hooks";

const StatusSection = ({ route, title, content }) => {

    const history = useHistory();
    const theme = useTheme();

    return (
        <Paper 
            contains={
                <div style={{ padding: theme.spacing(2), paddingBottom: theme.spacing(1) }}>
                    <div
                        role="button"
                        tabIndex="0"
                        onKeyPress={() => history.push(route)}
                        onClick={() => history.push(route)}
                        style={{ cursor: "pointer" }}
                    >   
                        <Typography color="primary" size="6" text={title} />                                                    
                    </div>
                    { content }
                    <Divider />
                    <Button 
                        color="primary"
                        variant="text" 
                        label="Application" 
                        endIcon="chevron_right"
                        onClick={() => history.push("/Admin/Application")} 
                    />
                </div>
            }
        />);
};

const Status = () => {

    const {application, extensions, adaptors, services, logs} = useAppCore();
    const {server} = useServer();
    const {systemInfo} = useSystemInfo();
    const history = useHistory();
    const theme = useTheme();

    const {startTime, concurrent, maxConcurrent, threadCount, serverVersion} = server ? server : {};
    const {title, description} = application ? application.getPropertyValues() : {};    

    const uptime = server ? new Date(startTime) : undefined;

    let general;
    if (systemInfo) 
        systemInfo.forEach(info => {
            if (info._meta_.objectId === "general")
                general = info;
        });

    const {nodename, machine, release, sysname} = general ? general : {};

    return (
        <>
            <Typography size="10" text="Administration" />
            <div style={{ height: theme.spacing(5) }} />
            <Typography text="The Administration allows you to view and configure your Adaptive Application.  Create and manage extensions, services and policies that will impact how object data will be viewed, stored, accessed and shared with external entities." />
            <div style={{ height: theme.spacing(5) }} />
            <Divider />
            <div style={{ height: theme.spacing(5) }} />

            <Responsive
                spacing={2}
                items={[                   
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                            large: 6
                        },
                        contains:
                            <StatusSection 
                                route="/Admin/Application"
                                title={title}
                                content={
                                    <div style={{ height: "170px" }}>
                                        <Typography
                                            size="3"
                                            text={description}
                                        />
                                    </div>
                                }
                            />                                    
                    },
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                            large: 6
                        },
                        contains:                                   
                            <Paper                                        
                                contains={
                                    <div style={{ padding: theme.spacing(2), paddingBottom: theme.spacing(1) }}>
                                        <div
                                            role="button"
                                            tabIndex="0"
                                            onKeyPress={() => history.push("/Admin/Server")}
                                            onClick={() => history.push("/Admin/Server")}
                                            style={{ cursor: "pointer" }}
                                        >
                                            <div style={{ display: "flex", justifyContent: "space-between" }}>
                                                <Typography color="primary" size="6" text={nodename} />
                                                <Typography size="6" color="primary" text={serverVersion} />
                                            </div>                                                    
                                            <div style={{ height:  theme.spacing(2) }} />
                                        </div>
                                        <div style={{ height: "150px" }}>
                                            <LinearProgress variant="determinate" value={Math.round((concurrent / threadCount) * 100)} />
                                            <div style={{ height: theme.spacing(0.5) }} />
                                            <LinearProgress variant="determinate" value={Math.round((maxConcurrent / threadCount) * 100)} />
                                            <div style={{ height: theme.spacing(3) }} />
                                            <Typography size="3" text={sysname + " " + release + " (" + machine + ")"} />
                                            <div style={{ height: theme.spacing(3) }} />
                                            <div style={{ display: "flex", alignItems: "flex-end" }}>
                                                <Icon iconName="arrow_upward" color="primary" size="small" />
                                                <Typography
                                                    style={{ marginLeft: theme.spacing(0.5) }}
                                                    size="2"
                                                    text={uptime ? uptime.toLocaleDateString() + " " + uptime.toLocaleTimeString() : ""}
                                                />
                                            </div>                                                    
                                        </div>
                                        <Divider />
                                        <Button 
                                            color="primary"
                                            variant="text" 
                                            label="Server" 
                                            onClick={() => history.push("/Admin/Server")} 
                                            endIcon="chevron_right"
                                        />
                                    </div>
                                }
                            />
                    },
                
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                            large: 3
                        },
                        contains:
                            <StatusSection 
                                route="/Admin/Schema"
                                title="Schema"
                                content={
                                    <div style={{ height: "125px" }}>
                                        <Typography size="3" text="Use Adaptive Schema to examine how Objects and their Properties are described." />
                                    </div>
                                }
                            />
                    },
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                            large: 3
                        },
                        contains:
                            <StatusSection 
                                route="/Admin/Models"
                                title="Models"
                                content={
                                    <div style={{ height: "125px" }}>
                                        <Typography size="3" text="Use Adaptive Models to Map and Transform Objects and their Properties." />
                                    </div>
                                }
                            />                                    
                    },
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                            large: 3
                        },
                        contains:
                            <StatusSection 
                                route="/Admin/Authorization"
                                title="Authorization"
                                content={
                                    <div style={{ height: "125px" }}>
                                        <Typography size="3" text="Authorization Policies and Rules describe how data is accessed." />
                                    </div>
                                }
                            />                                    
                    },
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                            large: 3
                        },
                        contains:
                            <StatusSection 
                                route="/Admin/Provisioning"
                                title="Provisioning"
                                content={
                                    <div style={{ height: "125px" }}>
                                        <Typography size="3" text="Provisioning describes how Objects are synchronized with external Provisioning Peers." />
                                    </div>
                                }
                            />                                     
                    },
                
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                            xxl: 3
                        },
                        contains:                                       
                            <Paper
                                contains={
                                    <div style={{ padding:  theme.spacing(2) }}>
                                        <div
                                            role="button"
                                            tabIndex="0"
                                            onKeyPress={() => history.push("/Admin/Extensions")}
                                            onClick={() => history.push("/Admin/Extensions")}
                                            style={{ display: "flex", justifyContent: "space-between", minWidth: "150px", cursor: "pointer" }}
                                        >
                                            <div>
                                                <Typography color="primary" size="9" text={extensions ? extensions.length : ""} />
                                                <Typography color="primary" size="6" text="Extensions" />
                                            </div>
                                            <div>
                                                <Icon color="primary" iconName="extension" />
                                            </div>
                                        </div>
                                        <Divider />
                                        <div style={{ height: "350px", overflow: "auto" }}>
                                            {
                                                extensions && extensions.map(extension =>
                                                    <div key={extension.extensionId} style={{ padding: theme.spacing(0.5) }}>
                                                        <Typography size="3" text={extension.extensionId} />
                                                    </div>
                                                )
                                            }
                                        </div>
                                        <Divider />
                                        <Button 
                                            color="primary"
                                            variant="text" 
                                            label="Extensions" 
                                            onClick={() => history.push("/Admin/Extensions")} 
                                            endIcon="chevron_right"
                                        />
                                    </div>
                                }
                            />
                    },
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                            xxl: 3
                        },
                        contains:
                            <Paper
                                contains={
                                    <div style={{ padding:  theme.spacing(2) }}>
                                        <div
                                            role="button"
                                            tabIndex="0"
                                            onKeyPress={() => history.push("/Admin/Services")}
                                            onClick={() => history.push("/Admin/Services")}
                                            style={{ display: "flex", justifyContent: "space-between", minWidth: "150px", cursor: "pointer" }}
                                        >
                                            <div>
                                                <Typography color="primary" size="9" text={services ? services.length : ""} />
                                                <Typography color="primary" size="6" text="Services" />
                                            </div>
                                            <div>
                                                <Cogs color="primary" />
                                            </div>
                                        </div>
                                        <Divider />
                                        <div style={{ height: "350px", overflow: "auto" }}>
                                            {
                                                services && services.map(service =>
                                                    <div key={service.getPropertyValue("serviceId")} style={{ padding:  theme.spacing(0.5) }}>
                                                        <Link
                                                            text={service.getPropertyValue("serviceId")}
                                                            url={"/Admin/Services/" + service.getPropertyValue("serviceId") }
                                                        />
                                                    </div>
                                                )
                                            }
                                        </div>
                                        <Divider />
                                        <Button 
                                            color="primary"
                                            variant="text" 
                                            label="Services" 
                                            onClick={() => history.push("/Admin/Services")} 
                                            endIcon="chevron_right"
                                        />
                                    </div>
                                }
                            />
                    },
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                            xxl: 3
                        },
                        contains:
                            <Paper
                                contains={
                                    <div style={{ padding:  theme.spacing(2) }}>
                                        <div
                                            role="button"
                                            tabIndex="0"
                                            onKeyPress={() => history.push("/Admin/Adaptors")}
                                            onClick={() => history.push("/Admin/Adaptors")}
                                            style={{ display: "flex", justifyContent: "space-between", minWidth: "150px", cursor: "pointer" }}
                                        >
                                            <div>
                                                <Typography color="primary" size="9" text={adaptors ? adaptors.length : ""} />
                                                <Typography color="primary" size="6" text="Adaptors" />
                                            </div>
                                            <div>
                                                <Database color="primary" />
                                            </div>
                                        </div>
                                        <Divider />
                                        <div style={{ height: "350px", overflow: "auto" }}>
                                            {
                                                adaptors && adaptors.map(adaptor =>
                                                    <div key={adaptor.adaptorId} style={{ padding:  theme.spacing(0.5) }}>
                                                        <Link
                                                            text={adaptor.adaptorId}
                                                            url={"/Admin/Adaptors/" + adaptor.adaptorId}
                                                        />
                                                    </div>
                                                )
                                            }
                                        </div>
                                        <Divider />
                                        <Button 
                                            color="primary"
                                            variant="text" 
                                            label="Adaptors" 
                                            onClick={() => history.push("/Admin/Adaptors")} 
                                            endIcon="chevron_right"
                                        />
                                    </div>
                                }
                            />
                    },
                    {
                        breakpoints: {
                            small: 12,
                            medium: 6,
                            xxl: 3
                        },
                        contains:
                            <Paper
                                contains={
                                    <div style={{ padding: theme.spacing(2) }}>
                                        <div
                                            role="button"
                                            tabIndex="0"
                                            onKeyPress={() => history.push("/Admin/Logs")}
                                            onClick={() => history.push("/Admin/Logs")}
                                            style={{ display: "flex", justifyContent: "space-between", minWidth: "150px", cursor: "pointer" }}
                                        >
                                            <div>
                                                <Typography color="primary" size="9" text={logs ? logs.length : ""} />
                                                <Typography color="primary" size="6" text={"Logs"} />
                                            </div>
                                            <div>
                                                <Icon color="primary" iconName="list_alt" />
                                            </div>
                                        </div>
                                        <Divider />
                                        <div style={{ height: "350px", overflow: "auto" }}>
                                            {
                                                logs && logs.map(log =>
                                                    <div key={log.logId} style={{ padding:  theme.spacing(0.5) }}>
                                                        <Link
                                                            text={log.logId}
                                                            url={"/Admin/Logs/" + log.logId}
                                                        />
                                                    </div>
                                                )
                                            }
                                        </div>
                                        <Divider />
                                        <Button 
                                            color="primary"
                                            variant="text" 
                                            label="Logs" 
                                            onClick={() => history.push("/Admin/Logs")} 
                                            endIcon="chevron_right"
                                        />
                                    </div>
                                }
                            />
                    },
                        
                ]}
            />
        </>
    );
};

export default Status;
