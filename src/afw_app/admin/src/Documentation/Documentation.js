// See the 'COPYING' file in the project root for licensing information.
import {Route, Switch} from "react-router";

import Container from "../common/Container";

import {Reference} from "./Reference";

import {
    Divider,   
    Typography
} from "@afw/react";

import {useTheme} from "../hooks";


const Documentation = () => {

    const theme = useTheme();
   
    return (
        <Container style={{ height: "100%" }}>
            <Switch>
                <Route exact path="/Documentation" render={() => {
                    return (
                        <>
                            <Typography size="10" text="Documentation" />
                            <div style={{ height: theme.spacing(5) }} />
                            <Typography text="The Documentation provides Reference material for the objects that are available in the currently running instance of Adaptive Framework." />
                            <div style={{ height: theme.spacing(5) }} />
                            <Divider />                            
                        </>
                    );
                }} />                                
                <Route path="/Documentation/Reference/:doc" render={(props) => {                    
                    const doc = props.match.params.doc;                    

                    if (["DataTypes", "Functions", "Schema", "Components"].includes(doc))
                        return <Reference doc={doc} />;
                    else
                        null;             
                }} />
                <Route path="/Documentation/:category" render={(props) => {
                    const category = props.match.params.category;

                    /* Reference is handled specially, as it involves fetching live objects */
                    if (category === "Reference")
                        return <Reference />;

                    else
                        return null;
                }} />
            </Switch>
        </Container>
    );
};

export default Documentation;
