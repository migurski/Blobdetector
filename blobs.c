#include <Python.h>
#include <string.h>
#include <stdio.h>

#ifndef bloffset
    #define bloffset(b, o) ( (b - 1) * 4 + o )
#endif

/* blobs.detect()
 *   Given image dimensions and a raw string of grayscale pixels, detects blobs in the "image"
 */
static PyObject *detect(PyObject *self, PyObject *args)
{
    int w, h, off, len, blob, blobs = 0;
    int x, y, xmin, ymin, xmax, ymax;
    uint32_t response[256 * 4];
    unsigned char *pixels;
    
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
    
    // an array to hold the labels
    int labels[w * h];
    
    for(y = 0; y < h; y++)
    {
        for(x = 0; x < w; x++)
        {
            // offset in the string for a given (x, y) pixel
            off = (y * w) + x;
            
            if(pixels[off] < 0x80)
            {
                // dark pixel means it's part of a blob
                labels[off] = 0;
            }
            else
            {
                // light pixel means it's part of a blob

                if(x > 0 && labels[off - 1] > 0) {
                    // pixel to the left is a known blob
                    blob = labels[off - 1];
                
                } else if(y > 0 && labels[off - w] > 0) {
                    // pixel one row up is a known blob
                    blob = labels[off - w];
                
                } else {
                    // a new blob!
                    blobs++;
                    blob = blobs;
                    
                    // save the current pixel as the bounds for this blob
                    response[bloffset(blob, 0)] = x;
                    response[bloffset(blob, 1)] = y;
                    response[bloffset(blob, 2)] = x;
                    response[bloffset(blob, 3)] = y;
                }
                
                labels[off] = blob;

                // read the known bounds for the current blob
                xmin = response[bloffset(blob, 0)];
                ymin = response[bloffset(blob, 1)];
                xmax = response[bloffset(blob, 2)];
                ymax = response[bloffset(blob, 3)];
                
                if(x < xmin) {
                    response[bloffset(blob, 0)] = x;
                }
                
                if(y < ymin) {
                    response[bloffset(blob, 1)] = y;
                }
                
                if(x > xmax) {
                    response[bloffset(blob, 2)] = x;
                }
                
                if(y > ymax) {
                    response[bloffset(blob, 3)] = y;
                }
            }
        }
    }
    
    return Py_BuildValue("is#", blobs, response, blobs * sizeof(uint32_t) * 4);
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
