#-- start of make_header -----------------

#====================================
#  Library highlandIO
#
#   Generated Mon Oct  9 06:40:36 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_highlandIO_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_highlandIO_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_highlandIO

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandIO = /tmp/CMT_$(highlandIO_tag)_highlandIO.make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandIO = $(highlandIO_tag)_highlandIO.make
cmt_local_tagfile_highlandIO = $(bin)$(highlandIO_tag)_highlandIO.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandIO_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandIO = /tmp/CMT_$(highlandIO_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandIO = $(highlandIO_tag).make
cmt_local_tagfile_highlandIO = $(bin)$(highlandIO_tag).make
endif

endif

-include $(cmt_local_tagfile_highlandIO)

ifdef cmt_highlandIO_has_target_tag

ifdef READONLY
cmt_final_setup_highlandIO = /tmp/CMT_highlandIO_highlandIOsetup.make
cmt_local_highlandIO_makefile = /tmp/CMT_highlandIO$(cmt_lock_pid).make
else
cmt_final_setup_highlandIO = $(bin)highlandIO_highlandIOsetup.make
cmt_local_highlandIO_makefile = $(bin)highlandIO.make
endif

else

ifdef READONLY
cmt_final_setup_highlandIO = /tmp/CMT_highlandIOsetup.make
cmt_local_highlandIO_makefile = /tmp/CMT_highlandIO$(cmt_lock_pid).make
else
cmt_final_setup_highlandIO = $(bin)highlandIOsetup.make
cmt_local_highlandIO_makefile = $(bin)highlandIO.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandIOsetup.make
else
cmt_final_setup = $(bin)highlandIOsetup.make
endif

highlandIO ::


ifdef READONLY
highlandIO ::
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
	$(echo) 'highlandIO'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = highlandIO/
highlandIO::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

highlandIOlibname   = $(bin)$(library_prefix)highlandIO$(library_suffix)
highlandIOlib       = $(highlandIOlibname).a
highlandIOstamp     = $(bin)highlandIO.stamp
highlandIOshstamp   = $(bin)highlandIO.shstamp

highlandIO :: dirs  highlandIOLIB
	$(echo) "highlandIO ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

highlandIOLIB :: $(highlandIOlib) $(highlandIOshstamp)
	$(echo) "highlandIO : library ok"

$(highlandIOlib) :: $(bin)CreateFlatTree.o $(bin)CreateMiniTree.o $(bin)DataClassesIO.o $(bin)FlatTreeConverter.o $(bin)oaAnalysisP0DTreeConverter.o $(bin)oaAnalysisTreeConverter.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(highlandIOlib) $?
	$(lib_silent) $(ranlib) $(highlandIOlib)
	$(lib_silent) cat /dev/null >$(highlandIOstamp)

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

$(highlandIOlibname).$(shlibsuffix) :: $(highlandIOlib) requirements $(use_requirements) $(highlandIOstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" highlandIO $(highlandIO_shlibflags)

$(highlandIOshstamp) :: $(highlandIOlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(highlandIOlibname).$(shlibsuffix) ; then cat /dev/null >$(highlandIOshstamp) ; fi

highlandIOclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)CreateFlatTree.o $(bin)CreateMiniTree.o $(bin)DataClassesIO.o $(bin)FlatTreeConverter.o $(bin)oaAnalysisP0DTreeConverter.o $(bin)oaAnalysisTreeConverter.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf highlandIO_deps highlandIO_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
highlandIOinstallname = $(library_prefix)highlandIO$(library_suffix).$(shlibsuffix)

highlandIO :: highlandIOinstall

install :: highlandIOinstall

highlandIOinstall :: $(install_dir)/$(highlandIOinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(highlandIOinstallname) :: $(bin)$(highlandIOinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandIOinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##highlandIOclean :: highlandIOuninstall

uninstall :: highlandIOuninstall

highlandIOuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(highlandIOinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),highlandIOclean)

#$(bin)highlandIO_dependencies.make :: dirs

ifndef QUICK
$(bin)highlandIO_dependencies.make : ../src/CreateFlatTree.cxx ../src/CreateMiniTree.cxx ../src/DataClassesIO.cxx ../src/FlatTreeConverter.cxx ../src/oaAnalysisP0DTreeConverter.cxx ../src/oaAnalysisTreeConverter.cxx $(use_requirements) $(cmt_final_setup_highlandIO)
	$(echo) "(highlandIO.make) Rebuilding $@"; \
	  $(build_dependencies) highlandIO -all_sources -out=$@ ../src/CreateFlatTree.cxx ../src/CreateMiniTree.cxx ../src/DataClassesIO.cxx ../src/FlatTreeConverter.cxx ../src/oaAnalysisP0DTreeConverter.cxx ../src/oaAnalysisTreeConverter.cxx
endif

#$(highlandIO_dependencies)

-include $(bin)highlandIO_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)highlandIO_dependencies.make : $(CreateFlatTree_cxx_dependencies)

$(bin)$(binobj)CreateFlatTree.o : $(CreateFlatTree_cxx_dependencies)
	$(cpp_echo) $(src)CreateFlatTree.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandIO_pp_cppflags) $(lib_highlandIO_pp_cppflags) $(CreateFlatTree_pp_cppflags) $(use_cppflags) $(highlandIO_cppflags) $(lib_highlandIO_cppflags) $(CreateFlatTree_cppflags) $(CreateFlatTree_cxx_cppflags)  $(src)CreateFlatTree.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandIO_dependencies.make : $(CreateMiniTree_cxx_dependencies)

$(bin)$(binobj)CreateMiniTree.o : $(CreateMiniTree_cxx_dependencies)
	$(cpp_echo) $(src)CreateMiniTree.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandIO_pp_cppflags) $(lib_highlandIO_pp_cppflags) $(CreateMiniTree_pp_cppflags) $(use_cppflags) $(highlandIO_cppflags) $(lib_highlandIO_cppflags) $(CreateMiniTree_cppflags) $(CreateMiniTree_cxx_cppflags)  $(src)CreateMiniTree.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandIO_dependencies.make : $(DataClassesIO_cxx_dependencies)

