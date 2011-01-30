all: BlobDetector/_blobs.so

BlobDetector/_blobs.so: blobs.cpp
	python setup.py build
	mv -v build/lib.*/_blobs.so BlobDetector/_blobs.so

clean:
	rm -f BlobDetector/_blobs.so
