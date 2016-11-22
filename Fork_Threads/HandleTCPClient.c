#include <stdio.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "HandleTCPClient.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

void HandleTCPClient (int clntSocket)
{
    // 'clntSocket' is obtained from AcceptTCPConnection()

    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int  recvMsgSize;                   /* Size of received message */

    /* Receive message from client */
    recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
    if (recvMsgSize < 0)
    {
        DieWithError ("recv() failed");
    }
    info_d ("Recv", recvMsgSize);

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        // TODO: add code to print the received string; use printf()
        
        // TODO: add code to convert the upper/lower chars of the received string
        
        delaying ();
        
        /* Echo message back to client */
        if (send (clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
        {
            DieWithError ("send() failed");
        }

        // TODO: add code to display the transmitted string in verbose mode; use info_s()

        // receive next string
        recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
        if (recvMsgSize < 0)
        {
            DieWithError ("recv() failed");
        }
        info_d ("recv", recvMsgSize);
    }

    close (clntSocket);    /* Close client socket */
    info ("close");
}
