#-- start of make_header -----------------

#====================================
#  Library highlandCorrections
#
#   Generated Mon Oct  9 06:40:19 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_highlandCorrections_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_highlandCorrections_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_highlandCorrections

highlandCorrections_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandCorrections = /tmp/CMT_$(highlandCorrections_tag)_highlandCorrections.make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandCorrections = $(highlandCorrections_tag)_highlandCorrections.make
cmt_local_tagfile_highlandCorrections = $(bin)$(highlandCorrections_tag)_highlandCorrections.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandCorrections_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandCorrections = /tmp/CMT_$(highlandCorrections_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandCorrections = $(highlandCorrections_tag).make
cmt_local_tagfile_highlandCorrections = $(bin)$(highlandCorrections_tag).make
endif

endif

-include $(cmt_local_tagfile_highlandCorrections)

ifdef cmt_highlandCorrections_has_target_tag

ifdef READONLY
cmt_final_setup_highlandCorrections = /tmp/CMT_highlandCorrections_highlandCorrectionssetup.make
cmt_local_highlandCorrections_makefile = /tmp/CMT_highlandCorrections$(cmt_lock_pid).make
else
cmt_final_setup_highlandCorrections = $(bin)highlandCorrections_highlandCorrectionssetup.make
cmt_local_highlandCorrections_makefile = $(bin)highlandCorrections.make
endif

else

ifdef READONLY
cmt_final_setup_highlandCorrections = /tmp/CMT_highlandCorrectionssetup.make
cmt_local_highlandCorrections_makefile = /tmp/CMT_highlandCorrections$(cmt_lock_pid).make
else
cmt_final_setup_highlandCorrections = $(bin)highlandCorrectionssetup.make
cmt_local_highlandCorrections_makefile = $(bin)highlandCorrections.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandCorrectionssetup.make
else
cmt_final_setup = $(bin)highlandCorrectionssetup.make
endif

highlandCorrections ::


ifdef READONLY
highlandCorrections ::
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
	$(echo) 'highlandCorrections'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = highlandCorrections/
highlandCorrections::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

highlandCorrectionslibname   = $(bin)$(library_prefix)highlandCorrections$(library_suffix)
highlandCorrectionslib       = $(highlandCorrectionslibname).a
highlandCorrectionsstamp     = $(bin)highlandCorrections.stamp
highlandCorrectionsshstamp   = $(bin)highlandCorrections.shstamp

highlandCorrections :: dirs  highlandCorrectionsLIB
	$(echo) "highlandCorrections ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

highlandCorrectionsLIB :: $(highlandCorrectionslib) $(highlandCorrectionsshstamp)
	$(echo) "highlandCorrections : library ok"

$(highlandCorrectionslib) :: $(bin)CT4POTCorrection.o $(bin)DataQualityCorrection.o $(bin)FlipKinematicsCorrection.o $(bin)IgnoreRightECalRuns3and4Correction.o $(bin)MomRangeCorrection.o $(bin)MomResolMCCorrection.o $(bin)TPCExpecteddEdxCorrection.o $(bin)TPCPIDPullMCCorrection.o $(bin)TPCdEdxDataCorrection.o $(bin)TPCdEdxMCCorrection.o $(bin)ToFCorrection.o $(bin)UseGlobalAltMomCorrection.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(highlandCorrectionslib) $?
	$(lib_silent) $(ranlib) $(highlandCorrectionslib)
	$(lib_silent) cat /dev/null >$(highlandCorrectionsstamp)

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

$(highlandCorrectionslibname).$(shlibsuffix) :: $(highlandCorrectionslib) requirements $(use_requirements) $(highlandCorrectionsstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" highlandCorrections $(highlandCorrections_shlibflags)

$(highlandCorrectionsshstamp) :: $(highlandCorrectionslibname).$(shlibsuffix)
	$(lib_silent) if test -f $(highlandCorrectionslibname).$(shlibsuffix) ; then cat /dev/null >$(highlandCorrectionsshstamp) ; fi

highlandCorrectionsclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)CT4POTCorrection.o $(bin)DataQualityCorrection.o $(bin)FlipKinematicsCorrection.o $(bin)IgnoreRightECalRuns3and4Correction.o $(bin)MomRangeCorrection.o $(bin)MomResolMCCorrection.o $(bin)TPCExpecteddEdxCorrection.o $(bin)TPCPIDPullMCCorrection.o $(bin)TPCdEdxDataCorrection.o $(bin)TPCdEdxMCCorrection.o $(bin)ToFCorrection.o $(bin)UseGlobalAltMomCorrection.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf highlandCorrections_deps highlandCorrections_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
highlandCorrectionsinstallname = $(library_prefix)highlandCorrections$(library_suffix).$(shlibsuffix)

highlandCorrections :: highlandCorrectionsinstall

install :: highlandCorrectionsinstall

highlandCorrectionsinstall :: $(install_dir)/$(highlandCorrectionsinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(highlandCorrectionsinstallname) :: $(bin)$(highlandCorrectionsinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandCorrectionsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##highlandCorrectionsclean :: highlandCorrectionsuninstall

uninstall :: highlandCorrectionsuninstall

highlandCorrectionsuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandCorrectionsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),highlandCorrectionsclean)

#$(bin)highlandCorrections_dependencies.make :: dirs

ifndef QUICK
$(bin)highlandCorrections_dependencies.make : ../src/CT4POTCorrection.cxx ../src/DataQualityCorrection.cxx ../src/FlipKinematicsCorrection.cxx ../src/IgnoreRightECalRuns3and4Correction.cxx ../src/MomRangeCorrection.cxx ../src/MomResolMCCorrection.cxx ../src/TPCExpecteddEdxCorrection.cxx ../src/TPCPIDPullMCCorrection.cxx ../src/TPCdEdxDataCorrection.cxx ../src/TPCdEdxMCCorrection.cxx ../src/ToFCorrection.cxx ../src/UseGlobalAltMomCorrection.cxx $(use_requirements) $(cmt_final_setup_highlandCorrections)
	$(echo) "(highlandCorrections.make) Rebuilding $@"; \
	  $(build_dependencies) highlandCorrections -all_sources -out=$@ ../src/CT4POTCorrection.cxx ../src/DataQualityCorrection.cxx ../src/FlipKinematicsCorrection.cxx ../src/IgnoreRightECalRuns3and4Correction.cxx ../src/MomRangeCorrection.cxx ../src/MomResolMCCorrection.cxx ../src/TPCExpecteddEdxCorrection.cxx ../src/TPCPIDPullMCCorrection.cxx ../src/TPCdEdxDataCorrection.cxx ../src/TPCdEdxMCCorrection.cxx ../src/ToFCorrection.cxx ../src/UseGlobalAltMomCorrection.cxx
endif

#$(highlandCorrections_dependencies)

-include $(bin)highlandCorrections_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(CT4POTCorrection_cxx_dependencies)

$(bin)$(binobj)CT4POTCorrection.o : $(CT4POTCorrection_cxx_dependencies)
	$(cpp_echo) $(src)CT4POTCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(CT4POTCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(CT4POTCorrection_cppflags) $(CT4POTCorrection_cxx_cppflags)  $(src)CT4POTCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(DataQualityCorrection_cxx_dependencies)

$(bin)$(binobj)DataQualityCorrection.o : $(DataQualityCorrection_cxx_dependencies)
	$(cpp_echo) $(src)DataQualityCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(DataQualityCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(DataQualityCorrection_cppflags) $(DataQualityCorrection_cxx_cppflags)  $(src)DataQualityCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(FlipKinematicsCorrection_cxx_dependencies)

$(bin)$(binobj)FlipKinematicsCorrection.o : $(FlipKinematicsCorrection_cxx_dependencies)
	$(cpp_echo) $(src)FlipKinematicsCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(FlipKinematicsCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(FlipKinematicsCorrection_cppflags) $(FlipKinematicsCorrection_cxx_cppflags)  $(src)FlipKinematicsCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(IgnoreRightECalRuns3and4Correction_cxx_dependencies)

$(bin)$(binobj)IgnoreRightECalRuns3and4Correction.o : $(IgnoreRightECalRuns3and4Correction_cxx_dependencies)
	$(cpp_echo) $(src)IgnoreRightECalRuns3and4Correction.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(IgnoreRightECalRuns3and4Correction_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(IgnoreRightECalRuns3and4Correction_cppflags) $(IgnoreRightECalRuns3and4Correction_cxx_cppflags)  $(src)IgnoreRightECalRuns3and4Correction.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(MomRangeCorrection_cxx_dependencies)

$(bin)$(binobj)MomRangeCorrection.o : $(MomRangeCorrection_cxx_dependencies)
	$(cpp_echo) $(src)MomRangeCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(MomRangeCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(MomRangeCorrection_cppflags) $(MomRangeCorrection_cxx_cppflags)  $(src)MomRangeCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(MomResolMCCorrection_cxx_dependencies)

$(bin)$(binobj)MomResolMCCorrection.o : $(MomResolMCCorrection_cxx_dependencies)
	$(cpp_echo) $(src)MomResolMCCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(MomResolMCCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(MomResolMCCorrection_cppflags) $(MomResolMCCorrection_cxx_cppflags)  $(src)MomResolMCCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(TPCExpecteddEdxCorrection_cxx_dependencies)

$(bin)$(binobj)TPCExpecteddEdxCorrection.o : $(TPCExpecteddEdxCorrection_cxx_dependencies)
	$(cpp_echo) $(src)TPCExpecteddEdxCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(TPCExpecteddEdxCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(TPCExpecteddEdxCorrection_cppflags) $(TPCExpecteddEdxCorrection_cxx_cppflags)  $(src)TPCExpecteddEdxCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(TPCPIDPullMCCorrection_cxx_dependencies)

$(bin)$(binobj)TPCPIDPullMCCorrection.o : $(TPCPIDPullMCCorrection_cxx_dependencies)
	$(cpp_echo) $(src)TPCPIDPullMCCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(TPCPIDPullMCCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(TPCPIDPullMCCorrection_cppflags) $(TPCPIDPullMCCorrection_cxx_cppflags)  $(src)TPCPIDPullMCCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(TPCdEdxDataCorrection_cxx_dependencies)

$(bin)$(binobj)TPCdEdxDataCorrection.o : $(TPCdEdxDataCorrection_cxx_dependencies)
	$(cpp_echo) $(src)TPCdEdxDataCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(TPCdEdxDataCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(TPCdEdxDataCorrection_cppflags) $(TPCdEdxDataCorrection_cxx_cppflags)  $(src)TPCdEdxDataCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(TPCdEdxMCCorrection_cxx_dependencies)

$(bin)$(binobj)TPCdEdxMCCorrection.o : $(TPCdEdxMCCorrection_cxx_dependencies)
	$(cpp_echo) $(src)TPCdEdxMCCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(TPCdEdxMCCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(TPCdEdxMCCorrection_cppflags) $(TPCdEdxMCCorrection_cxx_cppflags)  $(src)TPCdEdxMCCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(ToFCorrection_cxx_dependencies)

$(bin)$(binobj)ToFCorrection.o : $(ToFCorrection_cxx_dependencies)
	$(cpp_echo) $(src)ToFCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(ToFCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(ToFCorrection_cppflags) $(ToFCorrection_cxx_cppflags)  $(src)ToFCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandCorrections_dependencies.make : $(UseGlobalAltMomCorrection_cxx_dependencies)

$(bin)$(binobj)UseGlobalAltMomCorrection.o : $(UseGlobalAltMomCorrection_cxx_dependencies)
	$(cpp_echo) $(src)UseGlobalAltMomCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandCorrections_pp_cppflags) $(lib_highlandCorrections_pp_cppflags) $(UseGlobalAltMomCorrection_pp_cppflags) $(use_cppflags) $(highlandCorrections_cppflags) $(lib_highlandCorrections_cppflags) $(UseGlobalAltMomCorrection_cppflags) $(UseGlobalAltMomCorrection_cxx_cppflags)  $(src)UseGlobalAltMomCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: highlandCorrectionsclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(highlandCorrections.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(highlandCorrections.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_highlandCorrections)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandCorrections.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandCorrections.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandCorrections.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

highlandCorrectionsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)highlandCorrections$(library_suffix).a $(binobj)$(library_prefix)highlandCorrections$(library_suffix).s? $(binobj)highlandCorrections.stamp $(binobj)highlandCorrections.shstamp
#-- end of cleanup_library ---------------

