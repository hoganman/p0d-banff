#!/bin/sh

# Prelim / P0D-BANFF Project independent #
#######################
OLD_PATH=$PATH
#######################

export ND280_CPP_COMPILER=$(which g++)
export ND280_CC_COMPILER=$(which gcc)
export ND280_FORTRAN_COMPILER=$(which gfortran)
export ND280VERSION=v11r31
export ROOTVERSION=v5r34p09n04
alias gmake="make"

if [ $HOSTNAME == "S50-A" ]; then
    export SOFTWARE=/home/mhogan/software
    export ND280=$SOFTWARE/p0d-banff
    export PYTHONPATH=.:$HOME:$HOME/bin
fi

if [ $HOSTNAME == "ens-hpc" ] || [ $HOSTNAME == "node7" ]; then
    export SOFTWARE=/physics/software
    export ND280=$SOFTWARE/t2k/ND280/v11r31
    export PYTHONPATH=.:/physics/INSTALLATION/bin:/physics/home/mhogan/bin:/physics/home/mhogan:$HOME:$HOME/bin
fi

if [ $HOSTNAME == "hep" ]; then
    export SOFTWARE=/Raid/software/t2k
    export ND280=$SOFTWARE/t2k/nd280
    export PYTHONPATH=.:$HOME:$HOME/bin
fi

if [ $HOSTNAME == "bkup" ]; then
    export SOFTWARE=/Raid/software/t2k
    export ND280=$SOFTWARE/t2k/nd280
    export PYTHONPATH=.:$HOME:$HOME/bin
fi


# CERNLIB and NEUT
export CERN_LEVEL=2005
export CERN=${SOFTWARE}/cernlib-${CERN_LEVEL}
export NEUT=/physics/software/neut
export NEUT_VERSION=v1r27p1_5.3.3
export NEUT_ROOT=${NEUT}/${NEUT_VERSION}

#source cmt
export CMTROOT=${SOFTWARE}/CMT/v1r20p20081118
source ${CMTROOT}/mgr/setup.sh
export ND280REPO=:ext:anoncvs@repo.nd280.org:/home/trt2kmgr/ND280Repository
#use this for GlobalAnalysisTools/NIWGReWeight
export T2KREPO=:ext:anoncvs@repo.t2k.org:/home/trt2kmgr/T2KRepository
export CVSROOT=$ND280REPO
export CVS_RSH=ssh
unset CVS_SERVER
export CMTPATH=$ND280
export CMTPROJECTPATH=$(dirname ${ND280})
source ${ND280}/nd280/$ND280VERSION/cmt/setup.sh
# source ${ND280}/nd280AnalysisTools/*/cmt/setup.sh

#done
source $ND280/ROOT/$ROOTVERSION/Linux-x86_64/root/bin/thisroot.sh

#######################
PATH=$OLD_PATH:$PATH
#######################
