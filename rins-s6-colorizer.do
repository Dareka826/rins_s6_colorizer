redo-ifchange config.sh
. ./config.sh

OBJ="main.o"
redo-ifchange $OBJ

gcc $LDFLAGS $OBJ -o $3
