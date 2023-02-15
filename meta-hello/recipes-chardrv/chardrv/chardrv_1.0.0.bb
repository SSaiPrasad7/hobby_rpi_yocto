#Package Summary
SUMMARY = "Character device driver"

# Decription about the receipe
DESCRIPTION = "Basic example for testing character driver with RPI"

# License, for example GPLv2
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=d41d8cd98f00b204e9800998ecf8427e"

inherit module

# Source files
SRC_URI = "file://chardrv_mod.c \
           file://Makefile \
           file://COPYING \
          "

S = "${WORKDIR}"

# Pass arguments to linker
#TARGET_CC_ARCH += "${LDFLAGS}"

# Cross-compile source code
#do_compile() {
#    ${CC} -o char_user char_user.c
#}

# Create /usr/bin in rootfs and copy program to it
#do_install() {
#    install -d ${D}${bindir}
#    install -m 0755 char_user ${D}${bindir}
#}