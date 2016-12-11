CFLAGS=-O2 -Wall -Wno-write-strings

GCC=g++

all: A B

A: A.c
	$(GCC) $(CFLAGS) -o a A.c -lrt -lpthread

B: B.c
	$(GCC) $(CFLAGS) -o b B.c -lrt -lpthread

clean:
	@rm -rf a b
