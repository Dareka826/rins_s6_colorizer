redo-ifchange config.sh
. ./config.sh

OBJ="utils.o colorizer.o main.o"
redo-ifchange $OBJ

gcc $LDFLAGS $OBJ -o $3
