# echo "Setting nd280Psyche v3r32 in /home/other/mhogan/software/p0d-banff"

if test "${CMTROOT}" = ""; then
  CMTROOT=/physics/software/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=nd280Psyche -version=v3r32 -path=/home/other/mhogan/software/p0d-banff  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

