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
BIN	:= ${P0DBANFFROOT}/bin
DIC     := ${P0DBANFFROOT}/dict
APP     := ${P0DBANFFROOT}/app
VPATH    = ${PWD}:$(SRC):$(APP):$(DIC)

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
INCLUDES := $(ROOT_INCLUDES)

# compiler and preprocessor flags
# Optimize, Warnings on, Generate code that can be copied and executed anywhere in the memory
CXXFLAGS	:= -O -Wall -fPIC -MMD -MP $(INCLUDES)
# Optimize, create a shared library
LDFLAGS		= $(ROOTGLIBS) -O -shared -g -Wl,--no-as-needed

ALLCLASSES :=  HEPConstants XMLTools MakeClSampleSummary MakeClFlatTree P0DBANFFInterface BenchmarkProcess Header TotalPOT TN80POT AnalysisBins Samples SampleId DefineCuts CanvasCoordinates

# dict.o Object
ROOTDICTS := $(foreach obj, $(ALLCLASSES), $(obj)dict.o)
# regular C++ .o objects
OBJS     := $(foreach obj, $(ALLCLASSES), $(obj).o)

##### All our targets#####
ALLOBJS := $(ROOTDICTS) $(OBJS)
# lib%.so Shared library Objects
ALLLIBS		:= libP0DBANFF.so
# Executables
ALLEXES         :=
##### Rules #####

TGT =  $(ALLOBS) $(ALLLIBS) $(ALLEXES)

.PHONY: all clean

all: $(TGT)

##### compile all the objects "ALLOBJS" #####
%dict.o: %dict.C
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: $(SRC)/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@
#############################################

##### compile ROOTCINT dictionary #####
%dict.C: %.hxx
	$(ROOTCINT) -f $@ -c $<
#############################################

##########  # master library   #############
libP0DBANFF.so: $(ALLOBJS)
	$(LD) $(LDFLAGS) $^ -o $(LIB)/$@
#############################################

#add a rule to clean all generated files from your directory
clean:
	$(RM) $(SRC)/*.d $(SRC)/*.o ./*.o ./*.d ./*dict.* $(APP)/*pyc

distclean:
	$(RM) $(BIN)/*exe $(LIB)/*.so $(SRC)/*.d $(SRC)/*.o ./*.o ./*.d $(DIC)/*dict.* ./*dict.* $(APP)/*pyc
