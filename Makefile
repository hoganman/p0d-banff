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
#
# If there are changes to FlatTree gneration only, then "make CC1pi_v2.exe" is all you need to do

LIB	:= ${P0DBANFFROOT}/lib
SRC	:= ${P0DBANFFROOT}/src
MACROS  := ${P0DBANFFROOT}/macros
BIN	:= ${P0DBANFFROOT}/bin
VPATH    = ${PWD}:$(SRC)

EMPTYSTRING := 

# Include machine specific flags and locations (inc. files & libs)
#include $(P0DBANFFROOT)/src/make/Make.include

#Your compiler
CXX	= $(shell which g++)
LD	= $(shell which g++)
#command to run to remove files (see 'clean' later)
RM	= $(shell which rm) -f

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
#              Optimize, Warnings on, Generate code that can be copied and executed anywhere in the memory
CXXFLAGS	:= -O -Wall -fPIC -MMD -MP $(INCLUDES) 
#               Optimize, create a shared library
LDFLAGS		= $(ROOTGLIBS) -O -shared -g -Wl,--no-as-needed 

ROOTLIBRARIES := -L$(ROOTLIBSDIR) -lGui -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -lMinuit -lGeom -lEG -lGenVector -lTMVA

##### All our targets#####
# .o Objects
ALLOBJS         := 
# lib%.so Shared library Objects
ALLLIBS		:= libROOTUtils.so
# Executables
ALLEXES         := 
##### Rules #####

TGT =  $(ALLOBS) $(ALLLIBS) $(ALLEXES)

.PHONY: all clean

all: $(TGT) 

##### compile all the objects "ALLOBJS"
%.o: src/%.cxx
	$(CXX) -c $< $(CXXFLAGS)


##### create a shared library from the above compiled objects
# lib%.so: %.o
# 	$(CXX) $(LDFLAGS) $(SOFLAGS) -o $(LIB)/$@ $^
################################

libROOTUtils.so: ROOTUtils.o
	$(LD) $(LDFLAGS) ROOTUtils.o -o $(LIB)/$@

#add a rule to clean all generated files from your directory
clean: 
	$(RM) $(BIN)/*exe $(LIB)/*.so $(SRC)/*.d $(SRC)/*.o ./*.o ./*.d
