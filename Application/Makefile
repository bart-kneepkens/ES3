CFLAGS=-O2 -Wall -Werror -lusb-1.0

DEAMON_SOURCE=daemon/usb-daemon.c
MOCK_DAEMON_SOURCE=daemon/MockDaemon.cpp
CGI_SOURCE=cgi/cgi.cpp

ARM-GCC=arm-linux-gcc
GPP=g++

.PHONY: cgi

cgi: 
	$(GPP) -O2 -Wall $(CGI_SOURCE) -o cgi/cgi.cgi -lrt
	
mockdaemon:
	$(GPP) $(MOCK_DAEMON_SOURCE) -o daemon/mock -lrt

daemon:
	$(ARM-GCC) $(CFLAGS) $(DEAMON_SOURCE) -o daemon/usb-daemon

clean:
	@rm -rf daemon/usb-daemon cgi/cgi.cgi daemon/mock