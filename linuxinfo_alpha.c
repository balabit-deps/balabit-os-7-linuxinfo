/*
	linuxinfo_alpha.c

        Copyright (C) 1998-2000
        All Rights Reserved.

        Alex Buell <alex.buell@munted.eu>

	Copyright (C) 2004,2005
	Helge Kreutzmann <debian@helgefjell.de>

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   AIB     199803??        Initial development
	1.0.1	AIB	20000405	Now detects MHz, code rewritten
	1.0.2	AIB	20010809	Added getphysicalmemory() call
	1.0.3	KRE	20051118	Include <string.h>

        This is the Alpha port of linuxinfo

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linuxinfo.h"

#ifdef system_alpha

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
	int processors = 0; 
	float bogomips = 0.0;
	float megahertz = 0.0;
	float tempbogo;

	char temp_string[BUFSIZ], temp_string2[BUFSIZ];
	char chip[BUFSIZ], vendor[BUFSIZ], model[BUFSIZ] = "Unknown";

	while (read_line(fd, temp_string, BUFSIZ) != 0)
	{
		splitstring(temp_string, temp_string2);

		if (!(strncmp(temp_string, "cpu model", strlen("cpu model"))))
			strcpy(model, temp_string2);

		if (!(strncmp(temp_string, "BogoMIPS", strlen("BogoMIPS"))))
		{
			processors++;
			sscanf(temp_string2, "%f", &tempbogo);
			bogomips += tempbogo;	
		}

		if (!(strncmp(temp_string, "cycle frequency [Hz]", strlen("cycle frequency [Hz]"))))
		{
			sscanf(temp_string2, "%f", &megahertz);
			if (megahertz > 0)
				megahertz /= 1000000;

			if (megahertz != 0)
				sprintf(hw->hw_megahertz, "%.2f", megahertz);
			else
				strcpy(hw->hw_megahertz, "?");
		}
		if (!(strncmp(temp_string, "cpus active", strlen("cpus active"))))
		    {
			// FIXME Check which counting method really works
			// (and bogomips ok?)
			processors = atoi(temp_string2);
		    }
	}

	sprintf(hw->hw_memory, LONGSPEC, getphysicalmemory());
	hw->hw_processors = processors;
	
	sprintf(hw->hw_cpuinfo, "Alpha %s", model);
	if (bogomips == 0.0)
	    sprintf(hw->hw_bogomips, "?");
	else
	    sprintf(hw->hw_bogomips, "%0.2f", bogomips);

}

#endif /* system_alpha */
