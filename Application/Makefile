CFLAGS=-O2 -Wall -Werror -lusb-1.0

DEAMON_SOURCE=daemon/usb-daemon.c

ARM-GCC=arm-linux-gcc

usb: 
	$(ARM-GCC) $(CFLAGS) $(DEAMON_SOURCE) -o daemon/usb-daemon

clean:
	@rm -rf daemon/usb-daemon