#include <stdio.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()
#include <cstring>

#include "Auxiliary.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */


/* This function waits for the first message to be received, then starts the chat. */
void HandleTCPClientChat (int clntSocket)
{
    char message[RCVBUFSIZE];        /* Buffer for message string */
    int recvMsgSize;                   /* Size of received message */
    int onGoing = 1;
    
    recvMsgSize = recv(clntSocket, message, RCVBUFSIZE - 1, 0);
    
    if(recvMsgSize > 0){
        
        printf("%s", "Conversation has started. \n\n");
        
        while(strcmp(message, "/quit\n") != 0){
        
            printf("PEER: %s \n", message);
            
            fgets(message, RCVBUFSIZE, stdin);
            
            printf("YOU: %s \n", message);
            
            send(clntSocket, message, RCVBUFSIZE - 1, 0);
            
            recv(clntSocket, message, RCVBUFSIZE - 1, 0);
        }
        
        printf("%s", "EXITED");
        send(clntSocket, message, RCVBUFSIZE - 1, 0);

    }
    
    close (clntSocket);    /* Close client socket */
    info ("close");
}
