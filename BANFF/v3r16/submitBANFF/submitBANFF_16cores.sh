#!/bin/bash

#$ -j yes
#$ -wd /home/other/mhogan/physics/software/p0d-banff/p0d-banff/BANFF/v3r16/inputs
#$ -o  /home/other/mhogan/physics/software/p0d-banff/p0d-banff/BANFF/v3r16/Linux-x86_64
#$ -l qname="physics.q"
#$ -l h_cpu=9999::
#$ -l h_data=64G
#$ -v OMP_NUM_THREADS=16
#$ -pe OpenMP 16
source /physics/home/mhogan/software/p0d-banff/p0d-banff/Setup-P0DBANFF.sh
# source $BANFFROOT/cmt/setup.sh
$BANFFROOT/Linux-x86_64/RunJointFit2018.exe
# $BANFFROOT/Linux-x86_64/RunP0DFitOnly.exe # -i /physics/home/mhogan/software/p0d-banff/p0d-banff/BANFF/v3r16/inputs/P0DSamplesOnly.xml -o /physics/home/mhogan/software/p0d-banff/p0d-banff/BANFF/v3r16/Linux-x86_64/P0DFitOnly.root
