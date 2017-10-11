#-- start of make_header -----------------

#====================================
#  Library DrawingToolsNueCC
#
#   Generated Mon Oct  9 06:41:48 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_DrawingToolsNueCC_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DrawingToolsNueCC_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DrawingToolsNueCC

nueCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_DrawingToolsNueCC = /tmp/CMT_$(nueCCAnalysis_tag)_DrawingToolsNueCC.make$(cmt_lock_pid)
else
#cmt_local_tagfile_DrawingToolsNueCC = $(nueCCAnalysis_tag)_DrawingToolsNueCC.make
cmt_local_tagfile_DrawingToolsNueCC = $(bin)$(nueCCAnalysis_tag)_DrawingToolsNueCC.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

nueCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_DrawingToolsNueCC = /tmp/CMT_$(nueCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_DrawingToolsNueCC = $(nueCCAnalysis_tag).make
cmt_local_tagfile_DrawingToolsNueCC = $(bin)$(nueCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_DrawingToolsNueCC)

ifdef cmt_DrawingToolsNueCC_has_target_tag

ifdef READONLY
cmt_final_setup_DrawingToolsNueCC = /tmp/CMT_nueCCAnalysis_DrawingToolsNueCCsetup.make
cmt_local_DrawingToolsNueCC_makefile = /tmp/CMT_DrawingToolsNueCC$(cmt_lock_pid).make
else
cmt_final_setup_DrawingToolsNueCC = $(bin)nueCCAnalysis_DrawingToolsNueCCsetup.make
cmt_local_DrawingToolsNueCC_makefile = $(bin)DrawingToolsNueCC.make
endif

else

ifdef READONLY
cmt_final_setup_DrawingToolsNueCC = /tmp/CMT_nueCCAnalysissetup.make
cmt_local_DrawingToolsNueCC_makefile = /tmp/CMT_DrawingToolsNueCC$(cmt_lock_pid).make
else
cmt_final_setup_DrawingToolsNueCC = $(bin)nueCCAnalysissetup.make
cmt_local_DrawingToolsNueCC_makefile = $(bin)DrawingToolsNueCC.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_nueCCAnalysissetup.make
else
cmt_final_setup = $(bin)nueCCAnalysissetup.make
endif

DrawingToolsNueCC ::


ifdef READONLY
DrawingToolsNueCC ::
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
	$(echo) 'DrawingToolsNueCC'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DrawingToolsNueCC/
DrawingToolsNueCC::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

DrawingToolsNueCClibname   = $(bin)$(library_prefix)DrawingToolsNueCC$(library_suffix)
DrawingToolsNueCClib       = $(DrawingToolsNueCClibname).a
DrawingToolsNueCCstamp     = $(bin)DrawingToolsNueCC.stamp
DrawingToolsNueCCshstamp   = $(bin)DrawingToolsNueCC.shstamp

DrawingToolsNueCC :: dirs  DrawingToolsNueCCLIB
	$(echo) "DrawingToolsNueCC ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

DrawingToolsNueCCLIB :: $(DrawingToolsNueCClib) $(DrawingToolsNueCCshstamp)
	$(echo) "DrawingToolsNueCC : library ok"

$(DrawingToolsNueCClib) :: $(bin)DrawingToolsNueCC.o $(bin)DrawingToolsNueCC_Dict.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(DrawingToolsNueCClib) $?
	$(lib_silent) $(ranlib) $(DrawingToolsNueCClib)
	$(lib_silent) cat /dev/null >$(DrawingToolsNueCCstamp)

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

$(DrawingToolsNueCClibname).$(shlibsuffix) :: $(DrawingToolsNueCClib) requirements $(use_requirements) $(DrawingToolsNueCCstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" DrawingToolsNueCC $(DrawingToolsNueCC_shlibflags)

$(DrawingToolsNueCCshstamp) :: $(DrawingToolsNueCClibname).$(shlibsuffix)
	$(lib_silent) if test -f $(DrawingToolsNueCClibname).$(shlibsuffix) ; then cat /dev/null >$(DrawingToolsNueCCshstamp) ; fi

DrawingToolsNueCCclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)DrawingToolsNueCC.o $(bin)DrawingToolsNueCC_Dict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf DrawingToolsNueCC_deps DrawingToolsNueCC_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
DrawingToolsNueCCinstallname = $(library_prefix)DrawingToolsNueCC$(library_suffix).$(shlibsuffix)

DrawingToolsNueCC :: DrawingToolsNueCCinstall

install :: DrawingToolsNueCCinstall

DrawingToolsNueCCinstall :: $(install_dir)/$(DrawingToolsNueCCinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(DrawingToolsNueCCinstallname) :: $(bin)$(DrawingToolsNueCCinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(DrawingToolsNueCCinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##DrawingToolsNueCCclean :: DrawingToolsNueCCuninstall

uninstall :: DrawingToolsNueCCuninstall

DrawingToolsNueCCuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(DrawingToolsNueCCinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),DrawingToolsNueCCclean)

#$(bin)DrawingToolsNueCC_dependencies.make :: dirs

ifndef QUICK
$(bin)DrawingToolsNueCC_dependencies.make : ../src/DrawingToolsNueCC.cxx ../dict/DrawingToolsNueCC_Dict.cxx $(use_requirements) $(cmt_final_setup_DrawingToolsNueCC)
	$(echo) "(DrawingToolsNueCC.make) Rebuilding $@"; \
	  $(build_dependencies) DrawingToolsNueCC -all_sources -out=$@ ../src/DrawingToolsNueCC.cxx ../dict/DrawingToolsNueCC_Dict.cxx
endif

#$(DrawingToolsNueCC_dependencies)

-include $(bin)DrawingToolsNueCC_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)DrawingToolsNueCC_dependencies.make : $(DrawingToolsNueCC_cxx_dependencies)

$(bin)$(binobj)DrawingToolsNueCC.o : $(DrawingToolsNueCC_cxx_dependencies)
	$(cpp_echo) $(src)DrawingToolsNueCC.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(DrawingToolsNueCC_pp_cppflags) $(lib_DrawingToolsNueCC_pp_cppflags) $(DrawingToolsNueCC_pp_cppflags) $(use_cppflags) $(DrawingToolsNueCC_cppflags) $(lib_DrawingToolsNueCC_cppflags) $(DrawingToolsNueCC_cppflags) $(DrawingToolsNueCC_cxx_cppflags)  $(src)DrawingToolsNueCC.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)DrawingToolsNueCC_dependencies.make : $(DrawingToolsNueCC_Dict_cxx_dependencies)

$(bin)$(binobj)DrawingToolsNueCC_Dict.o : $(DrawingToolsNueCC_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/DrawingToolsNueCC_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(DrawingToolsNueCC_pp_cppflags) $(lib_DrawingToolsNueCC_pp_cppflags) $(DrawingToolsNueCC_Dict_pp_cppflags) $(use_cppflags) $(DrawingToolsNueCC_cppflags) $(lib_DrawingToolsNueCC_cppflags) $(DrawingToolsNueCC_Dict_cppflags) $(DrawingToolsNueCC_Dict_cxx_cppflags) -I../dict ../dict/DrawingToolsNueCC_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: DrawingToolsNueCCclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DrawingToolsNueCC.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(DrawingToolsNueCC.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_DrawingToolsNueCC)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(DrawingToolsNueCC.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(DrawingToolsNueCC.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(DrawingToolsNueCC.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

DrawingToolsNueCCclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)DrawingToolsNueCC$(library_suffix).a $(binobj)$(library_prefix)DrawingToolsNueCC$(library_suffix).s? $(binobj)DrawingToolsNueCC.stamp $(binobj)DrawingToolsNueCC.shstamp
#-- end of cleanup_library ---------------

