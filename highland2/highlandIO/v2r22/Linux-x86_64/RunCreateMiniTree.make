#-- start of make_header -----------------

#====================================
#  Application RunCreateMiniTree
#
#   Generated Mon Oct  9 06:40:57 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunCreateMiniTree_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunCreateMiniTree_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunCreateMiniTree

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunCreateMiniTree = /tmp/CMT_$(highlandIO_tag)_RunCreateMiniTree.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunCreateMiniTree = $(highlandIO_tag)_RunCreateMiniTree.make
cmt_local_tagfile_RunCreateMiniTree = $(bin)$(highlandIO_tag)_RunCreateMiniTree.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunCreateMiniTree = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunCreateMiniTree = $(highlandIO_tag).make
cmt_local_tagfile_RunCreateMiniTree = $(bin)$(highlandIO_tag).make
endif

endif

-include $(cmt_local_tagfile_RunCreateMiniTree)

ifdef cmt_RunCreateMiniTree_has_target_tag

ifdef READONLY
cmt_final_setup_RunCreateMiniTree = /tmp/CMT_highlandIO_RunCreateMiniTreesetup.make
cmt_local_RunCreateMiniTree_makefile = /tmp/CMT_RunCreateMiniTree$(cmt_lock_pid).make
else
cmt_final_setup_RunCreateMiniTree = $(bin)highlandIO_RunCreateMiniTreesetup.make
cmt_local_RunCreateMiniTree_makefile = $(bin)RunCreateMiniTree.make
endif

else

ifdef READONLY
cmt_final_setup_RunCreateMiniTree = /tmp/CMT_highlandIOsetup.make
cmt_local_RunCreateMiniTree_makefile = /tmp/CMT_RunCreateMiniTree$(cmt_lock_pid).make
else
cmt_final_setup_RunCreateMiniTree = $(bin)highlandIOsetup.make
cmt_local_RunCreateMiniTree_makefile = $(bin)RunCreateMiniTree.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandIOsetup.make
else
cmt_final_setup = $(bin)highlandIOsetup.make
endif

RunCreateMiniTree ::


ifdef READONLY
RunCreateMiniTree ::
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
	$(echo) 'RunCreateMiniTree'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunCreateMiniTree/
RunCreateMiniTree::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunCreateMiniTree :: dirs  $(bin)RunCreateMiniTree${application_suffix}
	$(echo) "RunCreateMiniTree ok"

#-- end of application_header
#-- start of application

$(bin)RunCreateMiniTree${application_suffix} :: $(bin)RunCreateMiniTree.o $(use_stamps) $(RunCreateMiniTreestamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunCreateMiniTree.o $(cmt_installarea_linkopts) $(RunCreateMiniTree_use_linkopts) $(RunCreateMiniTreelinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunCreateMiniTreeinstallname = RunCreateMiniTree${application_suffix}

RunCreateMiniTree :: RunCreateMiniTreeinstall

install :: RunCreateMiniTreeinstall

RunCreateMiniTreeinstall :: $(install_dir)/$(RunCreateMiniTreeinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunCreateMiniTreeinstallname) :: $(bin)$(RunCreateMiniTreeinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunCreateMiniTreeinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunCreateMiniTreeclean :: RunCreateMiniTreeuninstall

uninstall :: RunCreateMiniTreeuninstall

RunCreateMiniTreeuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunCreateMiniTreeinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunCreateMiniTree.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunCreateMiniTreeclean)

#$(bin)RunCreateMiniTree_dependencies.make :: dirs

ifndef QUICK
$(bin)RunCreateMiniTree_dependencies.make : ../app/RunCreateMiniTree.cxx $(use_requirements) $(cmt_final_setup_RunCreateMiniTree)
	$(echo) "(RunCreateMiniTree.make) Rebuilding $@"; \
	  $(build_dependencies) RunCreateMiniTree -all_sources -out=$@ ../app/RunCreateMiniTree.cxx
endif

#$(RunCreateMiniTree_dependencies)

-include $(bin)RunCreateMiniTree_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunCreateMiniTree_dependencies.make : $(RunCreateMiniTree_cxx_dependencies)

$(bin)$(binobj)RunCreateMiniTree.o : $(RunCreateMiniTree_cxx_dependencies)
	$(cpp_echo) ../app/RunCreateMiniTree.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunCreateMiniTree_pp_cppflags) $(app_RunCreateMiniTree_pp_cppflags) $(RunCreateMiniTree_pp_cppflags) $(use_cppflags) $(RunCreateMiniTree_cppflags) $(app_RunCreateMiniTree_cppflags) $(RunCreateMiniTree_cppflags) $(RunCreateMiniTree_cxx_cppflags) -I../app ../app/RunCreateMiniTree.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunCreateMiniTreeclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunCreateMiniTree.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunCreateMiniTree.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunCreateMiniTree)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunCreateMiniTree.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunCreateMiniTree.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunCreateMiniTree.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunCreateMiniTreeclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunCreateMiniTree${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunCreateMiniTree${application_suffix}

#	@echo "------> (RunCreateMiniTree.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunCreateMiniTree.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunCreateMiniTree_deps RunCreateMiniTree_dependencies.make
#-- end of cleanup_objects ------
