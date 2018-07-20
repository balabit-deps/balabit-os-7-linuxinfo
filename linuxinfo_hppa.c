/*
        linuxinfo_hppa.c

        Copyright (C) 2002
	All Rights Reserved for Modifications.
	Ola Lundqvist <opal@debian.org>

        Copyright (C) 1998-2000
        All Rights Reserved.

        Alex Buell <alex.buell@munted.eu>

        Advanced Buell Software Engineering Ltd
        Hampshire, GU31 5DG
        United Kingdom

        Copyright (C) 2005,2016
        Helge Kreutzmann <debian@helgefjell.de>

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   AIB     199803??        Initial development
	1.0.1	AIB	20000405	Updated and reworked code
	1.0.2	AIB	20000527	Added AMD K7
        1.0.3   OPAL    20021225	Rewritten some for hppa.
	1.0.4   KRE     20050318        Print CPU
	1.0.5   KRE     20051118        Include <string.h>
	1.0.6   KRE     20160918        Determine fastest CPU

        This is the hppa port of linuxinfo
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "linuxinfo.h"

#ifdef system_hppa

static char *models[] =
{
  //"9000/800/A500-5X", "Crescendo 550",
  //"9000/778/B180L", "Merlin L2+ 180",
  //"9000/785/C3600", "Allegro W+",
  //"9000/871/D270", "UL 2w U160 512K/512K D370",
  NULL, NULL
};

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
	char chip[BUFSIZ], model[BUFSIZ], Mhz[BUFSIZ], family[BUFSIZ];
	char temp_string[BUFSIZ], temp_string2[BUFSIZ];

	struct stat st_buf;

	int processors = 0; 
	float bogomips = 0.0;
	float tempMHz = 0.0;
	LONGLONG memory = 0;

	char *p;

	strcpy(chip, "Unknown");
	strcpy(model, "Unknown");
	strcpy(family, "Unknown");

	while (read_line(fd, temp_string, BUFSIZ) != 0) 
	{
		if (splitstring(temp_string, temp_string2))
		{
			if (strncasecmp(temp_string, "processor", strlen("processor")) == 0)
				processors++;

			// DEBUG printf("%s:\n", temp_string);
//			if (strncasecmp(temp_string, "model", strlen("model")) == 0) 
//			{
//				int i = 0;
//		
//                                if (strncmp(temp_string2, "0", 1) == 0)
//                                {
//                                    strcpy(model, family);
//                                }
//                                else
//                                {
//                                        while (models[i] != NULL)
//                                        {
//					        if (strncasecmp(temp_string2, models[i], strlen(models[i])) == 0)
//						        strcpy(model, models[++i]);
//
//					        i += 2;
//                                        }
//				}
//			}

			if (strncasecmp(temp_string, "family", strlen("cpu family")) == 0)
				strcpy(family, temp_string2);

			if (strncasecmp(temp_string, "cpu  ", strlen("cpu  ")) == 0)
			{
				strcpy(chip, temp_string2);
				strcpy(model, temp_string2);
			}

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
	sprintf(hw->hw_cpuinfo, "%s", model);

	if (bogomips == 0.0)
	    sprintf(hw->hw_bogomips, "?");
	else
	    sprintf(hw->hw_bogomips, "%0.2f", bogomips);

	if (tempMHz == 0.0)
		sprintf(hw->hw_megahertz, "?");
	else
		sprintf(hw->hw_megahertz, "%d", (int)tempMHz);
}

#endif /* system_hppa */
