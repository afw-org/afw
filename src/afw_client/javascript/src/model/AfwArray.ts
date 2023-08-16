// See the 'COPYING' file in the project root for licensing information.
import {AfwArrayEntry, AfwObject, AfwEvent, AfwEventId} from ".";

import {IAnyObject} from "./types";

interface Config {    
    parent?:        any;
    propertyName?:  string;
    value?:         any;
    getMeta?:       () => any;
}

export class AfwArray extends AfwEvent {
    
    parent:                 any;
    private propertyName?:  string;
    private value?:         any;
    private getMeta?:       () => any;
    private _entries?:      AfwArrayEntry[];
    private length?:        number;

    constructor(config: Config) {        
        super(config);

        this.parent         = config.parent;
        this.propertyName   = config.propertyName;
        this.value          = config.value;
        this.getMeta        = config.getMeta;
        this.length         = config.value?.length;
    }

    /**
     * entries()     
     */
    entries() {
        const {getMeta, value, propertyName} = this;

        this._entries = [];
        if (value) {
            this._entries = value.map((v: any) => 
                new AfwArrayEntry({
                    value: v, 
                    parent: this, 
                    getMeta,
                    propertyName 
                })
            );
        }

        this.length = this._entries?.length;

        return this._entries?.entries();
    }

    /**
     * getAt()
     */
    getAt(index: number) {
        if (!this._entries)
            this.entries();
        
        return this._entries![index];
    }

    /**
     * 
     * @param {*} callback 
     */
    forEach(callback : (value: any, index: number, array: any[]) => void, thisArg?: any) : void {    
        if (!this._entries)
            this.entries();

        return this._entries!.forEach(callback, thisArg); 
    }

    /**
     * map()
     */
    map(callback : (value: any, index: number, array: any[]) => void, thisArg?: any) : void[] {
        if (!this._entries)
            this.entries();

        return this._entries!.map(callback, thisArg);
    }
    
    /**
     * newValue()
     */
    newValue() {        
        const {getMeta, propertyName} = this;

        if (!this._entries)
            this.entries();

        const newEntry = new AfwArrayEntry({ 
            parent: this, 
            getMeta,
            propertyName 
        });

        if (this._entries) {            
            this._entries.push( newEntry );            
        } else 
            this._entries = [ newEntry ];

        this.length = this._entries.length;   
        
        this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value: this.toJSON() });

        return newEntry;        
    }

    /**
     * removeValue()
     */
    removeValue(entry: AfwArrayEntry) {
        if (!this._entries)
            this.entries();

        if (this._entries)
            this._entries = this._entries.filter(e => e !== entry);

        this.length = this._entries ? this._entries.length : 0;

        this.dispatchEvent({ eventId: AfwEventId.onChanged, source: this, value: this.toJSON() });
    }

    /**
     * getEmbeddingObject()
     */
    getEmbeddingObject() {
        if (this.parent instanceof AfwObject)
            return this.parent;
        else if (this.parent)
            return this.parent.getEmbeddingObject();
    }

    /**
     * This iterator will allow users to call (for... of) on it.
     */
    *[Symbol.iterator]() {
        if (!this._entries)
            this.entries();

        if (this._entries) {
            for (let entry of this._entries) {
                yield entry;
            }
        }
    }

    /* Allows the includes() method to be called, like a list */
    // \fixme should all of these array methods use the AfwArrayEntry or the AfwArrayEntry.getValue()?
    includes(value: any) {
        if (!this._entries)
            this.entries();

        if (this._entries) {
            for (let entry of this._entries) {
                if (entry.getValue() === value)
                    return true;
            }
        }

        return false;
    }

    /**
     * toJSON()
     */
    toJSON(meta = false) {
        if (!this.value)
            return undefined;

        if (!this._entries)
            this.entries();

        let listJSON: IAnyObject = [];

        if (this._entries) {
            this._entries.forEach((entry) => {            
                listJSON.push(entry.toJSON(meta));
            });
        }

        return listJSON;      
    }

    /**
     * Turns this AfwArray into a string.
     * 
     * @param {function}    replacer
     * @param {number}      space
     */
    stringify(replacer? : ((key : any, value : any) => any), space : number = 4) {
        const entries = this.toJSON(true);

        if (entries)
            return JSON.stringify(entries, replacer, space);
    }

    /**
     * validate()
     *
     * Validates a list conforms to its propertyType.
     */
    validate() {
        /* \fixme */
        return true;
    }
}

export default AfwArray;