#include <stdio.h>
#include <string.h>     // for strlen()
#include <stdlib.h>     // for exit()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "CreateTCPClientSocket.h"

#define RCVBUFSIZE 32   // Size of receive buffer

int main (int argc, char *argv[])
{
    int sock;                   // Socket descriptor
    int onGoing = 1;            // Used for flow control

    parse_args (argc, argv);

    sock = CreateTCPClientSocket (argv_ip, argv_port);
    
    printf("%s", "Conversation has started. \n\n");
    
    while(onGoing){
        char message[RCVBUFSIZE];
        
        fgets(message, RCVBUFSIZE, stdin);          // Read the input string from terminal
        
        if(strcmp(message, "/quit\n") == 0){        // IF the entered input is '/quit'
            printf("%s", "EXITED");
            onGoing = 0;                            // Break out of the loop
            send(sock, message, RCVBUFSIZE - 1, 0); // Send to server so it knows
            break;
        }
        
        printf("YOU: %s \n", message);
        
        message[RCVBUFSIZE] = '\0';
        
        send(sock, message, RCVBUFSIZE - 1, 0);     // Send input to the server
        
        char reply[RCVBUFSIZE];
        
        recv(sock, reply, RCVBUFSIZE - 1, 0);       // Wait for the reply of the server
        
        printf("PEER: %s \n", reply);
    }

    delaying ();

    close (sock);
    info ("close & exit");
    exit (0);
}
