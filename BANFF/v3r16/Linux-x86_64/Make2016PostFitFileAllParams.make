#-- start of make_header -----------------

#====================================
#  Application Make2016PostFitFileAllParams
#
#   Generated Wed Oct  4 12:22:20 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_Make2016PostFitFileAllParams_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_Make2016PostFitFileAllParams_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_Make2016PostFitFileAllParams

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Make2016PostFitFileAllParams = /tmp/CMT_$(BANFF_tag)_Make2016PostFitFileAllParams.make$(cmt_lock_pid)
else
#cmt_local_tagfile_Make2016PostFitFileAllParams = $(BANFF_tag)_Make2016PostFitFileAllParams.make
cmt_local_tagfile_Make2016PostFitFileAllParams = $(bin)$(BANFF_tag)_Make2016PostFitFileAllParams.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_Make2016PostFitFileAllParams = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_Make2016PostFitFileAllParams = $(BANFF_tag).make
cmt_local_tagfile_Make2016PostFitFileAllParams = $(bin)$(BANFF_tag).make
endif

endif

-include $(cmt_local_tagfile_Make2016PostFitFileAllParams)

ifdef cmt_Make2016PostFitFileAllParams_has_target_tag

ifdef READONLY
cmt_final_setup_Make2016PostFitFileAllParams = /tmp/CMT_BANFF_Make2016PostFitFileAllParamssetup.make
cmt_local_Make2016PostFitFileAllParams_makefile = /tmp/CMT_Make2016PostFitFileAllParams$(cmt_lock_pid).make
else
cmt_final_setup_Make2016PostFitFileAllParams = $(bin)BANFF_Make2016PostFitFileAllParamssetup.make
cmt_local_Make2016PostFitFileAllParams_makefile = $(bin)Make2016PostFitFileAllParams.make
endif

else

ifdef READONLY
cmt_final_setup_Make2016PostFitFileAllParams = /tmp/CMT_BANFFsetup.make
cmt_local_Make2016PostFitFileAllParams_makefile = /tmp/CMT_Make2016PostFitFileAllParams$(cmt_lock_pid).make
else
cmt_final_setup_Make2016PostFitFileAllParams = $(bin)BANFFsetup.make
cmt_local_Make2016PostFitFileAllParams_makefile = $(bin)Make2016PostFitFileAllParams.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_BANFFsetup.make
else
cmt_final_setup = $(bin)BANFFsetup.make
endif

Make2016PostFitFileAllParams ::


ifdef READONLY
Make2016PostFitFileAllParams ::
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
	$(echo) 'Make2016PostFitFileAllParams'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = Make2016PostFitFileAllParams/
Make2016PostFitFileAllParams::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

Make2016PostFitFileAllParams :: dirs  $(bin)Make2016PostFitFileAllParams${application_suffix}
	$(echo) "Make2016PostFitFileAllParams ok"

#-- end of application_header
#-- start of application

$(bin)Make2016PostFitFileAllParams${application_suffix} :: $(bin)Make2016PostFitFileAllParams.o $(use_stamps) $(Make2016PostFitFileAllParamsstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)Make2016PostFitFileAllParams.o $(cmt_installarea_linkopts) $(Make2016PostFitFileAllParams_use_linkopts) $(Make2016PostFitFileAllParamslinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
Make2016PostFitFileAllParamsinstallname = Make2016PostFitFileAllParams${application_suffix}

Make2016PostFitFileAllParams :: Make2016PostFitFileAllParamsinstall

install :: Make2016PostFitFileAllParamsinstall

Make2016PostFitFileAllParamsinstall :: $(install_dir)/$(Make2016PostFitFileAllParamsinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(Make2016PostFitFileAllParamsinstallname) :: $(bin)$(Make2016PostFitFileAllParamsinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Make2016PostFitFileAllParamsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##Make2016PostFitFileAllParamsclean :: Make2016PostFitFileAllParamsuninstall

uninstall :: Make2016PostFitFileAllParamsuninstall

Make2016PostFitFileAllParamsuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(Make2016PostFitFileAllParamsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (Make2016PostFitFileAllParams.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),Make2016PostFitFileAllParamsclean)

#$(bin)Make2016PostFitFileAllParams_dependencies.make :: dirs

ifndef QUICK
$(bin)Make2016PostFitFileAllParams_dependencies.make : ../app/Make2016PostFitFileAllParams.cxx $(use_requirements) $(cmt_final_setup_Make2016PostFitFileAllParams)
	$(echo) "(Make2016PostFitFileAllParams.make) Rebuilding $@"; \
	  $(build_dependencies) Make2016PostFitFileAllParams -all_sources -out=$@ ../app/Make2016PostFitFileAllParams.cxx
endif

#$(Make2016PostFitFileAllParams_dependencies)

-include $(bin)Make2016PostFitFileAllParams_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)Make2016PostFitFileAllParams_dependencies.make : $(Make2016PostFitFileAllParams_cxx_dependencies)

$(bin)$(binobj)Make2016PostFitFileAllParams.o : $(Make2016PostFitFileAllParams_cxx_dependencies)
	$(cpp_echo) ../app/Make2016PostFitFileAllParams.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(Make2016PostFitFileAllParams_pp_cppflags) $(app_Make2016PostFitFileAllParams_pp_cppflags) $(Make2016PostFitFileAllParams_pp_cppflags) $(use_cppflags) $(Make2016PostFitFileAllParams_cppflags) $(app_Make2016PostFitFileAllParams_cppflags) $(Make2016PostFitFileAllParams_cppflags) $(Make2016PostFitFileAllParams_cxx_cppflags) -I../app ../app/Make2016PostFitFileAllParams.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: Make2016PostFitFileAllParamsclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(Make2016PostFitFileAllParams.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(Make2016PostFitFileAllParams.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_Make2016PostFitFileAllParams)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(Make2016PostFitFileAllParams.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(Make2016PostFitFileAllParams.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(Make2016PostFitFileAllParams.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

Make2016PostFitFileAllParamsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) Make2016PostFitFileAllParams${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f Make2016PostFitFileAllParams${application_suffix}

#	@echo "------> (Make2016PostFitFileAllParams.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)Make2016PostFitFileAllParams.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf Make2016PostFitFileAllParams_deps Make2016PostFitFileAllParams_dependencies.make
#-- end of cleanup_objects ------
