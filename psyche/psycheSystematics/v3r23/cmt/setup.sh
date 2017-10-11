# echo "Setting psycheSystematics v3r23 in /physics/home/mhogan/software/t2k-software/psyche"

if test "${CMTROOT}" = ""; then
  CMTROOT=/physics/software/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh

tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=psycheSystematics -version=v3r23 -path=/physics/home/mhogan/software/t2k-software/psyche  -no_cleanup $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

