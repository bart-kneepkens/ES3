CFLAGS=-O2 -Wall -Werror -lusb-1.0

USB_SOURCE=cgi.c

GCC=gcc

usb: 
	$(GCC) $(CFLAGS) $(USB_SOURCE) -o cgi.cgi

clean:
	@rm -rf cgi.cgi