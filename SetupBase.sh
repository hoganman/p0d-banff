#!/bin/sh

# Prelim / P0D-BANFF Project independent #
#######################
SOFTWARE=/physics/software
export PYTHONPATH=./:/physics/INSTALLATION/bin:/physics/home/mhogan/software/macros
export SCRIPTS=/physics/scripts
source ${SCRIPTS}/setup_cluster.sh

# CERNLIB and NEUT
export CERN_LEVEL=2005
export CERN=${SOFTWARE}/cernlib-${CERN_LEVEL}
export NEUT=/physics/software/neut
export NEUT_VERSION=v1r27p1_5.3.3
export NEUT_ROOT=${NEUT}/${NEUT_VERSION}
export NIWG=/physics/home/mhogan/software/v11r31/GlobalAnalysisTools/NIWGReWeight

#source cmt
export CMTROOT=${SOFTWARE}/CMT/v1r20p20081118
source ${CMTROOT}/mgr/setup.sh
export CVSROOT=:ext:anoncvs@repo.nd280.org:/home/trt2kmgr/ND280Repository
#use this for GlobalAnalysisTools/NIWGReWeight
#export CVSROOT=:ext:anoncvs@repo.t2k.org:/home/trt2kmgr/T2KRepository
export CVS_RSH=ssh
unset CVS_SERVER


#source cmt packages
export ND280ROOT=/physics/software/t2k/ND280/v11r31
export CMTPATH=$ND280ROOT
export CMTPROJECTPATH=${ND280ROOT}/../
source ${ND280ROOT}/nd280/*/cmt/setup.sh
source ${ND280ROOT}/nd280AnalysisTools/*/cmt/setup.sh
#for SUBSTRINGCMTPATH in $(echo $CMTPATH | tr ":" "\n"); do
#    if [ -f ${SUBSTRINGCMTPATH}/nd280/*/cmt/setup.sh ]; then
#	source ${SUBSTRINGCMTPATH}/nd280/*/cmt/setup.sh
#	source ${SUBSTRINGCMTPATH}/nd280AnalysisTools/*/cmt/setup.sh
#	break
#    fi
#done
source ${ND280ROOT}/ROOT/v5r34p09n04/Linux-x86_64/root/bin/thisroot.sh

