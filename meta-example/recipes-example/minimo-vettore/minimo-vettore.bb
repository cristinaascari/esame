DESCRIPTION = "Dato un vettore ne ritorna l'elemento minimo"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://minimo-vettore.c"

S = "${WORKDIR}"

do_compile() {
	${CC} minimo-vettore.c ${LDFLAGS} -o minimovettore
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 minimovettore ${D}${bindir}
}
