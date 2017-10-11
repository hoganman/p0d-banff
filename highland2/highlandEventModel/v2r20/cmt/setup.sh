# echo "Setting highlandEventModel v2r20 in /physics/home/mhogan/software/t2k-software-clone/highland2"

if test "${CMTROOT}" = ""; then
  CMTROOT=/physics/software/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=highlandEventModel -version=v2r20 -path=/physics/home/mhogan/software/t2k-software-clone/highland2  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

