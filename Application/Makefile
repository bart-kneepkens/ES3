CFLAGS=-O2 -Wall -lusb-1.0

DEAMON_SOURCE=daemon/usb-daemon.cpp
MOCK_DAEMON_SOURCE=daemon/MockDaemon.cpp
CGI_SOURCE=cgi/cgi.cpp

ARM-GCC=arm-linux-g++
GPP=arm-linux-g++

.PHONY: cgi daemon

cgi: 
	$(GPP) -O2 -Wall $(CGI_SOURCE) -o cgi/cgi.cgi -lrt
	
mockdaemon:
	$(GPP) $(MOCK_DAEMON_SOURCE) -o daemon/mock -lrt -pthread

daemon:
	$(ARM-GCC) $(CFLAGS) $(DEAMON_SOURCE) -o daemon/usb-daemon -lrt -pthread

clean:
	@rm -rf daemon/usb-daemon cgi/cgi.cgi daemon/mock