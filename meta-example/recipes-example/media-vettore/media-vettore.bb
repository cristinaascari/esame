DESCRIPTION = "Dato un vettore si calcola la media dei suoi elementi"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://media-vettore.c"

S = "${WORKDIR}"

do_compile() {
	${CC} media-vettore.c ${LDFLAGS} -o mediavettore
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 mediavettore ${D}${bindir}
}
