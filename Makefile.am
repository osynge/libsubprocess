topdir=@RPM_DIR@

AUX_DIST             =  $(ac_aux_dir)/config.guess \
                        $(ac_aux_dir)/config.sub \
                        $(ac_aux_dir)/install-sh \
                        $(ac_aux_dir)/ltmain.sh \
                        $(ac_aux_dir)/missing 

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure config-h.in \
                        stamp-h.in $(AUX_DIST)

ACLOCAL              = aclocal -I $(ac_aux_dir)

# There is something I don't fully understand in the next part
# Sometimes 'config' exist sometimes not, I have to check to be sure
# Maybe it's because I rerun the process without erasing the previous
# version ?
# Et Hop! Another tricky thing I didnt understood: depcomp exist on
# ccdevli05 but not on ccdesk05... Have fun !


SUBDIRS = src



dist-hook:
	(cd $(distdir); \
	if [ -a $(ac_aux_dir) ]; then \
	   echo "config exist!"; \
	else \
	   mkdir $(ac_aux_dir); \
	fi)
	(if [ -a $(ac_aux_dir)/depcomp ]; then \
	   echo "depcomp exist!"; \
	else \
	   touch $(ac_aux_dir)/depcomp; \
	fi)
	for file in $(AUX_DIST) $(AUX_DIST_EXTRA); do \
	  cp $$file $(distdir)/$$file; \
	done



rpm: dist @PACKAGE@.spec
	mkdir -p $(topdir)/SOURCES \
	$(topdir)/SPECS \
	$(topdir)/BUILD \
	$(topdir)/RPMS $(topdir)/SRPMS
	cp @PACKAGE@-@VERSION@.tar.gz $(topdir)/SOURCES
	cp @PACKAGE@.spec $(topdir)/SPECS
	rpmbuild --define "_topdir $(topdir)" -ba @PACKAGE@.spec

userdoc:
	doxygen doxygen.ini
	
apidoc:
	doxygen doxygen.ini


.PHONY: userdoc apidoc



docfiles = \
	AUTHORS \
	COPYING \
	ChangeLog \
	INSTALL \
	NEWS \
	README



docdir = $(DOC_DIR)
doc_DATA = $(docfiles)
EXTRA_DIST = $(docfiles)

