# Copyright © 2005 Scott James Remnant <scott@netsplit.com>
# Copyright © 2007 Frank Lichtenheld <djpig@debian.org>
# Copyright © 2007, 2009, 2011 Guillem Jover <guillem@debian.org>

# LINUXINFO_PROG_PO4A
# --------------
AC_DEFUN([LINUXINFO_PROG_PO4A], [
AC_REQUIRE([AM_NLS])
AC_CHECK_PROGS([PO4A], [po4a])
if test "$USE_NLS" = "yes" && test -n "$PO4A"; then
  USE_PO4A=yes
else
  USE_PO4A=no
fi
AC_SUBST([USE_PO4A])
])# LINUXINFO_PROG_PO4A
