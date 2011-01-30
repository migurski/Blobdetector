all: BlobDetector/_blobs.so

BlobDetector/_blobs.so: blobs.cpp
	python setup.py build
	mv -v build/lib.macosx-10.6-x86_64-2.6/_blobs.so BlobDetector/_blobs.so

clean:
	rm -f BlobDetector/_blobs.so
