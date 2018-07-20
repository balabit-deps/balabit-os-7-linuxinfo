/*
        linuxinfo_unknown.c

        Copyright (C) 1998-2000
        All Rights Reserved.

        Alex Buell <alex.buell@munted.eu>

        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.0   AIB     199803??        Initial development
	1.0.1	AIB	20000406	Modifications to tidy up results

	This allows compilation on Linux system that linuxinfo does not
	know about.

*/

#include <stdio.h>
#include "linuxinfo.h"

#ifdef system_unknown

void GetHardwareInfo(int fd, struct hw_stat *hw)
{
	sprintf(hw->hw_memory, LONGSPEC, 0);

	hw->hw_processors = 0;

	sprintf(hw->hw_cpuinfo, "%s", "Unknown");
	sprintf(hw->hw_bogomips, "%0.2f", 0.0);
	sprintf(hw->hw_megahertz, "?");
	hw->hw_processors = 0;
}

#endif /* system_unknown */
