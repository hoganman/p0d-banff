#!/bin/bash

#$ -j yes
#$ -wd /home/other/mhogan/physics/software/v11r31/BANFF/v3r17/inputs
#$ -o /home/other/mhogan/physics/software/v11r31/BANFF/v3r17/Linux-x86_64
#$ -l qname="physics.q"
#$ -l h_cpu=96::
#$ -l h_data=24G
#$ -v OMP_NUM_THREADS=4
#$ -pe OpenMP 4
source /physics/home/mhogan/Run_At_Start.sh
source /physics/home/mhogan/software/v11r31/BANFF/v3r17/cmt/setup.sh
/home/other/mhogan/physics/software/v11r31/BANFF/v3r17/Linux-x86_64/RunJointFit2015.exe
