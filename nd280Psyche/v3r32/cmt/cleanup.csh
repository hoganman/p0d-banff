if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /physics/software/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh
set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=nd280Psyche -version=v3r32 -path=/home/other/mhogan/software/p0d-banff $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

