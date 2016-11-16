CFLAGS=-O2 -Wall -Werror -lusb-1.0

USB_SOURCE=USB.c

GCC=arm-linux-gcc

usb: 
	$(GCC) $(CFLAGS) $(USB_SOURCE) -o xbox

clean:
	@rm -rf xbox