#!/bin/bash

#$ -j yes
#$ -wd /home/other/mhogan/physics/software/p0d-banff/p0d-banff/BANFF/v3r16/inputs
#$ -o  /home/other/mhogan/physics/software/p0d-banff/p0d-banff/BANFF/v3r16
#$ -l qname="physics.q"
#$ -l h_cpu=96::
#$ -l h_data=60G
#$ -v OMP_NUM_THREADS=16
#$ -pe OpenMP 16
source /physics/home/mhogan/software/p0d-banff/p0d-banff/Setup-P0DBANFF.sh
source $BANFFROOT/cmt/setup.sh
INPUTFILE=$BANFFROOT/inputs/Samples.xml
OUTPUTFILE=$BANFFROOT/ApplyPostFitToNominalMC_BANFF2017.root
$BANFFROOT/Linux-x86_64/ApplyPostFitToNominalMC.exe -i $INPUTFILE -o $OUTPUTFILE
