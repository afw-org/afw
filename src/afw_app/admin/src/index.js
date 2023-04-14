// See the 'COPYING' file in the project root for licensing information.
import React from "react";
import ReactDOM from "react-dom";
import * as serviceWorker from "./serviceWorker";

/* eliminate the 8px margin that browsers all seem to request */
import "./index.css";

/* import material design icon fonts */
import "material-design-icons-iconfont/dist/material-design-icons.css";

/* import google fonts for material design */
import "typeface-roboto/index.css";
import "typeface-roboto-mono/index.css";

import App from "./App/App";

/* For development mode, use accessibility warnings */
if (process.env.NODE_ENV !== "production") {
    // Un comment these to receive Accessability warnings in the console.     
    //const axe = require("@axe-core/react");
    //axe(React, ReactDOM, 1000);    
}

ReactDOM.render(
    <App/>,    
    document.getElementById("root")
);

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: http://bit.ly/CRA-PWA
serviceWorker.unregister();
