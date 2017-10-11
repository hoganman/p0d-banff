#-- start of make_header -----------------

#====================================
#  Library psycheSteering
#
#   Generated Mon Oct  9 06:43:43 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_psycheSteering_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_psycheSteering_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_psycheSteering

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheSteering = /tmp/CMT_$(psycheSteering_tag)_psycheSteering.make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheSteering = $(psycheSteering_tag)_psycheSteering.make
cmt_local_tagfile_psycheSteering = $(bin)$(psycheSteering_tag)_psycheSteering.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheSteering = /tmp/CMT_$(psycheSteering_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheSteering = $(psycheSteering_tag).make
cmt_local_tagfile_psycheSteering = $(bin)$(psycheSteering_tag).make
endif

endif

-include $(cmt_local_tagfile_psycheSteering)

ifdef cmt_psycheSteering_has_target_tag

ifdef READONLY
cmt_final_setup_psycheSteering = /tmp/CMT_psycheSteering_psycheSteeringsetup.make
cmt_local_psycheSteering_makefile = /tmp/CMT_psycheSteering$(cmt_lock_pid).make
else
cmt_final_setup_psycheSteering = $(bin)psycheSteering_psycheSteeringsetup.make
cmt_local_psycheSteering_makefile = $(bin)psycheSteering.make
endif

else

ifdef READONLY
cmt_final_setup_psycheSteering = /tmp/CMT_psycheSteeringsetup.make
cmt_local_psycheSteering_makefile = /tmp/CMT_psycheSteering$(cmt_lock_pid).make
else
cmt_final_setup_psycheSteering = $(bin)psycheSteeringsetup.make
cmt_local_psycheSteering_makefile = $(bin)psycheSteering.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheSteeringsetup.make
else
cmt_final_setup = $(bin)psycheSteeringsetup.make
endif

psycheSteering ::


ifdef READONLY
psycheSteering ::
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
	$(echo) 'psycheSteering'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = psycheSteering/
psycheSteering::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

psycheSteeringlibname   = $(bin)$(library_prefix)psycheSteering$(library_suffix)
psycheSteeringlib       = $(psycheSteeringlibname).a
psycheSteeringstamp     = $(bin)psycheSteering.stamp
psycheSteeringshstamp   = $(bin)psycheSteering.shstamp

psycheSteering :: dirs  psycheSteeringLIB
	$(echo) "psycheSteering ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

psycheSteeringLIB :: $(psycheSteeringlib) $(psycheSteeringshstamp)
	$(echo) "psycheSteering : library ok"

$(psycheSteeringlib) :: $(bin)AnalysisManager.o $(bin)ToyMakerExample.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(psycheSteeringlib) $?
	$(lib_silent) $(ranlib) $(psycheSteeringlib)
	$(lib_silent) cat /dev/null >$(psycheSteeringstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

#
# We add one level of dependency upon the true shared library 
# (rather than simply upon the stamp file)
# this is for cases where the shared library has not been built
# while the stamp was created (error??) 
#

$(psycheSteeringlibname).$(shlibsuffix) :: $(psycheSteeringlib) requirements $(use_requirements) $(psycheSteeringstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" psycheSteering $(psycheSteering_shlibflags)

$(psycheSteeringshstamp) :: $(psycheSteeringlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(psycheSteeringlibname).$(shlibsuffix) ; then cat /dev/null >$(psycheSteeringshstamp) ; fi

psycheSteeringclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)AnalysisManager.o $(bin)ToyMakerExample.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf psycheSteering_deps psycheSteering_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
psycheSteeringinstallname = $(library_prefix)psycheSteering$(library_suffix).$(shlibsuffix)

psycheSteering :: psycheSteeringinstall

install :: psycheSteeringinstall

psycheSteeringinstall :: $(install_dir)/$(psycheSteeringinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(psycheSteeringinstallname) :: $(bin)$(psycheSteeringinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheSteeringinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##psycheSteeringclean :: psycheSteeringuninstall

uninstall :: psycheSteeringuninstall

psycheSteeringuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheSteeringinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),psycheSteeringclean)

#$(bin)psycheSteering_dependencies.make :: dirs

ifndef QUICK
$(bin)psycheSteering_dependencies.make : ../src/AnalysisManager.cxx ../src/ToyMakerExample.cxx $(use_requirements) $(cmt_final_setup_psycheSteering)
	$(echo) "(psycheSteering.make) Rebuilding $@"; \
	  $(build_dependencies) psycheSteering -all_sources -out=$@ ../src/AnalysisManager.cxx ../src/ToyMakerExample.cxx
endif

#$(psycheSteering_dependencies)

-include $(bin)psycheSteering_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)psycheSteering_dependencies.make : $(AnalysisManager_cxx_dependencies)

$(bin)$(binobj)AnalysisManager.o : $(AnalysisManager_cxx_dependencies)
	$(cpp_echo) $(src)AnalysisManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSteering_pp_cppflags) $(lib_psycheSteering_pp_cppflags) $(AnalysisManager_pp_cppflags) $(use_cppflags) $(psycheSteering_cppflags) $(lib_psycheSteering_cppflags) $(AnalysisManager_cppflags) $(AnalysisManager_cxx_cppflags)  $(src)AnalysisManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheSteering_dependencies.make : $(ToyMakerExample_cxx_dependencies)

$(bin)$(binobj)ToyMakerExample.o : $(ToyMakerExample_cxx_dependencies)
	$(cpp_echo) $(src)ToyMakerExample.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheSteering_pp_cppflags) $(lib_psycheSteering_pp_cppflags) $(ToyMakerExample_pp_cppflags) $(use_cppflags) $(psycheSteering_cppflags) $(lib_psycheSteering_cppflags) $(ToyMakerExample_cppflags) $(ToyMakerExample_cxx_cppflags)  $(src)ToyMakerExample.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: psycheSteeringclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(psycheSteering.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(psycheSteering.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_psycheSteering)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheSteering.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheSteering.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheSteering.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

psycheSteeringclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)psycheSteering$(library_suffix).a $(binobj)$(library_prefix)psycheSteering$(library_suffix).s? $(binobj)psycheSteering.stamp $(binobj)psycheSteering.shstamp
#-- end of cleanup_library ---------------

