#!/bin/bash

#$ -j yes
#$ -wd /home/other/mhogan/physics/software/p0d-banff/p0d-banff/BANFF/v3r16/inputs
#$ -o  /home/other/mhogan/physics/software/p0d-banff/p0d-banff/BANFF/v3r16/Linux-x86_64
#$ -l qname="physics.q"
#$ -l h_cpu=96::
#$ -l h_data=24G
#$ -v OMP_NUM_THREADS=12
#$ -pe OpenMP 12
source /physics/home/mhogan/software/p0d-banff/p0d-banff/Setup-P0DBANFF.sh
source $BANFFROOT/cmt/setup.sh
$BANFFROOT/Linux-x86_64/RunJointFit2017.exe $BANFFROOT/parameters/BANFF.parameters_Data.dat
