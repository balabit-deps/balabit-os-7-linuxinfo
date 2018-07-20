/*
	linuxinfo_common.c
	
        Copyright (C) 1998-2000
        All Rights Reserved.

        Alex Buell <alex.buell@munted.eu>

        Copyright (C) 2004-2007,2009,2010,2013
        Helge Kreutzmann <debian@helgefjell.de>

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   AIB     199803??        Initial development
	1.0.1	AIB	20000405	Added read_line function
	1.0.2	AIB	20000405	Moved strstr() from linuxinfo.h to here
	1.0.3	AIB	20010809	Added getphysicalmemory()
	1.0.4   KRE     20110104        Fix resource leak found by DACA/cppcheck

	Common functions module
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <libintl.h>
#define _(String) gettext (String)

#include "linuxinfo.h"

void GetOperatingSystemInfo(struct os_stat *os)
{
#ifndef system_unknown
	struct utsname buf;
	uname(&buf);

	strcpy(os->os_hostname, buf.nodename);
	strcpy(os->os_name, buf.sysname);
	strcpy(os->os_version, buf.release);
	strcpy(os->os_revision, buf.version);
#else
	strcpy(os->os_hostname, "Unknown");
	strcpy(os->os_name, "Unknown");
	strcpy(os->os_version, "Unknown");
	strcpy(os->os_revision, "Unknown");
#endif /* system_unknown */
}

/* Really neat hack to detect all libraries known on Linux */
#ifndef system_unknown
asm (".weak gnu_get_libc_version");
asm (".weak __libc_version");
asm (".weak __linux_C_lib_version");

extern char *gnu_get_libc_version (void);
extern char *__linux_C_lib_version;
extern char __libc_version [];
#endif /* only define if on a Linux system :o) */

void GetSystemLibcInfo(struct lib_stat *lib)
{
	int libc_major = 0, libc_minor = 0, libc_teeny = 0;
	char *ptr;

	/* initialise to unknown */
	strcpy(lib->lib_version, "Unknown");

#ifndef system_unknown
	if (gnu_get_libc_version != 0)
        {
		ptr = gnu_get_libc_version();
        }
        else if (&__libc_version != 0)
        {
		ptr = __libc_version;
        }
        else
        	ptr = __linux_C_lib_version;

        while (!isdigit (*ptr))
	        ptr++;

        sscanf (ptr, "%d.%d.%d", &libc_major, &libc_minor, &libc_teeny);
	sprintf(lib->lib_version, "%d.%d.%d", libc_major, libc_minor, libc_teeny);
#endif /* system_unknown */
}

int read_line(int fd, char *buffer, size_t length)
{
        off_t curpos = lseek(fd, 0, SEEK_CUR);
        int len = read(fd, buffer, length);
        char *p = strchr(buffer, 0x0a);

	if (fd < 0)
		return 0;

        if (len < 0)
                return len;

        if (p != NULL)
                len = (p - buffer) + 1;

        lseek(fd, curpos + len, SEEK_SET);

        if (p != NULL)
                *p = 0;

        return len;
}

int splitstring(char *first_string, char *second_string)
{
	char *p;

	p = strchr(first_string, ':');
	if (!p)
		return 0;

	*(p-1) = '\0', p++; 
	while (isspace(*p)) p++;
	strcpy(second_string, p);
}

LONGLONG getphysicalmemory(void)
{
	LONGLONG memory;
        struct stat st_buf;
	int meminfo_fd;
	char temp_string[BUFSIZ], temp_string2[BUFSIZ];
	int found;

	memory = 0;
        meminfo_fd = open(MEMINFO_FILE, O_RDONLY);
        found=0;
        if (meminfo_fd < 0)
	{
	    printf(_("Could not stat /proc/meminfo, result can be inaccurate\n"));
	    // printf(gettext("Could not stat /proc/meminfo, result can be inaccurate\n"));
        }
        else
        { while (read_line(meminfo_fd, temp_string, BUFSIZ) != 0)
	    {
		if (splitstring(temp_string, temp_string2))
		{
		    if ((strncmp(temp_string, "MemTota", strlen("MemTota")) == 0)&&!found)
		    {
		        found=1;
		        memory = (LONGLONG)atol(temp_string2);
		        memory /= 1024; 
		    }
		}
	    }
	    close(meminfo_fd);
	}
	return memory;
}


#if !(HAVE_STRSTR)
char *strstr (const char *haystack, const char *needle)
{
        c, sc;
        size_t len;

        if ((c = *find++) != 0)
        {
                len = strlen(find);
                do
                {
                        do
                        {
                                if ((sc = *s++) == 0)
                                        return (NULL);
                        }
                        while (sc != c);
                }
                while (strncmp(s, find, len) != 0);

                s--;
        }

        return ((char *)s);
}
#endif

