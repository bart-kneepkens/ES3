#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */

#include "Auxiliary.h"
#include "AcceptTCPConnection.h"

int AcceptTCPConnection (int servSock)
{
    // 'servSock' is obtained from CreateTCPServerSocket()

    int                 clntSock;     /* Socket descriptor for client */
    struct sockaddr_in  echoClntAddr; /* Client address */
    unsigned int        clntLen;      /* Length of client address data structure */

    /* Set the size of the in-out parameter */
    clntLen = sizeof (echoClntAddr);
    
    /* Wait for a client to connect */
    clntSock = accept (servSock, (struct sockaddr *) &echoClntAddr, &clntLen);
    if (clntSock < 0)
    {
        DieWithError ("accept() failed");
    }
    /* clntSock is connected to a client! */
    
    info_s ("accept", inet_ntoa (echoClntAddr.sin_addr));
    info_set_local_peer (clntSock);
    
    return (clntSock);
}
