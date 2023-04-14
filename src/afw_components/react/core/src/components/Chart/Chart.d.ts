// See the 'COPYING' file in the project root for licensing information.
import * as React from "react";


/**
 *
 */
export interface IChartProps {
    /**
     * animateDuration
     * Data Type: (integer)
     * 
     * Duration in milliseconds to animate the chart data.
     */
    animateDuration?:                   number;
    /**
     * data
     * Data Type: (list, object _AdaptiveLayoutComponentType_Chart_Data)
     * 
     * The data for the Chart.
     */
    data?:                              any[];
    /**
     * tooltip
     * Data Type: (boolean)
     * 
     * Specifies whether or not the Chart contains tooltips.
     */
    tooltip?:                           boolean;
    /**
     * type
     * Data Type: (string)
     * 
     * The type of Chart to display (bar, area, line, pie).
     */
    type?:                              string;
}

/**
 *
 * Component to display data in a Chart.
 * 
 * This component can display a variety of simple or complex chart data in
 * forms such as Pie, Line or Bar to offer a visual representation of the
 * provided data.
 * 
 */
export default function Chart(props: IChartProps): JSX.Element;
