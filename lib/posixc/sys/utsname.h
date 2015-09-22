#ifndef _POSIXUTSNAME_H
#define _POSIXUTSNAME_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct utsname
{
   char sysname[33];    /* Operating system name (e.g., "Linux") */
   char nodename[33];   /* Name within "some implementation-defined
                         network" */
   char release[33];    /* Operating system release (e.g., "2.6.28") */
   char version[33];    /* Operating system version */
   char machine[33];    /* Hardware identifier */
#ifdef _GNU_SOURCE
   char domainname[]; /* NIS or YP domain name */
#endif
} utsname_t;

int uname(utsname_t* buf);

#ifdef __cplusplus
}
#endif
#endif
