#!/bin/bash

#$ -j yes
#$ -wd /home/other/mhogan/physics/software/t2k-software-clone/BANFF/v3r16/inputs
#$ -o  /home/other/mhogan/physics/software/t2k-software-clone/BANFF/v3r16/Linux-x86_64
#$ -l qname="physics.q"
#$ -l h_cpu=96::
#$ -l h_data=24G
#$ -v OMP_NUM_THREADS=16
#$ -pe OpenMP 16
source /physics/home/mhogan/software/t2k-software-clone/SetupBANFF-Head.sh
source $BANFFROOT/cmt/setup.sh
$BANFFROOT/Linux-x86_64/RunJointFit2017.exe
