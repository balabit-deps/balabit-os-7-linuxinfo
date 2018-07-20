/*
	linuxinfo.h

	Header file for linuxinfo

	Copyright (C) 1998-2000
	All Rights Reserved.

	Alex Buell <alex.buell@munted.eu>

	Copyright (C) 1999 
	Erick Kinnee <cerb@debian.org>

	Copyright (C) 2002
	Gerhard Tonn <gt@debian.org>

	Copyright (C) 2002 
	John R. Daily <jdaily@progeny.com>

	Copyright (C) 2002
	Ola Lundqvist <opal@debian.org>

	Copyright (C) 2004
	Andreas Jochens <aj@andaco.de>

	Copyright (C) 2004,2010 
	Helge Kreutzmann <debian@helgefjell.de>

	Copyright (C) 2010 
	Nobuhiro Iwamatsu <iwamatsu@nigauri.org>


        Version Author  Date            Comments
        ----------------------------------------------------------------------
        1.0.1   AIB     199803??        Initial development
	1.0.2	AIB	1998????	Modification for compatibility with 
					both libc5 & glibc2 libraries
	1.0.4	AIB	1998????	Removed the glibc ifdefs.
	1.0.5	AIB	1998????	Added a new field to hw_stat struct.
	1.0.8	AIB	1999????	Ported to ARM architecture.
	1.1.3	AIB	1999????	mc68000 define replaced with __mc68000__
	1.1.4	AIB	20000405	Moved strstr() to linuxinfo_common.h
        1.1.5   OPAL	20021225	Added hppa
	1.1.8   KRE     20040429        Added mips
	1.1.8   KRE     20040807        Add support for HIGHMEM
	1.1.8   KRE     20010201        Added sh (Nobuhiro Iwamatsu)
	1.1.8   KRE     20010201        Added avr32

*/

#ifndef _LINUXINFO_H_
#define _LINUXINFO_H_

#if !defined(linux)
#define system_unknown
#endif

#if defined(__i386__) || defined(__x86_64__)
#define system_intel
#endif

#if defined(__mc68000__)
#define system_m68k
#endif 

#if defined(__alpha__)
#define system_alpha
#endif

#if defined(__sparc__)
#define system_sparc
#endif

#if defined(__s390__)
#define system_s390
#endif

#if defined(__hppa__)
#define system_hppa
#endif

#if defined(__PPC__)
#define system_ppc
#endif

#if defined(__arm__) || defined(__aarch64__)
#define system_arm
#endif

#if defined(__ia64__)
#define system_ia64
#endif

#if defined(__mips__)
#define system_mips
#endif

#if defined(__sh__)
#define system_sh
#endif

#if defined(__avr32__)
#define system_avr32
#endif

#if (SIZEOF_LONG > 4)
#define LONGLONG long int
#define LONGSPEC "%ld"
#else
#define LONGLONG long long int
#define LONGSPEC "%lld"
#endif

struct os_stat 
{
	char os_hostname[BUFSIZ];
	char os_name[BUFSIZ];
	char os_version[BUFSIZ];
	char os_revision[BUFSIZ];
}; 

struct hw_stat 
{
	char hw_cpuinfo[BUFSIZ];
	char hw_bogomips[BUFSIZ];
	char hw_memory[BUFSIZ];
	char hw_megahertz[BUFSIZ];
	int hw_processors;
};

struct lib_stat 
{
	char lib_version[BUFSIZ];
};

void GetOperatingSystemInfo(struct os_stat *os);
void GetHardwareInfo(int fd, struct hw_stat *hw);
void GetSystemLibcInfo(struct lib_stat *lib);

int read_line(int fd, char *buffer, size_t length);
int splitstring(char *first_string, char *second_string); 

LONGLONG getphysicalmemory(void);

#define CPUINFO_FILE	"/proc/cpuinfo"
#define MEMINFO_FILE	"/proc/meminfo"

#endif /* _LINUXINFO_H_ */
