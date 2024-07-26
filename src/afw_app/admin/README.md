# Adaptive Framework App

#### A ReactJS based Web Application front end for Adaptive Framework.

AFW App is a collection of adaptive layouts and components, implemented in ReactJS, to provide an administrative front-end to managing an Adaptive Framework application.  Out of the box, it provides a UI to help you setup and configure Services, Adapters, and Logs to get your Adaptive Application up and running.  Once configured, it provides an Object editor, built-in layouts for managing Authorization, Models and Provisioning.  In addition, it provides a mechanism to define new layouts tailored to a specific application.

Although, ReactJS was the Javascript technology used throughout this implementation, many of the design practices we used throughout Adaptive Framework were used here in order to create a sustainable way to implement these Adaptive Layouts in any UI framework that may be better suited in the future.

## Contents

- [Prerequisites](#Prerequisites)
- [Architecture](#Architecture)
- [Building](#Building)
- [Testing](#Testing)
- [Deployment](#Deployment)

### Prerequisites

In order to build the web application, you must have NodeJS and NPM installed on your system.

### Architecture

#### Design

ReactJS is a Javascript-based framework that also supports ES6 (ECMAScript 6).  This app was created using the create-react-app, which handles many of the underlying web packaging complexities for us, such as Babel (a Javascript transpiler), webpack (a bundling tool), along with a default browser service worker in order to create a modern Progressive Web App (PWA).

The app itself may be compiled with a prefix (defaulted to "/apps/afw/admin/"), which allows a web server to identify requests for the app to separate it's URI from the RESTful ones provided by the adapters.  Using this prefix, the app leverages React Router to break apart its major layouts into the following URI's:

  * /apps/afw/admin/Home
  * /apps/afw/admin/Objects
  * /apps/afw/admin/Admin
  * /apps/afw/admin/Documentation
  * /apps/afw/admin/Tools

Another way to architect this, without "reserving" the /apps/afw/admin prefix would be for the web application to listen on a TCP port, separate from the RESTful interfaces provided by the adapters, or separate them out via Virtual Hosts.  This, of course, would result in the Web App and RESTful Web Services to be separated by different hosts.

#### Style

This app uses the Material UI style and component framework.  Material UI is based off of Google's Material Design, which is prominently featured on the Android platform.  This framework offers many built-in features to make development easier for both desktop and mobile applications and support a wide range of accessibility and international features, including LTR (left to right) support.

#### File Structure

The directory structure is laid out as follows:

```
    |-- public/
    |   |-- favicon.ico
    |   |-- index.html
    |   |-- manifest.json
    |   |-- monaco-editor/
    |-- src/
    |   |-- Admin/
    |   |-- App/
    |   |-- common/
    |   |-- Documentation/
    |   |-- Home/
    |   |-- images/
    |   |-- Objects/
    |   |-- Tools/
    |   |-- context.js
    |   |-- hooks.js
    |   |-- index.js
    |   |-- serviceWorker.js
    |   |-- test-utils.js
    |   |-- utils.js
    |-- build_app.sh
    |-- package.json    
    |-- prebuild.sh
    |-- prestart.sh
```

* public
  * Once bundled, the public directory contains any static assets used by the app, including its main index.html from which to launch.
* src
  * Admin
  * App
  * common
    * Common routines, used by a variety of places in the app.  This also includes the adaptiveObject, adaptiveProperty and adaptiveModel classes, which encapsulate adaptive objects and their metadata behind helper functions and callbacks.
  * Documentation 
  * Home
  * images
    Any static images used by the app.
  * Objects
  * Tools
* build_app.sh
  * shell script to build the application.
* package.json
  * Configuration file for the app, specifying all required dependencies and build/test/run targets.


### Building

There is a convenience script that may be used to create a production build, build_app.sh.  It runs some pre-build tasks, sets some environment variables, and does the build, using the "npm run build" target.

### Testing

There are multiple levels of testing that were used in the development of this web app.  

React Testing Library is used to perform unit testing.  You will find \__test__ directories throughout the source code, which contain individual unit test files.

Cypress is an end-to-end, integration testing suite.  It uses Javascript testing commands to run inside an actual browser environment in order to interact with the DOM.  These tests can be run interactively, or via the cypress CLI, which allows you to save the recorded output as video files.

### Deployment

The "build" output will contain a directory of files, some of which are compressed Javascript.  This entire directory needs to be served up by your Web engine, such as Apache or Nginx.

