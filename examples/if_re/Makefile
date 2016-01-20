PROG=	example
NOMAN=	man, no man

# wireless
LDADD+=	-lrumpdev_pci_if_re -lrumpdev_miiphy -lrumpnet_config

# wpa_supplicant reads /dev/urandom
LDADD+=	-lrumpdev_rnd

.include "${.CURDIR}/../Makefile.inc"

.include <bsd.prog.mk>
