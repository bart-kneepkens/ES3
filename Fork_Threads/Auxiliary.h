#ifndef _AUXILIARY_H_
#define _AUXILIARY_H_

#include <stdbool.h>    /* for bool,true,false */

extern char *           argv_ip;
extern unsigned short   argv_port;
extern int              argv_timeout;
extern int              argv_forkmax;
//extern char *           argv_tty;
//extern bool             argv_verbose;
//extern bool             argv_debug;
//extern bool             argv_delay;
//extern bool             argv_userprefix;
extern int              argv_nrofdata;
extern char *           argv_data[];

extern void delaying (void);
extern void DieWithError(const char * errorMessage);  /* Error handling function */
extern void info (const char * s);
extern void info_d (const char * s, int d);
extern void info_x (const char * s, unsigned int d);
extern void info_s (const char * s, const char * d);
extern void info_peer (const char * s);
extern void info_local (const char * s);
extern void info_set_output (const char * name);
extern void info_set_local_peer (int sock);
extern char * remove_nl (char * s);
extern void parse_args (int argc, char * argv[]);

#endif
