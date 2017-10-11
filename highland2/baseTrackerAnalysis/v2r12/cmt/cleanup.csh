if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /physics/software/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh
set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=baseTrackerAnalysis -version=v2r12 -path=/physics/home/mhogan/software/t2k-software-clone/highland2 $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

