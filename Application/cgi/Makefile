CFLAGS=-O2 -Wall -Werror 

USB_SOURCE=cgi.c

GAMECONTROLLER_SOURCE=../GameController.h

GCC=g++

usb: 
	$(GCC) $(CFLAGS) $(USB_SOURCE) $(GAMECONTROLLER_SOURCE) -o cgi.cgi -lrt

clean:
	@rm -rf cgi.cgi