$(bin)$(binobj)DataClassesIO.o : $(DataClassesIO_cxx_dependencies)
	$(cpp_echo) $(src)DataClassesIO.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandIO_pp_cppflags) $(lib_highlandIO_pp_cppflags) $(DataClassesIO_pp_cppflags) $(use_cppflags) $(highlandIO_cppflags) $(lib_highlandIO_cppflags) $(DataClassesIO_cppflags) $(DataClassesIO_cxx_cppflags)  $(src)DataClassesIO.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandIO_dependencies.make : $(FlatTreeConverter_cxx_dependencies)

$(bin)$(binobj)FlatTreeConverter.o : $(FlatTreeConverter_cxx_dependencies)
	$(cpp_echo) $(src)FlatTreeConverter.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandIO_pp_cppflags) $(lib_highlandIO_pp_cppflags) $(FlatTreeConverter_pp_cppflags) $(use_cppflags) $(highlandIO_cppflags) $(lib_highlandIO_cppflags) $(FlatTreeConverter_cppflags) $(FlatTreeConverter_cxx_cppflags)  $(src)FlatTreeConverter.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandIO_dependencies.make : $(oaAnalysisP0DTreeConverter_cxx_dependencies)

$(bin)$(binobj)oaAnalysisP0DTreeConverter.o : $(oaAnalysisP0DTreeConverter_cxx_dependencies)
	$(cpp_echo) $(src)oaAnalysisP0DTreeConverter.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandIO_pp_cppflags) $(lib_highlandIO_pp_cppflags) $(oaAnalysisP0DTreeConverter_pp_cppflags) $(use_cppflags) $(highlandIO_cppflags) $(lib_highlandIO_cppflags) $(oaAnalysisP0DTreeConverter_cppflags) $(oaAnalysisP0DTreeConverter_cxx_cppflags)  $(src)oaAnalysisP0DTreeConverter.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)highlandIO_dependencies.make : $(oaAnalysisTreeConverter_cxx_dependencies)

$(bin)$(binobj)oaAnalysisTreeConverter.o : $(oaAnalysisTreeConverter_cxx_dependencies)
	$(cpp_echo) $(src)oaAnalysisTreeConverter.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(highlandIO_pp_cppflags) $(lib_highlandIO_pp_cppflags) $(oaAnalysisTreeConverter_pp_cppflags) $(use_cppflags) $(highlandIO_cppflags) $(lib_highlandIO_cppflags) $(oaAnalysisTreeConverter_cppflags) $(oaAnalysisTreeConverter_cxx_cppflags)  $(src)oaAnalysisTreeConverter.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: highlandIOclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(highlandIO.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(highlandIO.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_highlandIO)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandIO.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandIO.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandIO.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

highlandIOclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)highlandIO$(library_suffix).a $(binobj)$(library_prefix)highlandIO$(library_suffix).s? $(binobj)highlandIO.stamp $(binobj)highlandIO.shstamp
#-- end of cleanup_library ---------------

