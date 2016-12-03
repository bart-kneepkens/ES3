#include <stdlib.h>     // for atoi() and exit()
#include <unistd.h>     // for fork()
#include <sys/wait.h>   // for waitpid()

#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"

int main(int argc, char *argv[])
{
    int     servSock;                  // Socket descriptor for server 
    int     clntSock;                  // Socket descriptor for client
    pid_t   processID;                 // Process ID from fork()
    bool    to_quit = false;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    while (!to_quit)                // run until someone indicates to quit...
    {
        clntSock = AcceptTCPConnection(servSock);
        
        processID = fork();
        
        if(processID == 0){                     // Child process
            info("Child Process : 0");
            HandleTCPClient(clntSock);
        }
        else if (processID > 0){                // Parent process
            // Do nothing, move on to the next loop
            info_d("Parent Process : ", processID);
            continue;
        }
        else {
            // The PID must be < 0 at this point
            // This indicates error
            info_d("Fatal Error: Cant fork!", processID);
            to_quit = true;
        }
    }
    
    close(servSock);
    
	return (0);
}
