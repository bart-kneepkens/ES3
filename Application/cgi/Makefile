CFLAGS=-O2 -Wall -Werror -lusb-1.0

USB_SOURCE=usb-daemon.c

GCC=arm-linux-gcc

usb: 
	$(GCC) $(CFLAGS) $(USB_SOURCE) -o usb-daemon

clean:
	@rm -rf usb-daemon