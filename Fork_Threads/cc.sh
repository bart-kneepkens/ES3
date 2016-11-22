#!/bin/bash

#CCC="i586-linux-g++"
CCC="g++"
FLAGS="-Wall -W -Wshadow -Wcast-qual -Wwrite-strings"

for i in $(ls *.c); do
    echo `basename $i .c`.o
    $CCC -o `basename $i .c`.o -g3 -c $FLAGS $i || exit
done

echo

for i in $(ls T*.o); do
    echo `basename $i .o`
    $CCC -o `basename $i .o` $i A*.o C*.o  H*.o -lrt $FLAGS || exit
done


