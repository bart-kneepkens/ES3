CC=gcc
CFLAGS=-O2 -Wall -Werror
LIBS=-lusb-1.0
OBJ = USB

$(OBJ) : $(OBJ).c \
    $(OBJ).c $(LIBS)
