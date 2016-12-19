CFLAGS=-O2 -Wall -Werror 

USB_SOURCE=cgi.c

GCC=g++

usb: 
	$(GCC) $(CFLAGS) $(USB_SOURCE) -o cgi.cgi -lrt

clean:
	@rm -rf cgi.cgi