# Root of the project
BASEDIR := $(shell pwd)

# Board configuration
BOARD ?= maple_mini
MEMORY_TARGET ?= flash

# Custom modules listed here. E.g.
# USER_MODULES := $(BASEDIR)/libfoo

# For compiling the icons
n3_icons/%.h: n3_icons/%.png
	python n3_icons/png_to_lcd_chars.py $< > $@
n3_icons: n3_icons/about_window_splash.h         \
          n3_icons/altitude_window_splash.h      \
          n3_icons/os_altitude_window_splash.h   \
          n3_icons/os_grid_ref_window_splash.h   \
          n3_icons/png_to_lcd_chars.py           \
          n3_icons/satellite_window_splash.h     \
          n3_icons/wgs_altitude_window_splash.h  \
          n3_icons/wgs_lat_lon_window_splash.h   \
          n3_icons/datetime_window_splash.h      \
          n3_icons/no_fix_window_graphic.h       \
          n3_icons/no_fix_beam0_window_graphic.h \
          n3_icons/no_fix_beam1_window_graphic.h \
          n3_icons/no_fix_beam2_window_graphic.h \
          n3_icons/no_fix_beam3_window_graphic.h


# === You don't have to edit anything below this line ==========================

# Export them to make it available in libmaple Makefile
export BOARD
export MEMORY_TARGET

# Convert fs links and '~' to absolute path
export LIB_MAPLE_HOME := $(shell dirname $(LIB_MAPLE_HOME)/stub)

# Root of the project should be listed as a user module too,
# custom user modules path fix
export USER_MODULES := $(BASEDIR) $(USER_MODULES)


.DEFAULT_GOAL := sketch

# To make it compatible with default project settings of Eclipse CDT.
# If it still reporting "No rule to make target `all'" false error
# while project build, then you may fix it this way:
#     1) go to Project proprties -> C/C++ Build -> Behaviour tab
#     2) change build target name from "all" to "sketch"   

all: sketch

# proxy target
%:
	$(MAKE) -f $(LIB_MAPLE_HOME)/Makefile $@
