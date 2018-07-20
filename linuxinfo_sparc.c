/*
        linuxinfo_sparc.c

        Copyright (C) 1998-2001
        All Rights Reserved.

        Alex Buell <alex.buell@munted.eu>

	Copyright (C) 2005,2006
	Helge Kreutzmann <debian@helgefjell.de>

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   AIB     199803??        Initial development
	1.0.1	AIB	20000405	Rewritten
	1.0.2	AIB	20010129	Fixed bug and added Fujitsu
	1.0.3	AIB	20010909	Complete rewrite
	1.0.4   HKM     20060723        Added more machines and fixed a typo
	1.0.5   HKM     20061017        Added the Niagra

        This is the Sparc32 port of linuxinfo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linuxinfo.h"

#ifdef system_sparc

static char *vendors[] =
{
	"Texas", "TI",
	"TI", "TI",
	"Fujitsu", "Fujitsu",
	"Cypress", "Cypress",
	"ROSS", "Ross Technologies",
	NULL, NULL
};

static char *models[] = 
{
	"Fujitsu  MB86904", "MB86904",
	"Fujitsu TurboSparc MB86907", "TurboSparc MB86907",
	"Cypress/ROSS CY7C601", "CY7C601",
	"Cypress/ROSS CY7C611", "CY7C611",
	"ROSS HyperSparc RT620", "RT620",
	"ROSS HyperSparc RT625 or RT626", "RT625/RT626",
	"Texas Instruments, Inc. - MicroSparc", "MicroSparc",
	"Texas Instruments, Inc. - MicroSparc II", "MicroSparc II",
	"Texas Instruments, Inc. - SuperSparc 50", "SuperSparc 50",
	"Texas Instruments, Inc. - SuperSparc 51", "SuperSparc 51",
	"Texas Instruments, Inc. - SuperSparc 61", "SuperSparc 61",
	"TI UltraSparc II  (BlackBird)","UltraSparc II (BlackBird)",
	"TI UltraSparc IIi","UltraSparc IIi",
	"TI UltraSparc IIi (Sabre)","UltraSparc IIi (Sabre)",
	"UltraSparc T1 (Niagara)","UltraSparc T1 (Niagara)",
	NULL, NULL
};

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
        float bogomips = 0.0;
        int processors = 0;
        float tempbogo;

        char temp_string[BUFSIZ], temp_string2[BUFSIZ];
        char chip[BUFSIZ], vendor[BUFSIZ], model[BUFSIZ];

        strcpy(chip, "Unknown");
        strcpy(vendor, "Unknown");
        strcpy(model, "Unknown");

	while (read_line(fd, temp_string, BUFSIZ) != 0)
	{
		splitstring(temp_string, temp_string2);
		if (strncmp(temp_string, "cpu", strlen("cpu")) == 0)
		{
			int i = 0;
		
			while (vendors[i] != NULL)
			{
				if (strncmp(temp_string2, vendors[i], strlen(vendors[i])) == 0)
					strcpy(vendor, vendors[++i]);

				i += 2;
			}

			i = 0;
			while (models[i] != NULL)
			{
				if (strncmp(temp_string2, models[i], strlen(models[i])) == 0)
					strcpy(model, models[++i]);
		
				i += 2;
			}
		}

		if (!(strncmp(temp_string, "ncpus", strlen("ncpus"))) && (processors == 0))
			sscanf(temp_string2, "%d", &processors);

		if (!(strncmp(temp_string, "BogoMips", strlen("BogoMips"))))
			sscanf(temp_string2, "%f", &tempbogo);

		if (!(strncmp(temp_string, "Cpu0Bogo", strlen("Cpu0Bogo"))))
			sscanf(temp_string2, "%f", &tempbogo);
	}

        sprintf(hw->hw_memory, LONGSPEC, getphysicalmemory());
        hw->hw_processors = processors;

	strcat(hw->hw_megahertz, "?");
        sprintf(hw->hw_cpuinfo, "%s %s", vendor, model);
	if (bogomips == 0.0)
	    sprintf(hw->hw_bogomips, "?");
	else
	    sprintf(hw->hw_bogomips, "%0.2f",tempbogo * processors);

}

#endif /* system_sparc */
