#include <stdio.h>
#include <stdlib.h>     // for atoi() and exit()
#include <string.h>     // for strlen()
#include <arpa/inet.h>  // for sockaddr_in and inet_ntoa()
#include <pthread.h>    // for POSIX threads
#include <sys/socket.h> // for socket(), bind(), getsockname and connect()
#include <time.h>       // for time()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"

#define MAX_DATA       10

char *          argv_ip             = NULL;
unsigned short  argv_port           = 0;
int             argv_timeout        = 1;
char *          argv_tty            = NULL;
int             argv_forkmax        = 0;
bool            argv_verbose        = false;
bool            argv_delay          = false;
bool            argv_debug          = false;
bool            argv_userprefix     = false;
int             argv_nrofdata       = 0;
char *          argv_data[MAX_DATA] = { NULL };


static FILE *   tty_fptr = NULL;
static char     local_prefix [80] = { '\0' };
static char     peer_prefix [80]  = { '\0' };

void
delaying (void)
{
    if (argv_delay == true)
    {
        sleep (1);
    }
}

char * 
remove_nl (char * s)
{
    // remove the trailing nerwline in 's' (if present)
    int len;
    
    len = strlen (s);
    if (s [len - 1] == '\n')
    {
        s [len - 1] = '\0';
    }
    
    return (s);
}

void
DieWithError (const char * errorMessage)
{
    perror (errorMessage);
    exit (1);
}

void
info_set_tty (const char * name)
{
    char s[80];
    
    tty_fptr = fopen (name, "w");
    if (tty_fptr == NULL)
    {
        sprintf (s, "ERROR: unable to open '%s' for writing", name);
        perror (s);
    }
}

void 
info_set_local_peer (int sock)
{
    unsigned int        len;
    struct sockaddr_in  a;
    
    len = sizeof(a);
    if (getsockname(sock, (struct sockaddr *) &a, &len) != 0)
    {
        DieWithError ("ERROR in getsockname()");
    }
    if (argv_verbose == true)
    {
        printf ("sock:\n");
        printf ("    family: %d\n", a.sin_family);
        printf ("    addr:   %s\n", inet_ntoa(a.sin_addr));
        printf ("    port:   %d\n", ntohl (a.sin_port));
    }
    
    sprintf (local_prefix, "LOCAL %s(%05d)", inet_ntoa(a.sin_addr), ntohl (a.sin_port));
    
    len = sizeof(a);
    if (getpeername(sock, (struct sockaddr *) &a, &len) != 0)
    {
        //perror ("getpeername()");
        DieWithError ("ERROR in getpeername()");
    }
    if (argv_verbose == true)
    {
        printf ("peer:\n");
        printf ("    family: %d\n", a.sin_family);
        printf ("    addr:   %s\n", inet_ntoa(a.sin_addr));
        printf ("    port:   %d\n", ntohl (a.sin_port));
    }
    
    sprintf (peer_prefix, "PEER  %s(%05d)", inet_ntoa(a.sin_addr), ntohl (a.sin_port));
}

void 
info (const char * s)
{
    struct timespec     ts;
    
    if (argv_verbose == true)
    {
        clock_gettime(CLOCK_REALTIME, &ts);
        fprintf (stdout, "(%d,%lx) %3ld.%06ld %s\n", 
            getpid(), 
            pthread_self(), 
            ts.tv_sec%1000, 
            ts.tv_nsec/1000, s);
    }
}

static void
info_user (const char * prefix, const char * s)
{
    struct tm   ti;
    time_t      t;
    
    if (tty_fptr == NULL)
    {
        tty_fptr = stdout;
    }
    
    if (argv_userprefix == true)
    {
        
        if (time (&t) == -1)
        {
            DieWithError ("time()");
        }
        if (localtime_r (&t, &ti) == NULL)
        {
            DieWithError ("localtime()");
        }
        
        fprintf (tty_fptr, "%02d:%02d:%02d %s:> ",
            ti.tm_hour, 
            ti.tm_min, 
            ti.tm_sec, 
            prefix);
    }
    fprintf (tty_fptr, "%s", s);
}

void 
info_local (const char * s)
{
    info_user (local_prefix, s);
}

void 
info_peer (const char * s)
{
    info_user (peer_prefix, s);
}

void
info_d (const char * s, int i)
{
    char    str[80];
    
    sprintf (str, "%s: %d", s, i);
    info (str);
}

void
info_x (const char * s, unsigned int i)
{
    char    str[80];
    
    sprintf (str, "%s: %x", s, i);
    info (str);
}

void
info_s (const char * s, const char * i)
{
    char    str[80];
    
    sprintf (str, "%s '%s'", s, i);
    info (str);
}

void parse_args (int argc, char *argv[])
{
    int opt;
    int i;
    
    while ((opt = getopt(argc, argv, "i:t:p:y:f:uvdgh")) != -1)
    {
        switch (opt)
        {
            case 'i':
                argv_ip = optarg;
                break;
            case 'y':
                argv_tty = optarg;
                info_set_tty (argv_tty);
                break;
            case 'p':
                argv_port = atoi(optarg);
                break;
            case 't':
                argv_timeout = atoi(optarg);
                break;
            case 'f':
                argv_forkmax = atoi(optarg);
                break;
            case 'v':
                argv_verbose = true;
                break;
            case 'u':
                argv_userprefix = true;
                break;
            case 'd':
                argv_delay = true;
                break;
            case 'g':
                argv_debug = true;
                break;
            case '?':
            case 'h':
                printf("\noptions: \n"
                    "    -i <ip> \n"
                    "    -y <tty-name> \n"
                    "    -t <timeout> \n"
                    "    -p <port> \n"
                    "    -f <fork-max>\n"
                    "    -d         delay operation\n"
                    "    -g         debug info\n"
                    "    -u         user prefix\n"
                    "    -v         verbose\n"
                    "    <data>*\n"
                    "\n");
                exit(0);
                break;
            default:
                fprintf(stderr, "ERROR: unknown option '%c'\n", opt);
                exit(1);
                break;
        }
    }
    
    for (i = 0; (i < MAX_DATA) && (optind < argc); i++, optind++)
    {
        argv_data[i] = argv [optind];
    }
    argv_nrofdata = i;
        
    if (argv_debug == true)
    {
        printf ("\ncompiler version: %s\n\n", __VERSION__);
        printf ("user settings:\n"
                "    ip:        %s\n"
                "    port:      %d\n"
                "    tty:       %s\n"
                "    timeout:   %d\n"
                "    verbose:   %s\n"
                "    delay:     %s\n"
                "    debug:     %s\n"
                "    userprefix:%s\n"
                "    data(%d):   ",
                argv_ip, argv_port, argv_tty, argv_timeout, 
                argv_verbose?"true":"false",
                argv_delay?"true":"false",
                argv_debug?"true":"false",
                argv_userprefix?"true":"false",
                argv_nrofdata);
        for (i = 0; i < argv_nrofdata; i++)
        {
            printf ("'%s' ", argv_data [i]);
        }
        printf ("\n");
    }
}

