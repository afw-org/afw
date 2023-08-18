// See the 'COPYING' file in the project root for licensing information.
import {
    Button,
    Divider,
    Icon,
    Typography
} from "@afw/react";

import {useTheme, useClasses} from "../hooks";

const styles = theme => ({
    table: {
        display: "table",
        padding: theme.spacing(2)
    },
    row: {
        display: "table-row",
        padding: theme.spacing(1)
    },
    cellNoWrap: {
        display: "table-cell",
        padding: theme.spacing(0.5),
        whiteSpace: "nowrap"
    },
    cell: {
        display: "table-cell",
        padding: theme.spacing(0.5)
    }
});

const HelpText = ({ text }) => {
    return (
        <Typography 
            size="3"
            style={{ whiteSpace: "pre-line" }}
            paragraph={true}
            text={text}
        />
    );
};

const InlineHelpText = ({ text }) => 
    <Typography style={{ display: "inline-block" }} component="span" size="3" text={text} />;

const MoreOptionsOption = ({ command, description }) => {
    const {cell, cellNoWrap, row} = useClasses(styles);

    return (
        <div className={row}>
            <div className={cellNoWrap}>
                <Typography size="3" color="primary" text={command} />
            </div>
            <div className={cell}>
                <Typography size="3" color="textSecondary" text={description} />
            </div>
        </div>
    );
};

const FiddleHelpMoreOptions = () => {

    const theme = useTheme();
    const {table} = useClasses(styles);

    return (
        <>
            <Typography style={{ display: "inline-block" }}  size="3" text="The More Options Button" />
            <Icon style={{ display: "inline-block", marginLeft: theme.spacing(0.5), marginRight: theme.spacing(0.5) }}  iconName="more_vert" />
            <Typography style={{ display: "inline-block" }} size="3" text="presents a number of optional commands:" />

            <div style={{ height: theme.spacing(2) }} />

            <div className={table}>
                <MoreOptionsOption command="New" description="Creates a New Script, Template or Expression" />      
                <MoreOptionsOption command="Open" description="Opens a previously saved script, template or expression, stored in local storage" />                
                <MoreOptionsOption command="Save" description="Save the current script, template or expression to local storage" />                
                <MoreOptionsOption command="Save As" description="Save the current script, template or expression with a given identifier" />                
                <MoreOptionsOption command="Delete" description="Delete the current script, template or expression from local storage" />                
                <MoreOptionsOption command="Format" description="Change the Format of the current content" />                
                <MoreOptionsOption command="Editor" description="Change between Source Editor and Builder Mode" />                
                <MoreOptionsOption command="Close Tab" description="Close the current Tab" />                
                <MoreOptionsOption command="Close All Tabs" description="Close all Tabs" />                
                <MoreOptionsOption command="Copy Input" description="Copy the current input" />                
                <MoreOptionsOption command="Paste" description="Paste copied input into this tab" />                            
                <MoreOptionsOption command="Convert to Expression" description="Convert the current input into expression format" />               
            </div>
        </>
    );
};

