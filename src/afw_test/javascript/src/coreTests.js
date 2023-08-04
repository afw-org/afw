// See the 'COPYING' file in the project root for licensing information.
import "@testing-library/jest-dom";

/**
 * This entry point loads all of the core tests and makes them available for external
 * component libraries to import and use for full end-to-end component library testing.
 */

import {MemoryRouter} from "react-router-dom";
import {ThemeProvider, createTheme} from "@mui/material/styles";

import Box from "./components/Box/Box.test";
import Callout from "./components/Callout/Callout.test";
import Collapsible from "./components/Collapsible/Collapsible.test";
import Dialog from "./components/Dialog/Dialog.test";
import Drawer from "./components/Drawer/Drawer.test";
import List from "./components/List/List.test";
import Modal from "./components/Modal/Modal.test";
import Paper from "./components/Paper/Paper.test";
import Responsive from "./components/Responsive/Responsive.test";
import Toolbar from "./components/Toolbar/Toolbar.test";

import Divider from "./components/Divider/Divider.test";
import Icon from "./components/Icon/Icon.test";
import Image from "./components/Image/Image.test";
import Message from "./components/Message/Message.test";
import Typography from "./components/Typography/Typography.test";

import CodeEditor from "./components/CodeEditor/CodeEditor.test";
import DiffEditor from "./components/DiffEditor/DiffEditor.test";
import DropdownEditor from "./components/DropdownEditor/DropdownEditor.test";
import EditableCallout from "./components/EditableCallout/EditableCallout.test";
import EditableLabel from "./components/EditableLabel/EditableLabel.test";
import ListEditor from "./components/ListEditor/ListEditor.test";

import Autocomplete from "./components/Autocomplete/Autocomplete.test";
import Button from "./components/Button/Button.test";
import Checkbox from "./components/Checkbox/Checkbox.test";
import ChoiceGroup from "./components/ChoiceGroup/ChoiceGroup.test";
import Slider from "./components/Slider/Slider.test";
import SpinButton from "./components/SpinButton/SpinButton.test";
import TextField from "./components/TextField/TextField.test";
import TimeDuration from "./components/TimeDuration/TimeDuration.test";
import Toggle from "./components/Toggle/Toggle.test";
import ToggleButtons from "./components/ToggleButtons/ToggleButtons.test";

import Breadcrumb from "./components/Breadcrumb/Breadcrumb.test";
import Link from "./components/Link/Link.test";
import Menu from "./components/Menu/Menu.test";
import Nav from "./components/Nav/Nav.test";
import Tabs from "./components/Tabs/Tabs.test";

import DatePicker from "./components/DatePicker/DatePicker.test";
import DateTimePicker from "./components/DateTimePicker/DateTimePicker.test";
import Dropdown from "./components/Dropdown/Dropdown.test";
import Picker from "./components/Picker/Picker.test";
import TimePicker from "./components/TimePicker/TimePicker.test";
import TimezonePicker from "./components/TimezonePicker/TimezonePicker.test";

import Chart from "./components/Chart/Chart.test";
import Table from "./components/Table/Table.test";
import Tree from "./components/Tree/Tree.test";

import Hidden from "./components/Hidden/Hidden.test";
import Skeleton from "./components/Skeleton/Skeleton.test";
import Snackbar from "./components/Snackbar/Snackbar.test";
import Spinner from "./components/Spinner/Spinner.test";
import Stepper from "./components/Stepper/Stepper.test";
import Tooltip from "./components/Tooltip/Tooltip.test";

import PropertyResponsive from "./layouts/objectTypes/ObjectResponsive/PropertyResponsive.test";
import ListResponsive from "./layouts/objectTypes/ObjectResponsive/ListResponsive.test";
import ObjectResponsive from "./layouts/objectTypes/ObjectResponsive/ObjectResponsive.test";
import ObjectFlattened from "./layouts/objectTypes/ObjectFlattened/ObjectFlattened.test";
import ObjectEditor from "./layouts/objectTypes/ObjectEditor/ObjectEditor.test";
import ObjectTree from "./layouts/objectTypes/ObjectTree/ObjectTree.test";

import AdaptorDropdown from "./layouts/propertyTypes/AdaptorDropdown/AdaptorDropdown.test";
import ObjectTypeDropdown from "./layouts/propertyTypes/ObjectTypeDropdown/ObjectTypeDropdown.test";
import PropertyTypeDropdown from "./layouts/propertyTypes/PropertyTypeDropdown/PropertyTypeDropdown.test";
import DataTypeDropdown from "./layouts/propertyTypes/DataTypeDropdown/DataTypeDropdown.test";

