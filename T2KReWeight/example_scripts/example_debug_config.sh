#
# Example config script. Default is all engines disabled. Before 
# enabling engines, make sure environment is set correctly in 
# example_setup.sh
#
#!/bin/bash

source example_setup.sh;

./configure \
    --disable-neut \
        --with-cern=$CERN_ROOT \
    --disable-jnubeam \
	--disable-oaanalysis \
		--with-oaanalysis-lib=$OAANALYSISLIBS \
	--disable-genie \
		--with-pythia6-lib=$PYTHIA6_LIB \
		--with-lhapdf-inc=$LHAPDF_INC \
 		--with-lhapdf-lib=$LHAPDF_LIB \
		--with-libxml2-inc=$LIBXML_INC \
		--with-libxml2-lib=$LIBXML_LIB \
		--with-log4cpp-inc=$LOG4CPP_INC \
		--with-log4cpp-lib=$LOG4CPP_LIB \
    --disable-niwg \
    --disable-geant;

