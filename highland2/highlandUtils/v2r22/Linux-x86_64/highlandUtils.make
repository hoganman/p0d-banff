#-- start of make_header -----------------

#====================================
#  Library highlandUtils
#
#   Generated Mon Oct  9 06:39:30 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_highlandUtils_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_highlandUtils_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_highlandUtils

highlandUtils_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandUtils = /tmp/CMT_$(highlandUtils_tag)_highlandUtils.make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandUtils = $(highlandUtils_tag)_highlandUtils.make
cmt_local_tagfile_highlandUtils = $(bin)$(highlandUtils_tag)_highlandUtils.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandUtils_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandUtils = /tmp/CMT_$(highlandUtils_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandUtils = $(highlandUtils_tag).make
cmt_local_tagfile_highlandUtils = $(bin)$(highlandUtils_tag).make
endif

endif

-include $(cmt_local_tagfile_highlandUtils)

ifdef cmt_highlandUtils_has_target_tag

ifdef READONLY
cmt_final_setup_highlandUtils = /tmp/CMT_highlandUtils_highlandUtilssetup.make
cmt_local_highlandUtils_makefile = /tmp/CMT_highlandUtils$(cmt_lock_pid).make
else
cmt_final_setup_highlandUtils = $(bin)highlandUtils_highlandUtilssetup.make
cmt_local_highlandUtils_makefile = $(bin)highlandUtils.make
endif

else

ifdef READONLY
cmt_final_setup_highlandUtils = /tmp/CMT_highlandUtilssetup.make
cmt_local_highlandUtils_makefile = /tmp/CMT_highlandUtils$(cmt_lock_pid).make
else
cmt_final_setup_highlandUtils = $(bin)highlandUtilssetup.make
cmt_local_highlandUtils_makefile = $(bin)highlandUtils.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandUtilssetup.make
else
cmt_final_setup = $(bin)highlandUtilssetup.make
endif

highlandUtils ::


ifdef READONLY
highlandUtils ::
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
	$(echo) 'highlandUtils'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = highlandUtils/
highlandUtils::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

highlandUtilslibname   = $(bin)$(library_prefix)highlandUtils$(library_suffix)
highlandUtilslib       = $(highlandUtilslibname).a
highlandUtilsstamp     = $(bin)highlandUtils.stamp
highlandUtilsshstamp   = $(bin)highlandUtils.shstamp

highlandUtils :: dirs  highlandUtilsLIB
	$(echo) "highlandUtils ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

highlandUtilsLIB :: $(highlandUtilslib) $(highlandUtilsshstamp)
	$(echo) "highlandUtils : library ok"

$(highlandUtilslib) :: $(bin)CategToolsNueCC.o $(bin)CategoriesUtils.o $(bin)HighlandAnalysisUtils.o $(bin)HighlandConstituentsUtils.o $(bin)HighlandPIDUtils.o $(bin)HighlandTreeConverterUtils.o $(bin)IntersectionUtils.o $(bin)ND280GeomId.o $(bin)P0DAnalysisUtils.o $(bin)P0DGeometryManager.o $(bin)P0DMuonProtonDisc.o $(bin)ProductionUtils.o $(bin)RooTrackerVtxManager.o $(bin)TECALPidDefs.o $(bin)TECALPidLikelihood.o $(bin)TECALPidLikelihoodHypothesisTest.o $(bin)TTruthUtils.o $(bin)BitUtils.o $(bin)ControlPanel.o $(bin)DetectorForager.o $(bin)EventPlotter.o $(bin)EventReader.o $(bin)ControlPanel_Dict.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(highlandUtilslib) $?
	$(lib_silent) $(ranlib) $(highlandUtilslib)
	$(lib_silent) cat /dev/null >$(highlandUtilsstamp)

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

$(highlandUtilslibname).$(shlibsuffix) :: $(highlandUtilslib) requirements $(use_requirements) $(highlandUtilsstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" highlandUtils $(highlandUtils_shlibflags)

$(highlandUtilsshstamp) :: $(highlandUtilslibname).$(shlibsuffix)
	$(lib_silent) if test -f $(highlandUtilslibname).$(shlibsuffix) ; then cat /dev/null >$(highlandUtilsshstamp) ; fi

highlandUtilsclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)CategToolsNueCC.o $(bin)CategoriesUtils.o $(bin)HighlandAnalysisUtils.o $(bin)HighlandConstituentsUtils.o $(bin)HighlandPIDUtils.o $(bin)HighlandTreeConverterUtils.o $(bin)IntersectionUtils.o $(bin)ND280GeomId.o $(bin)P0DAnalysisUtils.o $(bin)P0DGeometryManager.o $(bin)P0DMuonProtonDisc.o $(bin)ProductionUtils.o $(bin)RooTrackerVtxManager.o $(bin)TECALPidDefs.o $(bin)TECALPidLikelihood.o $(bin)TECALPidLikelihoodHypothesisTest.o $(bin)TTruthUtils.o $(bin)BitUtils.o $(bin)ControlPanel.o $(bin)DetectorForager.o $(bin)EventPlotter.o $(bin)EventReader.o $(bin)ControlPanel_Dict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf highlandUtils_deps highlandUtils_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
highlandUtilsinstallname = $(library_prefix)highlandUtils$(library_suffix).$(shlibsuffix)

highlandUtils :: highlandUtilsinstall

install :: highlandUtilsinstall

highlandUtilsinstall :: $(install_dir)/$(highlandUtilsinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(highlandUtilsinstallname) :: $(bin)$(highlandUtilsinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandUtilsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##highlandUtilsclean :: highlandUtilsuninstall

uninstall :: highlandUtilsuninstall

highlandUtilsuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandUtilsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),highlandUtilsclean)

#$(bin)highlandUtils_dependencies.make :: dirs

ifndef QUICK
$(bin)highlandUtils_dependencies.make : ../src/CategToolsNueCC.cxx ../src/CategoriesUtils.cxx ../src/HighlandAnalysisUtils.cxx ../src/HighlandConstituentsUtils.cxx ../src/HighlandPIDUtils.cxx ../src/HighlandTreeConverterUtils.cxx ../src/IntersectionUtils.cxx ../src/ND280GeomId.cxx ../src/P0DAnalysisUtils.cxx ../src/P0DGeometryManager.cxx ../src/P0DMuonProtonDisc.cxx ../src/ProductionUtils.cxx ../src/RooTrackerVtxManager.cxx ../src/TECALPidDefs.cxx ../src/TECALPidLikelihood.cxx ../src/TECALPidLikelihoodHypothesisTest.cxx ../src/TTruthUtils.cxx ../src/EventDisplay/BitUtils.cxx ../src/EventDisplay/ControlPanel.cxx ../src/EventDisplay/DetectorForager.cxx ../src/EventDisplay/EventPlotter.cxx ../src/EventDisplay/EventReader.cxx ../dict/ControlPanel_Dict.cxx $(use_requirements) $(cmt_final_setup_highlandUtils)
	$(echo) "(highlandUtils.make) Rebuilding $@"; \
	  $(build_dependencies) highlandUtils -all_sources -out=$@ ../src/CategToolsNueCC.cxx ../src/CategoriesUtils.cxx ../src/HighlandAnalysisUtils.cxx ../src/HighlandConstituentsUtils.cxx ../src/HighlandPIDUtils.cxx ../src/HighlandTreeConverterUtils.cxx ../src/IntersectionUtils.cxx ../src/ND280GeomId.cxx ../src/P0DAnalysisUtils.cxx ../src/P0DGeometryManager.cxx ../src/P0DMuonProtonDisc.cxx ../src/ProductionUtils.cxx ../src/RooTrackerVtxManager.cxx ../src/TECALPidDefs.cxx ../src/TECALPidLikelihood.cxx ../src/TECALPidLikelihoodHypothesisTest.cxx ../src/TTruthUtils.cxx ../src/EventDisplay/BitUtils.cxx ../src/EventDisplay/ControlPanel.cxx ../src/EventDisplay/DetectorForager.cxx ../src/EventDisplay/EventPlotter.cxx ../src/EventDisplay/EventReader.cxx ../dict/ControlPanel_Dict.cxx
endif

#$(highlandUtils_dependencies)

-include $(bin)highlandUtils_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(CategToolsNueCC_cxx_dependencies)

$(bin)$(binobj)CategToolsNueCC.o : $(CategToolsNueCC_cxx_dependencies)
	$(cpp_echo) $(src)CategToolsNueCC.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(CategToolsNueCC_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(CategToolsNueCC_cppflags) $(CategToolsNueCC_cxx_cppflags)  $(src)CategToolsNueCC.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(CategoriesUtils_cxx_dependencies)

$(bin)$(binobj)CategoriesUtils.o : $(CategoriesUtils_cxx_dependencies)
	$(cpp_echo) $(src)CategoriesUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(CategoriesUtils_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(CategoriesUtils_cppflags) $(CategoriesUtils_cxx_cppflags)  $(src)CategoriesUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(HighlandAnalysisUtils_cxx_dependencies)

$(bin)$(binobj)HighlandAnalysisUtils.o : $(HighlandAnalysisUtils_cxx_dependencies)
	$(cpp_echo) $(src)HighlandAnalysisUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(HighlandAnalysisUtils_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(HighlandAnalysisUtils_cppflags) $(HighlandAnalysisUtils_cxx_cppflags)  $(src)HighlandAnalysisUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(HighlandConstituentsUtils_cxx_dependencies)

$(bin)$(binobj)HighlandConstituentsUtils.o : $(HighlandConstituentsUtils_cxx_dependencies)
	$(cpp_echo) $(src)HighlandConstituentsUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(HighlandConstituentsUtils_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(HighlandConstituentsUtils_cppflags) $(HighlandConstituentsUtils_cxx_cppflags)  $(src)HighlandConstituentsUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(HighlandPIDUtils_cxx_dependencies)

$(bin)$(binobj)HighlandPIDUtils.o : $(HighlandPIDUtils_cxx_dependencies)
	$(cpp_echo) $(src)HighlandPIDUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(HighlandPIDUtils_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(HighlandPIDUtils_cppflags) $(HighlandPIDUtils_cxx_cppflags)  $(src)HighlandPIDUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(HighlandTreeConverterUtils_cxx_dependencies)

$(bin)$(binobj)HighlandTreeConverterUtils.o : $(HighlandTreeConverterUtils_cxx_dependencies)
	$(cpp_echo) $(src)HighlandTreeConverterUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(HighlandTreeConverterUtils_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(HighlandTreeConverterUtils_cppflags) $(HighlandTreeConverterUtils_cxx_cppflags)  $(src)HighlandTreeConverterUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(IntersectionUtils_cxx_dependencies)

$(bin)$(binobj)IntersectionUtils.o : $(IntersectionUtils_cxx_dependencies)
	$(cpp_echo) $(src)IntersectionUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(IntersectionUtils_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(IntersectionUtils_cppflags) $(IntersectionUtils_cxx_cppflags)  $(src)IntersectionUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(ND280GeomId_cxx_dependencies)

$(bin)$(binobj)ND280GeomId.o : $(ND280GeomId_cxx_dependencies)
	$(cpp_echo) $(src)ND280GeomId.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(ND280GeomId_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(ND280GeomId_cppflags) $(ND280GeomId_cxx_cppflags)  $(src)ND280GeomId.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(P0DAnalysisUtils_cxx_dependencies)

$(bin)$(binobj)P0DAnalysisUtils.o : $(P0DAnalysisUtils_cxx_dependencies)
	$(cpp_echo) $(src)P0DAnalysisUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(P0DAnalysisUtils_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(P0DAnalysisUtils_cppflags) $(P0DAnalysisUtils_cxx_cppflags)  $(src)P0DAnalysisUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(P0DGeometryManager_cxx_dependencies)

$(bin)$(binobj)P0DGeometryManager.o : $(P0DGeometryManager_cxx_dependencies)
	$(cpp_echo) $(src)P0DGeometryManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(P0DGeometryManager_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(P0DGeometryManager_cppflags) $(P0DGeometryManager_cxx_cppflags)  $(src)P0DGeometryManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(P0DMuonProtonDisc_cxx_dependencies)

$(bin)$(binobj)P0DMuonProtonDisc.o : $(P0DMuonProtonDisc_cxx_dependencies)
	$(cpp_echo) $(src)P0DMuonProtonDisc.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(P0DMuonProtonDisc_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(P0DMuonProtonDisc_cppflags) $(P0DMuonProtonDisc_cxx_cppflags)  $(src)P0DMuonProtonDisc.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(ProductionUtils_cxx_dependencies)

$(bin)$(binobj)ProductionUtils.o : $(ProductionUtils_cxx_dependencies)
	$(cpp_echo) $(src)ProductionUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(ProductionUtils_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(ProductionUtils_cppflags) $(ProductionUtils_cxx_cppflags)  $(src)ProductionUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(RooTrackerVtxManager_cxx_dependencies)

$(bin)$(binobj)RooTrackerVtxManager.o : $(RooTrackerVtxManager_cxx_dependencies)
	$(cpp_echo) $(src)RooTrackerVtxManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(RooTrackerVtxManager_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(RooTrackerVtxManager_cppflags) $(RooTrackerVtxManager_cxx_cppflags)  $(src)RooTrackerVtxManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(TECALPidDefs_cxx_dependencies)

$(bin)$(binobj)TECALPidDefs.o : $(TECALPidDefs_cxx_dependencies)
	$(cpp_echo) $(src)TECALPidDefs.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(TECALPidDefs_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(TECALPidDefs_cppflags) $(TECALPidDefs_cxx_cppflags)  $(src)TECALPidDefs.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(TECALPidLikelihood_cxx_dependencies)

$(bin)$(binobj)TECALPidLikelihood.o : $(TECALPidLikelihood_cxx_dependencies)
	$(cpp_echo) $(src)TECALPidLikelihood.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(TECALPidLikelihood_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(TECALPidLikelihood_cppflags) $(TECALPidLikelihood_cxx_cppflags)  $(src)TECALPidLikelihood.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(TECALPidLikelihoodHypothesisTest_cxx_dependencies)

$(bin)$(binobj)TECALPidLikelihoodHypothesisTest.o : $(TECALPidLikelihoodHypothesisTest_cxx_dependencies)
	$(cpp_echo) $(src)TECALPidLikelihoodHypothesisTest.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(TECALPidLikelihoodHypothesisTest_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(TECALPidLikelihoodHypothesisTest_cppflags) $(TECALPidLikelihoodHypothesisTest_cxx_cppflags)  $(src)TECALPidLikelihoodHypothesisTest.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(TTruthUtils_cxx_dependencies)

$(bin)$(binobj)TTruthUtils.o : $(TTruthUtils_cxx_dependencies)
	$(cpp_echo) $(src)TTruthUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(TTruthUtils_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(TTruthUtils_cppflags) $(TTruthUtils_cxx_cppflags)  $(src)TTruthUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(BitUtils_cxx_dependencies)

$(bin)$(binobj)BitUtils.o : $(BitUtils_cxx_dependencies)
	$(cpp_echo) $(src)EventDisplay/BitUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(BitUtils_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(BitUtils_cppflags) $(BitUtils_cxx_cppflags) -I../src/EventDisplay $(src)EventDisplay/BitUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(ControlPanel_cxx_dependencies)

$(bin)$(binobj)ControlPanel.o : $(ControlPanel_cxx_dependencies)
	$(cpp_echo) $(src)EventDisplay/ControlPanel.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(ControlPanel_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(ControlPanel_cppflags) $(ControlPanel_cxx_cppflags) -I../src/EventDisplay $(src)EventDisplay/ControlPanel.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(DetectorForager_cxx_dependencies)

$(bin)$(binobj)DetectorForager.o : $(DetectorForager_cxx_dependencies)
	$(cpp_echo) $(src)EventDisplay/DetectorForager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(DetectorForager_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(DetectorForager_cppflags) $(DetectorForager_cxx_cppflags) -I../src/EventDisplay $(src)EventDisplay/DetectorForager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(EventPlotter_cxx_dependencies)

$(bin)$(binobj)EventPlotter.o : $(EventPlotter_cxx_dependencies)
	$(cpp_echo) $(src)EventDisplay/EventPlotter.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(EventPlotter_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(EventPlotter_cppflags) $(EventPlotter_cxx_cppflags) -I../src/EventDisplay $(src)EventDisplay/EventPlotter.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(EventReader_cxx_dependencies)

$(bin)$(binobj)EventReader.o : $(EventReader_cxx_dependencies)
	$(cpp_echo) $(src)EventDisplay/EventReader.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(EventReader_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(EventReader_cppflags) $(EventReader_cxx_cppflags) -I../src/EventDisplay $(src)EventDisplay/EventReader.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandUtils_dependencies.make : $(ControlPanel_Dict_cxx_dependencies)

$(bin)$(binobj)ControlPanel_Dict.o : $(ControlPanel_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/ControlPanel_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandUtils_pp_cppflags) $(lib_highlandUtils_pp_cppflags) $(ControlPanel_Dict_pp_cppflags) $(use_cppflags) $(highlandUtils_cppflags) $(lib_highlandUtils_cppflags) $(ControlPanel_Dict_cppflags) $(ControlPanel_Dict_cxx_cppflags) -I../dict ../dict/ControlPanel_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: highlandUtilsclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(highlandUtils.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(highlandUtils.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_highlandUtils)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandUtils.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandUtils.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandUtils.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

highlandUtilsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)highlandUtils$(library_suffix).a $(binobj)$(library_prefix)highlandUtils$(library_suffix).s? $(binobj)highlandUtils.stamp $(binobj)highlandUtils.shstamp
#-- end of cleanup_library ---------------

