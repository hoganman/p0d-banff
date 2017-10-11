#-- start of make_header -----------------

#====================================
#  Application RunCreateFlatTree
#
#   Generated Mon Oct  9 06:40:54 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunCreateFlatTree_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunCreateFlatTree_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunCreateFlatTree

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunCreateFlatTree = /tmp/CMT_$(highlandIO_tag)_RunCreateFlatTree.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunCreateFlatTree = $(highlandIO_tag)_RunCreateFlatTree.make
cmt_local_tagfile_RunCreateFlatTree = $(bin)$(highlandIO_tag)_RunCreateFlatTree.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunCreateFlatTree = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunCreateFlatTree = $(highlandIO_tag).make
cmt_local_tagfile_RunCreateFlatTree = $(bin)$(highlandIO_tag).make
endif

endif

-include $(cmt_local_tagfile_RunCreateFlatTree)

ifdef cmt_RunCreateFlatTree_has_target_tag

ifdef READONLY
cmt_final_setup_RunCreateFlatTree = /tmp/CMT_highlandIO_RunCreateFlatTreesetup.make
cmt_local_RunCreateFlatTree_makefile = /tmp/CMT_RunCreateFlatTree$(cmt_lock_pid).make
else
cmt_final_setup_RunCreateFlatTree = $(bin)highlandIO_RunCreateFlatTreesetup.make
cmt_local_RunCreateFlatTree_makefile = $(bin)RunCreateFlatTree.make
endif

else

ifdef READONLY
cmt_final_setup_RunCreateFlatTree = /tmp/CMT_highlandIOsetup.make
cmt_local_RunCreateFlatTree_makefile = /tmp/CMT_RunCreateFlatTree$(cmt_lock_pid).make
else
cmt_final_setup_RunCreateFlatTree = $(bin)highlandIOsetup.make
cmt_local_RunCreateFlatTree_makefile = $(bin)RunCreateFlatTree.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandIOsetup.make
else
cmt_final_setup = $(bin)highlandIOsetup.make
endif

RunCreateFlatTree ::


ifdef READONLY
RunCreateFlatTree ::
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
	$(echo) 'RunCreateFlatTree'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunCreateFlatTree/
RunCreateFlatTree::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunCreateFlatTree :: dirs  $(bin)RunCreateFlatTree${application_suffix}
	$(echo) "RunCreateFlatTree ok"

#-- end of application_header
#-- start of application

$(bin)RunCreateFlatTree${application_suffix} :: $(bin)RunCreateFlatTree.o $(use_stamps) $(RunCreateFlatTreestamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunCreateFlatTree.o $(cmt_installarea_linkopts) $(RunCreateFlatTree_use_linkopts) $(RunCreateFlatTreelinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunCreateFlatTreeinstallname = RunCreateFlatTree${application_suffix}

RunCreateFlatTree :: RunCreateFlatTreeinstall

install :: RunCreateFlatTreeinstall

RunCreateFlatTreeinstall :: $(install_dir)/$(RunCreateFlatTreeinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunCreateFlatTreeinstallname) :: $(bin)$(RunCreateFlatTreeinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunCreateFlatTreeinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunCreateFlatTreeclean :: RunCreateFlatTreeuninstall

uninstall :: RunCreateFlatTreeuninstall

RunCreateFlatTreeuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunCreateFlatTreeinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunCreateFlatTree.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunCreateFlatTreeclean)

#$(bin)RunCreateFlatTree_dependencies.make :: dirs

ifndef QUICK
$(bin)RunCreateFlatTree_dependencies.make : ../app/RunCreateFlatTree.cxx $(use_requirements) $(cmt_final_setup_RunCreateFlatTree)
	$(echo) "(RunCreateFlatTree.make) Rebuilding $@"; \
	  $(build_dependencies) RunCreateFlatTree -all_sources -out=$@ ../app/RunCreateFlatTree.cxx
endif

#$(RunCreateFlatTree_dependencies)

-include $(bin)RunCreateFlatTree_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunCreateFlatTree_dependencies.make : $(RunCreateFlatTree_cxx_dependencies)

$(bin)$(binobj)RunCreateFlatTree.o : $(RunCreateFlatTree_cxx_dependencies)
	$(cpp_echo) ../app/RunCreateFlatTree.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunCreateFlatTree_pp_cppflags) $(app_RunCreateFlatTree_pp_cppflags) $(RunCreateFlatTree_pp_cppflags) $(use_cppflags) $(RunCreateFlatTree_cppflags) $(app_RunCreateFlatTree_cppflags) $(RunCreateFlatTree_cppflags) $(RunCreateFlatTree_cxx_cppflags) -I../app ../app/RunCreateFlatTree.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunCreateFlatTreeclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunCreateFlatTree.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunCreateFlatTree.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunCreateFlatTree)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunCreateFlatTree.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunCreateFlatTree.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunCreateFlatTree.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunCreateFlatTreeclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunCreateFlatTree${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunCreateFlatTree${application_suffix}

#	@echo "------> (RunCreateFlatTree.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunCreateFlatTree.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunCreateFlatTree_deps RunCreateFlatTree_dependencies.make
#-- end of cleanup_objects ------
