/*
        linuxinfo_m68k.c

        Copyright (C) 1998-2000
        All Rights Reserved.

        Alex Buell <alex.buell@munted.eu>

	Copyright (C) 2004,2005 
	Helge Kreutzmann <debian@helgefjell.de>

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   AIB     199803??        Initial development
	1.0.1	AIB	20000405	Rework of code
	1.0.2	AIB	20010909	Added getphysicalmemory() call
	1.0.3	KRE	20051118	Include <string.h>

        This is the m68k port of linuxinfo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linuxinfo.h"

#ifdef system_m68k

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
	int processors = 0; 
	float bogomips = 0.0;
	float tempbogo;

	char temp_string[BUFSIZ], temp_string2[BUFSIZ];
	char chip[BUFSIZ] = "Unknown";
	char megahertz[BUFSIZ]="?";
	char *p;

	FILE *procfile;

	while (read_line(fd, temp_string, BUFSIZ) != 0)
	{
		splitstring(temp_string, temp_string2);

		if (processors == 0)
                	if (!(strcmp(temp_string, "CP")))
				strcpy(chip, temp_string2);

		if (!(strcmp(temp_string, "Clockin")))
			strcpy(megahertz, temp_string2);

                if (!(strcmp(temp_string, "BogoMip")))
                {
                        processors++;
                        sscanf(temp_string2, "%f", &tempbogo);
                        bogomips += tempbogo;
                }
	}

	sprintf(hw->hw_memory, LONGSPEC, getphysicalmemory());
	hw->hw_processors = processors;

	sprintf(hw->hw_cpuinfo, "%s", chip);
	if (bogomips == 0.0)
	    sprintf(hw->hw_bogomips, "?");
	else
	    sprintf(hw->hw_bogomips, "%0.2f", bogomips);

	p = strchr(megahertz, 'M');
        if (p) *(p) = '\0';
	strcpy(hw->hw_megahertz, megahertz);
}
#endif /* system_m68k */
