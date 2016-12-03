#include <pthread.h>

#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"
#include <unistd.h>     // for close()

static void * myThread (void * arg);            // thread that does the work

int main (int argc, char *argv[])
{
    int         servSock;     // Socket descriptor for server
    int         clntSock;     // Socket descriptor for client
    pthread_t   threadID;     // Thread ID from pthread_create()
    bool        to_quit = false;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    while (!to_quit)                // run until someone indicates to quit...
    {
        clntSock = AcceptTCPConnection (servSock);
        
        if(pthread_create(&threadID, NULL, myThread, &clntSock) != 0){
            info_d("Error creating thread. Exiting.", clntSock);
            to_quit = true;
        }
    }
    
    close(servSock);
    
	return (0);
}

static void * myThread (void * threadArgs)
{
    int * clntSock_ptr = (int *) threadArgs;
    info_d("Thread started with clntSock: ", *clntSock_ptr);
    HandleTCPClient(*clntSock_ptr);
    pthread_detach(pthread_self());
    return (NULL);
}
