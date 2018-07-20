/*
        linuxinfo_ppc.c

        Copyright (C) 1998-2000
        All Rights Reserved.

        Alex Buell <alex.buell@munted.eu>

	Copyright (C) 2005,2016 
	Helge Kreutzmann <debian@helgefjell.de>

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   AIB     199803??        Initial development
	1.0.1	AIB	20000405	Rewritten
	1.0.2	AIB	20010909	Added getphysicalmemory() call
	1.0.3   KRE     20050328        Fix multicpu-detection without bogomips
	1.0.4   KRE     20051118        Include <string.h>
	1.0.5   KRE     20160918        Determine fastest CPU

        This is the PPC port of linuxinfo
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "linuxinfo.h"

#ifdef system_ppc

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
	int processors = 0; 
	int megahertz = 0;
	float tempbogo;

	char temp_string[BUFSIZ], temp_string2[BUFSIZ];
	char chip[BUFSIZ], vendor[BUFSIZ], model[BUFSIZ];

	while (read_line(fd, temp_string, BUFSIZ) != 0)
	{
		splitstring(temp_string, temp_string2);

		//if (!(strncmp(temp_string, "cpu", strlen("cpu"))) && (processors == 0))
		if (!(strncmp(temp_string, "cpu", strlen("cpu"))))
		{
			if (processors == 0) strcpy(chip, temp_string2);
			processors++;
		}

		if (!(strncmp(temp_string, "model", strlen("model"))))
			strcpy(vendor, temp_string2);

		if (!(strncmp(temp_string, "machine", strlen("machine"))))
			strcpy(vendor, temp_string2);

		if (!(strncmp(temp_string, "clock", strlen("clock"))))
		{
		    if (atoi(temp_string2) > megahertz)
		    {
			megahertz = atoi(temp_string2);
		    }
		}

		if (!(strncmp(temp_string, "bogomips", strlen("bogomips"))))
		{
			sscanf(temp_string2, "%f", &tempbogo);
		}
	}

	sprintf(hw->hw_memory, LONGSPEC, getphysicalmemory());
	hw->hw_processors = processors;

	if (megahertz > 0)
		sprintf(hw->hw_megahertz, "%d", megahertz);
	else
		strcpy(hw->hw_megahertz, "?");

	sprintf(hw->hw_cpuinfo, "%s %s", chip, vendor);
	if (tempbogo == 0.0)
	    sprintf(hw->hw_bogomips, "?");
	else
	    sprintf(hw->hw_bogomips, "%0.2f",tempbogo * processors);

}
#endif /* system_ppc */
