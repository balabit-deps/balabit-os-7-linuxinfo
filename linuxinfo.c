/*

	linuxinfo.c

	Copyright (C) 1998-2000
	All Rights Reserved.

	Alex Buell <alex.buell@munted.eu>

        Copyright (C) 2004
	Erick Kinnee <cerb@debian.org>

        Copyright (C) 2005,2006,2013,2014
        Helge Kreutzmann <debian@helgefjell.de>


	Version	Author	Date		Comments
 	----------------------------------------------------------------------
	1.0.0	AIB	199803??	Initial development
	1.0.2	AIB	199803??	Renamed from sysinfo to linuxinfo
					to avoid infringement with a
					commercial product
	1.0.5	AIB	199803??	Added new MHz field for 2.2.x kernels
	1.0.6	AIB	19980306	Added option for testing
	1.1.3	AIB	1999????	If MHz field not detected, does not
					print it
	1.1.4	AIB	1999????	Added -v for versioning
	1.1.6	AIB	20000405	Updates & changes to linuxinfo
	1.1.7	AIB	20000406	Changed to file descriptors

	1.1.8   KRE     20051118        Include <string.h>
        1.1.9   KRE     20060730        Support more than 9 CPUs and bail out
                                        if /proc/cpuinfo is unreadable
        1.2.0   KRE     20131206        i18n this file

	Modelled on Vince Weaver's Linux_logo 2.10
	
	Prints out a line of information about your system.
	Supports Linux on Intel, Sparc, Alpha, m68k and others.	

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <libintl.h>
#define _(String) gettext (String)
#define __(String) String

#include "linuxinfo.h"

int main(int argc, char *argv[])
{
    /* Translators: This number is followed by the type of the CPU and the word "processor(s)" */
	char ordinals[13][10] = { __("Unknown"), __("One"), __("Two"), __("Three"), __("Four"), __("Five"), __("Six"), __("Seven"), __("Eight"), __("Nine"), __("Ten"), __("Eleven"), __("Twelve") };

	struct os_stat os;
	struct hw_stat hw;
	struct lib_stat lib;

	int cpuinfo_fd;

	/* Set locale via LC_ALL.  */
	setlocale (LC_ALL, "");

	bindtextdomain (PACKAGE, LOCALEDIR);
	textdomain (PACKAGE);

	if (argc > 1)
	{
		if (strcmp(argv[1], ("-v")) == 0)
		{
			printf("%s %s\n", argv[0], VERSION);
			return 0;
		}
		if (strcmp(argv[1], ("-h")) == 0)
		{
			printf("%s %s\n", argv[0], VERSION);
			printf(_(" -h   print this help\n"));
	    printf(_(" -v   print linuxinfo version\n"));
			return 0;
		}
		
		cpuinfo_fd = open(argv[1], O_RDONLY);
                if (cpuinfo_fd < 0)
		{
                    fprintf(stderr,_("Unsupported option or file %s not found.\n"),argv[1]);
                    return -1;
		}
	}
	else
	{
		cpuinfo_fd = open(CPUINFO_FILE, O_RDONLY);
                if (cpuinfo_fd < 0)
		{
                    fprintf(stderr,_("Could not open %s.\n"),CPUINFO_FILE);
                    return -2;
		}
	}

	GetOperatingSystemInfo(&os);
	setlocale(LC_NUMERIC, "C");
	GetHardwareInfo(cpuinfo_fd, &hw);
	setlocale(LC_NUMERIC, "");
	GetSystemLibcInfo(&lib);

	printf("%s %s %s %s\n", os.os_name, os.os_hostname, os.os_version, os.os_revision);
	if (hw.hw_processors > 12)
	{
	    printf("%i %s ", hw.hw_processors, hw.hw_cpuinfo);
	}
	else
	{
	    printf("%s %s ", _(ordinals[hw.hw_processors]), hw.hw_cpuinfo);
	}

	// FIXME Next string proper i18n (value is string, unfortunately)
	if (strncmp(hw.hw_megahertz, "?", strlen("?")) != 0)
	{
	    printf("%sMHz ", hw.hw_megahertz);
	}
	printf(ngettext("processor", "processors", hw.hw_processors));
	
	// FIXME Next string proper i18n (value is string, unfortunately)
	printf(_(", %s total bogomips, %sM RAM\n"), hw.hw_bogomips, hw.hw_memory);
//	if (strncmp(hw.hw_megahertz, "?", strlen("?")) == 0)
//		printf("processor%s, %s total bogomips, %sM RAM\n", (hw.hw_processors > 1) ? "s" : "", hw.hw_bogomips, hw.hw_memory);
//	else
//	{
//		printf("%sMHz processor%s, %s total bogomips, %sM RAM\n", hw.hw_megahertz, (hw.hw_processors > 1) ? "s" : "", hw.hw_bogomips, hw.hw_memory);
//	}
	printf(_("System library %s\n"), lib.lib_version);

	close(cpuinfo_fd);
	return 0;
}
