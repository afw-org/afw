// See the 'COPYING' file in the project root for licensing information.
import {useParams} from "@tanstack/react-router";

import LogDetails from "./LogDetails";
import {useAppCore} from "../../../hooks";

import {
    Link,
    Message,
    Table
} from "@afw/react";

const Logs = () => {

    const {logs} = useAppCore();
    /* the route's optional param (see ../../routes.js) */
    const {logId: routeLogId} = useParams({ strict: false });

    if (!logs)
        return null;

    /* one log, chosen by the route's logId */
    const renderLog = () => {
        let selectedLog;

        if (!logs)
            return <div />;

        logs.forEach((log) => {
            if (log.logId === routeLogId)
                selectedLog = log;
        });
            
        return (
            <div>                                
                <LogDetails log={selectedLog} />
            </div>
        );
    };

    return (
        <div>                
            {
                /* one log (by the route's logId), or the list */
                routeLogId ? renderLog() : (
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
                                        let url = "/Admin/Logs/" + encodeURIComponent(logId);

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
                )
            }
        </div>
    );
};

export default Logs;
