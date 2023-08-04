// See the 'COPYING' file in the project root for licensing information.
/*
 * React Component definition for Divider
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */
import {memo} from "react";
import MuiDivider from "@mui/material/Divider";

/**
 * Implementation Id : Divider
 * Category          : content
 * 
 * A component that displays a Divider line to separate content.
 * 
 * A very simple way to divide content from the remaining content with the
 * user of an dividing line.
 * 
 */
export const Divider = (props) => {
    return (
        <MuiDivider {...props} />
    );
};

/**
 * PropTypes generated by _AdaptiveLayoutComponentType_Divider
 */
Divider.propTypes = {
};

Divider.defaultProps = {
};

export default memo(Divider);
