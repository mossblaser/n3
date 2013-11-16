# First, include the standard libmaple rules.mk header. Leave this
# line alone.
include $(MAKEDIR)/header.mk

###############################################################################

### Change this middle section for your project.

### Source subdirectories

# If any subdirectories contain source files, we have to add them to
# the variable BUILDDIRS, like this. $(BUILD_PATH) is the directory
# where compilation output (like object files) goes. The variable $(d)
# gets expanded to the directory containing this rules.mk file.

PROJECT_BUILD_DIRS := os_coord
PROJECT_BUILD_DIRS += TinyGPS
PROJECT_BUILD_DIRS += maple_sdfat

BUILDDIRS += $(addprefix $(BUILD_PATH)/$(d)/, $(PROJECT_BUILD_DIRS))

### Local flags: these control how the compiler gets called.

# Here we set a variable for our project's include directories.  Note
# that the project top-level directory (i.e., the one containing this
# rules.mk file) is automatically used for include files, so you don't
# need to add it here.
PROJECT_INCLUDE_DIRS :=
EXTERNAL_INCLUDE_DIRS :=

FLAGS_ABS_INCLUDE := $(addprefix -I$(d)/, $(PROJECT_INCLUDE_DIRS))
FLAGS_ABS_INCLUDE += $(addprefix -I/, $(EXTERNAL_INCLUDE_DIRS))

# CFLAGS_$(d) are additional flags you want to give the C compiler.
# WIRISH_INCLUDES and LIBMAPLE_INCLUDES provide the appropriate GCC -I
# switches to let you include libmaple headers.
CFLAGS_$(d) := $(WIRISH_INCLUDES) $(LIBMAPLE_INCLUDES)
# We'll also want our local include directory
CFLAGS_$(d) += $(FLAGS_ABS_INCLUDE)
# C99, mann!
CFLAGS_$(d) += -std=c99

# CXXFLAGS_$(d) are extra flags passed to the C++ compiler.
CXXFLAGS_$(D) := -std=gnu++98

# ASFLAGS_$(d) are extra flags passed to the assembler. We don't have any
# assembly language files in this example, so we'll just leave it empty.
ASFLAGS_$(d) :=

### Local rules

# You can add any additional rules you want here. We don't have
# any extra rules to add.

### Source files

# cSRCS_$(d) are the C source files we want compiled.
cSRCS_$(d) := os_coord/os_coord_transform.c
cSRCS_$(d) += os_coord/os_coord_ordinance_survey.c

# cppSRCS_$(d) are the C++ sources we want compiled.
#
# We can't call our main file main.cpp, or libmaple's build system
# will get confused and try to build it without our CXXFLAGS. So call
# it something else. Annoying! Hopefully LeafLabs will fix it soon.
cppSRCS_$(d) := n3_main.cpp
cppSRCS_$(d) += n3_globals.cpp
cppSRCS_$(d) += n3_bat.cpp
cppSRCS_$(d) += n3_gps.cpp
cppSRCS_$(d) += n3_btn.cpp
cppSRCS_$(d) += n3_sd_log.cpp
cppSRCS_$(d) += n3_sleep.cpp

cppSRCS_$(d) += n3_wgs84_to_os.cpp

cppSRCS_$(d) += n3_ui.cpp
cppSRCS_$(d) += n3_ui_about.cpp
cppSRCS_$(d) += n3_ui_satellites.cpp
cppSRCS_$(d) += n3_ui_altitude.cpp
cppSRCS_$(d) += n3_ui_grid_ref.cpp
cppSRCS_$(d) += n3_ui_lat_lon.cpp
cppSRCS_$(d) += n3_ui_datetime.cpp
cppSRCS_$(d) += n3_ui_no_fix.cpp
cppSRCS_$(d) += n3_ui_sd_log.cpp

# Tiny GPS
cppSRCS_$(d) += TinyGPS/TinyGPS.cpp

# maple_sdfat
cppSRCS_$(d) += maple_sdfat/Sd2Card.cpp
cppSRCS_$(d) += maple_sdfat/SdFile.cpp
cppSRCS_$(d) += maple_sdfat/SdVolume.cpp

# sSRCS_$(d) are the assembly sources. We don't have any.
sSRCS_$(d) :=

###############################################################################

# Include the libmaple rules.mk footer. Leave this line alone.
include $(MAKEDIR)/footer.mk