import ValueLayout from "./layouts/ValueLayout/ValueLayout.test";

import HybridEditor from "./layouts/dataTypes/Hybrid/HybridEditor.test";
import HybridBuilder from "./layouts/dataTypes/Hybrid/HybridBuilder.test";

import {AdaptiveProvider} from "@afw/react";
import {AfwClient} from "@afw/client";

export const ComponentTestRunners = {
    Box,
    Callout,
    Collapsible,
    Dialog,
    Drawer,
    List,
    Modal,
    Paper,
    Responsive,
    Toolbar,
    Divider,
    Icon,
    Image,
    Message,
    Typography,
    CodeEditor,
    DiffEditor,
    DropdownEditor,
    EditableCallout,
    EditableLabel,
    ListEditor,
    Autocomplete,
    Button,
    Checkbox,
    ChoiceGroup,
    Slider,
    SpinButton,
    TextField,
    TimeDuration,
    Toggle,
    ToggleButtons,
    Breadcrumb,
    Link,
    Menu,
    Nav,
    Tabs,
    DatePicker,
    DateTimePicker,
    Dropdown,
    Picker,
    TimePicker,
    TimezonePicker,
    Chart,
    Table,
    Tree,
    Hidden,
    Skeleton,
    Snackbar,
    Spinner,
    Stepper,
    Tooltip,
};

export const LayoutTestRunners = {
    HybridBuilder,
    HybridEditor,
    PropertyResponsive,
    ListResponsive,
    ObjectResponsive,
    ObjectFlattened,      
    ObjectEditor,  
    ObjectTree,
    AdaptorDropdown,
    ObjectTypeDropdown,
    PropertyTypeDropdown,
    DataTypeDropdown,    
    ValueLayout,
};
                                  

export const Providers = (componentRegistry) => ({ children }) => {
    const client = new AfwClient({ url: "/afw" });

    return (        
        <ThemeProvider theme={createTheme({})}>
            <MemoryRouter>
                <AdaptiveProvider 
                    client={client} 
                    componentRegistry={componentRegistry}
                >
                    { children }
                </AdaptiveProvider>
            </MemoryRouter>
        </ThemeProvider>
    );
};

export const runComponentTests = (componentRegistry) => {

    /* run core component testers */
    for (const [key, component] of Object.entries(ComponentTestRunners)) {
        if (componentRegistry.components[key]) {
            describe("Core Component Tests for [" + key + "]", () => {
                component(Providers(componentRegistry), componentRegistry.components[key]);
            });
        }
    }

};

export const runLayoutTests = (componentRegistry) => {

    /* run core layout testers */
    for (const [key, component] of Object.entries(LayoutTestRunners)) {
        describe("Core Layout Tests for [" + key + "]", () => {
            component(Providers(componentRegistry));
        });
    } 
};

export const runTests = (componentRegistry) => {

    runComponentTests(componentRegistry);
    runLayoutTests(componentRegistry);
};

/* export each test runner as well, so they can be imported individually */
export {
    Box,
    Callout,
    Collapsible,
    Dialog,
    Drawer,
    List,
    Modal,
    Paper,
    Responsive,
    Toolbar,
    Divider,
    Icon,
    Image,
    Message,
    Typography,
    CodeEditor,
    DiffEditor,
    DropdownEditor,
    EditableCallout,
    EditableLabel,
    ListEditor,
    Autocomplete,
    Button,
    Checkbox,
    ChoiceGroup,
    Slider,
    SpinButton,
    TextField,
    TimeDuration,
    Toggle,
    ToggleButtons,
    Breadcrumb,
    Link,
    Menu,
    Nav,
    Tabs,
    DatePicker,
    DateTimePicker,
    Dropdown,
    Picker,
    TimePicker,
    TimezonePicker,
    Chart,
    Table,
    Tree,
    Hidden,
    Skeleton,
    Snackbar,
    Spinner,
    Stepper,
    Tooltip,
    HybridBuilder,
    HybridEditor,
    PropertyResponsive,
    ListResponsive,
    ObjectResponsive,
    ObjectFlattened,      
    ObjectEditor,  
    ObjectTree,
    AdaptorDropdown,
    ObjectTypeDropdown,
    PropertyTypeDropdown,
    DataTypeDropdown,    
    ValueLayout,
};
