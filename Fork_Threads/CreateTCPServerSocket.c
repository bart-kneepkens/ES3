#include <memory.h>     // for memset()
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */

#include "Auxiliary.h"
#include "CreateTCPServerSocket.h"

#define MAXPENDING 5    /* Maximum outstanding connection requests */

int CreateTCPServerSocket (unsigned short port)
{
    int                 sock;         /* socket to create */
    struct sockaddr_in  echoServAddr; /* Local address */

    /* Create socket for incoming connections */
    if ((sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        DieWithError ("socket() failed");
    }
    info ("socket");
      
    /* Construct local address structure */
    memset( &echoServAddr, 0, sizeof(echoServAddr));  /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(port);              /* Local port */

    /* Bind to the local address */
    if (bind (sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
    {
        DieWithError ("bind() failed");
    }
    info_d ("bind", port);

    /* Mark the socket so it will listen for incoming connections */
    if (listen (sock, MAXPENDING) < 0)
    {
        DieWithError ("listen() failed");
    }
    info ("listen");

    return (sock);
}
