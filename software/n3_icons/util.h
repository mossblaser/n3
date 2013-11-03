/**
 * Utilities for drawing icons in a headerfile
 */

#ifndef N3_ICONS_UTIL_H
#define N3_ICONS_UTIL_H

#include <wirish/wirish.h>

/**
 * Given an octal number with 5 digits produce the binary number with those bits
 * set. Should be evaluated into a simple constant at compile time.
 */
#define N3_ICON_CHAR_LINE(num) ( (((num)>> 0)&1)<<0 \
                               | (((num)>> 3)&1)<<1 \
                               | (((num)>> 6)&1)<<2 \
                               | (((num)>> 9)&1)<<3 \
                               | (((num)>>12)&1)<<4 \
                               )

#endif
