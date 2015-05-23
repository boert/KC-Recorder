#!/usr/bin/env python

import sys
import Image

img = Image.open( sys.argv[1] )
#img = img.thumbnail((17, 17), Image.ANTIALIAS)
# try ANTIALIAS, NEAREST, BILINEAR, BICUBIC
border = 0
img = img.crop( (border , border, img.size[0] - border, img.size[1] - border ))
img = img.resize( (20, 16) , Image.NEAREST)
#.convert('1')
img = img.convert('1')

for row in xrange(img.size[1]):
    for column in xrange(img.size[0]):
	pixel_value = img.getpixel((column, row))
        sys.stdout.write(' *'[int(pixel_value < 128)])
    sys.stdout.write('\n')
