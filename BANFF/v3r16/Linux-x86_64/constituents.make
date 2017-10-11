
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile = $(BANFF_tag).make
cmt_local_tagfile = $(bin)$(BANFF_tag).make
endif

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

ifdef READONLY
cmt_local_setup = /tmp/CMT_BANFFsetup$(cmt_lock_pid).make
cmt_final_setup = /tmp/CMT_BANFFsetup.make
else
#cmt_local_setup = $(bin)BANFFsetup$(cmt_lock_pid).make
cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)BANFFsetup.make
cmt_final_setup = $(bin)$(package)setup.make
endif

#--------------------------------------------------------

#cmt_lock_setup = /tmp/lock$(cmt_lock_pid).make
#cmt_temp_tag = /tmp/tag$(cmt_lock_pid).make

#first :: $(cmt_local_tagfile)
#	@echo $(cmt_local_tagfile) ok
ifndef QUICK
first :: $(cmt_final_setup) ;
else
first :: ;
endif

##	@bin=`$(cmtexe) show macro_value bin`

#$(cmt_local_tagfile) : $(cmt_lock_setup)
#	@echo "#CMT> Error: $@: No such file" >&2; exit 1
$(cmt_local_tagfile) :
	@echo "#CMT> Warning: $@: No such file" >&2; exit
#	@echo "#CMT> Info: $@: No need to rebuild file" >&2; exit

$(cmt_final_setup) : $(cmt_local_tagfile) 
	$(echo) "(constituents.make) Rebuilding $@"
	@if test ! -d $(@D); then $(mkdir) -p $(@D); fi; \
	  if test -f $(cmt_local_setup); then /bin/rm -f $(cmt_local_setup); fi; \
	  trap '/bin/rm -f $(cmt_local_setup)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) show setup >>$(cmt_local_setup); \
	  if test ! -f $@; then \
	    mv $(cmt_local_setup) $@; \
	  else \
	    if /usr/bin/diff $(cmt_local_setup) $@ >/dev/null ; then \
	      : ; \
	    else \
	      mv $(cmt_local_setup) $@; \
	    fi; \
	  fi

#	@/bin/echo $@ ok   

config :: checkuses
	@exit 0
checkuses : ;

env.make ::
	printenv >env.make.tmp; $(cmtexe) check files env.make.tmp env.make

ifndef QUICK
all :: build_library_links
	$(echo) "(constituents.make) all done"
endif

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

build_library_links : dirs requirements
	$(echo) "(constituents.make) Rebuilding library links"; \
	if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi; \
	$(build_library_links)

.DEFAULT ::
	$(echo) "(constituents.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: Using default commands" >&2; exit

#	@if test "$@" = "$(cmt_lock_setup)"; then \
	#  /bin/rm -f $(cmt_lock_setup); \
	 # touch $(cmt_lock_setup); \
	#fi

#-- end of constituents_header ------
#-- start of group ------

all_groups :: all

all :: $(all_dependencies)  $(all_pre_constituents) $(all_constituents)  $(all_post_constituents)
	$(echo) "all ok."

#	@/bin/echo " all ok."

clean :: allclean

allclean ::  $(all_constituentsclean)
	$(echo) $(all_constituentsclean)
	$(echo) "allclean ok."

#	@echo $(all_constituentsclean)
#	@/bin/echo " allclean ok."

allclean :: makefilesclean

#-- end of group ------
#-- start of group ------

all_groups :: cmt_actions

cmt_actions :: $(cmt_actions_dependencies)  $(cmt_actions_pre_constituents) $(cmt_actions_constituents)  $(cmt_actions_post_constituents)
	$(echo) "cmt_actions ok."

#	@/bin/echo " cmt_actions ok."

clean :: allclean

cmt_actionsclean ::  $(cmt_actions_constituentsclean)
	$(echo) $(cmt_actions_constituentsclean)
	$(echo) "cmt_actionsclean ok."

#	@echo $(cmt_actions_constituentsclean)
#	@/bin/echo " cmt_actionsclean ok."

cmt_actionsclean :: makefilesclean

#-- end of group ------
#-- start of group ------

all_groups :: documentation

documentation :: $(documentation_dependencies)  $(documentation_pre_constituents) $(documentation_constituents)  $(documentation_post_constituents)
	$(echo) "documentation ok."

#	@/bin/echo " documentation ok."

clean :: allclean

documentationclean ::  $(documentation_constituentsclean)
	$(echo) $(documentation_constituentsclean)
	$(echo) "documentationclean ok."

#	@echo $(documentation_constituentsclean)
#	@/bin/echo " documentationclean ok."

documentationclean :: makefilesclean

#-- end of group ------
#-- start of constituent_lock ------

cmt_version_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_version_has_target_tag

ifdef READONLY
cmt_local_tagfile_version = /tmp/CMT_$(BANFF_tag)_version.make$(cmt_lock_pid)
cmt_final_setup_version = /tmp/CMT_BANFF_versionsetup.make
cmt_local_version_makefile = /tmp/CMT_version$(cmt_lock_pid).make
else
#cmt_local_tagfile_version = $(BANFF_tag)_version.make
cmt_local_tagfile_version = $(bin)$(BANFF_tag)_version.make
cmt_final_setup_version = $(bin)BANFF_versionsetup.make
cmt_local_version_makefile = $(bin)version.make
endif

version_extratags = -tag_add=target_version

