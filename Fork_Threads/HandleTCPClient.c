#include <stdio.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()
#include <cstring>

#include "Auxiliary.h"
#include "HandleTCPClient.h"

#define RCVBUFSIZE 32   // Size of receive buffer

void reverseCase (char * input){
    for(int i=0;input[i];i++)
    {
        if((input[i] > 64)&&(input[i] < 91))    // check if in range of upper case characters
        {
            input[i] += 32;                     // is upper, offset 32 to make lower
        }
        else if((input[i] > 96)&&(input[i] < 123)) // check if in range of lower case characters
        {
            input[i] -= 32;                     // is lower, offset -32 to make upper
        }
    }
}

void HandleTCPClient (int clntSocket)
{
    // 'clntSocket' is obtained from AcceptTCPConnection()

    char echoBuffer[RCVBUFSIZE];        // Buffer for echo string
    int  recvMsgSize;                   // Size of received message

    // Receive message from client
    recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
    if (recvMsgSize < 0)
    {
        DieWithError ("recv() failed");
    }

    // Send received string and receive again until end of transmission
    while (recvMsgSize > 0)      // zero indicates end of transmission
    {
        printf("Handler received string: %s \n", echoBuffer);
        
        reverseCase(&echoBuffer[0]);
        
        delaying ();
        
        // Echo message back to client
        if (send (clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
        {
            DieWithError ("send() failed");
        }

        info_s("Handler sent string: ", echoBuffer);

        // receive next string
        recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
        if (recvMsgSize < 0)
        {
            DieWithError ("recv() failed");
        }
    }

    close (clntSocket);
    info ("close");
}
