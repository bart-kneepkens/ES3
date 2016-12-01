#include <pthread.h>

#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"
#include <unistd.h>     // for close()

static void * myThread (void * arg);            /* thread that does the work */

int main (int argc, char *argv[])
{
    int         servSock;     /* Socket descriptor for server */
    int         clntSock;     /* Socket descriptor for client */
    pthread_t   threadID;     /* Thread ID from pthread_create() */
    bool        to_quit = false;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    while (!to_quit)                /* run until someone indicates to quit... */
    {
        clntSock = AcceptTCPConnection (servSock);

        // DONE: create&start the thread myThread() te creeeren
        // use the POSIX operation pthread_create()
        //
        // make sure that clntSock and servSock are closed at the correct locations 
        // (in particular: at those places where you don't need them any more)
        
        if(pthread_create(&threadID, NULL, myThread, &clntSock) != 0){
            info_d("Error creating thread. Exiting.", clntSock);
            to_quit = true;
        }
    }
    
    close(servSock);
    
    // server stops...
	return (0);
}

static void * myThread (void * threadArgs)
{
    // DONE: write the code to handle the client data
    // use operation HandleTCPClient()
    //  
    // Hint: use the info(), info_d(), info_s() operations to trace what happens
    //
    // Note: a call of pthread_detach() is obligatory
    
    int * clntSock_ptr = (int *) threadArgs;
    
    info_d("Thread started with clntSock: ", *clntSock_ptr);
    HandleTCPClient(*clntSock_ptr);
    pthread_detach(pthread_self());
    return (NULL);
}
