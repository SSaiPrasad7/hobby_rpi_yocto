SUMMARY = "GPIO driver for RPI"
LICENSE = "MIT"

# License, for example GPLv2
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"

inherit module

# Source files
SRC_URI = "file://gpio_drv.c \
           file://Makefile \
           file://COPYING \
          "

S = "${WORKDIR}"