#$(cmt_local_tagfile_version) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_version) ::
else
$(cmt_local_tagfile_version) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_version)"
	@if test -f $(cmt_local_tagfile_version); then /bin/rm -f $(cmt_local_tagfile_version); fi ; \
	  $(cmtexe) -tag=$(tags) $(version_extratags) build tag_makefile >>$(cmt_local_tagfile_version); \
	  if test -f $(cmt_final_setup_version); then /bin/rm -f $(cmt_final_setup_version); fi; \
	  $(cmtexe) -tag=$(tags) $(version_extratags) show setup >>$(cmt_final_setup_version)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_version = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
cmt_final_setup_version = /tmp/CMT_BANFFsetup.make
cmt_local_version_makefile = /tmp/CMT_version$(cmt_lock_pid).make
else
#cmt_local_tagfile_version = $(BANFF_tag).make
cmt_local_tagfile_version = $(bin)$(BANFF_tag).make
cmt_final_setup_version = $(bin)BANFFsetup.make
cmt_local_version_makefile = $(bin)version.make
endif

endif

ifndef QUICK
$(cmt_local_version_makefile) :: $(version_dependencies) $(cmt_local_tagfile_version) build_library_links dirs
else
$(cmt_local_version_makefile) :: $(cmt_local_tagfile_version)
endif
	$(echo) "(constituents.make) Building version.make"; \
	  $(cmtexe) -tag=$(tags) $(version_extratags) build constituent_makefile -out=$(cmt_local_version_makefile) version

version :: $(version_dependencies) $(cmt_local_version_makefile)
	$(echo) "(constituents.make) Creating version${lock_suffix} and Starting version"
	@${lock_command} version${lock_suffix} || exit $$?; \
	  retval=$$?; \
	  trap '${unlock_command} version${lock_suffix}; exit $${retval}' 1 2 15; \
	  $(MAKE) -f $(cmt_local_version_makefile) cmt_lock_pid=$${cmt_lock_pid} version; \
	  retval=$$?; ${unlock_command} version${lock_suffix}; exit $${retval}
	$(echo) "(constituents.make) version done"

clean :: versionclean

versionclean :: $(versionclean_dependencies) ##$(cmt_local_version_makefile)
	$(echo) "(constituents.make) Starting versionclean"
	@-if test -f $(cmt_local_version_makefile); then \
	  $(MAKE) -f $(cmt_local_version_makefile) cmt_lock_pid=$${cmt_lock_pid} versionclean; \
	fi

##	  /bin/rm -f $(cmt_local_version_makefile) $(bin)version_dependencies.make

install :: versioninstall

versioninstall :: $(version_dependencies) $(cmt_local_version_makefile)
	$(echo) "(constituents.make) Starting install version"
	@-$(MAKE) -f $(cmt_local_version_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install version done"

uninstall :: versionuninstall

versionuninstall :: $(cmt_local_version_makefile)
	$(echo) "(constituents.make) Starting uninstall version"
	@-$(MAKE) -f $(cmt_local_version_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall version done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ version"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ version done"
endif


#-- end of constituent_lock ------
#-- start of constituent_lock ------

cmt_register_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_register_has_target_tag

ifdef READONLY
cmt_local_tagfile_register = /tmp/CMT_$(BANFF_tag)_register.make$(cmt_lock_pid)
cmt_final_setup_register = /tmp/CMT_BANFF_registersetup.make
cmt_local_register_makefile = /tmp/CMT_register$(cmt_lock_pid).make
else
#cmt_local_tagfile_register = $(BANFF_tag)_register.make
cmt_local_tagfile_register = $(bin)$(BANFF_tag)_register.make
cmt_final_setup_register = $(bin)BANFF_registersetup.make
cmt_local_register_makefile = $(bin)register.make
endif

register_extratags = -tag_add=target_register

#$(cmt_local_tagfile_register) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_register) ::
else
$(cmt_local_tagfile_register) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_register)"
	@if test -f $(cmt_local_tagfile_register); then /bin/rm -f $(cmt_local_tagfile_register); fi ; \
	  $(cmtexe) -tag=$(tags) $(register_extratags) build tag_makefile >>$(cmt_local_tagfile_register); \
	  if test -f $(cmt_final_setup_register); then /bin/rm -f $(cmt_final_setup_register); fi; \
	  $(cmtexe) -tag=$(tags) $(register_extratags) show setup >>$(cmt_final_setup_register)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_register = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
cmt_final_setup_register = /tmp/CMT_BANFFsetup.make
cmt_local_register_makefile = /tmp/CMT_register$(cmt_lock_pid).make
else
#cmt_local_tagfile_register = $(BANFF_tag).make
cmt_local_tagfile_register = $(bin)$(BANFF_tag).make
cmt_final_setup_register = $(bin)BANFFsetup.make
cmt_local_register_makefile = $(bin)register.make
endif

endif

ifndef QUICK
$(cmt_local_register_makefile) :: $(register_dependencies) $(cmt_local_tagfile_register) build_library_links dirs
else
$(cmt_local_register_makefile) :: $(cmt_local_tagfile_register)
endif
	$(echo) "(constituents.make) Building register.make"; \
	  $(cmtexe) -tag=$(tags) $(register_extratags) build constituent_makefile -out=$(cmt_local_register_makefile) register

register :: $(register_dependencies) $(cmt_local_register_makefile)
	$(echo) "(constituents.make) Creating register${lock_suffix} and Starting register"
	@${lock_command} register${lock_suffix} || exit $$?; \
	  retval=$$?; \
	  trap '${unlock_command} register${lock_suffix}; exit $${retval}' 1 2 15; \
	  $(MAKE) -f $(cmt_local_register_makefile) cmt_lock_pid=$${cmt_lock_pid} register; \
	  retval=$$?; ${unlock_command} register${lock_suffix}; exit $${retval}
	$(echo) "(constituents.make) register done"

clean :: registerclean

registerclean :: $(registerclean_dependencies) ##$(cmt_local_register_makefile)
	$(echo) "(constituents.make) Starting registerclean"
	@-if test -f $(cmt_local_register_makefile); then \
	  $(MAKE) -f $(cmt_local_register_makefile) cmt_lock_pid=$${cmt_lock_pid} registerclean; \
	fi

##	  /bin/rm -f $(cmt_local_register_makefile) $(bin)register_dependencies.make

install :: registerinstall

registerinstall :: $(register_dependencies) $(cmt_local_register_makefile)
	$(echo) "(constituents.make) Starting install register"
	@-$(MAKE) -f $(cmt_local_register_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install register done"

uninstall :: registeruninstall

registeruninstall :: $(cmt_local_register_makefile)
	$(echo) "(constituents.make) Starting uninstall register"
	@-$(MAKE) -f $(cmt_local_register_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall register done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ register"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ register done"
endif


#-- end of constituent_lock ------
#-- start of constituent_lock ------

cmt_doxygen_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_doxygen_has_target_tag

ifdef READONLY
cmt_local_tagfile_doxygen = /tmp/CMT_$(BANFF_tag)_doxygen.make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_BANFF_doxygensetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(BANFF_tag)_doxygen.make
cmt_local_tagfile_doxygen = $(bin)$(BANFF_tag)_doxygen.make
cmt_final_setup_doxygen = $(bin)BANFF_doxygensetup.make
cmt_local_doxygen_makefile = $(bin)doxygen.make
endif

doxygen_extratags = -tag_add=target_doxygen

#$(cmt_local_tagfile_doxygen) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_doxygen) ::
else
$(cmt_local_tagfile_doxygen) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_doxygen)"
	@if test -f $(cmt_local_tagfile_doxygen); then /bin/rm -f $(cmt_local_tagfile_doxygen); fi ; \
	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) build tag_makefile >>$(cmt_local_tagfile_doxygen); \
	  if test -f $(cmt_final_setup_doxygen); then /bin/rm -f $(cmt_final_setup_doxygen); fi; \
	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) show setup >>$(cmt_final_setup_doxygen)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_doxygen = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
