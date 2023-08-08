
/**
 * Typescript interface definition for propTypes
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
     * Data Type: (array, object _AdaptiveLayoutComponentType_Chart_Data)
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

