#include <stdio.h>
#include <string.h>     // for strlen()
#include <stdlib.h>     // for exit()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "CreateTCPClientSocket.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

int main (int argc, char *argv[])
{
    int         sock;                   /* Socket descriptor */
    int onGoing = 1;

    parse_args (argc, argv);

    sock = CreateTCPClientSocket (argv_ip, argv_port);
    
    printf("%s", "Conversation has started. \n\n");
    
    while(onGoing){
        
        char message[RCVBUFSIZE];
        
        fgets(message, RCVBUFSIZE, stdin);
        
        if(strcmp(message, "/quit\n") == 0){
            printf("%s", "EXITED");
            onGoing = 0;
            send(sock, message, RCVBUFSIZE - 1, 0);
            break;
        }
        
        printf("YOU: %s \n", message);
        
        message[RCVBUFSIZE] = '\0';
        
        send(sock, message, RCVBUFSIZE - 1, 0);
        
        char reply[RCVBUFSIZE];
        
        recv(sock, reply, RCVBUFSIZE - 1, 0);
        
        printf("PEER: %s \n", reply);
    }

    delaying ();

    close (sock);
    info ("close & exit");
    exit (0);
}
