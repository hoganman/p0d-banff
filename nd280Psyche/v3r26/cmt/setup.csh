# echo "Setting nd280Psyche v3r26 in /physics/home/mhogan/software/t2k-software-clone"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /physics/software/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=nd280Psyche -version=v3r26 -path=/physics/home/mhogan/software/t2k-software-clone  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

