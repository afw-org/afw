// See the 'COPYING' file in the project root for licensing information.
import {Route, Switch} from "react-router";

import LogDetails from "./LogDetails";
import {useAppCore} from "../../../hooks";

import {
    Link,
    Message,
    Table
} from "@afw/react";

const Logs = () => {

    const {logs} = useAppCore();

    if (!logs)
        return null;

    return (
        <div>                
            <Switch>
                <Route exact path="/Admin/Logs/" render={(props) => 
                    <div>                               
                        <Message
                            contains={
                                <div>
                                    <span>To create a new Log, add a new Log Type Service </span>
                                    <Link style={{ display: "inline-block" }} url="/Admin/Services/" text="here" />
                                    <span>.</span>
                                </div>
                            }
                        />                   
                        <Table 
                            rows={logs}
                            columns={[
                                { 
                                    key: "LogId", name: "Id", minWidth: 150, maxWidth: 200, isResizable: true,
                                    onRender: (log) => {                                            
                                        let logId = log.logId;
                                        let url = props.match.url + "/" + encodeURIComponent(logId);

                                        return (
                                            <Link url={url} text={logId} />
                                        );
                                    }
                                },
                                {
                                    key: "LogType", name: "Type", minWidth: 150, maxWidth: 200, isResizable: true,
                                    onRender: (log) => {                          
                                        let properties = log.properties;                                            
                                        let logType = properties.logType;

                                        return (
                                            <span>{logType}</span>
                                        );
                                    }
                                }
                            ]}
                            compact={true}
                            selectionMode="none"
                        />
                    </div>
                }/>
                <Route path="/Admin/Logs/:logId" render={(props) => {
                    let selectedLog;

                    if (!logs)
                        return <div />;

                    logs.forEach((log) => {
                        if (log.logId === decodeURIComponent(props.match.params.logId))
                            selectedLog = log;
                    });
                    
                    return (
                        <div>                                
                            <LogDetails log={selectedLog} />
                        </div>
                    );
                }} />
            </Switch>
        </div>
    );
};

export default Logs;
