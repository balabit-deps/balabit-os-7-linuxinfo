/*
	linuxinfo_arm.c

        Copyright (C) 1998-2000
        All Rights Reserved.

        Alex Buell <alex.buell@munted.eu>

        Copyright (C) 2005
        Helge Kreutzmann <debian@helgefjell.de>

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   AIB     199803??        Initial development
	1.0.1	AIB	20000405	Rewritten
	1.0.2	AIB	20010909	Added getphysicalmemory() call
	1.0.3	KRE	20051118	Include <string.h>

	This is the ARM port of linuxinfo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linuxinfo.h"

#ifdef system_arm

static char *implementer[] =
{
    "0x41", "ARM",
    "0x50", "APM",
    NULL, NULL
};

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
	int processors = 0; 
	float bogomips = 0.0;
	float tempMHz = 0.0;

	char temp_string[BUFSIZ], temp_string2[BUFSIZ];
	char chip[BUFSIZ], vendor[BUFSIZ], model[BUFSIZ], Mhz[BUFSIZ];

	strcpy(chip, "Unknown");
	strcpy(vendor, "Unknown");
	strcpy(model, "Unknown");

	char *p;

	while (read_line(fd, temp_string, BUFSIZ) != 0)
	{
		if (splitstring(temp_string, temp_string2))
		{
			if (strncmp(temp_string, "Processor", strlen("Processor")) == 0)
			{
			    // This branch is historic (??)
				if (strncmp(temp_string2, "Intel sa110", strlen("Intel sa110")) == 0)
				{
					strcpy(vendor, "Intel");
					strcpy(model, "SA110");
				}
				processors++;
			}

			if (strncmp(temp_string, "CPU implementer", strlen("CPU implementer")) == 0)
                        {
			    int i = 0;
			    int found = 0;

                            while ((implementer[i] != NULL)&&(!found))
			    {
				if (strncmp(temp_string2, implementer[i], strlen(implementer[i])) == 0)
                                {
				    strcpy(vendor, implementer[++i]);
		                    found=1;
				    processors++;
				}
				i += 2;

			    }
			}

			if (strncmp(temp_string, "CPU architecture", strlen("CPU architecture")) == 0)
                        {
			    strcpy(model,temp_string2); // v .. instruction set
			}

			if (strncmp(temp_string, "Type", strlen("Type")) == 0)
				processors++;

			if (strncmp(temp_string, "cpu MHz", strlen("cpu MHz")) == 0)
				tempMHz = atol(temp_string2);

			if (strncmp(temp_string, "BogoMips", strlen("BogoMips")) == 0)
				bogomips = bogomips + atof(temp_string2);
		}

		
	}

	sprintf(hw->hw_memory, LONGSPEC, getphysicalmemory());
	hw->hw_processors = processors;

	// sprintf(hw->hw_cpuinfo, "%s %s", vendor, model);
	sprintf(hw->hw_cpuinfo, "%s", vendor);

        if (strcmp(model, "Unknown") == 0)
        {
	    sprintf(hw->hw_cpuinfo, "%s", vendor);
	}
	else
	{
	    sprintf(hw->hw_cpuinfo, "%s v%s instruction set", vendor, model);
	}


	if (bogomips == 0.0)
	    sprintf(hw->hw_bogomips, "?");
	else
	    sprintf(hw->hw_bogomips, "%0.2f", bogomips);

	if (tempMHz == 0.0)
		strcpy(hw->hw_megahertz, "?");
	else
		sprintf(hw->hw_megahertz, "%d", (int)tempMHz);
}

#endif /* system_arm */
