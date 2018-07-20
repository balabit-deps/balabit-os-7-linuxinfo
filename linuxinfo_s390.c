/*
        linuxinfo_s390.c

        Copyright (C) 1998-2000
        All Rights Reserved.

        Alex Buell <alex.buell@munted.eu>

        Advanced Buell Software Engineering Ltd
        Hampshire, GU31 5DG
        United Kingdom

	Copyright (C) 2002 
	Gerhard Tonn <gt@debian.org>

	Copyright (C) 2003
	Ola Lundqvist <opal@debian.org>

	Copyright (C) 2005 
	Helge Kreutzmann <debian@helgefjell.de>


        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   AIB     199803??        Initial development
	1.0.1	AIB	20000405	Rewritten
	1.0.2	KRE	20051118        Include <string.h>
	1.0.3	KRE	20060315	Fix memory reporting

        This is the S/390 port of linuxinfo
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "linuxinfo.h"

#ifdef system_s390

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
	LONGLONG memory = 0;
	int processors = 0; 
	int megahertz = 0;
	float tempbogo;

	char temp_string[BUFSIZ], temp_string2[BUFSIZ];
	char chip[BUFSIZ], vendor[BUFSIZ], model[BUFSIZ];

	struct stat st_buf;

	while (read_line(fd, temp_string, BUFSIZ) != 0)
	{
		splitstring(temp_string, temp_string2);

		if (!(strncmp(temp_string, "vendor_id", strlen("vendor_id"))))
			strcpy(vendor, temp_string2);

		if (!(strncmp(temp_string, "# processors", strlen("# processors"))))
			processors = atoi(temp_string2);

		if (!(strncmp(temp_string, "bogomips per cpu", strlen("bogomips per cp"))))
		{
			sscanf(temp_string2, "%f", &tempbogo);
		}
	}

	memory = getphysicalmemory();
	sprintf(hw->hw_memory, "%ld", memory);
	hw->hw_processors = processors;

	sprintf(hw->hw_cpuinfo, "%s", vendor);
	if (tempbogo == 0.0)
	    sprintf(hw->hw_bogomips, "?");
	else
	    sprintf(hw->hw_bogomips, "%0.2f",tempbogo * processors);

}
#endif /* system_s390 */
