#!/usr/bin/env gmake
# Makefile
#
# – $@ : shortcut for the ‘target’;
# – $< : shortcut for the first dependency;
# – $^ : shortcut for all dependencies;
# – %  : signifies string substitution.
#
# using this makefile from scratch
# executing "make" will generate all libraries and executiables

LIB	:= ${P0DBANFFROOT}/lib
SRC	:= ${P0DBANFFROOT}/src
MACROS  := ${P0DBANFFROOT}/macros
DIC     := ${P0DBANFFROOT}/dict
VPATH    = ${P0DBANFFROOT}:$(SRC):$(DIC):$(LIB)

EMPTYSTRING :=

# Include machine specific flags and locations (inc. files & libs)
ifneq ("$(wildcard ${SRC}/make/Make.include)", "")
include ${SRC}/make/Make.include
endif

#ROOT provides root-config to automate building of your include path and libs path
ROOT          := $(shell which root) -l -b -q
ROOTCONFIG    := $(shell which root-config)
ROOTCINT      := $(shell which rootcint)
ROOTLIBSDIR   := $(shell root-config --libdir)
ROOTGLIBS     := $(shell root-config --glibs)
ROOTINCDIR    := $(shell root-config --incdir)
ROOT_INCLUDES := -I$(ROOTINCDIR)

#Your compiler
CXX	= $(shell root-config --cxx --cflags) $(CXX_MACHINE_FLAGS)
LD	= $(CXX)
#command to run to remove files (see 'clean' later)
RM	= $(shell which rm) -f

PARSED_C_INCLUDE_PATH	:= -I${C_INCLUDE_PATH} -I$(MACROS)
PARSED_C_INCLUDE_PATH	:= $(subst :, -I,$(PARSED_C_INCLUDE_PATH))

##### Library path and compiler / linker / rootcint options #####
INCLUDES := -I${P0DBANFFROOT}

# compiler and preprocessor flags
# Optimize, Warnings on, Generate code that can be copied and executed anywhere in the memory
CXXFLAGS	:= -O -Wall -fPIC -MMD -MP $(INCLUDES)
# Optimize, create a shared library
LDFLAGS		= $(ROOTGLIBS) -O -shared -g -Wl,--no-as-needed

ALLCLASSES_CXX :=  src/RunName.cxx src/T2KDataMC.cxx src/PlottingSelectionInfo.cxx src/PlottingSample.cxx src/HEPConstants.cxx src/XMLTools.cxx src/MakeClSampleSummary.cxx src/MakeClFlatTree.cxx src/P0DBANFFInterface.cxx src/BenchmarkProcess.cxx src/POTHeader.cxx src/TotalPOT.cxx src/TN80POT.cxx src/AnalysisBins.cxx src/Samples.cxx src/SampleId.cxx src/DefineCuts.cxx src/CanvasCoordinates.cxx src/AnalysisBins2D.cxx src/BANFFPostFit.cxx src/MakeClRunSyst_New.cxx
ALLCLASSES_HXX := $(ALLCLASSES_CXX:.cxx=.hxx)

# dict.o Object
ROOTDICTS_O := $(subst src, lib, $(ALLCLASSES_HXX:.hxx=dict.o))
# regular C++ .o objects
# OBJS     := $(foreach obj, $(ALLCLASSES), $(obj).o)
OBJS     := $(subst src, lib, $(ALLCLASSES_CXX:.cxx=.o))

##### All our targets#####
ALLOBJS := $(OBJS) $(ROOTDICTS_O)
# lib%.so Shared library Objects
ALLLIBS	:= libP0DBANFF.so
# Executables
ALLEXES :=
##### Rules #####

TGT =  $(ALLOBS) $(ALLLIBS) $(ALLEXES)

.PHONY: all clean distclean

all: $(TGT)

##### compile all the objects "ALLOBJS" #####
lib/%dict.o: src/%.hxx
	$(ROOTCINT) -f $(subst src, dict, $(<:.hxx=dict.C)) -c $<
	$(CXX) $(CXXFLAGS) -c $(subst src, dict, $(<:.hxx=dict.C)) -o $@

lib/%.o: src/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@
#############################################

##########  # master library   #############
libP0DBANFF.so: $(ALLOBJS)
	$(LD) $(LDFLAGS) $^ -o lib/$@
#############################################

#add a rule to clean all generated files from your directory
clean:
	$(RM) app/*pyc

distclean:
	$(RM) bin/*exe lib/*o src/*.d src/*.o ./*.o ./*.d dict/*dict.* ./*dict.* app/*pyc lib/*.d
