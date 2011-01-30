"""
1100     11--
1100  >  11--
0011     --22
0011     --22

>>> input = '\xee\xee\x11\x11\xee\xee\x11\x11\x11\x11\xee\xee\x11\x11\xee\xee'
>>> detect(PIL.Image.fromstring('L', (4, 4), input))
[(0, 0, 1, 1), (2, 2, 3, 3)]

1100     11--
1000  >  1---
1011     1-22
0011     --22

>>> input = '\xee\xee\x11\x11\xee\x11\x11\x11\xee\x11\xee\xee\x11\x11\xee\xee'
>>> detect(PIL.Image.fromstring('L', (4, 4), input))
[(0, 0, 1, 2), (2, 2, 3, 3)]

0100     -1--     -1--
1101  >  21-3  >  11-2
1001     2--3     1--2
0011     --43     --22

>>> input = '\x11\xee\x11\x11\xee\xee\x11\xee\xee\x11\x11\xee\x11\x11\xee\xee'
>>> detect(PIL.Image.fromstring('L', (4, 4), input))
[(0, 0, 1, 2), (2, 1, 3, 3)]

00000000     --------     --------
00011100     ---111--     ---111--
00111110     --21111-     --11111-
01111110  >  -321111-  >  -111111-
01111110     -321111-     -111111-
00111110     --21111-     --11111-
00111100     --2111--     --1111--
00000000     --------     --------

>>> input = '\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11\xee\xee\xee\x11\x11\x11\x11\xee\xee\xee\xee\xee\x11\x11\xee\xee\xee\xee\xee\xee\x11\x11\xee\xee\xee\xee\xee\xee\x11\x11\x11\xee\xee\xee\xee\xee\x11\x11\x11\xee\xee\xee\xee\x11\x11\x11\x11\x11\x11\x11\x11\x11\x11'
>>> detect(PIL.Image.fromstring('L', (8, 8), input))
[(1, 1, 6, 6)]
"""

import _blobs
import struct
import PIL.Image

def detect(i):
    """ Take an instance of single-channel PIL.Image, detect blobs and return
    """
    assert i.mode == 'L'
    l, s = _blobs.detect(i.size[0], i.size[1], i.tostring())
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
