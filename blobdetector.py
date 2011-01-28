"""
Test detect() with a small 4x4 square
>>> input = '\xee\xee\x11\x11\xee\x11\x11\x11\xee\x11\xee\xee\x11\x11\xee\xee'
>>> output = detect(PIL.Image.fromstring('L', (4, 4), input))
>>> output
[(0, 0, 1, 2), (2, 2, 3, 3)]
>>> input
'\\xee\\xee\\x11\\x11\\xee\\x11\\x11\\x11\\xee\\x11\\xee\\xee\\x11\\x11\\xee\\xee'
"""

import blobs
import struct
import PIL.Image

def detect(i):
    """ Take an instance of single-channel PIL.Image, detect blobs and return
    """
    assert i.mode == 'L'
    l, s = blobs.detect(i.size[0], i.size[1], i.tostring())
    bounds = _expand(s)
    assert l == len(bounds)
    return bounds

def _expand(s):
    """
    """
    bits = [s[o:o+16] for o in range(0, len(s), 16)]
    bits = [struct.unpack('IIII', s) for s in bits]
    return bits

if __name__ == '__main__':
    import doctest
    doctest.testmod()
