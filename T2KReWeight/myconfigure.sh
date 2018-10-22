source $P0DBANFFROOT/highland2/oaAnalysisReader/*/cmt/setup.sh
./configure \
    --enable-neut \
        --with-cern=$CERN_ROOT \
    --enable-oaanalysis \
        --with-oaanalysis-lib=$OAANALYSISLIBS \
    --disable-genie \
    --enable-niwg \
    --enable-psyche;
#   --enable-jnubeam \
#       --with-pythia6-lib=$PYTHIA_LIB \
#       --with-lhapdf-inc=$LHAPDF_INC \
#       --with-lhapdf-lib=$LHAPDF_LIB \
#       --with-libxml2-inc=$LIBXML_INC \
#       --with-libxml2-lib=$LIBXML_LIB \
#       --with-log4cpp-inc=$LOG4CPP_INC \
#       --with-log4cpp-lib=$LOG4CPP_LIB \
#   --enable-geant \
