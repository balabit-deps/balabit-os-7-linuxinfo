# This file has been automatically generated from po4a/Makefile.am.in by aclocal, do not edit
## Process this file with automake to produce Makefile.in

dist_man_MANS = linuxinfo.1

EXTRA_DIST = \
	po4a.cfg \
	linuxinfo.pot


# Extract the list of languages from the po4a config file.
LINGUAS_DIST = `sed -ne 's/^.*\[po4a_langs\] \(.*\)$$/\1/p' $(srcdir)/po4a.cfg`

# If the user has not defined it let's use the default.
LINGUAS ?= $(LINGUAS_DIST)


PO4A_V = $(PO4A_V_@AM_V@)
PO4A_V_ = $(PO4A_V_@AM_DEFAULT_V@)
PO4A_V_0 = @echo "  PO4A   $@";

### PO4A_OPTS = --previous --srcdir $(srcdir) --destdir $(CURDIR) --no-backups \
###            --package-name linuxinfo --package-version $(PACKAGE_VERSION) \
###            --msgid-bugs-address "Helge Kreutzmann <debian@helgefjell.de>"

PO4A_OPTS = --previous --srcdir $(srcdir) --destdir $(CURDIR) --no-backups \
            --package-name linuxinfo --package-version 2.5.0 \
            --msgid-bugs-address "Helge Kreutzmann <debian@helgefjell.de>"

all-local: all-local-@USE_PO4A@

all-local-no:
all-local-yes: man.stamp

# FIXME: Use a stamp file until po4a supports them internally.
man.stamp:
	$(PO4A_V) $(PO4A) $(PO4A_OPTS) $(srcdir)/po4a.cfg
	$(AM_V_at) touch $@

clean-local: clean-local-@USE_PO4A@

clean-local-no:
clean-local-yes:
	rm -rf $(LINGUAS_DIST)
	$(AM_V_at) rm -f man.stamp

.PHONY: update-po

update-po:
	$(PO4A_V) $(PO4A) $(PO4A_OPTS) --force $(srcdir)/po4a.cfg

install-data-local: install-data-local-@USE_PO4A@

install-data-local-no:
install-data-local-yes:
	for lang in $(LINGUAS); do \
		files=""; \
		for trans in $(dist_man_MANS); do \
			if [ -f $(CURDIR)/$$lang/$$trans ]; then \
				files="$$files $(CURDIR)/$$lang/$$trans"; \
			elif [ -f $(srcdir)/$$lang/$$trans ]; then \
				files="$$files $(srcdir)/$$lang/$$trans"; \
			fi; \
		done; \
		$(MAKE) install-man \
			mandir="$(mandir)/$$lang" \
			man_MANS="" \
			dist_man_MANS="$$files"; \
	done

uninstall-local: uninstall-local-@USE_PO4A@

uninstall-local-no:
uninstall-local-yes:
	for lang in $(LINGUAS); do \
		files=""; \
		for trans in $(dist_man_MANS); do \
			if [ -f $(CURDIR)/$$lang/$$trans ]; then \
				files="$$files $(CURDIR)/$$lang/$$trans"; \
			elif [ -f $(srcdir)/$$lang/$$trans ]; then \
				files="$$files $(srcdir)/$$lang/$$trans"; \
			fi; \
		done; \
		$(MAKE) uninstall-man \
			mandir="$(mandir)/$$lang" \
			man_MANS="" \
			dist_man_MANS="$$files"; \
	done

dist-hook: man.stamp
	cp $(srcdir)/man.stamp $(distdir)/
	for lang in $(LINGUAS_DIST); do \
		cp $(srcdir)/$$lang.po $(distdir); \
		cp $(srcdir)/$$lang.add $(distdir); \
		$(MKDIR_P) $(distdir)/$$lang; \
		cp -r $(srcdir)/$$lang $(distdir)/; \
	done
