# echo "Setting baseP0DAnalysis v2r6 in /physics/home/mhogan/software/t2k-software-clone/highland2"

if test "${CMTROOT}" = ""; then
  CMTROOT=/physics/software/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=baseP0DAnalysis -version=v2r6 -path=/physics/home/mhogan/software/t2k-software-clone/highland2  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

