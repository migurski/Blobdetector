# totally swiped from http://www.python.org/doc/2.5.2/ext/building.html
from distutils.core import setup, Extension

blobs = Extension('_blobs', sources=['blobs.cpp'])

setup(name = 'BlobDetector',
      version = '0.9.1',
      description = 'A small package that detects and categorizes blobs in images.',
      author = 'Michal Migurski',
      author_email = 'mike@stamen.com',
      url = 'https://github.com/migurski/Blobdetector',
      packages = ['BlobDetector'],
      ext_modules = [blobs],
      download_url = 'https://github.com/downloads/migurski/Blobdetector/BlobDetector-0.9.1.tar.gz',
      license = 'BSD')
