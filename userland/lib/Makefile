#
# Makefile for both static and dynamic PF_RING user libraries
#
#

#
# Installation directory
#
INSTDIR   = ${DESTDIR}//usr/local

#
# Search directories
#
INCLUDE   = -I../../kernel -I../../vPF_RING/guest/kernel -I../libpcap

#
# Zero ToolKit
#
ZERO_OBJS  = pfring_dna_bouncer.o pfring_mod_dna_cluster.o pfring_dna_cluster.o pfring_dna_vas.o pfring_dna_sal.o pfring_libzero.o hugepages_lib.o 

#
# DNA
#
DNA_OBJS  = pfring_mod_dna.o pfring_dna.o pfring_e1000e_dna.o pfring_e1000_dna.o pfring_ixgbe_dna.o pfring_igb_dna.o silicom_ts.o pfring_dna_utils.o 

#
# Virtual PF_RING
#
VIRTUAL_OBJS = pfring_mod_virtual.o 

#
# DAG
#
DAG_OBJS  = 

#
# Object files
#
OBJS      = pfring.o pfring_mod.o pfring_utils.o pfring_mod_stack.o pfring_mod_usring.o pfring_hw_filtering.o ${ZERO_OBJS} ${DNA_OBJS} ${VIRTUAL_OBJS} ${DAG_OBJS}

#
# C compiler and flags
#
#
# CROSS_COMPILE=arm-mv5sft-linux-gnueabi-
#
CC        = ${CROSS_COMPILE}gcc
CFLAGS    =   -Wall -fPIC ${INCLUDE} -D HAVE_ZERO -D HAVE_DNA -D HAVE_VIRTUAL  -D ENABLE_BPF   -O2 # -g
LDFLAGS   = -g -shared
SYSLIBS   = -lpthread 

# How to make an object file
%.o: %.c %.h Makefile
#	@echo "=*= making object $@ =*="
	${CC} ${CFLAGS} -c $< -o $@

#
# Main targets
#
STATICLIB  = libpfring.a
DYNAMICLIB = libpfring.so
TARGETS    = ${STATICLIB} ${DYNAMICLIB}
RING_H     = ../../kernel/linux/pf_ring.h

all: ${TARGETS}

${STATICLIB}: Makefile extract_zero_lib extract_dna_lib extract_virtual_lib ${OBJS} pfring.h ${RING_H}
	@echo "=*= making library $@ =*="
	ar rs $@ ${OBJS}
	ranlib $@

${DYNAMICLIB}: ${OBJS} extract_zero_lib extract_dna_lib extract_virtual_lib pfring.h ${RING_H} Makefile
	@echo "=*= making library $@ =*="
	${CC} ${LDFLAGS} ${OBJS} ${SYSLIBS} -o $@

extract_dna_lib:
	ar x libs/libpfring_dna_x86_64.a

extract_zero_lib:
	ar x libs/libpfring_zero_x86_64.a

extract_virtual_lib:
	ar x libs/libpfring_mod_virtual_x86_64.a

extract_userspace_ring_lib:
	ar x @USERSPACE_RING_LIB@

install: install-static install-shared

install-includes:
	mkdir -p ${INSTDIR}/include
	cp pfring.h ${INSTDIR}/include/

install-static: ${STATICLIB} install-includes
	mkdir -p ${INSTDIR}/lib
	cp ${STATICLIB} ${INSTDIR}/lib/

install-shared:	${DYNAMICLIB} install-includes
	mkdir -p ${INSTDIR}/lib
	cp ${DYNAMICLIB} ${INSTDIR}/lib/
	@if test "$(USER)" = "root"; then \
		ldconfig; \
	fi

clean:
	@rm -f ${TARGETS} *.o *~