export const FiddleHelp = () => {

    const theme = useTheme();
    return (
        <>
            <HelpText text={` \
                Fiddle is a tool for entering templates, expressions or scripts and evaluating their output by executing the code \
                on the server and reporting back the results.  This is useful if you are trying to develop a rule and need to \
                quickly see the results.  It may also be useful for executing a task that needs to run once, for maintenance, \
                for example.

                The Toolbar at the top provides five command buttons, which are described below.

            `} />

            <div style={{ display: "flex", justifyContent: "center", alignItems: "center" }}>
                <Button type="icon" color="primary" icon="more_vert" label="More Options" />
                <Button variant="text" size="small" icon="play_arrow" label="Evaluate" />
                <Button variant="text" size="small" icon="format_list_bulleted" label="Listing" />
                <Button variant="text" size="small" icon="code" label="Context" />
                <Button variant="text" size="small" icon="outlined_flag" label="Flags" />
            </div>

            <div style={{ height: theme.spacing(2) }} />

            <FiddleHelpMoreOptions />

            <div style={{ height: theme.spacing(1) }} />
            <Divider />
            <div style={{ height: theme.spacing(1) }} />

            <Typography color="primary" size="5" text="Using the Editor" />            
            <HelpText text={`
                To get started writing some code, choose a new Tab by using the Options Menu, Clicking the + Button, or by typing 'n' for New.  \
                This will create a new Source Editor instance with some rich code editor capabilities.  Adaptive syntax will be highlighted \
                appropriately, and hover tips as well as right-click contextual menus can help you facilitate your code writing. 

                If, at anytime, you wish to save your content, you can use the Options Menu and select "Save" or "Save As", which will prompt \
                you for an identifier to name it.  This will not save your input on the server.  Rather, it uses Local Storage, which is stored \
                in your web browser long-term memory.
            `} />

            <div style={{ height: theme.spacing(1) }} />
            <Divider />
            <div style={{ height: theme.spacing(1) }} />

            <Typography color="primary" size="5" text="Code Evaluation" />
            <div style={{ height: theme.spacing(1) }} />                   

            <InlineHelpText text="After you have entered a template, script or expression, use the" />
            <Button style={{ display: "inline-blocK", marginLeft: theme.spacing(0.5), marginRight: theme.spacing(0.5) }} variant="text" size="small" icon="play_arrow" label="Evaluate"  />            
            <InlineHelpText text="Button to evaluate the current input on the server.  The results results, along with any output, error or trace information will be displayed at the bottom." />            

            <div style={{ height: theme.spacing(2) }} />

            <InlineHelpText text="After you have entered a template, script or expression, you may also use the" />
            <Button style={{ display: "inline-blocK", marginLeft: theme.spacing(0.5), marginRight: theme.spacing(0.5) }} variant="text" size="small" icon="format_list_bulleted" label="Listing" />
            <InlineHelpText text="Button to get the compiled listing of the source code, which may be useful in troubleshooting what your script is producing." />

            <div style={{ height: theme.spacing(2) }} />

            <InlineHelpText text="Some of your Fiddle code will expect context variables to be present in order to execute their intended purpose.  Use the" />
            <Button style={{ display: "inline-blocK", marginLeft: theme.spacing(0.5), marginRight: theme.spacing(0.5) }} variant="text" size="small" icon="code" label="Context" />
            <InlineHelpText text="Button to assign values to contextual variables that you know will be there ahead of time.  These are intended to simulate what the values will be at runtime." />

            <div style={{ height: theme.spacing(2) }} />

            <InlineHelpText text="Use the " />
            <Button style={{ display: "inline-blocK", marginLeft: theme.spacing(0.5), marginRight: theme.spacing(0.5) }} variant="text" size="small" icon="outlined_flag" label="Flags" />
            <InlineHelpText text="Button to change the runtime behavior of Adaptive Framework during this evaluation.  You may want to toggle more trace options, for instance." />

            <div style={{ height: theme.spacing(2) }} />

            <div style={{ height: theme.spacing(1) }} />
            <Divider />
            <div style={{ height: theme.spacing(1) }} />

            <Typography color="primary" size="5" text="Context Editor" />            

            <HelpText text={`
                If your code requires contextual variables present in order to execute, use the Context Editor to add or remove variables as needed.
            `} />    

            <Typography color="primary" size="5" text="Flags" />            

            <HelpText text={`
                It may be helpful to turn on some flags during evaluation.  For instance, certain debugging features can be enabled using flags.                  
            `} />  
        </>
    );
};


export const RequestsHelp = () => {

    const theme = useTheme();
    return (
        <>
            <HelpText text={` \
                Requests is a tool for executing raw HTTP requests. 
            `} />

            <div style={{ height: theme.spacing(2) }} />

        </>
    );
};


export const ContextualHelpRoutes = [

    { path: "/Tools/Fiddle",    title: "Fiddle Help",       Component: FiddleHelp           },
    { path: "/Tools/Requests",  title: "Requests Help",     Component: RequestsHelp         },

];

export default ContextualHelpRoutes;
