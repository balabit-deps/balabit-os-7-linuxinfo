/*
        linuxinfo_avr.c

        Copyright (C) 2010
        All Rights Reserved.

	Copyright (C) 2010
        Helge Kreutzmann <debian@helgefjell.de>
	based on linuxinfo_sh.c from Nobuhiro Iwamatsu <iwamatsu@nigauri.org>,

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   KRE     20010215        Initial development

        This is the avr32 port of linuxinfo
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linuxinfo.h"

#ifdef system_avr32

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
	int processors = 0;
	float bogomips = 0.0, tempbogo;
	char temp_string[BUFSIZ], temp_string2[BUFSIZ];
	char chip[BUFSIZ] = "Unknown";
	char family[BUFSIZ]="Unknown";
	char clock[BUFSIZ]="Unknown";

	while (read_line(fd, temp_string, BUFSIZ) != 0)
	{
		splitstring(temp_string, temp_string2);

		/* CPU count */
		if (strncmp(temp_string, "processor", strlen("processor")) == 0)
			processors++;

		/* "cpu type" -> chip */ /* FIXME Check with Brad? */
		if (!(strncmp(temp_string, "cpu type", strlen("cpu type"))) || !(strncmp(temp_string, "cpu core", strlen("cpu core"))))
			strcpy(chip, temp_string2);

		/* cpu family */ /* FIXME Check with Brad? */
		if (!(strncmp(temp_string, "cpu family", strlen("cpu family"))) || !(strncmp(temp_string, "cpu arch", strlen("cpu arch"))))
			strcpy(family, temp_string2);

		/* bogomips and clock FIXME clock */
                if (!(strncmp(temp_string, "bogomips", strlen("bogomips")))) {
			strcpy(clock, temp_string2);
                        sscanf(temp_string2, "%f", &tempbogo);
                        bogomips += tempbogo;
                }
	}

	sprintf(hw->hw_memory, LONGSPEC, getphysicalmemory());
	hw->hw_processors = processors;
	sprintf(hw->hw_cpuinfo, "%s %s", family, chip);
	if (bogomips == 0.0)
	    sprintf(hw->hw_bogomips, "?");
	else
	    sprintf(hw->hw_bogomips, "%0.2f", bogomips);

	sprintf(hw->hw_megahertz, "%s", clock);
}
#endif /* system_avr */
