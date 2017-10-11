# echo "Setting nd280Psyche v3r26 in /physics/home/mhogan/software/t2k-software-clone"

if test "${CMTROOT}" = ""; then
  CMTROOT=/physics/software/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=nd280Psyche -version=v3r26 -path=/physics/home/mhogan/software/t2k-software-clone  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

