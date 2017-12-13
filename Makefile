#
# Makefile 
#
# Matthew Hogan
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
VPATH    = ${PWD}:$(SRC)

EMPTYSTRING := 

# Include machine specific flags and locations (inc. files & libs)
ifneq ("$(wildcard ${SRC}/make/Make.include)", "")
include ${SRC}/make/Make.include
endif

#Your compiler
CXX	= $(shell which g++) $(CXX_MACHINE_FLAGS)
LD	= $(shell which g++)
#command to run to remove files (see 'clean' later)
RM	= $(shell which rm) -f
ROOTCINT = $(shell which rootcint) -f

#ROOT provides root-config to automate building of your include path and libs path
ROOT            := $(shell which root) -l -b -q
ROOTGLIBS    	:= $(shell root-config --glibs)
ROOT_INCLUDES	:= -I$(shell root-config --incdir)
ROOTLIBSDIR     := $(shell root-config --libdir)


PARSED_C_INCLUDE_PATH	:= -I${C_INCLUDE_PATH} -I$(MACROS)
PARSED_C_INCLUDE_PATH	:= $(subst :, -I,$(PARSED_C_INCLUDE_PATH))


##### Library path and compiler / linker / rootcint options #####
INCLUDES := $(ROOT_INCLUDES)


# compiler and preprocessor flags
# Optimize, Warnings on, Generate code that can be copied and executed anywhere in the memory
CXXFLAGS	:= -O -Wall -fPIC -MMD -MP $(INCLUDES) 
# Optimize, create a shared library
LDFLAGS		= $(ROOTGLIBS) -O -shared -g -Wl,--no-as-needed 

ALLCLASSES := MakeClSampleSummary MakeClFlatTree P0DBANFFInterface P0DBANFFTypes ROOTUtils BenchmarkProcess ColorBlindFriendly PsycheHeader_Minimal TotalPOT

##### All our targets#####
# dict.o Object
ROOTDICTS := $(foreach obj, $(ALLCLASSES), $(obj)dict.o)
OBJS     := $(foreach obj, $(ALLCLASSES), $(obj).o)
ALLOBJS := $(ROOTDICTS) $(OBJS)

#ALLDICS         := MakeClSampleSummarydict.o MakeClFlatTreedict.o P0DBANFFInterfacedict.o P0DBANFFTypesdict.o ROOTUtilsdict.o BenchmarkProcessdict.o ColorBlindFriendlydict.o PsycheHeader_Minimaldict.o TotalPOTdict.o 
# .o Objects 
#ALLOBJS         := $(ALLDICS) MakeClSampleSummary.o MakeClFlatTree.o P0DBANFFInterface.o P0DBANFFTypes.o ROOTUtils.o BenchmarkProcess.o ColorBlindFriendly.o PsycheHeader_Minimal.o TotalPOT.o
# lib%.so Shared library Objects
ALLLIBS		:= libP0DBANFF.so
# Executables
ALLEXES         := 
##### Rules #####

TGT =  $(ALLOBS) $(ALLLIBS) $(ALLEXES)

.PHONY: all clean

all: $(TGT) 

##### compile all the objects "ALLOBJS"
%dict.o: %dict.C
	$(CXX) $(CXXFLAGS) -c dict/$<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $<

%dict.C: %.hxx
	$(ROOTCINT) dict/$@ -c $<
#######################

libP0DBANFF.so: $(ALLOBJS)
	$(LD) $(LDFLAGS) $^ -o $(LIB)/$@

#add a rule to clean all generated files from your directory
clean: 
	$(RM) $(BIN)/*exe $(LIB)/*.so $(SRC)/*.d $(SRC)/*.o ./*.o ./*.d $(DIC)/*dict.*
