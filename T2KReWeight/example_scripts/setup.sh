#!/bin/bash
#
# Example environment setup script (called by example_config.sh)
#


# The following are the standard libs and env variables needed when 
# compiling T2KReWeight with oaAnalysis enabled

# GlobalAnalysisTools... where you are working now
GAT=/home/hastings/t2k/reweight/dev7/GlobalAnalysisTools
export T2KREWEIGHT=${GAT}/T2KReWeight


# ROOT installation (I may have a few versions)
export ROOTSYS="$(echo ${HOME}/opt/tools/ROOT/* | tr ' ' '\n' | sort -r | head -1)/$(uname)-$(arch)/root"

# Where I keep most other dependencies (geniew, pythia, cernlib...). Used below.
OPT="${HOME}/t2k/opt"

export PATH=$T2KREWEIGHT/bin:$PATH:$T2KREWEIGHT/app:$ROOTSYS/bin:$PATH;
export LD_LIBRARY_PATH=$T2KREWEIGHT/lib:$LD_LIBRARY_PATH;

# Get the analysis reader from new higland2
# Only needed with ./configure --enable-oaanalysis
export CMTPATH="/home/hastings/t2k/highland/dev03:/home/hastings/opt/tools"
. /home/hastings/t2k/highland/dev03/nd280Highland2/v2r13/cmt/setup.sh
export OAANALYSISLIBS=$OAANALYSISREADERROOT/$OAANALYSISREADERCONFIG
export LD_LIBRARY_PATH=$OAANALYSISLIBS:$LD_LIBRARY_PATH;



# GENIE and dependencies
# Only needed with ./configure --enable-genie
export GENIE=${OPT}/genie/GENIE-Generator_v2.10.0
export PYTHIA6_LIB=${OPT}/pythia/v6_424/lib

#export LIBXML_LIB=/usr/lib/x86_64-linux-gnu
#export LOG4CPP_LIB=/usr/lib
#export LHAPDF_LIB=/usr/lib/x86_64-linux-gnu

#export LD_LIBRARY_PATH=$LOG4CPP_LIB:$LIBXML_LIB:$LHAPDF_LIB:$PYTHIA6_LIB:$ROOTSYS/lib:$GENIE/lib:$LD_LIBRARY_PATH;
export LD_LIBRARY_PATH=$PYTHIA6_LIB:$ROOTSYS/lib:$GENIE/lib:$LD_LIBRARY_PATH;
export LIBXML_INC=/usr/include/libxml2
#export LOG4CPP_INC=/usr/include
export LHAPDF_INC=/usr/include
export LHAPATH=/usr/share/lhapdf/PDFsets

export PATH=$GENIE/bin:$PATH;



# NEUT and CERNLIB
# Only needed with ./configure --enable-neut
export CERN=${OPT}/cern
export CERN_LEVEL=2005
export CERN_ROOT=$CERN/$CERN_LEVEL
export CERNLIB=$CERN_ROOT/lib
export LD_LIBRARY_PATH=$CERNLIB:$LD_LIBRARY_PATH
export PATH=$CERN_ROOT/bin:$PATH




export NEUT_ROOT=${OPT}/neut/neut_5.3.3_Autumn2016
#export NEUT_ROOT=${OPT}/neut/neut_5.3.6_assembla
export PATH=$NEUT_ROOT/src/neutsmpl/bin:$PATH
export LD_LIBRARY_PATH=$NEUT_ROOT/src/reweight:$LD_LIBRARY_PATH;



# JNuBeam
# Only needed with ./configure --enable-jnubeam
export JNUBEAM=${GAT}/JReWeight
export LD_LIBRARY_PATH=${JNUBEAM}:$LD_LIBRARY_PATH;
export JREWEIGHT_INPUTS=${JNUBEAM}/inputs


# NIWG
# Only needed with ./configure --enable-niwg
export NIWG=${GAT}/NIWGReWeight
export LD_LIBRARY_PATH=${NIWG}:$LD_LIBRARY_PATH;
export NIWGREWEIGHT_INPUTS=${NIWG}/inputs


# GEANT
# Only needed with ./configure --enable-geant
export GEANTRW=${OPT}/GlobalAnalysisTools/GEANTReWeight
export LD_LIBRARY_PATH=${GEANTRW}:$LD_LIBRARY_PATH;
export GEANTREWEIGHT_INPUTS=${GEANTRW}/inputs

#TRACKERFILE
# Only needed with ./configure --enable-trackerfile
#export TRACKERFILE_LIB=/path/to/numuCCBANFFAnalysis_macros_directory
#export LD_LIBRARY_PATH=$TRACKERFILE_LIB:$LD_LIBRARY_PATH

# PSYCHE
# No need to do this explicitly since it is extracted from highland2 setup above

# For Mac's
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH
