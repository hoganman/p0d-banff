if test "${CMTROOT}" = ""; then
  CMTROOT=/physics/software/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=psycheIO -version=v3r17 -path=/physics/home/mhogan/software/t2k-software/psyche $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

