PGM=${ARCH}/dummy_drv.exe

# optional managers required
MANAGERS=all

# C source names, if any, go here -- minus the .c
CSRCS = init.c dummy_drv.c
COBJS = $(CSRCS:%.c=${ARCH}/%.o)
OBJS=$(COBJS)
CLEAN_ADDITIONS = rtems.img *~

include $(RTEMS_MAKEFILE_PATH)/Makefile.inc
include $(RTEMS_CUSTOM)
include $(PROJECT_ROOT)/make/leaf.cfg

all:	${ARCH} $(PGM)

${PGM}: ${OBJS}
	$(make-exe)

clean:
	rm -rf *~
