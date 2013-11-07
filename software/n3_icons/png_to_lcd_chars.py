#!/usr/bin/env python

from PIL import Image

def image_to_chars(image, name):
	"""
	Returns (width_chars, height_chars, 
	"""
	
	# Get the image's size
	w,h = image.size
	w_chars = (w+1)/6
	h_chars = (h+1)/9
	
	chars = {}
	
	for y_char in range(h_chars):
		for x_char in range(w_chars):
			chars[(x_char, y_char)] = []
			for y_pixel in range(8):
				y = y_char*9 + y_pixel
				bits = "".join( str(image.getpixel((x,y)))
				                for x in range(x_char*6, x_char*6 + 5)
				              )
				
				chars[(x_char, y_char)].append("N3_ICON_CHAR_LINE(0%s),"%bits)
	
	if len(chars) == 1:
		contents = "\t" + "\t\n".join(chars[(0,0)])
	else:
		contents = ""
		for x_char in range(w_chars):
			contents += "\t{\n"
			for y_char in range(h_chars):
				contents += "\t\t{ /* %d,%d */\n\t\t\t%s\n\t\t},\n"%( x_char
				                                                    , y_char
				                                                    , "\n\t\t\t".join(chars[(x_char,y_char)])
				                                                    )
			contents += "\t},\n"
	
	return ("""
/**
 * Icon converted to characters by png_to_lcd_chars.py.
 */

#ifndef N3_ICONS_%(uname)s_H
#define N3_ICONS_%(uname)s_H

#include "util.h"

uint8 N3_ICON_%(uname)s%(size)s[8] = {
%(contents)s
};

#endif
	"""%{
		"uname": name.upper(),
		"size": "[%d][%d]"%(w_chars,h_chars) if w_chars > 1 or h_chars > 1 else "",
		"contents": contents
	}).strip()


if __name__=="__main__":
	import sys, os
	print image_to_chars( Image.open(sys.argv[1])
	                    , os.path.basename(sys.argv[1]).split(".")[0]
	                    )
