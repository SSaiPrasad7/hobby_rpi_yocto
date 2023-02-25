#Package Summary
SUMMARY = ""

# Decription about the receipe
DESCRIPTION = "Basic character driver"

# License, for example GPLv2
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"

inherit module

# Source files
SRC_URI = "file://chardrv_mod.c \
           file://chardrv_wq_mod.c \
           file://chardrv_poll_mod.c \
           file://chardrv_asyncio_mod.c \
           file://Makefile \
           file://COPYING \
          "

S = "${WORKDIR}"