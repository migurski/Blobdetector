# totally swiped from http://www.python.org/doc/2.5.2/ext/building.html
from distutils.core import setup, Extension

blobs = Extension('_blobs', sources=['blobs.cpp'])

setup (name = 'BlobDetector',
       version = '0.9.0',
       description = 'A small package that detects and categorizes blobs in images.',
       author='Michal Migurski',
       author_email='mike@stamen.com',
       ext_modules = [blobs])
