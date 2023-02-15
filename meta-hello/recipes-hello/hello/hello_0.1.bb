SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"

inherit module

SRC_URI = "file://hello_mod.c \
           file://Makefile \
           file://COPYING \
           "
S = "${WORKDIR}"