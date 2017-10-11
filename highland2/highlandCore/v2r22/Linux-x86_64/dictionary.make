#-- start of make_header -----------------

#====================================
#  Document dictionary
#
#   Generated Mon Oct  9 06:37:36 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_dictionary_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_dictionary_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_dictionary

highlandCore_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_dictionary = /tmp/CMT_$(highlandCore_tag)_dictionary.make$(cmt_lock_pid)
else
#cmt_local_tagfile_dictionary = $(highlandCore_tag)_dictionary.make
cmt_local_tagfile_dictionary = $(bin)$(highlandCore_tag)_dictionary.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandCore_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_dictionary = /tmp/CMT_$(highlandCore_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_dictionary = $(highlandCore_tag).make
cmt_local_tagfile_dictionary = $(bin)$(highlandCore_tag).make
endif

endif

-include $(cmt_local_tagfile_dictionary)

ifdef cmt_dictionary_has_target_tag

ifdef READONLY
cmt_final_setup_dictionary = /tmp/CMT_highlandCore_dictionarysetup.make
cmt_local_dictionary_makefile = /tmp/CMT_dictionary$(cmt_lock_pid).make
else
cmt_final_setup_dictionary = $(bin)highlandCore_dictionarysetup.make
cmt_local_dictionary_makefile = $(bin)dictionary.make
endif

else

ifdef READONLY
cmt_final_setup_dictionary = /tmp/CMT_highlandCoresetup.make
cmt_local_dictionary_makefile = /tmp/CMT_dictionary$(cmt_lock_pid).make
else
cmt_final_setup_dictionary = $(bin)highlandCoresetup.make
cmt_local_dictionary_makefile = $(bin)dictionary.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandCoresetup.make
else
cmt_final_setup = $(bin)highlandCoresetup.make
endif

dictionary ::


ifdef READONLY
dictionary ::
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
	$(echo) 'dictionary'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = dictionary/
dictionary::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

dictionary_output = $(dict)

dictionary :: $(filter %_LinkDef.rootcint,$(dictionary_output)CategoryClasses_LinkDef.rootcint $(dictionary_output)ConfigurationBase_LinkDef.rootcint $(dictionary_output)CorrectionBase_LinkDef.rootcint $(dictionary_output)DocString_LinkDef.rootcint $(dictionary_output)OutputManager_LinkDef.rootcint $(dictionary_output)VersionManager_LinkDef.rootcint)
	@echo "------> dictionary ok"

CategoryClasses_LinkDef_h_dependencies = ../src/CategoryClasses_LinkDef.h
ConfigurationBase_LinkDef_h_dependencies = ../src/ConfigurationBase_LinkDef.h
CorrectionBase_LinkDef_h_dependencies = ../src/CorrectionBase_LinkDef.h
DocString_LinkDef_h_dependencies = ../src/DocString_LinkDef.h
OutputManager_LinkDef_h_dependencies = ../src/OutputManager_LinkDef.h
VersionManager_LinkDef_h_dependencies = ../src/VersionManager_LinkDef.h
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)CategoryClasses_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,CategoryClasses_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,CategoryClasses_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)CategoryClasses_LinkDef.h) \
    $(src)CategoryClasses_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)ConfigurationBase_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,ConfigurationBase_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,ConfigurationBase_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)ConfigurationBase_LinkDef.h) \
    $(src)ConfigurationBase_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)CorrectionBase_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,CorrectionBase_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,CorrectionBase_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)CorrectionBase_LinkDef.h) \
    $(src)CorrectionBase_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)DocString_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,DocString_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,DocString_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)DocString_LinkDef.h) \
    $(src)DocString_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)OutputManager_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,OutputManager_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,OutputManager_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)OutputManager_LinkDef.h) \
    $(src)OutputManager_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

$(dict)VersionManager_LinkDef.rootcint : $(dict)$(patsubst %_LinkDef,%_Dict,VersionManager_LinkDef).cxx
	@touch $@

$(dict)$(patsubst %_LinkDef,%_Dict,VersionManager_LinkDef).cxx : \
    $(patsubst %_LinkDef.h,%.h*,$(src)VersionManager_LinkDef.h) \
    $(src)VersionManager_LinkDef.h  $(ROOT_stamps)
	@echo "Generating ROOT Dictionary from " $(filter-out %~,$^)
	@if [ ! -d $(dict) ]; then mkdir $(dict); fi ;
	$(rootcint) -f $@ -c -p $(includes) $(cppflags) $(word 1, $(filter-out %~,$^)) $(word 2, $(filter-out %~,$^))

# End of fragment.
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build the ROOT dictionary files.
#

clean :: dictionaryclean
	@cd .

dictionaryclean ::
	@echo "Removing root dictionaries: $(dict)"
	/bin/rm -rf $(dict)
#-- start of cleanup_header --------------

clean :: dictionaryclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(dictionary.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(dictionary.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_dictionary)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(dictionary.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(dictionary.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(dictionary.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

dictionaryclean ::
#-- end of cleanup_header ---------------
