#include <Python.h>
#include <string.h>
#include <stdio.h>

/* blobs.detect()
 *   Given image dimensions and a raw string of grayscale pixels, detects blobs in the "image"
 */
static PyObject *detect(PyObject *self, PyObject *args)
{
    int i, x, y, w, h, off, len, blob = 1;
    unsigned char *pixels, threshold[256];
    
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
                    pixels[off] = pixels[off - 1];
                
                } else if(y > 0 && pixels[off - w] > 0) {
                    // pixel one row up is a known blob
                    pixels[off] = pixels[off - w];
                
                } else {
                    // new blob!
                    pixels[off] = blob;
                    blob++;
                }
            }
        }
    }
    
    return Py_BuildValue("s#", pixels, len);
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
