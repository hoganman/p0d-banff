#-- start of make_header -----------------

#====================================
#  Library highlandSystematics
#
#   Generated Mon Oct  9 06:40:16 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_highlandSystematics_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_highlandSystematics_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_highlandSystematics

highlandSystematics_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandSystematics = /tmp/CMT_$(highlandSystematics_tag)_highlandSystematics.make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandSystematics = $(highlandSystematics_tag)_highlandSystematics.make
cmt_local_tagfile_highlandSystematics = $(bin)$(highlandSystematics_tag)_highlandSystematics.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandSystematics_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandSystematics = /tmp/CMT_$(highlandSystematics_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandSystematics = $(highlandSystematics_tag).make
cmt_local_tagfile_highlandSystematics = $(bin)$(highlandSystematics_tag).make
endif

endif

-include $(cmt_local_tagfile_highlandSystematics)

ifdef cmt_highlandSystematics_has_target_tag

ifdef READONLY
cmt_final_setup_highlandSystematics = /tmp/CMT_highlandSystematics_highlandSystematicssetup.make
cmt_local_highlandSystematics_makefile = /tmp/CMT_highlandSystematics$(cmt_lock_pid).make
else
cmt_final_setup_highlandSystematics = $(bin)highlandSystematics_highlandSystematicssetup.make
cmt_local_highlandSystematics_makefile = $(bin)highlandSystematics.make
endif

else

ifdef READONLY
cmt_final_setup_highlandSystematics = /tmp/CMT_highlandSystematicssetup.make
cmt_local_highlandSystematics_makefile = /tmp/CMT_highlandSystematics$(cmt_lock_pid).make
else
cmt_final_setup_highlandSystematics = $(bin)highlandSystematicssetup.make
cmt_local_highlandSystematics_makefile = $(bin)highlandSystematics.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandSystematicssetup.make
else
cmt_final_setup = $(bin)highlandSystematicssetup.make
endif

highlandSystematics ::


ifdef READONLY
highlandSystematics ::
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
	$(echo) 'highlandSystematics'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = highlandSystematics/
highlandSystematics::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

highlandSystematicslibname   = $(bin)$(library_prefix)highlandSystematics$(library_suffix)
highlandSystematicslib       = $(highlandSystematicslibname).a
highlandSystematicsstamp     = $(bin)highlandSystematics.stamp
highlandSystematicsshstamp   = $(bin)highlandSystematics.shstamp

highlandSystematics :: dirs  highlandSystematicsLIB
	$(echo) "highlandSystematics ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

highlandSystematicsLIB :: $(highlandSystematicslib) $(highlandSystematicsshstamp)
	$(echo) "highlandSystematics : library ok"

$(highlandSystematicslib) :: $(bin)NuParentDecayPosSystematics.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(highlandSystematicslib) $?
	$(lib_silent) $(ranlib) $(highlandSystematicslib)
	$(lib_silent) cat /dev/null >$(highlandSystematicsstamp)

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

$(highlandSystematicslibname).$(shlibsuffix) :: $(highlandSystematicslib) requirements $(use_requirements) $(highlandSystematicsstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" highlandSystematics $(highlandSystematics_shlibflags)

$(highlandSystematicsshstamp) :: $(highlandSystematicslibname).$(shlibsuffix)
	$(lib_silent) if test -f $(highlandSystematicslibname).$(shlibsuffix) ; then cat /dev/null >$(highlandSystematicsshstamp) ; fi

highlandSystematicsclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)NuParentDecayPosSystematics.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf highlandSystematics_deps highlandSystematics_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
highlandSystematicsinstallname = $(library_prefix)highlandSystematics$(library_suffix).$(shlibsuffix)

highlandSystematics :: highlandSystematicsinstall

install :: highlandSystematicsinstall

highlandSystematicsinstall :: $(install_dir)/$(highlandSystematicsinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(highlandSystematicsinstallname) :: $(bin)$(highlandSystematicsinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandSystematicsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##highlandSystematicsclean :: highlandSystematicsuninstall

uninstall :: highlandSystematicsuninstall

highlandSystematicsuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandSystematicsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),highlandSystematicsclean)

#$(bin)highlandSystematics_dependencies.make :: dirs

ifndef QUICK
$(bin)highlandSystematics_dependencies.make : ../src/NuParentDecayPosSystematics.cxx $(use_requirements) $(cmt_final_setup_highlandSystematics)
	$(echo) "(highlandSystematics.make) Rebuilding $@"; \
	  $(build_dependencies) highlandSystematics -all_sources -out=$@ ../src/NuParentDecayPosSystematics.cxx
endif

#$(highlandSystematics_dependencies)

-include $(bin)highlandSystematics_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)highlandSystematics_dependencies.make : $(NuParentDecayPosSystematics_cxx_dependencies)

$(bin)$(binobj)NuParentDecayPosSystematics.o : $(NuParentDecayPosSystematics_cxx_dependencies)
	$(cpp_echo) $(src)NuParentDecayPosSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandSystematics_pp_cppflags) $(lib_highlandSystematics_pp_cppflags) $(NuParentDecayPosSystematics_pp_cppflags) $(use_cppflags) $(highlandSystematics_cppflags) $(lib_highlandSystematics_cppflags) $(NuParentDecayPosSystematics_cppflags) $(NuParentDecayPosSystematics_cxx_cppflags)  $(src)NuParentDecayPosSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: highlandSystematicsclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(highlandSystematics.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(highlandSystematics.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_highlandSystematics)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandSystematics.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandSystematics.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandSystematics.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

highlandSystematicsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)highlandSystematics$(library_suffix).a $(binobj)$(library_prefix)highlandSystematics$(library_suffix).s? $(binobj)highlandSystematics.stamp $(binobj)highlandSystematics.shstamp
#-- end of cleanup_library ---------------

