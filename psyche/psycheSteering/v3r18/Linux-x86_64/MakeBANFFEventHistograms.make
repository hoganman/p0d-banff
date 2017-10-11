#-- start of make_header -----------------

#====================================
#  Application MakeBANFFEventHistograms
#
#   Generated Mon Oct  9 06:43:46 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MakeBANFFEventHistograms_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MakeBANFFEventHistograms_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MakeBANFFEventHistograms

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_MakeBANFFEventHistograms = /tmp/CMT_$(psycheSteering_tag)_MakeBANFFEventHistograms.make$(cmt_lock_pid)
else
#cmt_local_tagfile_MakeBANFFEventHistograms = $(psycheSteering_tag)_MakeBANFFEventHistograms.make
cmt_local_tagfile_MakeBANFFEventHistograms = $(bin)$(psycheSteering_tag)_MakeBANFFEventHistograms.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_MakeBANFFEventHistograms = /tmp/CMT_$(psycheSteering_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_MakeBANFFEventHistograms = $(psycheSteering_tag).make
cmt_local_tagfile_MakeBANFFEventHistograms = $(bin)$(psycheSteering_tag).make
endif

endif

-include $(cmt_local_tagfile_MakeBANFFEventHistograms)

ifdef cmt_MakeBANFFEventHistograms_has_target_tag

ifdef READONLY
cmt_final_setup_MakeBANFFEventHistograms = /tmp/CMT_psycheSteering_MakeBANFFEventHistogramssetup.make
cmt_local_MakeBANFFEventHistograms_makefile = /tmp/CMT_MakeBANFFEventHistograms$(cmt_lock_pid).make
else
cmt_final_setup_MakeBANFFEventHistograms = $(bin)psycheSteering_MakeBANFFEventHistogramssetup.make
cmt_local_MakeBANFFEventHistograms_makefile = $(bin)MakeBANFFEventHistograms.make
endif

else

ifdef READONLY
cmt_final_setup_MakeBANFFEventHistograms = /tmp/CMT_psycheSteeringsetup.make
cmt_local_MakeBANFFEventHistograms_makefile = /tmp/CMT_MakeBANFFEventHistograms$(cmt_lock_pid).make
else
cmt_final_setup_MakeBANFFEventHistograms = $(bin)psycheSteeringsetup.make
cmt_local_MakeBANFFEventHistograms_makefile = $(bin)MakeBANFFEventHistograms.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheSteeringsetup.make
else
cmt_final_setup = $(bin)psycheSteeringsetup.make
endif

MakeBANFFEventHistograms ::


ifdef READONLY
MakeBANFFEventHistograms ::
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
	$(echo) 'MakeBANFFEventHistograms'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MakeBANFFEventHistograms/
MakeBANFFEventHistograms::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

MakeBANFFEventHistograms :: dirs  $(bin)MakeBANFFEventHistograms${application_suffix}
	$(echo) "MakeBANFFEventHistograms ok"

#-- end of application_header
#-- start of application

$(bin)MakeBANFFEventHistograms${application_suffix} :: $(bin)MakeBANFFEventHistograms.o $(use_stamps) $(MakeBANFFEventHistogramsstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)MakeBANFFEventHistograms.o $(cmt_installarea_linkopts) $(MakeBANFFEventHistograms_use_linkopts) $(MakeBANFFEventHistogramslinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
MakeBANFFEventHistogramsinstallname = MakeBANFFEventHistograms${application_suffix}

MakeBANFFEventHistograms :: MakeBANFFEventHistogramsinstall

install :: MakeBANFFEventHistogramsinstall

MakeBANFFEventHistogramsinstall :: $(install_dir)/$(MakeBANFFEventHistogramsinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(MakeBANFFEventHistogramsinstallname) :: $(bin)$(MakeBANFFEventHistogramsinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(MakeBANFFEventHistogramsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##MakeBANFFEventHistogramsclean :: MakeBANFFEventHistogramsuninstall

uninstall :: MakeBANFFEventHistogramsuninstall

MakeBANFFEventHistogramsuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(MakeBANFFEventHistogramsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (MakeBANFFEventHistograms.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),MakeBANFFEventHistogramsclean)

#$(bin)MakeBANFFEventHistograms_dependencies.make :: dirs

ifndef QUICK
$(bin)MakeBANFFEventHistograms_dependencies.make : ../app/MakeBANFFEventHistograms.cxx $(use_requirements) $(cmt_final_setup_MakeBANFFEventHistograms)
	$(echo) "(MakeBANFFEventHistograms.make) Rebuilding $@"; \
	  $(build_dependencies) MakeBANFFEventHistograms -all_sources -out=$@ ../app/MakeBANFFEventHistograms.cxx
endif

#$(MakeBANFFEventHistograms_dependencies)

-include $(bin)MakeBANFFEventHistograms_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)MakeBANFFEventHistograms_dependencies.make : $(MakeBANFFEventHistograms_cxx_dependencies)

$(bin)$(binobj)MakeBANFFEventHistograms.o : $(MakeBANFFEventHistograms_cxx_dependencies)
	$(cpp_echo) ../app/MakeBANFFEventHistograms.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(MakeBANFFEventHistograms_pp_cppflags) $(app_MakeBANFFEventHistograms_pp_cppflags) $(MakeBANFFEventHistograms_pp_cppflags) $(use_cppflags) $(MakeBANFFEventHistograms_cppflags) $(app_MakeBANFFEventHistograms_cppflags) $(MakeBANFFEventHistograms_cppflags) $(MakeBANFFEventHistograms_cxx_cppflags) -I../app ../app/MakeBANFFEventHistograms.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: MakeBANFFEventHistogramsclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MakeBANFFEventHistograms.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(MakeBANFFEventHistograms.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_MakeBANFFEventHistograms)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MakeBANFFEventHistograms.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MakeBANFFEventHistograms.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MakeBANFFEventHistograms.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

MakeBANFFEventHistogramsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) MakeBANFFEventHistograms${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f MakeBANFFEventHistograms${application_suffix}

#	@echo "------> (MakeBANFFEventHistograms.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)MakeBANFFEventHistograms.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf MakeBANFFEventHistograms_deps MakeBANFFEventHistograms_dependencies.make
#-- end of cleanup_objects ------
