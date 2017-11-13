#!/bin/sh
#################################################
#### Update P0DBANFFROOT for every checkout #####
#export P0DBANFFROOT=/physics/home/mhogan/software/p0d-banff/p0d-banff
#################################################

if [ -z ${P0DBANFFROOT+x} ]; then
    echo "P0DBANFFROOT was not declared. Setting it to the CWD="$PWD
    export P0DBANFFROOT=$PWD
fi

if [ -z ${ROOTSYS+x} ]; then 
#   echo "ROOTSYS is unset"; 
    source $P0DBANFFROOT/SetupBase.sh
fi


PYTHONPATH=${P0DBANFFROOT}/macros:${PYTHONPATH}

CMTPATH=${P0DBANFFROOT}:${CMTPATH}
CMTPROJECTPATH=${P0DBANFFROOT}/../:${CMTPROJECTPATH}

source ${P0DBANFFROOT}/BANFF/v3r16/cmt/setup.sh

export T2KREWEIGHT=${P0DBANFFROOT}/T2KReWeight
PATH=${T2KREWEIGHT}/bin:${T2KREWEIGHT}/app:${PATH}
LD_LIBRARY_PATH=${T2KREWEIGHT}/lib:${LD_LIBRARY_PATH}

# ND280 oaAnalysis libs (See README for generating this)
# Only needed with ./configure --enable-oaanalysis
export OAANALYSISREADERROOT=$T2KREWEIGHT/Linux-x86_64
export OAANALYSISLIBS=$OAANALYSISREADERROOT
export LD_LIBRARY_PATH=$OAANALYSISLIBS:$LD_LIBRARY_PATH
#T2KREWEIGHT complains that "Use of uninitialized value $ENV{"OAANALYSISREADERROOT"} in concatenation (.) or string at ./configure line 141."

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


#NEUT and CERNLIB
# Only needed with ./configure --enable-neut
############ Update this yourself 
#export CERN=/physics/software/cernlib-2005
#export CERN_LEVEL=2005
export CERN_ROOT=${CERN}/${CERN_LEVEL}
export CERNLIB=$CERN_ROOT/lib
export LD_LIBRARY_PATH=$CERNLIB:$LD_LIBRARY_PATH
export PATH=$CERN_ROOT/bin:$PATH

#export NEUT=/physics/software/neut
#export NEUT_VERSION=v1r27p1_5.3.3
#export NEUT_ROOT=${NEUT}/${NEUT_VERSION}
export PATH=$NEUT_ROOT/bin:$PATH
export LD_LIBRARY_PATH=$NEUT_ROOT/src/reweight:$LD_LIBRARY_PATH


#export JNUBEAM=/JReWeight
#export LD_LIBRARY_PATH=${JNUBEAM}:$LD_LIBRARY_PATH;
#export JREWEIGHT_INPUTS=${JNUBEAM}/inputs

# NIWG
# Only needed with ./configure --enable-niwg
#export NIWG=/physics/home/mhogan/software/v11r31/GlobalAnalysisTools/NIWGReWeight
LD_LIBRARY_PATH=${NIWG}:$LD_LIBRARY_PATH;
export NIWGREWEIGHT_INPUTS=${NIWG}/inputs


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
source ${P0DBANFFROOT}/psyche/psycheSteering/v3r18/cmt/setup.sh
LD_LIBRARY_PATH=${PSYCHECOREROOT}/Linux-x86_64:$LD_LIBRARY_PATH
###################
