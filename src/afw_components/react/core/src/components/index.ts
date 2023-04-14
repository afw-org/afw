// See the 'COPYING' file in the project root for licensing information.
export {default as Autocomplete} from "./Autocomplete";
export * from "./Autocomplete";

export {default as Box} from "./Box";
export * from "./Box";

export {default as Breadcrumb} from "./Breadcrumb";
export * from "./Breadcrumb";

export {default as Button} from "./Button";
export * from "./Button";

export {default as Callout} from "./Callout";
export * from "./Callout";

export {default as Chart} from "./Chart";
export * from "./Chart";

export {default as Checkbox} from "./Checkbox";
export * from "./Checkbox";

export {default as ChoiceGroup} from "./ChoiceGroup";
export * from "./ChoiceGroup";

export {default as CodeEditor} from "./CodeEditor";
export * from "./CodeEditor";

export {default as Collapsible} from "./Collapsible";
export * from "./Collapsible";

export {default as DatePicker} from "./DatePicker";
export * from "./DatePicker";

export {default as DateTimePicker} from "./DateTimePicker";
export * from "./DateTimePicker";

export {default as Dialog} from "./Dialog";
export * from "./Dialog";

export {default as DiffEditor} from "./DiffEditor";
export * from "./DiffEditor";

export {default as Divider} from "./Divider";
export * from "./Divider";

export {default as Dropdown} from "./Dropdown";
export * from "./Dropdown";

export {default as DropdownEditor} from "./DropdownEditor";
export * from "./DropdownEditor";

export {default as EditableCallout} from "./EditableCallout";
export * from "./EditableCallout";

export {default as EditableLabel} from "./EditableLabel";
export * from "./EditableLabel";

export {default as Hidden} from "./Hidden";
export * from "./Hidden";

export {default as Icon} from "./Icon";
export * from "./Icon";

export {default as Image} from "./Image";
export * from "./Image";

export {default as Link} from "./Link";
export * from "./Link";

export {default as List} from "./List";
export * from "./List";

export {default as ListEditor} from "./ListEditor";
export * from "./ListEditor";

export {default as Menu} from "./Menu";
export * from "./Menu";

export {default as Message} from "./Message";
export * from "./Message";

export {default as Modal} from "./Modal";
export * from "./Modal";

export {default as Native} from "./Native";
export * from "./Native";

export {default as Nav} from "./Nav";
export * from "./Nav";

export {default as Paper} from "./Paper";
export * from "./Paper";

export {default as Picker} from "./Picker";
export * from "./Picker";

export {default as Responsive} from "./Responsive";
export * from "./Responsive";

export {default as Skeleton} from "./Skeleton";
export * from "./Skeleton";

export {default as Slider} from "./Slider";
export * from "./Slider";

export {default as Snackbar} from "./Snackbar";
export * from "./Snackbar";

export {default as SpinButton} from "./SpinButton";
export * from "./SpinButton";

export {default as Spinner} from "./Spinner";
export * from "./Spinner";

export {default as Stepper} from "./Stepper";
export * from "./Stepper";

export {default as Table} from "./Table";
export * from "./Table";

export {default as Tabs} from "./Tabs";
export * from "./Tabs";

export {default as TextField} from "./TextField";
export * from "./TextField";

export {default as TimeDuration} from "./TimeDuration";
export * from "./TimeDuration";

export {default as TimePicker} from "./TimePicker";
export * from "./TimePicker";

export {default as TimezonePicker} from "./TimezonePicker";
export * from "./TimezonePicker";

export {default as Toggle} from "./Toggle";
export * from "./Toggle";

export {default as ToggleButtons} from "./ToggleButtons";
export * from "./ToggleButtons";

export {default as Toolbar} from "./Toolbar";
export * from "./Toolbar";

export {default as Tooltip} from "./Tooltip";
export * from "./Tooltip";

export {default as Tree} from "./Tree";
export * from "./Tree";

export {default as Typography} from "./Typography";
export * from "./Typography";


export {default as AdaptiveComponent} from "./AdaptiveComponent";
export * from "./AdaptiveComponent";

import {useGetObject, useRetrieveObjects} from "../hooks";

/**
 * AdaptiveObject
 *
 * This is a React component that uses render props to supply the data
 * from an adaptive store, based on the input props.
 */
export const AdaptiveObject = (props : any) => {

    const result = useGetObject(props);

    return props.children(result);
};

/**
 * AdaptiveObjects
 *
 * This is a React component that uses render props to supply the data
 * from an adaptive store, based on the input props.
 */
export const AdaptiveObjects = (props : any) => {

    const result = useRetrieveObjects(props);

    return props.children(result);
};