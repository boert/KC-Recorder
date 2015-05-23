#!/usr/bin/env python

import sys
import Image

def fill_char( img, startx, starty):
    for y in range(8):
	value = 0
	for x in range(5):
	    value <<= 1
	    pixel_value = img.getpixel( (startx + x, starty + y) )
	    if pixel_value < 128:
		value += 1
	print "0x%02x, " % value,
    print

if len(sys.argv)!=2:
    print "png-picture as parameter!"
    sys.exit()
    
img = Image.open( sys.argv[1] )
border = 0
img = img.crop( (border , border, img.size[0] - border, img.size[1] - border ))
img = img.resize( (23, 17) , Image.NEAREST).convert('L')

# vorspann
print "// Characters from", sys.argv[1]
print "static const PROGMEM char logo[] ="
print "{"

# hauptteil
fill_char( img, 0, 0);
fill_char( img, 6, 0);
fill_char( img, 12, 0);
fill_char( img, 18, 0);

fill_char( img, 0, 9);
fill_char( img, 6, 9);
fill_char( img, 12, 9);
fill_char( img, 18, 9);

# abspann
print "};"


"""
for row in xrange(img.size[1]):
    for column in xrange(img.size[0]):
	pixel_value = img.getpixel((column, row))
        sys.stdout.write(' *'[int(pixel_value < 128)])
    sys.stdout.write('\n')
"""
