redo-ifchange config.sh
. ./config.sh

redo-ifchange main.c

gcc -DEXEC_DEBUG $CFLAGS -MD -MF $2.d main.c -c -o $3

read DEPS <$2.d
redo-ifchange ${DEPS#*:}
