#
# Example environment setup script (called by example_config.sh)
#
#!/bin/bash

# The following are the standard libs and env variables needed when 
# compiling T2KReWeight with oaAnalysis enabled
export T2KREWEIGHT=/path/to/T2KReWeight

export ROOTSYS=/path/to/root/

export PATH=$T2KREWEIGHT/bin:$PATH:$T2KREWEIGHT/app:$ROOTSYS/bin:$PATH;
export LD_LIBRARY_PATH=$T2KREWEIGHT/lib:$LD_LIBRARY_PATH;



# ND280 oaAnalysis libs (See README for generating this)
# Only needed with ./configure --enable-oaanalysis
#export OAANALYSISLIBS=/path/to/libReadoaAnalysis
#export LD_LIBRARY_PATH=$OAANALYSISLIBS:$LD_LIBRARY_PATH;


# GENIE and dependencies
# Only needed with ./configure --enable-genie
#export GENIE=/path/to/genie
#export PYTHIA6_LIB=/path/to/pythia6/lib
#export LIBXML_LIB=/path/to/libxml/lib
#export LOG4CPP_LIB=/path/to/log4cpp/lib
#export LHAPDF_LIB=/path/to/lhapdf/lib
#export LD_LIBRARY_PATH=$LOG4CPP_LIB:$LIBXML_LIB:$LHAPDF_LIB:$PYTHIA6_LIB:$ROOTSYS/lib:$GENIE/lib:$LD_LIBRARY_PATH;
#export LIBXML_INC=/path/to/libxml2/include
#export LOG4CPP_INC=/path/to/log4cpp/include
#export LHAPDF_INC=/path/to/lhapdf/include/
#export LHAPATH=/path/to/lhapdf/PDFsets
#export PATH=$GENIE/bin:$PATH;



# NEUT and CERNLIB
# Only needed with ./configure --enable-neut
#export CERN=/path/to/cernlib
#export CERN_LEVEL=2005
#export CERN_ROOT=$CERN/$CERN_LEVEL
#export CERNLIB=$CERN_ROOT/lib
#export LD_LIBRARY_PATH=$CERNLIB:$LD_LIBRARY_PATH
#export PATH=$CERN_ROOT/bin:$PATH

#export NEUT_ROOT=/path/to/neut
#export PATH=$NEUT_ROOT/src/neutsmpl/bin:$PATH
#export LD_LIBRARY_PATH=$NEUT_ROOT/src/reweight:$LD_LIBRARY_PATH;



# JNuBeam
# Only needed with ./configure --enable-jnubeam
#export JNUBEAM=/path/to/JReWeight
#export LD_LIBRARY_PATH=${JNUBEAM}:$LD_LIBRARY_PATH;
#export JREWEIGHT_INPUTS=${JNUBEAM}/inputs



# NIWG
# Only needed with ./configure --enable-niwg
#export NIWG=/path/to/NIWGReWeight
#export LD_LIBRARY_PATH=${NIWG}:$LD_LIBRARY_PATH;
#export NIWGREWEIGHT_INPUTS=${NIWG}/inputs


# GEANT
# Only needed with ./configure --enable-geant
#export GEANTRW=/path/to/GEANTReWeight
#export LD_LIBRARY_PATH=${GEANTRW}:$LD_LIBRARY_PATH;
#export GEANTREWEIGHT_INPUTS=${GEANTRW}/inputs

#TRACKERFILE
# Only needed with ./configure --enable-trackerfile
#export TRACKERFILE_LIB=/path/to/numuCCBANFFAnalysis_macros_directory
#export LD_LIBRARY_PATH=$TRACKERFILE_LIB:$LD_LIBRARY_PATH

#PSYCHE
# Only needed with ./configure --enable-psyche
#See the README for instructions on how to set up
#psycheIncludes and psycheLibs
#NB: If you compiled against psyche you need to make sure that all of the
#environment variables required to run psyche have also been set properly
#before you try to run any program.
#This can be accomplished by sourcing the normal script used to setup CMT
#followed by the setup script in the cmt directory of psycheSteering.
#export PSYCHELIBS=/path/to/psycheLibs
#export PSYCHEINCLUDES=/path/to/psycheIncludes
#export LD_LIBRARY_PATH=$PSYCHELIBS:$LD_LIBRARY_PATH





# For Mac's
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH
