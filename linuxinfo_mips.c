/*
        linuxinfo_mips.c

        Copyright (C) 2004-2006
        All Rights Reserved.

        Helge Kreutzmann <debian@helgefjell.de>,
	based on linuxinfo_m68k.c from Alex Buell <alex.buell@munted.eu>

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   KRE     20040522        Initial development
        1.0.1   KRE     20051118        Include <string.h>
	1.0.2   KRE     20060309        Fix detection if no endian info is given
	1.0.3   KRE     20060409        Do not look for endianess

        This is the mips port of linuxinfo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "linuxinfo.h"

#ifdef system_mips

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
	int processors = 0; 
	float bogomips = 0.0;
	float tempbogo;

	char temp_string[BUFSIZ], temp_string2[BUFSIZ];
	char chip[BUFSIZ] = "Unknown";
	char endian[BUFSIZ]="Unknown";
	char fpu[BUFSIZ]="FPU Unknown";
	char fullchip[BUFSIZ];
	char *pos;

	FILE *procfile;

	while (read_line(fd, temp_string, BUFSIZ) != 0)
	{
		splitstring(temp_string, temp_string2);

		if (processors == 0)
                       if (!(strncmp(temp_string, "cpu model", strlen("cpu model"))))
			{
				pos=strstr(temp_string2, "FPU");
				if (pos)
				{
				    strcpy(fpu, pos);
				    while (isspace(*(pos-1))) pos--;
				    *(pos) = '\0';
				}
				strcpy(chip, temp_string2);
			}

//              if (!(strncmp(temp_string, "byteorder", strlen("byteorder"))))
//			strcpy(endian, temp_string2);

                if (!(strncmp(temp_string, "BogoMIPS", strlen("BogoMIPS"))))
                {
                        processors++;
                        sscanf(temp_string2, "%f", &tempbogo);
                        bogomips += tempbogo;
                }
	}

	sprintf(hw->hw_memory, LONGSPEC, getphysicalmemory());
	hw->hw_processors = processors;

	if (fpu)
	{
		sprintf(fullchip, "%s (%s)",chip,fpu);
	}
	else
	{
		strcpy(fullchip,chip);
	}
	if (strncmp(endian, "Unknown", strlen("Unknown")))
	{
		sprintf(hw->hw_cpuinfo, "%s %s", endian,fullchip);
	}
	else
	{
		sprintf(hw->hw_cpuinfo, "%s", fullchip);
	}

	if (bogomips == 0.0)
	    sprintf(hw->hw_bogomips, "?");
	else
	    sprintf(hw->hw_bogomips, "%0.2f", bogomips);

	sprintf(hw->hw_megahertz, "?");
}
#endif /* system_mips */
