#-- start of make_header -----------------

#====================================
#  Application EventDisplayND280
#
#   Generated Mon Oct  9 06:41:15 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_EventDisplayND280_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_EventDisplayND280_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_EventDisplayND280

baseAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_EventDisplayND280 = /tmp/CMT_$(baseAnalysis_tag)_EventDisplayND280.make$(cmt_lock_pid)
else
#cmt_local_tagfile_EventDisplayND280 = $(baseAnalysis_tag)_EventDisplayND280.make
cmt_local_tagfile_EventDisplayND280 = $(bin)$(baseAnalysis_tag)_EventDisplayND280.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

baseAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_EventDisplayND280 = /tmp/CMT_$(baseAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_EventDisplayND280 = $(baseAnalysis_tag).make
cmt_local_tagfile_EventDisplayND280 = $(bin)$(baseAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_EventDisplayND280)

ifdef cmt_EventDisplayND280_has_target_tag

ifdef READONLY
cmt_final_setup_EventDisplayND280 = /tmp/CMT_baseAnalysis_EventDisplayND280setup.make
cmt_local_EventDisplayND280_makefile = /tmp/CMT_EventDisplayND280$(cmt_lock_pid).make
else
cmt_final_setup_EventDisplayND280 = $(bin)baseAnalysis_EventDisplayND280setup.make
cmt_local_EventDisplayND280_makefile = $(bin)EventDisplayND280.make
endif

else

ifdef READONLY
cmt_final_setup_EventDisplayND280 = /tmp/CMT_baseAnalysissetup.make
cmt_local_EventDisplayND280_makefile = /tmp/CMT_EventDisplayND280$(cmt_lock_pid).make
else
cmt_final_setup_EventDisplayND280 = $(bin)baseAnalysissetup.make
cmt_local_EventDisplayND280_makefile = $(bin)EventDisplayND280.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_baseAnalysissetup.make
else
cmt_final_setup = $(bin)baseAnalysissetup.make
endif

EventDisplayND280 ::


ifdef READONLY
EventDisplayND280 ::
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
	$(echo) 'EventDisplayND280'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = EventDisplayND280/
EventDisplayND280::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

EventDisplayND280 :: dirs  $(bin)EventDisplayND280${application_suffix}
	$(echo) "EventDisplayND280 ok"

#-- end of application_header
#-- start of application

$(bin)EventDisplayND280${application_suffix} :: $(bin)EventDisplayND280.o $(use_stamps) $(EventDisplayND280stamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)EventDisplayND280.o $(cmt_installarea_linkopts) $(EventDisplayND280_use_linkopts) $(EventDisplayND280linkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
EventDisplayND280installname = EventDisplayND280${application_suffix}

EventDisplayND280 :: EventDisplayND280install

install :: EventDisplayND280install

EventDisplayND280install :: $(install_dir)/$(EventDisplayND280installname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(EventDisplayND280installname) :: $(bin)$(EventDisplayND280installname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(EventDisplayND280installname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##EventDisplayND280clean :: EventDisplayND280uninstall

uninstall :: EventDisplayND280uninstall

EventDisplayND280uninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(EventDisplayND280installname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (EventDisplayND280.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),EventDisplayND280clean)

#$(bin)EventDisplayND280_dependencies.make :: dirs

ifndef QUICK
$(bin)EventDisplayND280_dependencies.make : ../app/EventDisplayND280.cxx $(use_requirements) $(cmt_final_setup_EventDisplayND280)
	$(echo) "(EventDisplayND280.make) Rebuilding $@"; \
	  $(build_dependencies) EventDisplayND280 -all_sources -out=$@ ../app/EventDisplayND280.cxx
endif

#$(EventDisplayND280_dependencies)

-include $(bin)EventDisplayND280_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)EventDisplayND280_dependencies.make : $(EventDisplayND280_cxx_dependencies)

$(bin)$(binobj)EventDisplayND280.o : $(EventDisplayND280_cxx_dependencies)
	$(cpp_echo) ../app/EventDisplayND280.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(EventDisplayND280_pp_cppflags) $(app_EventDisplayND280_pp_cppflags) $(EventDisplayND280_pp_cppflags) $(use_cppflags) $(EventDisplayND280_cppflags) $(app_EventDisplayND280_cppflags) $(EventDisplayND280_cppflags) $(EventDisplayND280_cxx_cppflags) -I../app ../app/EventDisplayND280.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: EventDisplayND280clean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(EventDisplayND280.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(EventDisplayND280.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_EventDisplayND280)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(EventDisplayND280.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(EventDisplayND280.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(EventDisplayND280.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

EventDisplayND280clean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) EventDisplayND280${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f EventDisplayND280${application_suffix}

#	@echo "------> (EventDisplayND280.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)EventDisplayND280.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf EventDisplayND280_deps EventDisplayND280_dependencies.make
#-- end of cleanup_objects ------
