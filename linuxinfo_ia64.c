/*
        linuxinfo_ia64.c

        Copyright (C) 1998-2000
        All Rights Reserved.

        Alex Buell <alex.buell@munted.eu>

        Advanced Buell Software Engineering Ltd
        Hampshire, GU31 5DG
        United Kingdom

	Copyright (C) 2002 
	John R. Daily <jdaily@progeny.com>

	Copyright (C) 2005,2016 
	Helge Kreutzmann <debian@helgefjell.de>

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   AIB     199803??        Initial development
	1.0.1	AIB	20000405	Updated and reworked code
	1.0.2	AIB	20000527	Addded AMD K7
        1.0.3   OPAL    20030417	Fixed so that it compiles.
	1.0.4   KRE     20050318        For now always use family as model (improve later)
	1.0.5   KRE     20051118        Include <string.h>
	1.0.6   KRE     20160918        Determine fastest CPU

        This is the Intel ia64 port of linuxinfo
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "linuxinfo.h"

#ifdef system_ia64

static char *vendors[] =
{
	"GenuineIntel", "Intel",
	NULL, NULL
};

static char *models[] =
{
        NULL, NULL
};

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
	char chip[BUFSIZ], vendor[BUFSIZ], model[BUFSIZ], Mhz[BUFSIZ], family[BUFSIZ];
	char temp_string[BUFSIZ], temp_string2[BUFSIZ];

	struct stat st_buf;

	int processors = 0; 
	float bogomips = 0.0;
	float tempMHz = 0.0;
	LONGLONG memory = 0;

	char *p;

	strcpy(chip, "Unknown");
	strcpy(vendor, "Unknown");
	strcpy(model, "Unknown");
	strcpy(family, "Unknown");

	while (read_line(fd, temp_string, BUFSIZ) != 0) 
	{
		if (splitstring(temp_string, temp_string2))
		{
			if (strncasecmp(temp_string, "processor", strlen("processor")) == 0)
				processors++;

			if (strncasecmp(temp_string, "vendor", strlen("vendor")) == 0)
			{
				int i = 0;

				while (vendors[i] != NULL)
				{
					if (strncasecmp(temp_string2, vendors[i], strlen(vendors[i])) == 0)
						strcpy(vendor, vendors[++i]);

					i += 2;
				}
			}

			if (strncasecmp(temp_string, "model", strlen("model")) == 0) 
			{
				int i = 0;
		
                                //if (strncmp(temp_string2, "0", 1) == 0)
                                if (1)
                                {
                                    strcpy(model, family);
//				    printf("Model: %s\n",family);
                                }
                                else
                                {
                                        while (models[i] != NULL)
                                        {
					        if (strncasecmp(temp_string2, models[i], strlen(models[i])) == 0)
						        strcpy(model, models[++i]);

						printf("Model (m): %s\n",model);
					        i += 2;
                                        }
				}
			}

			if (strncasecmp(temp_string, "family", strlen("family")) == 0)
				strcpy(family, temp_string2);

			if (strncasecmp(temp_string, "cpu MHz", strlen("cpu MHz")) == 0)
			{
				if (atol(temp_string2) > tempMHz)
					tempMHz = atol(temp_string2);
			}

			if (strncasecmp(temp_string, "bogomips", strlen("bogomips")) == 0)
				bogomips = bogomips + atof(temp_string2);
		}

		
	}

        sprintf(hw->hw_memory, LONGSPEC, getphysicalmemory());
	
	hw->hw_processors = processors;
	sprintf(hw->hw_cpuinfo, "%s %s", vendor, model);

	if (bogomips == 0.0)
	    sprintf(hw->hw_bogomips, "?");
	else
	    sprintf(hw->hw_bogomips, "%0.2f", bogomips);

	if (tempMHz == 0.0)
		sprintf(hw->hw_megahertz, "?");
	else
		sprintf(hw->hw_megahertz, "%d", (int)tempMHz);
}

#endif /* system_ia64 */
