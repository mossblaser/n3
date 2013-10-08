N3: No-Nonsense Navigation
==========================

A hand-held GPS module, but without the nonsense! When you turn it on it tells
you where you are (using the most sensible coordinate system it knows about).
Then you turn it off again.

Software
--------

The software for a Maple Mini board can be found in `./software/`. The LeafLabs
[libmaple](http://leaflabs.com/docs/unix-toolchain.html]) should be installed,
the environment variable `LIB_MAPLE_HOME` set to its installed path and the
compiler in the `PATH`.

Build and flash using:
	
	cd software
	make
	make install


Hardware
--------

Designs for the hardware can be found in `./hardware/`.
