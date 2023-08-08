
/**
 * Typescript interface definition for propTypes
 */
export interface IDrawerProps {
    /**
     * anchor
     * Data Type: (string)
     * 
     * Side that this drawer is anchored to
     * 
     * This property describes which side of the page, the drawer should be
     * anchored to. It may be one of 'bottom', 'top', 'left' or 'right'.
     */
    anchor?:                            string;
    /**
     * contains
     * Data Type: (array, object _AdaptiveLayoutComponentType_DrawerItem)
     * 
     * Items inside of the Drawer
     * 
     * This declares a list of items to be rendered inside the Drawer.
     */
    contains?:                          any[];
    /**
     * open
     * Data Type: (boolean)
     * 
     * Specifies whether this Drawer component is open.
     */
    open:                               boolean;
    /**
     * variant
     * Data Type: (string)
     * 
     * The variant to use
     * 
     * This property describes whether the drawer should be anchored
     * temporary, persistent or permanent. If temporary, the drawer will be
     * dismissed when the user unblocks its focus. If persistent, the drawer
     * will be displayed until the user manually closes it. If permanent, the
     * drawer will always remain open.
     */
    variant?:                           string;
}

