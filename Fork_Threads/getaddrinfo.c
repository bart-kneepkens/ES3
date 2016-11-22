#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#ifndef   NI_MAXHOST
#define   NI_MAXHOST 1025
#endif // NI_MAX_HOST

#define E(e)  { #e, e }

typedef struct
{
    const char *    name;
    int             nr;
} ENUM_t;

static ENUM_t   socket_enum[] = { E(SOCK_STREAM), E(SOCK_DGRAM), E(SOCK_RAW), E(-1) };
static ENUM_t   family_enum[] = { E(PF_INET), E(-1) };

static char *
print_enum (int val, ENUM_t lut[])
{
    int i = 0;
    static char str[80];
    
    while (1)
    {
        if ((lut[i].nr == -1) && (strcmp (lut[i].name, "-1") == 0))
        {
            sprintf (str, "unknown [%d,%#x]", val, val);
            return (str);
        }
        if (lut[i].nr == val)
        {
            sprintf (str, "%d %s", lut[i].nr, lut[i].name);
            return (str);
        }
        i++;
    }
}


static void
print_protocol (int n)
{
    struct protoent *   p;
    char **             a;
    
    p = getprotobynumber (n);
    printf ("ai_protocol:  %d %s", p->p_proto, p->p_name);
    if (p->p_aliases != NULL)
    {
        printf (" [aliases:");
        a = p->p_aliases;
        while ((a != NULL) && (*a != NULL))
        {
            printf (" '%s'", *a);
            a++;
        }
        printf ("]");
    }
    printf ("\n");
}


static void
print_sockaddr_in (struct sockaddr_in * a)
{
    printf ("  sin_family: %s\n", print_enum (a->sin_family, family_enum));
    printf ("  sin_addr:   %s\n", inet_ntoa (a->sin_addr));
    printf ("  sin_port:   %d\n", ntohl (a->sin_port));
}

int main(int argc, char * argv[])
{
    struct addrinfo *   result;
    size_t              hostname_len = NI_MAXHOST;
    char                hostname[NI_MAXHOST];
    int                 error;

    if (argc != 2)
    {
        fprintf (stderr, "Usage: %s <URL>\n", argv[0]);
        exit (1);
    }
        
    error = getaddrinfo(argv[1], NULL, NULL, &result);
    if (error != 0)
    {
        fprintf(stderr, "error using getaddrinfo: %s\n", gai_strerror(error));
        exit (1);
    }

    while (result != NULL)
    {
        printf ("ai_flags:     %d\n", result->ai_flags);
        printf ("ai_family:    %s\n", print_enum (result->ai_family, family_enum));
        printf ("ai_socktype:  %s\n", print_enum (result->ai_socktype, socket_enum));
        //printf ("ai_protocol: %d\n", result->ai_protocol);
        print_protocol (result->ai_protocol);
        print_sockaddr_in ((struct sockaddr_in *) result->ai_addr);
        printf ("ai_canonname: '%s'\n", result->ai_canonname);

        error = getnameinfo(result->ai_addr, result->ai_addrlen, hostname, hostname_len, NULL, 0, 0);
        if (error != 0)
        {
            fprintf(stderr, "error using getaddrinfo: %s\n", gai_strerror(error));
            break;
        }
        printf ("name        : '%s'\n", hostname);
 
        result = result->ai_next;
        printf ("\n");
    }
    
    // TODO: free!!!!!
    
    return (0);
}
