#include <memory.h>     // for memset()
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */

#include "Auxiliary.h"
#include "CreateTCPClientSocket.h"

int CreateTCPClientSocket (const char * servIP, unsigned short port)
{
    int                 sock;         /* Socket descriptor */
    struct sockaddr_in  echoServAddr; /* Echo server address */

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        DieWithError("socket() failed");
    }
    info ("socket");
    delaying ();

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(port);         /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
    {
        DieWithError("connect() failed");
    }
    info ("connect");

    info_set_local_peer (sock);
    
    return (sock);
}
