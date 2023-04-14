// See the 'COPYING' file in the project root for licensing information.
import { isObject } from "./utils";

/**
 * AfwStreams
 * 
 * This class is a helper class to parsing Content-Type: application/x-afw.
 * It reads the input as a sequence of stream frames.
 * 
 */
export class AfwStreams {

    private stream  : ReadableStream<Uint8Array>;
    private reader  : ReadableStreamDefaultReader;
    private done    : boolean;
    private chunk?  : Uint8Array;
    private cursor  : number;
    private decoder : TextDecoder;

    constructor(stream : ReadableStream) {        
        this.stream = stream;
        this.reader = this.stream.getReader();
        this.done = false;
        this.cursor = 0; 
        this.decoder = new TextDecoder("utf-8");
    }

    /**
     * Reads a single chunk of data and sets the done flag, if the stream is finished.
     * Each chunk is an instance of Uint8Array.
     */
    async readChunk() {
        const {done, value} = await this.reader.read();

        this.done = done;
        this.chunk = value;
        this.cursor = 0;

        return value;
    }

    /**
     * Reads from the stream for size bytes long.
     * 
     * @param size The length to read
     * @param peek Advance the cursor or just peek
     * @returns Uint8Array
     */
    async read(size : number) {   

        let array : Uint8Array = new Uint8Array(size);
        let remaining : number = size;        
        let arrayPos : number = 0;

        /* read any chunks if we are ready and need more */
        if (!this.chunk || this.cursor === this.chunk.length)
            await this.readChunk();

        if (!this.chunk)
            throw new Error("End of stream");

        while (remaining > 0) {           

            /* check if our current chunk is adequate */
            let available = this.chunk.length - this.cursor;
            if (available > remaining) {                                
                array.set( this.chunk.slice(this.cursor, this.cursor + remaining), arrayPos );
                this.cursor += remaining;
                arrayPos += remaining;
                remaining = 0;
            } else {
                /* use the rest of this chunk and request a new one */
                array.set( this.chunk.slice(this.cursor, this.cursor + available), arrayPos );
                arrayPos += available;
                this.cursor = this.chunk.length;
                remaining -= available;   

                await this.readChunk();             
            }
        }
        return array;
    }

    /**
     * Given a byte and a string, this determines if one of the characters
     * in the string matches the byte.
     * 
     * @param byte The unicode byte to compare
     * @param str The string containing bytes to individually compare
     * @returns Whether the byte matches one of them
     */
    isOneOfChars(byte: number, str : string) : boolean {
        for (let i = 0; i < str.length; i++) {
            if (byte === str.charCodeAt(i))
                return true;
        }

        return false;
    }

    /**
     * Concatenates two Uint8Array's together.
     * 
     * @param a First array
     * @param b Second array
     * @returns New Uint8Array with the contents of a followed by the contents b.
     */
    concatArrays(a : Uint8Array, b : Uint8Array) {
        const c = new Uint8Array(a.length + b.length);
        c.set(a);
        c.set(b, a.length);

        return c;
    }
    
    /**
     * Reads a string, delimited by a characters (space default) and end of stream.
     * 
     * @param delimiters A string containing a set of delimiters     
     * @returns A string that was read up until the delimiter
     */
    async readString(delimiters : string = " ") {        

        /* read any chunks if we are ready */
        if (!this.chunk || this.cursor === this.chunk.length) {            
            await this.readChunk();
        }

        if (!this.chunk)
            throw new Error("End of stream");

        let pos;
        let array;

        /* find our delimiter amongst the bytes */        
        for (pos = this.cursor; !this.done && !this.isOneOfChars(this.chunk[pos], delimiters); pos++) 
        {
            /* getting ready to wrap to next chunk */
            if (pos === this.chunk.length) {
                if (array === undefined)
                    array = new Uint8Array(this.chunk.slice(this.cursor));
                else
                    array = this.concatArrays(array, this.chunk.slice(this.cursor));                

                await this.readChunk();                
                pos = -1;
            }
        }
 
        if (pos >= this.cursor) {
            if (array === undefined)
                array = this.chunk.slice(this.cursor, pos);
            else if (!this.done) {                
                array = this.concatArrays(array, this.chunk.slice(this.cursor, pos));
            }

            /* advance the cursor */
            this.cursor = pos + 1;            

            return this.decoder.decode(array);
        }
    }

    /**
     * This routine reads a single frame from an input buffer. A frame consists of:
     * 
     *   <sequence> <size> <streamId> <info>\n<data>
     * 
     * @param decode Specifies whether to decode the data in the frame automatically
     */
    async readFrame(decode : boolean = false) {              

        const _sequence = await this.readString();
        if (_sequence === undefined)            
            throw new Error("Invalid frame sequence number");
        
        const sequence : number = parseInt(_sequence);        
        if (isNaN(sequence)) {            
            throw new Error("Invalid sequence number: " + _sequence);
        }

        const _size = await this.readString();
        if (_size === undefined)
            throw new Error("Invalid frame data size for sequence:  " + sequence);
        const size : number = parseInt(_size);        

        /* now read up until \n and decide if we have just streamId, or streamId + info */
        let streamId = await this.readString("\n");        
        if (!streamId)
            throw new Error("No streamId found");

        let info;
        if (streamId.indexOf(" ") > 0) 
            ([streamId, info] = streamId.split(" "));
        
        let data;
        if (size > 0) {            
            data = await this.read(size);
            if (!data) {
                this.done = true;
                throw new Error("End of stream");
            }
        } else {
            this.done = true;
        }

        if (decode) {
            /* \fixme for now decode based on streamId, later use info */
            switch (streamId) {
            case "response":
            case "result":
                data = JSON.parse( this.decoder.decode(data) );
                break;
            case "end":
                break;
            default:
                data = this.decoder.decode(data);
            }
        }
        
        /* return an object containing the frame fields */
        return { sequence, size, streamId, info, data };
    }

    /**     
     * This routine reads from the stream completely and returns it as an Uint8Array.
     */
    async readFull() {       
        let fullText = new Uint8Array(), value;
        while (!this.done) {
            /* read() will read a chunk at a time, returning the 'value' chunk and a 'done' flag */
            value = await this.readChunk();
            if (!this.done) {
                fullText = this.concatArrays(fullText, value);                
            }            
        }

        return fullText;
    }

    /**
     * This routine reads all of the streams and returns an object containing each one, along
     * with its parsed data.
     * 
     * The response/result streams are interpreted as JSON, while all else is plaintext.
     */
    async readStreams() {        
        let streams : { [streamId : string] : any} = {};

        while (!this.done) {            
            const {streamId, data} = await this.readFrame(true);
            
            if (streamId !== "end") {
                /* when accumulating frames together, we either concat text strings or combine arrays */
                if (streams[streamId] === undefined) {
                    /* new stream data */
                    streams[streamId] = data;
                } else {
                    /* existing stream data */
                    if (typeof streams[streamId] === "string")
                        streams[streamId] += data;
                    else if (isObject(streams[streamId]))
                        streams[streamId] = [ streams[streamId], data ];
                    else
                        streams[streamId] = [ ...streams[streamId], data ];
                }
            }
        }

        return streams;
    }
}

export default AfwStreams;