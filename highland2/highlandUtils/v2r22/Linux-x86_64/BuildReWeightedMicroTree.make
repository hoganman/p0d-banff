#-- start of make_header -----------------

#====================================
#  Application BuildReWeightedMicroTree
#
#   Generated Mon Oct  9 06:39:59 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_BuildReWeightedMicroTree_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_BuildReWeightedMicroTree_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_BuildReWeightedMicroTree

highlandUtils_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_BuildReWeightedMicroTree = /tmp/CMT_$(highlandUtils_tag)_BuildReWeightedMicroTree.make$(cmt_lock_pid)
else
#cmt_local_tagfile_BuildReWeightedMicroTree = $(highlandUtils_tag)_BuildReWeightedMicroTree.make
cmt_local_tagfile_BuildReWeightedMicroTree = $(bin)$(highlandUtils_tag)_BuildReWeightedMicroTree.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandUtils_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_BuildReWeightedMicroTree = /tmp/CMT_$(highlandUtils_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_BuildReWeightedMicroTree = $(highlandUtils_tag).make
cmt_local_tagfile_BuildReWeightedMicroTree = $(bin)$(highlandUtils_tag).make
endif

endif

-include $(cmt_local_tagfile_BuildReWeightedMicroTree)

ifdef cmt_BuildReWeightedMicroTree_has_target_tag

ifdef READONLY
cmt_final_setup_BuildReWeightedMicroTree = /tmp/CMT_highlandUtils_BuildReWeightedMicroTreesetup.make
cmt_local_BuildReWeightedMicroTree_makefile = /tmp/CMT_BuildReWeightedMicroTree$(cmt_lock_pid).make
else
cmt_final_setup_BuildReWeightedMicroTree = $(bin)highlandUtils_BuildReWeightedMicroTreesetup.make
cmt_local_BuildReWeightedMicroTree_makefile = $(bin)BuildReWeightedMicroTree.make
endif

else

ifdef READONLY
cmt_final_setup_BuildReWeightedMicroTree = /tmp/CMT_highlandUtilssetup.make
cmt_local_BuildReWeightedMicroTree_makefile = /tmp/CMT_BuildReWeightedMicroTree$(cmt_lock_pid).make
else
cmt_final_setup_BuildReWeightedMicroTree = $(bin)highlandUtilssetup.make
cmt_local_BuildReWeightedMicroTree_makefile = $(bin)BuildReWeightedMicroTree.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandUtilssetup.make
else
cmt_final_setup = $(bin)highlandUtilssetup.make
endif

BuildReWeightedMicroTree ::


ifdef READONLY
BuildReWeightedMicroTree ::
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
	$(echo) 'BuildReWeightedMicroTree'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = BuildReWeightedMicroTree/
BuildReWeightedMicroTree::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

BuildReWeightedMicroTree :: dirs  $(bin)BuildReWeightedMicroTree${application_suffix}
	$(echo) "BuildReWeightedMicroTree ok"

#-- end of application_header
#-- start of application

$(bin)BuildReWeightedMicroTree${application_suffix} :: $(bin)BuildReWeightedMicroTree.o $(use_stamps) $(BuildReWeightedMicroTreestamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)BuildReWeightedMicroTree.o $(cmt_installarea_linkopts) $(BuildReWeightedMicroTree_use_linkopts) $(BuildReWeightedMicroTreelinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
BuildReWeightedMicroTreeinstallname = BuildReWeightedMicroTree${application_suffix}

BuildReWeightedMicroTree :: BuildReWeightedMicroTreeinstall

install :: BuildReWeightedMicroTreeinstall

BuildReWeightedMicroTreeinstall :: $(install_dir)/$(BuildReWeightedMicroTreeinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(BuildReWeightedMicroTreeinstallname) :: $(bin)$(BuildReWeightedMicroTreeinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(BuildReWeightedMicroTreeinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##BuildReWeightedMicroTreeclean :: BuildReWeightedMicroTreeuninstall

uninstall :: BuildReWeightedMicroTreeuninstall

BuildReWeightedMicroTreeuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(BuildReWeightedMicroTreeinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (BuildReWeightedMicroTree.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),BuildReWeightedMicroTreeclean)

#$(bin)BuildReWeightedMicroTree_dependencies.make :: dirs

ifndef QUICK
$(bin)BuildReWeightedMicroTree_dependencies.make : ../app/BuildReWeightedMicroTree.cxx $(use_requirements) $(cmt_final_setup_BuildReWeightedMicroTree)
	$(echo) "(BuildReWeightedMicroTree.make) Rebuilding $@"; \
	  $(build_dependencies) BuildReWeightedMicroTree -all_sources -out=$@ ../app/BuildReWeightedMicroTree.cxx
endif

#$(BuildReWeightedMicroTree_dependencies)

-include $(bin)BuildReWeightedMicroTree_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)BuildReWeightedMicroTree_dependencies.make : $(BuildReWeightedMicroTree_cxx_dependencies)

$(bin)$(binobj)BuildReWeightedMicroTree.o : $(BuildReWeightedMicroTree_cxx_dependencies)
	$(cpp_echo) ../app/BuildReWeightedMicroTree.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BuildReWeightedMicroTree_pp_cppflags) $(app_BuildReWeightedMicroTree_pp_cppflags) $(BuildReWeightedMicroTree_pp_cppflags) $(use_cppflags) $(BuildReWeightedMicroTree_cppflags) $(app_BuildReWeightedMicroTree_cppflags) $(BuildReWeightedMicroTree_cppflags) $(BuildReWeightedMicroTree_cxx_cppflags) -I../app ../app/BuildReWeightedMicroTree.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: BuildReWeightedMicroTreeclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(BuildReWeightedMicroTree.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(BuildReWeightedMicroTree.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_BuildReWeightedMicroTree)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(BuildReWeightedMicroTree.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(BuildReWeightedMicroTree.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(BuildReWeightedMicroTree.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

BuildReWeightedMicroTreeclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) BuildReWeightedMicroTree${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f BuildReWeightedMicroTree${application_suffix}

#	@echo "------> (BuildReWeightedMicroTree.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)BuildReWeightedMicroTree.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf BuildReWeightedMicroTree_deps BuildReWeightedMicroTree_dependencies.make
#-- end of cleanup_objects ------