cmt_final_setup_doxygen = /tmp/CMT_BANFFsetup.make
cmt_local_doxygen_makefile = /tmp/CMT_doxygen$(cmt_lock_pid).make
else
#cmt_local_tagfile_doxygen = $(BANFF_tag).make
cmt_local_tagfile_doxygen = $(bin)$(BANFF_tag).make
cmt_final_setup_doxygen = $(bin)BANFFsetup.make
cmt_local_doxygen_makefile = $(bin)doxygen.make
endif

endif

ifndef QUICK
$(cmt_local_doxygen_makefile) :: $(doxygen_dependencies) $(cmt_local_tagfile_doxygen) build_library_links dirs
else
$(cmt_local_doxygen_makefile) :: $(cmt_local_tagfile_doxygen)
endif
	$(echo) "(constituents.make) Building doxygen.make"; \
	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) build constituent_makefile -out=$(cmt_local_doxygen_makefile) doxygen

doxygen :: $(doxygen_dependencies) $(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Creating doxygen${lock_suffix} and Starting doxygen"
	@${lock_command} doxygen${lock_suffix} || exit $$?; \
	  retval=$$?; \
	  trap '${unlock_command} doxygen${lock_suffix}; exit $${retval}' 1 2 15; \
	  $(MAKE) -f $(cmt_local_doxygen_makefile) cmt_lock_pid=$${cmt_lock_pid} doxygen; \
	  retval=$$?; ${unlock_command} doxygen${lock_suffix}; exit $${retval}
	$(echo) "(constituents.make) doxygen done"

clean :: doxygenclean

doxygenclean :: $(doxygenclean_dependencies) ##$(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Starting doxygenclean"
	@-if test -f $(cmt_local_doxygen_makefile); then \
	  $(MAKE) -f $(cmt_local_doxygen_makefile) cmt_lock_pid=$${cmt_lock_pid} doxygenclean; \
	fi

##	  /bin/rm -f $(cmt_local_doxygen_makefile) $(bin)doxygen_dependencies.make

install :: doxygeninstall

doxygeninstall :: $(doxygen_dependencies) $(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Starting install doxygen"
	@-$(MAKE) -f $(cmt_local_doxygen_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install doxygen done"

uninstall :: doxygenuninstall

doxygenuninstall :: $(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Starting uninstall doxygen"
	@-$(MAKE) -f $(cmt_local_doxygen_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall doxygen done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ doxygen"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ doxygen done"
endif


#-- end of constituent_lock ------
#-- start of constituent ------

cmt_BANFF_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_BANFF_has_target_tag

ifdef READONLY
cmt_local_tagfile_BANFF = /tmp/CMT_$(BANFF_tag)_BANFF.make$(cmt_lock_pid)
cmt_final_setup_BANFF = /tmp/CMT_BANFF_BANFFsetup.make
cmt_local_BANFF_makefile = /tmp/CMT_BANFF$(cmt_lock_pid).make
else
#cmt_local_tagfile_BANFF = $(BANFF_tag)_BANFF.make
cmt_local_tagfile_BANFF = $(bin)$(BANFF_tag)_BANFF.make
cmt_final_setup_BANFF = $(bin)BANFF_BANFFsetup.make
cmt_local_BANFF_makefile = $(bin)BANFF.make
endif

BANFF_extratags = -tag_add=target_BANFF

#$(cmt_local_tagfile_BANFF) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_BANFF) ::
else
$(cmt_local_tagfile_BANFF) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_BANFF)"
	@if test -f $(cmt_local_tagfile_BANFF); then /bin/rm -f $(cmt_local_tagfile_BANFF); fi ; \
	  $(cmtexe) -tag=$(tags) $(BANFF_extratags) build tag_makefile >>$(cmt_local_tagfile_BANFF); \
	  if test -f $(cmt_final_setup_BANFF); then /bin/rm -f $(cmt_final_setup_BANFF); fi; \
	  $(cmtexe) -tag=$(tags) $(BANFF_extratags) show setup >>$(cmt_final_setup_BANFF)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_BANFF = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
cmt_final_setup_BANFF = /tmp/CMT_BANFFsetup.make
cmt_local_BANFF_makefile = /tmp/CMT_BANFF$(cmt_lock_pid).make
else
#cmt_local_tagfile_BANFF = $(BANFF_tag).make
cmt_local_tagfile_BANFF = $(bin)$(BANFF_tag).make
cmt_final_setup_BANFF = $(bin)BANFFsetup.make
cmt_local_BANFF_makefile = $(bin)BANFF.make
endif

endif

ifndef QUICK
$(cmt_local_BANFF_makefile) :: $(BANFF_dependencies) $(cmt_local_tagfile_BANFF) build_library_links dirs
else
$(cmt_local_BANFF_makefile) :: $(cmt_local_tagfile_BANFF)
endif
	$(echo) "(constituents.make) Building BANFF.make"; \
	  $(cmtexe) -tag=$(tags) $(BANFF_extratags) build constituent_makefile -out=$(cmt_local_BANFF_makefile) BANFF

BANFF :: $(BANFF_dependencies) $(cmt_local_BANFF_makefile)
	$(echo) "(constituents.make) Starting BANFF"
	@$(MAKE) -f $(cmt_local_BANFF_makefile) cmt_lock_pid=$${cmt_lock_pid} BANFF
	$(echo) "(constituents.make) BANFF done"

clean :: BANFFclean

BANFFclean :: $(BANFFclean_dependencies) ##$(cmt_local_BANFF_makefile)
	$(echo) "(constituents.make) Starting BANFFclean"
	@-if test -f $(cmt_local_BANFF_makefile); then \
	  $(MAKE) -f $(cmt_local_BANFF_makefile) cmt_lock_pid=$${cmt_lock_pid} BANFFclean; \
	fi

##	  /bin/rm -f $(cmt_local_BANFF_makefile) $(bin)BANFF_dependencies.make

install :: BANFFinstall

BANFFinstall :: $(BANFF_dependencies) $(cmt_local_BANFF_makefile)
	$(echo) "(constituents.make) Starting install BANFF"
	@-$(MAKE) -f $(cmt_local_BANFF_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install BANFF done"

uninstall :: BANFFuninstall

BANFFuninstall :: $(cmt_local_BANFF_makefile)
	$(echo) "(constituents.make) Starting uninstall BANFF"
	@-$(MAKE) -f $(cmt_local_BANFF_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall BANFF done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ BANFF"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ BANFF done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_RunJointFit2017_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunJointFit2017_has_target_tag

ifdef READONLY
cmt_local_tagfile_RunJointFit2017 = /tmp/CMT_$(BANFF_tag)_RunJointFit2017.make$(cmt_lock_pid)
cmt_final_setup_RunJointFit2017 = /tmp/CMT_BANFF_RunJointFit2017setup.make
cmt_local_RunJointFit2017_makefile = /tmp/CMT_RunJointFit2017$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunJointFit2017 = $(BANFF_tag)_RunJointFit2017.make
cmt_local_tagfile_RunJointFit2017 = $(bin)$(BANFF_tag)_RunJointFit2017.make
cmt_final_setup_RunJointFit2017 = $(bin)BANFF_RunJointFit2017setup.make
cmt_local_RunJointFit2017_makefile = $(bin)RunJointFit2017.make
endif

RunJointFit2017_extratags = -tag_add=target_RunJointFit2017

#$(cmt_local_tagfile_RunJointFit2017) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_RunJointFit2017) ::
else
$(cmt_local_tagfile_RunJointFit2017) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_RunJointFit2017)"
	@if test -f $(cmt_local_tagfile_RunJointFit2017); then /bin/rm -f $(cmt_local_tagfile_RunJointFit2017); fi ; \
	  $(cmtexe) -tag=$(tags) $(RunJointFit2017_extratags) build tag_makefile >>$(cmt_local_tagfile_RunJointFit2017); \
	  if test -f $(cmt_final_setup_RunJointFit2017); then /bin/rm -f $(cmt_final_setup_RunJointFit2017); fi; \
	  $(cmtexe) -tag=$(tags) $(RunJointFit2017_extratags) show setup >>$(cmt_final_setup_RunJointFit2017)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_RunJointFit2017 = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
cmt_final_setup_RunJointFit2017 = /tmp/CMT_BANFFsetup.make
cmt_local_RunJointFit2017_makefile = /tmp/CMT_RunJointFit2017$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunJointFit2017 = $(BANFF_tag).make
cmt_local_tagfile_RunJointFit2017 = $(bin)$(BANFF_tag).make
cmt_final_setup_RunJointFit2017 = $(bin)BANFFsetup.make
cmt_local_RunJointFit2017_makefile = $(bin)RunJointFit2017.make
endif

endif

ifndef QUICK
$(cmt_local_RunJointFit2017_makefile) :: $(RunJointFit2017_dependencies) $(cmt_local_tagfile_RunJointFit2017) build_library_links dirs
else
$(cmt_local_RunJointFit2017_makefile) :: $(cmt_local_tagfile_RunJointFit2017)
endif
	$(echo) "(constituents.make) Building RunJointFit2017.make"; \
	  $(cmtexe) -tag=$(tags) $(RunJointFit2017_extratags) build constituent_makefile -out=$(cmt_local_RunJointFit2017_makefile) RunJointFit2017

RunJointFit2017 :: $(RunJointFit2017_dependencies) $(cmt_local_RunJointFit2017_makefile)
	$(echo) "(constituents.make) Starting RunJointFit2017"
	@$(MAKE) -f $(cmt_local_RunJointFit2017_makefile) cmt_lock_pid=$${cmt_lock_pid} RunJointFit2017
	$(echo) "(constituents.make) RunJointFit2017 done"

clean :: RunJointFit2017clean

RunJointFit2017clean :: $(RunJointFit2017clean_dependencies) ##$(cmt_local_RunJointFit2017_makefile)
	$(echo) "(constituents.make) Starting RunJointFit2017clean"
	@-if test -f $(cmt_local_RunJointFit2017_makefile); then \
	  $(MAKE) -f $(cmt_local_RunJointFit2017_makefile) cmt_lock_pid=$${cmt_lock_pid} RunJointFit2017clean; \
	fi

##	  /bin/rm -f $(cmt_local_RunJointFit2017_makefile) $(bin)RunJointFit2017_dependencies.make

install :: RunJointFit2017install

RunJointFit2017install :: $(RunJointFit2017_dependencies) $(cmt_local_RunJointFit2017_makefile)
	$(echo) "(constituents.make) Starting install RunJointFit2017"
	@-$(MAKE) -f $(cmt_local_RunJointFit2017_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install RunJointFit2017 done"

uninstall :: RunJointFit2017uninstall

RunJointFit2017uninstall :: $(cmt_local_RunJointFit2017_makefile)
	$(echo) "(constituents.make) Starting uninstall RunJointFit2017"
	@-$(MAKE) -f $(cmt_local_RunJointFit2017_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall RunJointFit2017 done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ RunJointFit2017"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ RunJointFit2017 done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_RunJointFit2016_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunJointFit2016_has_target_tag

ifdef READONLY
cmt_local_tagfile_RunJointFit2016 = /tmp/CMT_$(BANFF_tag)_RunJointFit2016.make$(cmt_lock_pid)
cmt_final_setup_RunJointFit2016 = /tmp/CMT_BANFF_RunJointFit2016setup.make
cmt_local_RunJointFit2016_makefile = /tmp/CMT_RunJointFit2016$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunJointFit2016 = $(BANFF_tag)_RunJointFit2016.make
cmt_local_tagfile_RunJointFit2016 = $(bin)$(BANFF_tag)_RunJointFit2016.make
cmt_final_setup_RunJointFit2016 = $(bin)BANFF_RunJointFit2016setup.make
cmt_local_RunJointFit2016_makefile = $(bin)RunJointFit2016.make
endif

RunJointFit2016_extratags = -tag_add=target_RunJointFit2016

#$(cmt_local_tagfile_RunJointFit2016) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_RunJointFit2016) ::
else
$(cmt_local_tagfile_RunJointFit2016) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_RunJointFit2016)"
	@if test -f $(cmt_local_tagfile_RunJointFit2016); then /bin/rm -f $(cmt_local_tagfile_RunJointFit2016); fi ; \
	  $(cmtexe) -tag=$(tags) $(RunJointFit2016_extratags) build tag_makefile >>$(cmt_local_tagfile_RunJointFit2016); \
	  if test -f $(cmt_final_setup_RunJointFit2016); then /bin/rm -f $(cmt_final_setup_RunJointFit2016); fi; \
	  $(cmtexe) -tag=$(tags) $(RunJointFit2016_extratags) show setup >>$(cmt_final_setup_RunJointFit2016)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_RunJointFit2016 = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
cmt_final_setup_RunJointFit2016 = /tmp/CMT_BANFFsetup.make
cmt_local_RunJointFit2016_makefile = /tmp/CMT_RunJointFit2016$(cmt_lock_pid).make
else
#cmt_local_tagfile_RunJointFit2016 = $(BANFF_tag).make
cmt_local_tagfile_RunJointFit2016 = $(bin)$(BANFF_tag).make
cmt_final_setup_RunJointFit2016 = $(bin)BANFFsetup.make
cmt_local_RunJointFit2016_makefile = $(bin)RunJointFit2016.make
endif

endif

ifndef QUICK
$(cmt_local_RunJointFit2016_makefile) :: $(RunJointFit2016_dependencies) $(cmt_local_tagfile_RunJointFit2016) build_library_links dirs
else
$(cmt_local_RunJointFit2016_makefile) :: $(cmt_local_tagfile_RunJointFit2016)
endif
	$(echo) "(constituents.make) Building RunJointFit2016.make"; \
	  $(cmtexe) -tag=$(tags) $(RunJointFit2016_extratags) build constituent_makefile -out=$(cmt_local_RunJointFit2016_makefile) RunJointFit2016

RunJointFit2016 :: $(RunJointFit2016_dependencies) $(cmt_local_RunJointFit2016_makefile)
	$(echo) "(constituents.make) Starting RunJointFit2016"
	@$(MAKE) -f $(cmt_local_RunJointFit2016_makefile) cmt_lock_pid=$${cmt_lock_pid} RunJointFit2016
	$(echo) "(constituents.make) RunJointFit2016 done"

clean :: RunJointFit2016clean

RunJointFit2016clean :: $(RunJointFit2016clean_dependencies) ##$(cmt_local_RunJointFit2016_makefile)
	$(echo) "(constituents.make) Starting RunJointFit2016clean"
	@-if test -f $(cmt_local_RunJointFit2016_makefile); then \
	  $(MAKE) -f $(cmt_local_RunJointFit2016_makefile) cmt_lock_pid=$${cmt_lock_pid} RunJointFit2016clean; \
	fi

##	  /bin/rm -f $(cmt_local_RunJointFit2016_makefile) $(bin)RunJointFit2016_dependencies.make

install :: RunJointFit2016install

RunJointFit2016install :: $(RunJointFit2016_dependencies) $(cmt_local_RunJointFit2016_makefile)
	$(echo) "(constituents.make) Starting install RunJointFit2016"
	@-$(MAKE) -f $(cmt_local_RunJointFit2016_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install RunJointFit2016 done"

uninstall :: RunJointFit2016uninstall

RunJointFit2016uninstall :: $(cmt_local_RunJointFit2016_makefile)
	$(echo) "(constituents.make) Starting uninstall RunJointFit2016"
	@-$(MAKE) -f $(cmt_local_RunJointFit2016_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall RunJointFit2016 done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ RunJointFit2016"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ RunJointFit2016 done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_Make2016PostFitFileAllParams_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_Make2016PostFitFileAllParams_has_target_tag

ifdef READONLY
cmt_local_tagfile_Make2016PostFitFileAllParams = /tmp/CMT_$(BANFF_tag)_Make2016PostFitFileAllParams.make$(cmt_lock_pid)
cmt_final_setup_Make2016PostFitFileAllParams = /tmp/CMT_BANFF_Make2016PostFitFileAllParamssetup.make
cmt_local_Make2016PostFitFileAllParams_makefile = /tmp/CMT_Make2016PostFitFileAllParams$(cmt_lock_pid).make
else
#cmt_local_tagfile_Make2016PostFitFileAllParams = $(BANFF_tag)_Make2016PostFitFileAllParams.make
cmt_local_tagfile_Make2016PostFitFileAllParams = $(bin)$(BANFF_tag)_Make2016PostFitFileAllParams.make
cmt_final_setup_Make2016PostFitFileAllParams = $(bin)BANFF_Make2016PostFitFileAllParamssetup.make
cmt_local_Make2016PostFitFileAllParams_makefile = $(bin)Make2016PostFitFileAllParams.make
endif

Make2016PostFitFileAllParams_extratags = -tag_add=target_Make2016PostFitFileAllParams

#$(cmt_local_tagfile_Make2016PostFitFileAllParams) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_Make2016PostFitFileAllParams) ::
else
$(cmt_local_tagfile_Make2016PostFitFileAllParams) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_Make2016PostFitFileAllParams)"
	@if test -f $(cmt_local_tagfile_Make2016PostFitFileAllParams); then /bin/rm -f $(cmt_local_tagfile_Make2016PostFitFileAllParams); fi ; \
	  $(cmtexe) -tag=$(tags) $(Make2016PostFitFileAllParams_extratags) build tag_makefile >>$(cmt_local_tagfile_Make2016PostFitFileAllParams); \
	  if test -f $(cmt_final_setup_Make2016PostFitFileAllParams); then /bin/rm -f $(cmt_final_setup_Make2016PostFitFileAllParams); fi; \
	  $(cmtexe) -tag=$(tags) $(Make2016PostFitFileAllParams_extratags) show setup >>$(cmt_final_setup_Make2016PostFitFileAllParams)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_Make2016PostFitFileAllParams = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
cmt_final_setup_Make2016PostFitFileAllParams = /tmp/CMT_BANFFsetup.make
cmt_local_Make2016PostFitFileAllParams_makefile = /tmp/CMT_Make2016PostFitFileAllParams$(cmt_lock_pid).make
else
#cmt_local_tagfile_Make2016PostFitFileAllParams = $(BANFF_tag).make
cmt_local_tagfile_Make2016PostFitFileAllParams = $(bin)$(BANFF_tag).make
cmt_final_setup_Make2016PostFitFileAllParams = $(bin)BANFFsetup.make
cmt_local_Make2016PostFitFileAllParams_makefile = $(bin)Make2016PostFitFileAllParams.make
endif

endif

ifndef QUICK
$(cmt_local_Make2016PostFitFileAllParams_makefile) :: $(Make2016PostFitFileAllParams_dependencies) $(cmt_local_tagfile_Make2016PostFitFileAllParams) build_library_links dirs
else
$(cmt_local_Make2016PostFitFileAllParams_makefile) :: $(cmt_local_tagfile_Make2016PostFitFileAllParams)
endif
	$(echo) "(constituents.make) Building Make2016PostFitFileAllParams.make"; \
	  $(cmtexe) -tag=$(tags) $(Make2016PostFitFileAllParams_extratags) build constituent_makefile -out=$(cmt_local_Make2016PostFitFileAllParams_makefile) Make2016PostFitFileAllParams

Make2016PostFitFileAllParams :: $(Make2016PostFitFileAllParams_dependencies) $(cmt_local_Make2016PostFitFileAllParams_makefile)
	$(echo) "(constituents.make) Starting Make2016PostFitFileAllParams"
	@$(MAKE) -f $(cmt_local_Make2016PostFitFileAllParams_makefile) cmt_lock_pid=$${cmt_lock_pid} Make2016PostFitFileAllParams
	$(echo) "(constituents.make) Make2016PostFitFileAllParams done"

clean :: Make2016PostFitFileAllParamsclean

Make2016PostFitFileAllParamsclean :: $(Make2016PostFitFileAllParamsclean_dependencies) ##$(cmt_local_Make2016PostFitFileAllParams_makefile)
	$(echo) "(constituents.make) Starting Make2016PostFitFileAllParamsclean"
	@-if test -f $(cmt_local_Make2016PostFitFileAllParams_makefile); then \
	  $(MAKE) -f $(cmt_local_Make2016PostFitFileAllParams_makefile) cmt_lock_pid=$${cmt_lock_pid} Make2016PostFitFileAllParamsclean; \
	fi

##	  /bin/rm -f $(cmt_local_Make2016PostFitFileAllParams_makefile) $(bin)Make2016PostFitFileAllParams_dependencies.make

install :: Make2016PostFitFileAllParamsinstall

Make2016PostFitFileAllParamsinstall :: $(Make2016PostFitFileAllParams_dependencies) $(cmt_local_Make2016PostFitFileAllParams_makefile)
	$(echo) "(constituents.make) Starting install Make2016PostFitFileAllParams"
	@-$(MAKE) -f $(cmt_local_Make2016PostFitFileAllParams_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install Make2016PostFitFileAllParams done"

uninstall :: Make2016PostFitFileAllParamsuninstall

Make2016PostFitFileAllParamsuninstall :: $(cmt_local_Make2016PostFitFileAllParams_makefile)
	$(echo) "(constituents.make) Starting uninstall Make2016PostFitFileAllParams"
	@-$(MAKE) -f $(cmt_local_Make2016PostFitFileAllParams_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall Make2016PostFitFileAllParams done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ Make2016PostFitFileAllParams"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ Make2016PostFitFileAllParams done"
endif


#-- end of constituent ------
#-- start of constituent ------

cmt_MakeND280DetectorCov_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MakeND280DetectorCov_has_target_tag

ifdef READONLY
cmt_local_tagfile_MakeND280DetectorCov = /tmp/CMT_$(BANFF_tag)_MakeND280DetectorCov.make$(cmt_lock_pid)
cmt_final_setup_MakeND280DetectorCov = /tmp/CMT_BANFF_MakeND280DetectorCovsetup.make
cmt_local_MakeND280DetectorCov_makefile = /tmp/CMT_MakeND280DetectorCov$(cmt_lock_pid).make
else
#cmt_local_tagfile_MakeND280DetectorCov = $(BANFF_tag)_MakeND280DetectorCov.make
cmt_local_tagfile_MakeND280DetectorCov = $(bin)$(BANFF_tag)_MakeND280DetectorCov.make
cmt_final_setup_MakeND280DetectorCov = $(bin)BANFF_MakeND280DetectorCovsetup.make
cmt_local_MakeND280DetectorCov_makefile = $(bin)MakeND280DetectorCov.make
endif

MakeND280DetectorCov_extratags = -tag_add=target_MakeND280DetectorCov

#$(cmt_local_tagfile_MakeND280DetectorCov) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_MakeND280DetectorCov) ::
else
$(cmt_local_tagfile_MakeND280DetectorCov) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_MakeND280DetectorCov)"
	@if test -f $(cmt_local_tagfile_MakeND280DetectorCov); then /bin/rm -f $(cmt_local_tagfile_MakeND280DetectorCov); fi ; \
	  $(cmtexe) -tag=$(tags) $(MakeND280DetectorCov_extratags) build tag_makefile >>$(cmt_local_tagfile_MakeND280DetectorCov); \
	  if test -f $(cmt_final_setup_MakeND280DetectorCov); then /bin/rm -f $(cmt_final_setup_MakeND280DetectorCov); fi; \
	  $(cmtexe) -tag=$(tags) $(MakeND280DetectorCov_extratags) show setup >>$(cmt_final_setup_MakeND280DetectorCov)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_MakeND280DetectorCov = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
cmt_final_setup_MakeND280DetectorCov = /tmp/CMT_BANFFsetup.make
cmt_local_MakeND280DetectorCov_makefile = /tmp/CMT_MakeND280DetectorCov$(cmt_lock_pid).make
else
#cmt_local_tagfile_MakeND280DetectorCov = $(BANFF_tag).make
cmt_local_tagfile_MakeND280DetectorCov = $(bin)$(BANFF_tag).make
cmt_final_setup_MakeND280DetectorCov = $(bin)BANFFsetup.make
cmt_local_MakeND280DetectorCov_makefile = $(bin)MakeND280DetectorCov.make
endif

endif

ifndef QUICK
$(cmt_local_MakeND280DetectorCov_makefile) :: $(MakeND280DetectorCov_dependencies) $(cmt_local_tagfile_MakeND280DetectorCov) build_library_links dirs
else
$(cmt_local_MakeND280DetectorCov_makefile) :: $(cmt_local_tagfile_MakeND280DetectorCov)
endif
	$(echo) "(constituents.make) Building MakeND280DetectorCov.make"; \
	  $(cmtexe) -tag=$(tags) $(MakeND280DetectorCov_extratags) build constituent_makefile -out=$(cmt_local_MakeND280DetectorCov_makefile) MakeND280DetectorCov

MakeND280DetectorCov :: $(MakeND280DetectorCov_dependencies) $(cmt_local_MakeND280DetectorCov_makefile)
	$(echo) "(constituents.make) Starting MakeND280DetectorCov"
	@$(MAKE) -f $(cmt_local_MakeND280DetectorCov_makefile) cmt_lock_pid=$${cmt_lock_pid} MakeND280DetectorCov
	$(echo) "(constituents.make) MakeND280DetectorCov done"

clean :: MakeND280DetectorCovclean

MakeND280DetectorCovclean :: $(MakeND280DetectorCovclean_dependencies) ##$(cmt_local_MakeND280DetectorCov_makefile)
	$(echo) "(constituents.make) Starting MakeND280DetectorCovclean"
	@-if test -f $(cmt_local_MakeND280DetectorCov_makefile); then \
	  $(MAKE) -f $(cmt_local_MakeND280DetectorCov_makefile) cmt_lock_pid=$${cmt_lock_pid} MakeND280DetectorCovclean; \
	fi

##	  /bin/rm -f $(cmt_local_MakeND280DetectorCov_makefile) $(bin)MakeND280DetectorCov_dependencies.make

install :: MakeND280DetectorCovinstall

MakeND280DetectorCovinstall :: $(MakeND280DetectorCov_dependencies) $(cmt_local_MakeND280DetectorCov_makefile)
	$(echo) "(constituents.make) Starting install MakeND280DetectorCov"
	@-$(MAKE) -f $(cmt_local_MakeND280DetectorCov_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install MakeND280DetectorCov done"

uninstall :: MakeND280DetectorCovuninstall

MakeND280DetectorCovuninstall :: $(cmt_local_MakeND280DetectorCov_makefile)
	$(echo) "(constituents.make) Starting uninstall MakeND280DetectorCov"
	@-$(MAKE) -f $(cmt_local_MakeND280DetectorCov_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall MakeND280DetectorCov done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ MakeND280DetectorCov"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ MakeND280DetectorCov done"
endif


#-- end of constituent ------
#-- start of constituent_lock ------

cmt_make_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_make_has_target_tag

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(BANFF_tag)_make.make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_BANFF_makesetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(BANFF_tag)_make.make
cmt_local_tagfile_make = $(bin)$(BANFF_tag)_make.make
cmt_final_setup_make = $(bin)BANFF_makesetup.make
cmt_local_make_makefile = $(bin)make.make
endif

make_extratags = -tag_add=target_make

#$(cmt_local_tagfile_make) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_make) ::
else
$(cmt_local_tagfile_make) :
endif
	$(echo) "(constituents.make) Rebuilding setup.make $(cmt_local_tagfile_make)"
	@if test -f $(cmt_local_tagfile_make); then /bin/rm -f $(cmt_local_tagfile_make); fi ; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) build tag_makefile >>$(cmt_local_tagfile_make); \
	  if test -f $(cmt_final_setup_make); then /bin/rm -f $(cmt_final_setup_make); fi; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) show setup >>$(cmt_final_setup_make)
	$(echo) setup.make ok

else

ifdef READONLY
cmt_local_tagfile_make = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
cmt_final_setup_make = /tmp/CMT_BANFFsetup.make
cmt_local_make_makefile = /tmp/CMT_make$(cmt_lock_pid).make
else
#cmt_local_tagfile_make = $(BANFF_tag).make
cmt_local_tagfile_make = $(bin)$(BANFF_tag).make
cmt_final_setup_make = $(bin)BANFFsetup.make
cmt_local_make_makefile = $(bin)make.make
endif

endif

ifndef QUICK
$(cmt_local_make_makefile) :: $(make_dependencies) $(cmt_local_tagfile_make) build_library_links dirs
else
$(cmt_local_make_makefile) :: $(cmt_local_tagfile_make)
endif
	$(echo) "(constituents.make) Building make.make"; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) build constituent_makefile -out=$(cmt_local_make_makefile) make

make :: $(make_dependencies) $(cmt_local_make_makefile)
	$(echo) "(constituents.make) Creating make${lock_suffix} and Starting make"
	@${lock_command} make${lock_suffix} || exit $$?; \
	  retval=$$?; \
	  trap '${unlock_command} make${lock_suffix}; exit $${retval}' 1 2 15; \
	  $(MAKE) -f $(cmt_local_make_makefile) cmt_lock_pid=$${cmt_lock_pid} make; \
	  retval=$$?; ${unlock_command} make${lock_suffix}; exit $${retval}
	$(echo) "(constituents.make) make done"

clean :: makeclean

makeclean :: $(makeclean_dependencies) ##$(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting makeclean"
	@-if test -f $(cmt_local_make_makefile); then \
	  $(MAKE) -f $(cmt_local_make_makefile) cmt_lock_pid=$${cmt_lock_pid} makeclean; \
	fi

##	  /bin/rm -f $(cmt_local_make_makefile) $(bin)make_dependencies.make

install :: makeinstall

makeinstall :: $(make_dependencies) $(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting install make"
	@-$(MAKE) -f $(cmt_local_make_makefile) cmt_lock_pid=$${cmt_lock_pid} install
	$(echo) "(constituents.make) install make done"

uninstall :: makeuninstall

makeuninstall :: $(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting uninstall make"
	@-$(MAKE) -f $(cmt_local_make_makefile) cmt_lock_pid=$${cmt_lock_pid} uninstall
	$(echo) "(constituents.make) uninstall make done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ make"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ make done"
endif


#-- end of constituent_lock ------
#-- start of constituents_trailer ------

clean :: remove_library_links

remove_library_links ::
	$(echo) "Removing library links"; \
	  $(remove_library_links)

makefilesclean ::

###	@/bin/rm -f checkuses

###	/bin/rm -f *.make*

clean :: makefilesclean

binclean :: clean
	$(echo) "Removing binary directory $(bin)"
	@if test ! "$(bin)" = "./"; then \
	  /bin/rm -rf $(bin); \
	fi

#-- end of constituents_trailer ------
