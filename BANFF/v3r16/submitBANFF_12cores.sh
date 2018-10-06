#!/bin/bash

#$ -j yes
#$ -wd /home/other/mhogan/physics/software/p0d-banff/p0d-banff/BANFF/v3r16/inputs
#$ -o  /home/other/mhogan/physics/software/p0d-banff/p0d-banff/BANFF/v3r16/Linux-x86_64
#$ -l hostname="node1|node2|node3|node4|node5|node6|node7|node8|node9|node10|node11|node12|node13|node14|node15|node16|node17|node18|node19"
#$ -l h_cpu=120::
#$ -l h_data=250G
#$ -v OMP_NUM_THREADS=12
#$ -pe OpenMP 12
source /physics/home/mhogan/software/p0d-banff/p0d-banff/Setup-P0DBANFF.sh
# source $BANFFROOT/cmt/setup.sh
# $BANFFROOT/Linux-x86_64/RunJointFit2017.exe # $BANFFROOT/parameters/BANFF.parameters_Data.dat
$BANFFROOT/Linux-x86_64/RunP0DFitOnly_WithRHC.exe # -i /physics/home/mhogan/software/p0d-banff/p0d-banff/BANFF/v3r16/inputs/P0DSamplesOnly.xml -o /physics/home/mhogan/software/p0d-banff/p0d-banff/BANFF/v3r16/Linux-x86_64/P0DFitOnly.root
