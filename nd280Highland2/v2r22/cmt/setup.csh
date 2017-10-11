# echo "Setting nd280Highland2 v2r22 in /physics/home/mhogan/software/t2k-software-clone"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /physics/software/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=nd280Highland2 -version=v2r22 -path=/physics/home/mhogan/software/t2k-software-clone  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

