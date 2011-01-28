# totally swiped from http://www.python.org/doc/2.5.2/ext/building.html
from distutils.core import setup, Extension

blobs = Extension('blobs', sources = ['blobs.c'])

setup (name = 'blobs',
       version = '1.0',
       description = 'Stub function which takes a string and returns a string',
       ext_modules = [blobs])
