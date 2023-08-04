// See the 'COPYING' file in the project root for licensing information.
import {debounce} from "../utils";

/**
 * An enumeration of the types of eventId's that can be dispatched.
 * 
 */
export enum AfwEventId {
    onChanged           = "onChanged",
    onSavable           = "onSavable",
    onChildChanged      = "onChildChanged",
    onDiscardChanges    = "onDiscardChanges",
    onPropertiesChanged = "onPropertiesChanged",
}

interface Config {
    parent?:        any;   // fixme AfwObject|AfwProperty|AfwList...    
}

interface AfwDispatchEvent {
    eventId:            AfwEventId;
    [prop : string]:    any;
}

type AfwDispatchCallback = (event : AfwDispatchEvent) => void;

/**
 * AfwEvent
 * 
 * AfwObject, AfwProperty, AfwList, AfwListEntry and AfwValue all
 * inherit from AfwEvent to provide event notifications.  By doing so, a caller can
 * register an event listener, using:
 * 
 *      addEventListener(eventId, callback, options)
 * 
 * The eventId tells us which events the user is interested in.  This will be one of the
 * following:
 * 
 *      onChanged
 *      onChildChanged
 *      onSavable
 *      onPropertiesChanged
 *      onDiscardChanges
 * 
 * The callback should be a function, which will be called on the appropriate eventId.
 * The options is an optional object, containing event listener options.
 * 
 * In order to remove the Event Listener, use the function call:
 * 
 *      removeEventListener(eventId, callback)
 * 
 * Additionally, all Listeners can be removed at once via:
 * 
 *      clearEventListeners()
 */
export class AfwEvent {

    private eventCallbacks: {
        [prop: string]: Array<{ callback: AfwDispatchCallback, options: any }>
    };
    private detached:   boolean;
    parent:             any;
    debounce?:          boolean;

    constructor(config: Config) {
        this.eventCallbacks     = {};
        this.detached           = false;
        this.parent             = config?.parent;
    }

    getValue() {}    

    /**     
     * 
     * Adds an event listener to this instance for a given eventId.
     * 
     * @param eventId  {string} The eventId to get callbacks for.
     * @param callback The function to call on these events.
     * @param options  User-defined data for callback.
     */
    addEventListener(eventId: string, callback: () => void, options: any = {}) {
        /* check if we have any callbacks for this eventId yet */
        if (!this.eventCallbacks[eventId])
            this.eventCallbacks[eventId] = [];

        /* push the callback onto our stack of existing callbacks */
        this.eventCallbacks[eventId].push({ callback, options });

        return callback;
    }   

    /**
     * Clears all event listeners associated with eventId (or all, if omitted).
     * 
     * @param eventId {string} The eventId to clear all callbacks for.
     */
    clearEventListeners(eventId?: string) {
        if (eventId) {
            if (this.eventCallbacks[eventId])
                this.eventCallbacks[eventId] = [];
        } else {
            /* no eventId indicates to clear all registered callbacks */
            this.eventCallbacks = {};
        }        
    }

    /**
     * Re-attaches the value to its parent to allow notifications.
     * 
     * @param notify {boolean} Notify event listeners when re-attached.
     */
    attach(notify: boolean = true) {
        this.detached = false;

        if (notify) 
            this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value: this.getValue() });

        return this;
    }

    /**
     * Detaches this value, so changes do not automatically notify
     * parents.  This is useful to avoid unnecessary event dispatching.
     */
    detach() {
        this.detached = true;

        return this;
    }

    /**
     * Dispatches and propagates an event, and calls any dependant callbacks as a result.
     * 
     * @param {object} event Object containing eventId and associated event information.   
     */
    dispatchEvent(event: AfwDispatchEvent) {
        const {eventId} = event;
        
        /* fire off callbacks that were registered for this specific eventId */
        if (this.eventCallbacks[eventId] !== undefined) {
            this.eventCallbacks[eventId].forEach( cb => cb.callback({ ...event }) );
        }

        /* notify the parent chain */
        if (!this.detached && this.parent !== undefined) {
            if (eventId === AfwEventId.onChanged) {
                if (this.debounce) {
                    debounce(
                        (e : AfwDispatchEvent) => this.parent.dispatchEvent(e), 200)(
                        { ...event, eventId: AfwEventId.onChildChanged }
                    );
                } else {
                    /* change the eventId to onChildChanged instead of onChanged */
                    this.parent.dispatchEvent({ ...event, eventId: AfwEventId.onChildChanged });
                }
            } else
                this.parent.dispatchEvent({ ...event });
        }

        /* finally, if we're now in a savable state, emit those changes */
        if (
            ((eventId === AfwEventId.onChanged) || (eventId === AfwEventId.onChildChanged)) && 
            this.eventCallbacks[AfwEventId.onSavable]
        ) 
        {
            /* don't go through the effort, if nobody is interested in it */
            this.eventCallbacks[AfwEventId.onSavable].forEach(cb => 
                cb.callback({ eventId: AfwEventId.onSavable, source: this, value: true }) 
            );
        }
    }

    /**
     * removeEventListener()
     * 
     * @param eventId  The eventId that the callback has been registered for.
     * @param callback The callback function that has been registered for this eventId.
     */
    removeEventListener(eventId: string, callback : AfwDispatchCallback) {
        /* Remove the callback from this eventId */
        if (this.eventCallbacks[eventId]) {
            this.eventCallbacks[eventId] = this.eventCallbacks[eventId].filter( 
                cb => cb.callback !== callback 
            );
        }
    }
}

export default AfwEvent;