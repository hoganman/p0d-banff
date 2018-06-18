#!/usr/bin/env bash

# echo Setting up software #(something recent will work)
# source $CMTPATH/oaOfflineDatabase/v2r5/cmt/setup.sh
# which database_updater.py

echo Setup for database write access
## Choose between test or production DB - be very careful not to mix them up!!
export ENV_TSQL_URL=mysql://trcaldb.t2k.org/testnd280calib
##export ENV_TSQL_URL=mysql://trcaldb.t2k.org/nd280calib
export ENV_TSQL_USER=t2kP0D_writer
export ENV_TSQL_PSWD=p2Dbcal

echo Starting upload
date

list=$P0DBANFFROOT/run_lists/mcp6_Spin_B/neut/Run4_Water_3files.list

file_list=`cat ${list}`
for file in $file_list;
do
    echo Submitting $file
    head -1 $file
    # this is the imortant line (commented out for test!)
    # python database_updater.py --convert_unsigned apply_local_update $file
done
date
