#!/bin/bash

#$ -j yes
#$ -wd /home/other/mhogan/physics/software/p0d-banff/p0d-banff/BANFF/v3r16/inputs
#$ -o  /home/other/mhogan/physics/software/p0d-banff/p0d-banff/BANFF/v3r16
#$ -l qname="defaultfaculty.q|long.q|physics.q"
#$ -l hostname="node1|node2|node3|node4|node5|node6|node7|node8|node9|node10|node11|node12|node13|node14|node15|node16|node17|node18|node19|node40|node41|node42|node43|node44|node45"
#$ -l h_cpu=96::
#$ -l h_data=60G
#$ -v OMP_NUM_THREADS=12
#$ -pe OpenMP 12
source /physics/home/mhogan/software/p0d-banff/p0d-banff/Setup-P0DBANFF.sh
source $BANFFROOT/cmt/setup.sh
INPUTFILE=$BANFFROOT/inputs/Samples.xml
OUTPUTFILE=$BANFFROOT/ApplyPostFitToNominalMC_BANFF2017.root
$BANFFROOT/Linux-x86_64/ApplyPostFitToNominalMC.exe -i $INPUTFILE -o $OUTPUTFILE
