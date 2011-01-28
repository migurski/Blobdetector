#include <Python.h>
#include <string.h>
#include <stdio.h>

#ifndef adderror
    #define bloffset( b ) ( (b - 1) * 16 )
#endif

/* blobs.detect()
 *   Given image dimensions and a raw string of grayscale pixels, detects blobs in the "image"
 */
static PyObject *detect(PyObject *self, PyObject *args)
{
    int i, w, h, off, len;
    uint8_t blob, blobs = 0;
    uint32_t x, y, xmin, ymin, xmax, ymax;
    unsigned char *pixels, threshold[256];
    unsigned char response[256 * 16];
    
    if(!PyArg_ParseTuple(args, "iis#", &w, &h, &pixels, &len))
    {
        /* fail unless I got two ints and a single string as input */
        return NULL;
    }
    
    if(w * h != len)
    {
        // fail if the given dimensions don't seem to match the passed image
        return NULL;
    }
    
    // set up a basic threshold table to save some if/else later on
    for(i = 0; i < 128; i++) {
        threshold[i] = 0x00;
    }
    
    for(i = 128; i < 256; i++) {
        threshold[i] = 0xFF;
    }
    
    // clear out the response
    for(i = 0; i < 256 * 16; i += 1) {
        response[i] = 0x00;
    }
    
    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            // offset in the string for a given (x, y) pixel
            off = (y * w) + x;
            
            // threshold the input image to simple black/white
            pixels[off] = threshold[ pixels[off] ];
            
            // white pixel means it's part of a blob
            if(pixels[off] == 0xff)
            {
                if(x > 0 && pixels[off - 1] > 0) {
                    // pixel to the left is a known blob
                    blob = pixels[off - 1];
                
                } else if(y > 0 && pixels[off - w] > 0) {
                    // pixel one row up is a known blob
                    blob = pixels[off - w];
                
                } else {
                    // a new blob!
                    blobs++;
                    blob = blobs;
                    
                    // save the current pixel as the bounds for this blob
                    memcpy(response + bloffset(blob) +  0, &x, 4);
                    memcpy(response + bloffset(blob) +  4, &y, 4);
                    memcpy(response + bloffset(blob) +  8, &x, 4);
                    memcpy(response + bloffset(blob) + 12, &y, 4);
                }
                
                pixels[off] = blob;

                // read the known bounds for the current blob
                memcpy(&xmin, response + bloffset(blob) +  0, 4);
                memcpy(&ymin, response + bloffset(blob) +  4, 4);
                memcpy(&xmax, response + bloffset(blob) +  8, 4);
                memcpy(&ymax, response + bloffset(blob) + 12, 4);
                
                if(x < xmin) {
                    memcpy(response + bloffset(blob) +  0, &x, 4);
                }
                
                if(y < ymin) {
                    memcpy(response + bloffset(blob) +  4, &y, 4);
                }
                
                if(x > xmax) {
                    memcpy(response + bloffset(blob) +  8, &x, 4);
                }
                
                if(y > ymax) {
                    memcpy(response + bloffset(blob) + 12, &y, 4);
                }
            }
        }
    }
    
    return Py_BuildValue("is#", blobs, response, blobs * 16);
}

/* map between python function name and C function pointer */
static PyMethodDef BlobsMethods[] = {
    {"detect", detect, METH_VARARGS, "Detect blobs in an image. Arguments are a width, height, and image string."},
    {NULL, NULL, 0, NULL}
};

/* bootstrap function, called automatically when you 'import blobs' */
PyMODINIT_FUNC initblobs(void) {
    (void)Py_InitModule("blobs", BlobsMethods);
}
