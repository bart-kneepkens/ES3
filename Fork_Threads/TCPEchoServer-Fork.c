#include <stdlib.h>     // for atoi() and exit()
#include <unistd.h>     // for fork()
#include <sys/wait.h>   // for waitpid()

#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"

int main(int argc, char *argv[])
{
    int     servSock;                  /* Socket descriptor for server */
    int     clntSock;                  /* Socket descriptor for client */
    pid_t   processID;                 /* Process ID from fork() */
    bool    to_quit = false;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    while (to_quit == false)                /* run until someone indicates to quit... */
    {
        clntSock = AcceptTCPConnection (servSock);

        // TODO: write the code to realise the following:
        //
        // fork() a new process and:
        // * check if the fork() gave problems
        //   in this case, you must end the process (fatal error...)
        // * check if you are the child process
        //   in this case you have to:
        //   - handle the communication with the client (use HandleTCPClient())
        //   - and end the process
        // * check if you are the parent
        //   in this case you have to:
        //   - wait for the next client (again in the while-loop)
		//
		// make sure that clntSock and servSock are closed at the correct locations 
        // (in particular: at those places where you don't need them any more)
		//
		// Hint: use the info(), info_d(), info_s() operations to trace what happens
    }
    
    // server stops...
	return (0);
}
