#!/bin/bash

echo Setting up software #(something recent will work)
export CMTPATH=/data/izdanko/ND280soft/v10r11p25
source /data/CMT/v1r18p20061003/mgr/setup.sh
source $CMTPATH/oaOfflineDatabase/v2r3p1/cmt/setup.sh
which database_updater.py

echo Setup for database write access
## Choose between test or production DB - be very careful not to mix them up!!
export ENV_TSQL_URL=mysql://trcaldb.t2k.org/testnd280calib
##export ENV_TSQL_URL=mysql://trcaldb.t2k.org/nd280calib
export ENV_TSQL_USER=t2kP0D_writer
export ENV_TSQL_PSWD=p2Dbcal
env | grep ENV_TSQL

echo Starting upload
date

list=const_files.lst

file_list=`cat ${list}`
for file in ${file_list}
do
  echo Submitting $file
  head -1 $file
# this is the imortant line (commented out for test!)
#  python database_updater.py --convert_unsigned apply_local_update $file
done
date
