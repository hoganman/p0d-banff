#-- start of make_header -----------------

#====================================
#  Application MakeND280DetectorCov
#
#   Generated Wed Oct  4 12:22:21 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MakeND280DetectorCov_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MakeND280DetectorCov_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MakeND280DetectorCov

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_MakeND280DetectorCov = /tmp/CMT_$(BANFF_tag)_MakeND280DetectorCov.make$(cmt_lock_pid)
else
#cmt_local_tagfile_MakeND280DetectorCov = $(BANFF_tag)_MakeND280DetectorCov.make
cmt_local_tagfile_MakeND280DetectorCov = $(bin)$(BANFF_tag)_MakeND280DetectorCov.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_MakeND280DetectorCov = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_MakeND280DetectorCov = $(BANFF_tag).make
cmt_local_tagfile_MakeND280DetectorCov = $(bin)$(BANFF_tag).make
endif

endif

-include $(cmt_local_tagfile_MakeND280DetectorCov)

ifdef cmt_MakeND280DetectorCov_has_target_tag

ifdef READONLY
cmt_final_setup_MakeND280DetectorCov = /tmp/CMT_BANFF_MakeND280DetectorCovsetup.make
cmt_local_MakeND280DetectorCov_makefile = /tmp/CMT_MakeND280DetectorCov$(cmt_lock_pid).make
else
cmt_final_setup_MakeND280DetectorCov = $(bin)BANFF_MakeND280DetectorCovsetup.make
cmt_local_MakeND280DetectorCov_makefile = $(bin)MakeND280DetectorCov.make
endif

else

ifdef READONLY
cmt_final_setup_MakeND280DetectorCov = /tmp/CMT_BANFFsetup.make
cmt_local_MakeND280DetectorCov_makefile = /tmp/CMT_MakeND280DetectorCov$(cmt_lock_pid).make
else
cmt_final_setup_MakeND280DetectorCov = $(bin)BANFFsetup.make
cmt_local_MakeND280DetectorCov_makefile = $(bin)MakeND280DetectorCov.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_BANFFsetup.make
else
cmt_final_setup = $(bin)BANFFsetup.make
endif

MakeND280DetectorCov ::


ifdef READONLY
MakeND280DetectorCov ::
	@echo tags=$(tags)
	@echo cmt_local_tagfile=$(cmt_local_tagfile)
endif


dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'MakeND280DetectorCov'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MakeND280DetectorCov/
MakeND280DetectorCov::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

MakeND280DetectorCov :: dirs  $(bin)MakeND280DetectorCov${application_suffix}
	$(echo) "MakeND280DetectorCov ok"

#-- end of application_header
#-- start of application

$(bin)MakeND280DetectorCov${application_suffix} :: $(bin)MakeND280Cov.o $(use_stamps) $(MakeND280DetectorCovstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)MakeND280Cov.o $(cmt_installarea_linkopts) $(MakeND280DetectorCov_use_linkopts) $(MakeND280DetectorCovlinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
MakeND280DetectorCovinstallname = MakeND280DetectorCov${application_suffix}

MakeND280DetectorCov :: MakeND280DetectorCovinstall

install :: MakeND280DetectorCovinstall

MakeND280DetectorCovinstall :: $(install_dir)/$(MakeND280DetectorCovinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(MakeND280DetectorCovinstallname) :: $(bin)$(MakeND280DetectorCovinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(MakeND280DetectorCovinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##MakeND280DetectorCovclean :: MakeND280DetectorCovuninstall

uninstall :: MakeND280DetectorCovuninstall

MakeND280DetectorCovuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(MakeND280DetectorCovinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (MakeND280DetectorCov.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),MakeND280DetectorCovclean)

#$(bin)MakeND280DetectorCov_dependencies.make :: dirs

ifndef QUICK
$(bin)MakeND280DetectorCov_dependencies.make : ../app/MakeND280Cov.cxx $(use_requirements) $(cmt_final_setup_MakeND280DetectorCov)
	$(echo) "(MakeND280DetectorCov.make) Rebuilding $@"; \
	  $(build_dependencies) MakeND280DetectorCov -all_sources -out=$@ ../app/MakeND280Cov.cxx
endif

#$(MakeND280DetectorCov_dependencies)

-include $(bin)MakeND280DetectorCov_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)MakeND280DetectorCov_dependencies.make : $(MakeND280Cov_cxx_dependencies)

$(bin)$(binobj)MakeND280Cov.o : $(MakeND280Cov_cxx_dependencies)
	$(cpp_echo) ../app/MakeND280Cov.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(MakeND280DetectorCov_pp_cppflags) $(app_MakeND280DetectorCov_pp_cppflags) $(MakeND280Cov_pp_cppflags) $(use_cppflags) $(MakeND280DetectorCov_cppflags) $(app_MakeND280DetectorCov_cppflags) $(MakeND280Cov_cppflags) $(MakeND280Cov_cxx_cppflags) -I../app ../app/MakeND280Cov.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: MakeND280DetectorCovclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MakeND280DetectorCov.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(MakeND280DetectorCov.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_MakeND280DetectorCov)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MakeND280DetectorCov.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MakeND280DetectorCov.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MakeND280DetectorCov.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

MakeND280DetectorCovclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) MakeND280DetectorCov${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f MakeND280DetectorCov${application_suffix}

#	@echo "------> (MakeND280DetectorCov.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)MakeND280Cov.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf MakeND280DetectorCov_deps MakeND280DetectorCov_dependencies.make
#-- end of cleanup_objects ------
