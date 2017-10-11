#-- start of make_header -----------------

#====================================
#  Application RunJointFit2017
#
#   Generated Wed Oct  4 12:22:14 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunJointFit2017_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunJointFit2017_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunJointFit2017

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunJointFit2017 = /tmp/CMT_$(BANFF_tag)_RunJointFit2017.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunJointFit2017 = $(BANFF_tag)_RunJointFit2017.make
cmt_local_tagfile_RunJointFit2017 = $(bin)$(BANFF_tag)_RunJointFit2017.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunJointFit2017 = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunJointFit2017 = $(BANFF_tag).make
cmt_local_tagfile_RunJointFit2017 = $(bin)$(BANFF_tag).make
endif

endif

-include $(cmt_local_tagfile_RunJointFit2017)

ifdef cmt_RunJointFit2017_has_target_tag

ifdef READONLY
cmt_final_setup_RunJointFit2017 = /tmp/CMT_BANFF_RunJointFit2017setup.make
cmt_local_RunJointFit2017_makefile = /tmp/CMT_RunJointFit2017$(cmt_lock_pid).make
else
cmt_final_setup_RunJointFit2017 = $(bin)BANFF_RunJointFit2017setup.make
cmt_local_RunJointFit2017_makefile = $(bin)RunJointFit2017.make
endif

else

ifdef READONLY
cmt_final_setup_RunJointFit2017 = /tmp/CMT_BANFFsetup.make
cmt_local_RunJointFit2017_makefile = /tmp/CMT_RunJointFit2017$(cmt_lock_pid).make
else
cmt_final_setup_RunJointFit2017 = $(bin)BANFFsetup.make
cmt_local_RunJointFit2017_makefile = $(bin)RunJointFit2017.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_BANFFsetup.make
else
cmt_final_setup = $(bin)BANFFsetup.make
endif

RunJointFit2017 ::


ifdef READONLY
RunJointFit2017 ::
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
	$(echo) 'RunJointFit2017'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunJointFit2017/
RunJointFit2017::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunJointFit2017 :: dirs  $(bin)RunJointFit2017${application_suffix}
	$(echo) "RunJointFit2017 ok"

#-- end of application_header
#-- start of application

$(bin)RunJointFit2017${application_suffix} :: $(bin)RunJointFit2017.o $(use_stamps) $(RunJointFit2017stamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunJointFit2017.o $(cmt_installarea_linkopts) $(RunJointFit2017_use_linkopts) $(RunJointFit2017linkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunJointFit2017installname = RunJointFit2017${application_suffix}

RunJointFit2017 :: RunJointFit2017install

install :: RunJointFit2017install

RunJointFit2017install :: $(install_dir)/$(RunJointFit2017installname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunJointFit2017installname) :: $(bin)$(RunJointFit2017installname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunJointFit2017installname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunJointFit2017clean :: RunJointFit2017uninstall

uninstall :: RunJointFit2017uninstall

RunJointFit2017uninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunJointFit2017installname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunJointFit2017.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunJointFit2017clean)

#$(bin)RunJointFit2017_dependencies.make :: dirs

ifndef QUICK
$(bin)RunJointFit2017_dependencies.make : ../app/RunJointFit2017.cxx $(use_requirements) $(cmt_final_setup_RunJointFit2017)
	$(echo) "(RunJointFit2017.make) Rebuilding $@"; \
	  $(build_dependencies) RunJointFit2017 -all_sources -out=$@ ../app/RunJointFit2017.cxx
endif

#$(RunJointFit2017_dependencies)

-include $(bin)RunJointFit2017_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunJointFit2017_dependencies.make : $(RunJointFit2017_cxx_dependencies)

$(bin)$(binobj)RunJointFit2017.o : $(RunJointFit2017_cxx_dependencies)
	$(cpp_echo) ../app/RunJointFit2017.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunJointFit2017_pp_cppflags) $(app_RunJointFit2017_pp_cppflags) $(RunJointFit2017_pp_cppflags) $(use_cppflags) $(RunJointFit2017_cppflags) $(app_RunJointFit2017_cppflags) $(RunJointFit2017_cppflags) $(RunJointFit2017_cxx_cppflags) -I../app ../app/RunJointFit2017.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunJointFit2017clean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunJointFit2017.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunJointFit2017.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunJointFit2017)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunJointFit2017.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunJointFit2017.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunJointFit2017.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunJointFit2017clean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunJointFit2017${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunJointFit2017${application_suffix}

#	@echo "------> (RunJointFit2017.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunJointFit2017.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunJointFit2017_deps RunJointFit2017_dependencies.make
#-- end of cleanup_objects ------
