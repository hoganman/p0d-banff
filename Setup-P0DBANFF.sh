#!/usr/bin/env bash
#######################################################
#### Update P0DBANFFROOT for every host checkhout #####
####               also update SetupBase.sh       #####
#######################################################

export S50A="S50-A"
export TA50D="TECRA-A50-D"
export ENSHPC="ens-hpc"
export ENSSANDBOX="node7"
export HEP="hep.colostate.edu"
export BKUP="bkup.hep.colostate.edu"
export ISENSHPCQUEUENODE=0
ENSHPCNODES=( "node1" "node2" "node3")
for i in `seq 4 45`;
do
    ENSHPCNODES+=("node${i}")
done

export HOSTNAME=$(hostname)

if [ -z ${P0DBANFFROOT+x} ]; then

    # is it one of your laptops?
    if [ $HOSTNAME == $S50A ] || [ $HOSTNAME == $TA50D ]; then
        export P0DBANFFROOT=/home/mhogan/software/p0d-banff
    fi

    # is it one of the ENS login nodes?
    if [ $HOSTNAME == $ENSHPC ] || [ $HOSTNAME == $ENSSANDBOX ]; then
        export P0DBANFFROOT=/physics/home/mhogan/software/p0d-banff/p0d-banff
    fi

    # is it one of your HEPPA maintained servers?
    if [ $HOSTNAME == $HEP ] || [ $HOSTNAME == $BKUP ]; then
        export P0DBANFFROOT=/home/mhogan/software/p0d-banff
    fi

    # is it one of the ENS queue nodes?
    for nodename in "${ENSHPCNODES[@]}"
    do
        if [ $nodename == $HOSTNAME ]; then
	    ISENSHPCQUEUENODE=1
	fi
    done

    if [ $ISENSHPCQUEUENODE == 1 ]; then
	echo "Running on ENS-HPC compute node"
        export P0DBANFFROOT=/physics/home/mhogan/software/p0d-banff/p0d-banff
    fi

    if [ -z ${P0DBANFFROOT+x} ]; then
        echo "ERROR: host $HOSTNAME NOT found. Unable to set P0DBANFFROOT"
        return
    fi
    echo "P0DBANFFROOT was not declared. Setting it to ${P0DBANFFROOT}"
fi

#################################################
#################################################
#################################################

source $P0DBANFFROOT/SetupBase.sh

export HIGHLAND2VERSION=v2r28
export BANFFVERSION=v3r16
export PSYCHESTEERINGVERSION=v3r24

CMTPATH=${P0DBANFFROOT}:${CMTPATH}
CMTPROJECTPATH=${P0DBANFFROOT}/../:${CMTPROJECTPATH}

export PATH=$P0DBANFFROOT/bin:$PATH
export LD_LIBRARY_PATH=${P0DBANFFROOT}/lib:${P0DBANFFROOT}/dict:${P0DBANFFROOT}:${LD_LIBRARY_PATH}

APP=${P0DBANFFROOT}/app
export PYTHONPATH=$APP:$P0DBANFFROOT/python2.7:${PYTHONPATH}

source ${P0DBANFFROOT}/nd280Highland2/$HIGHLAND2VERSION/cmt/setup.sh
source ${P0DBANFFROOT}/BANFF/$BANFFVERSION/cmt/setup.sh

export T2KREWEIGHT=${P0DBANFFROOT}/T2KReWeight
PATH=${T2KREWEIGHT}/bin:${T2KREWEIGHT}/app:${PATH}
LD_LIBRARY_PATH=${T2KREWEIGHT}/lib:${LD_LIBRARY_PATH}

# ND280 oaAnalysis libs (See README for generating this)
# Only needed with ./configure --enable-oaanalysis
# export OAANALYSISREADERROOT=$T2KREWEIGHT/Linux-x86_64
export OAANALYSISREADERROOT=$T2KREWEIGHT
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
export CERN_ROOT=${CERN}/${CERN_LEVEL}
export CERNLIB=$CERN_ROOT/lib
export LD_LIBRARY_PATH=$CERNLIB:$LD_LIBRARY_PATH
export PATH=$CERN_ROOT/bin:$PATH

export PATH=$NEUT_ROOT/bin:$PATH
export LD_LIBRARY_PATH=$NEUT_ROOT/src/reweight:$LD_LIBRARY_PATH

#export JNUBEAM=/JReWeight
#export LD_LIBRARY_PATH=${JNUBEAM}:$LD_LIBRARY_PATH;
#export JREWEIGHT_INPUTS=${JNUBEAM}/inputs

# NIWG
# Only needed with ./configure --enable-niwg
export NIWG=/physics/home/mhogan/software/v11r31/GlobalAnalysisTools/NIWGReWeight
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
source ${P0DBANFFROOT}/psyche/psycheSteering/$PSYCHESTEERINGVERSION/cmt/setup.sh
LD_LIBRARY_PATH=${PSYCHECOREROOT}/Linux-x86_64:$LD_LIBRARY_PATH
LD_LIBRARY_PATH=${PSYCHESTEERINGROOT}/Linux-x86_64:$LD_LIBRARY_PATH
###################

if [ -z ${IRODSROOT+x} ]; then
    cd ${IRODSROOT}
    source addclients.sh
    cd -
fi

GOPATH=$P0DBANFFROOT/go:$GOPATH
