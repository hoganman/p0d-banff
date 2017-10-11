CMTPATH=/physics/home/mhogan/software/t2k-software-clone:/physics/software/t2k/ND280/v11r31
CMT_tag=$(tag)
CMTROOT=/physics/software/CMT/v1r20p20081118
CMT_root=/physics/software/CMT/v1r20p20081118
CMTVERSION=v1r20p20081118
cmt_hardware_query_command=uname -m
cmt_hardware=`$(cmt_hardware_query_command)`
cmt_system_version_query_command=${CMTROOT}/mgr/cmt_linux_version.sh | ${CMTROOT}/mgr/cmt_filter_version.sh
cmt_system_version=`$(cmt_system_version_query_command)`
cmt_compiler_version_query_command=${CMTROOT}/mgr/cmt_gcc_version.sh | ${CMTROOT}/mgr/cmt_filter3_version.sh
cmt_compiler_version=`$(cmt_compiler_version_query_command)`
PATH=/physics/home/mhogan/software/t2k-software-clone/highland2/oaAnalysisReader/v2r10/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ROOT/v5r34p09n04/Linux-x86_64/bin:/physics/software/t2k/ND280/v11r31/GSL/v1r15p0n00/Linux-x86_64/bin:/physics/software/t2k/ND280/v11r31/MYSQL/v5r051an01/Linux-x86_64/bin:/physics/software/t2k/ND280/v11r31/EXTERN/v3r7/scripts:/physics/software/CMT/v1r20p20081118/${CMTBIN}:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandValidation/v2r16/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/tutorialAnalysis/v2r16/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/baseP0DAnalysis/v2r6/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/p0dNumuCCQEAnalysis/v2r12/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/p0dNumuCCAnalysis/v2r14/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/gammaAnalysis/v2r12/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/antiNueCCAnalysis/v2r10/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/nueCCAnalysis/v2r18/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/numuBkgInAntiNuModeAnalysis/v2r6/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/numuCCMultiTargetAnalysis/v2r12/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/antiNumuCCMultiPiAnalysis/v2r2/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/antiNumuCCAnalysis/v2r4/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/numuCCMultiPiAnalysis/v2r8/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/numuCCAnalysis/v2r14/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/baseTrackerAnalysis/v2r12/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/baseAnalysis/v2r20/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandSystematics/v2r4/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandIO/v2r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandCorrections/v2r8/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandUtils/v2r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandTools/v2r18/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandEventModel/v2r20/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandCore/v2r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheSteering/v3r18/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheSystematics/v3r24/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheSelections/v3r28/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheIO/v3r18/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheND280Utils/v3r24/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheUtils/v3r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheEventModel/v3r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheCore/v3r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psychePolicy/v3r2/scripts:/physics/home/mhogan/software/t2k-software/psyche/psycheSteering/v3r18/Linux-x86_64:/physics/software/neut/v1r27p1_5.3.3/bin:/physics/software/cernlib-2005/2005/bin:/physics/home/mhogan/software/t2k-software-clone/T2KReWeight/bin:/physics/home/mhogan/software/t2k-software-clone/BANFF/v3r16/Linux-x86_64:/physics/software/t2k/ND280/v11r31/nd280AnalysisTools/v1r9p3/AnalysisTools/libReadoaAnalysis:/physics/software/t2k/ND280/v11r31/nd280AnalysisTools/v1r9p3/AnalysisTools:/physics/software/t2k/ND280/v11r31/nd280AnalysisTools/v1r9p3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaControlSample/v1r1/scripts:/physics/software/t2k/ND280/v11r31/oaControlSample/v1r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ecalTestBeam/v0r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaAnalysis/v5r29p19/Linux-x86_64:/physics/software/t2k/ND280/v11r31/eventDisplay/v3r63/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaAlignTools/v0r7/Linux-x86_64:/physics/software/t2k/ND280/v11r31/soffTasks/v1r23/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ecalCalib/v2r11/Linux-x86_64:/physics/software/t2k/ND280/v11r31/mppcCalib/v1r17/Linux-x86_64:/physics/software/t2k/ND280/v11r31/tfbCalib/v1r33/Linux-x86_64:/physics/software/t2k/ND280/v11r31/nd280/v11r31/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaRecon/v4r19/Linux-x86_64:/physics/software/t2k/ND280/v11r31/p0decalRecon/v2r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ecalRecon/v10r15p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/trackerRecon/v2r21/Linux-x86_64:/physics/software/t2k/ND280/v11r31/fgdRecon/v5r29p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/tpcRecon/v6r19p7/Linux-x86_64:/physics/software/t2k/ND280/v11r31/p0dRecon/v8r9p15/Linux-x86_64:/physics/software/t2k/ND280/v11r31/sbcat/v5r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/reconUtils/v1r17p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/elecSim/v6r23p3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/fgdCalib/v0r7/app/:/physics/software/t2k/ND280/v11r31/fgdCalib/v0r7/Linux-x86_64:/physics/software/t2k/ND280/v11r31/smrdCalib/v1r27p3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaCalib/v4r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaBeamData/v0r23/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaDataQuality/v1r21/src:/physics/software/t2k/ND280/v11r31/oaDataQuality/v1r21/Linux-x86_64:/physics/software/t2k/ND280/v11r31/tfbApplyCalib/v3r43/Linux-x86_64:/physics/software/t2k/ND280/v11r31/fgdRawData/v2r3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaUnpack/v3r3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaSandSimulation/v0r3p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/atmPitSim/v1r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaCosmicTrigger/v1r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaCherryPicker/v0r11/Linux-x86_64:/physics/software/t2k/ND280/v11r31/nd280mc/v5r3p5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaApplyAlign/v2r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaGeometryLookup/v0r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaGeomInfo/v5r3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaChanInfo/v3r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ingridUtils/v1r3p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/nd280Control/v1r67/app:/physics/software/t2k/ND280/v11r31/oaUtility/v5r3p3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/magnetCalib/v4r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/calibTables/v1r19/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaOfflineDatabase/v2r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaSlowControlDatabase/v1r11/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaRuntimeParameters/v0r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaRawEvent/v3r25/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaEvent/v8r9p11/Linux-x86_64:/physics/software/t2k/ND280/v11r31/testBase/v1r15/scripts:/physics/software/t2k/ND280/v11r31/testBase/v1r15/Linux-x86_64:/physics/software/t2k/ND280/v11r31/nd280Policy/v2r39/scripts:/physics/software/t2k/ND280/v11r31/GEANT/v9r4p04n00/:/physics/software/t2k/ND280/v11r31/CLHEP/v2r1p1n00/Linux-x86_64/bin:/physics/INSTALLATION/bin:/home/other/mhogan/bin:/bin:/usr/bin:/usr/sbin:/sbin:/usr/local/bin:/usr/local/Modules/3.2.10/bin:/usr/local/univa/8.1.5/bin/lx-amd64:/usr/lib64/qt-3.3/bin:/usr/local/openmpi/1.6.4/x86_64/ib/gnu/bin:/home/other/mhogan/.nvm/versions/node/v8.1.4/bin:/home/other/mhogan/software/jdk/jdk1.8.0_144/bin:/usr/local/11.06.011-R8/STAR-View+11.06.011/bin:/usr/local/11.06.011-R8/STAR-CCM+11.06.011-R8/star/bin:/usr/local/sbin:/sbin/./:/physics/software/t2k/ND280/v11r31/oaEvent/v8r9p11/scripts
CLASSPATH=/physics/software/CMT/v1r20p20081118/java
debug_option=-g
cc=gcc
ccomp=$(cc) -c $(includes) $(cdebugflags) $(cflags) $(pp_cflags)
clink=$(cc) $(clinkflags) $(cdebugflags)
ppcmd=-I
preproc=c++ -MD -c 
cpp=c++
cppflags=-pipe -ansi -pedantic -W -Wall -Wwrite-strings -Wpointer-arith -Woverloaded-virtual 
pp_cppflags=-D_GNU_SOURCE
cppcomp=$(cpp) -c $(includes) $(cppdebugflags) $(cppflags) $(pp_cppflags)
cpplink=$(cpp) $(cpplinkflags) $(cppdebugflags)
for=g77
fflags=$(debug_option)
fcomp=$(for) -c $(fincludes) $(fflags) $(pp_fflags)
flink=$(for) $(flinkflags)
javacomp=javac -classpath $(src):$(CLASSPATH) 
javacopy=cp
jar=jar
X11_cflags=-I/usr/include
Xm_cflags=-I/usr/include
X_linkopts=-L/usr/X11R6/lib -lXm -lXt -lXext -lX11 -lm
lex=lex $(lexflags)
yaccflags= -l -d 
yacc=yacc $(yaccflags)
ar=ar cr
ranlib=ranlib
make_shlib=${CMTROOT}/mgr/cmt_make_shlib_common.sh extract
shlibsuffix=so
shlibbuilder=g++ $(cmt_installarea_linkopts) 
shlibflags=-shared
symlink=/bin/ln -fs 
symunlink=/bin/rm -f 
library_install_command=${symlink}
build_library_links=$(cmtexe) build library_links -tag=$(tags)
remove_library_links=$(cmtexe) remove library_links -tag=$(tags)
cmtexe=${CMTROOT}/${CMTBIN}/cmt.exe
build_prototype=$(cmtexe) build prototype
build_dependencies=$(cmtexe) -tag=$(tags) build dependencies
build_triggers=$(cmtexe) build triggers
implied_library_prefix=-l
SHELL=/bin/sh
q="
src=../src/
doc=../doc/
inc=../src/
mgr=../cmt/
application_suffix=.exe
library_prefix=lib
lock_command=lockfile 
unlock_command=rm -rf 
lock_name=cmt
lock_suffix=.lock
lock_file=${lock_name}${lock_suffix}
MAKEFLAGS= --no-print-directory 
gmake_hosts=lx1 rsplus lxtest as7 dxplus ax7 hp2 aleph hp1 hpplus papou1-fe atlas
make_hosts=virgo-control1 rio0a vmpc38a
everywhere=hosts
install_command=cp 
uninstall_command=/bin/rm -f 
cmt_installarea_command=ln -fs 
cmt_uninstallarea_command=/bin/rm -f 
cmt_install_area_command=$(cmt_installarea_command)
cmt_uninstall_area_command=$(cmt_uninstallarea_command)
cmt_install_action=$(CMTROOT)/mgr/cmt_install_action.sh
cmt_installdir_action=$(CMTROOT)/mgr/cmt_installdir_action.sh
cmt_uninstall_action=$(CMTROOT)/mgr/cmt_uninstall_action.sh
cmt_uninstalldir_action=$(CMTROOT)/mgr/cmt_uninstalldir_action.sh
mkdir=mkdir
cmt_cvs_protocol_level=v1r1
cmt_installarea_prefix=InstallArea
CMT_PATH_remove_regexp=/[^/]*/
CMT_PATH_remove_share_regexp=/share/
NEWCMTCONFIG=x86_64-slc68-gcc447
oaAnalysisReader_tag=$(tag)
OAANALYSISREADERROOT=/physics/home/mhogan/software/t2k-software-clone/highland2/oaAnalysisReader/v2r10
oaAnalysisReader_root=/physics/home/mhogan/software/t2k-software-clone/highland2/oaAnalysisReader/v2r10
OAANALYSISREADERVERSION=v2r10
oaAnalysisReader_cmtpath=/physics/home/mhogan/software/t2k-software-clone
oaAnalysisReader_offset=highland2
oaAnalysisReader_project=software
oaAnalysisReader_project_release=t2k-software-clone
ROOT_tag=$(tag)
ROOTROOT=/physics/software/t2k/ND280/v11r31/ROOT/v5r34p09n04
ROOT_root=/physics/software/t2k/ND280/v11r31/ROOT/v5r34p09n04
ROOTVERSION=v5r34p09n04
ROOT_cmtpath=/physics/software/t2k/ND280/v11r31
ROOT_project=ND280
ROOT_project_release=v11r31
EXTERN_tag=$(tag)
EXTERNROOT=/physics/software/t2k/ND280/v11r31/EXTERN/v3r7
EXTERN_root=/physics/software/t2k/ND280/v11r31/EXTERN/v3r7
EXTERNVERSION=v3r7
EXTERN_cmtpath=/physics/software/t2k/ND280/v11r31
EXTERN_project=ND280
EXTERN_project_release=v11r31
MYSQL_tag=$(tag)
MYSQLROOT=/physics/software/t2k/ND280/v11r31/MYSQL/v5r051an01
MYSQL_root=/physics/software/t2k/ND280/v11r31/MYSQL/v5r051an01
MYSQLVERSION=v5r051an01
MYSQL_cmtpath=/physics/software/t2k/ND280/v11r31
MYSQL_project=ND280
MYSQL_project_release=v11r31
LD_LIBRARY_PATH=/physics/home/mhogan/software/t2k-software-clone/highland2/oaAnalysisReader/v2r10/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ROOT/v5r34p09n04/Linux-x86_64/lib/root:/physics/software/t2k/ND280/v11r31/GSL/v1r15p0n00/Linux-x86_64/lib:/physics/software/t2k/ND280/v11r31/MYSQL/v5r051an01/Linux-x86_64/lib/mysql:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandValidation/v2r16/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/tutorialAnalysis/v2r16/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/baseP0DAnalysis/v2r6/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/p0dNumuCCQEAnalysis/v2r12/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/p0dNumuCCAnalysis/v2r14/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/gammaAnalysis/v2r12/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/antiNueCCAnalysis/v2r10/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/nueCCAnalysis/v2r18/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/numuBkgInAntiNuModeAnalysis/v2r6/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/numuCCMultiTargetAnalysis/v2r12/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/antiNumuCCMultiPiAnalysis/v2r2/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/antiNumuCCAnalysis/v2r4/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/numuCCMultiPiAnalysis/v2r8/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/numuCCAnalysis/v2r14/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/baseTrackerAnalysis/v2r12/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/baseAnalysis/v2r20/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandSystematics/v2r4/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandIO/v2r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandCorrections/v2r8/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandUtils/v2r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandTools/v2r18/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandEventModel/v2r20/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/highland2/highlandCore/v2r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheSteering/v3r18/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheSystematics/v3r24/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheSelections/v3r28/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheIO/v3r18/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheND280Utils/v3r24/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheUtils/v3r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheEventModel/v3r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/psyche/psycheCore/v3r22/Linux-x86_64:/physics/home/mhogan/software/t2k-software/psyche/psycheSteering/v3r18/Linux-x86_64:/physics/home/mhogan/software/v11r31/GlobalAnalysisTools/NIWGReWeight:/physics/software/neut/v1r27p1_5.3.3/src/reweight:/physics/software/cernlib-2005/2005/lib:/physics/home/mhogan/software/t2k-software-clone/T2KReWeight/Linux-x86_64:/physics/home/mhogan/software/t2k-software-clone/T2KReWeight/lib:/physics/home/mhogan/software/t2k-software-clone/BANFF/v3r16/Linux-x86_64:/physics/software/t2k/ND280/v11r31/nd280AnalysisTools/v1r9p3/Linux-x86_64/AnalysisTools/libReadoaAnalysis:/physics/software/t2k/ND280/v11r31/oaControlSample/v1r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ecalTestBeam/v0r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaAnalysis/v5r29p19/Linux-x86_64:/physics/software/t2k/ND280/v11r31/eventDisplay/v3r63/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaAlignTools/v0r7/Linux-x86_64:/physics/software/t2k/ND280/v11r31/soffTasks/v1r23/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ecalCalib/v2r11/Linux-x86_64:/physics/software/t2k/ND280/v11r31/mppcCalib/v1r17/Linux-x86_64:/physics/software/t2k/ND280/v11r31/tfbCalib/v1r33/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ingridRecon/v2r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaRecon/v4r19/Linux-x86_64:/physics/software/t2k/ND280/v11r31/p0decalRecon/v2r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ecalRecon/v10r15p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/trackerRecon/v2r21/Linux-x86_64:/physics/software/t2k/ND280/v11r31/smrdRecon/v5r9p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/fgdRecon/v5r29p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/tpcRecon/v6r19p7/Linux-x86_64:/physics/software/t2k/ND280/v11r31/p0dRecon/v8r9p15/Linux-x86_64:/physics/software/t2k/ND280/v11r31/sbcat/v5r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaRecPack/v8r33p5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/RECPACK/v4r11/Linux-x86_64:/physics/software/t2k/ND280/v11r31/reconUtils/v1r17p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/elecSim/v6r23p3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/fgdCalib/v0r7/Linux-x86_64:/physics/software/t2k/ND280/v11r31/smrdCalib/v1r27p3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaCalib/v4r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaBeamData/v0r23/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaDataQuality/v1r21/Linux-x86_64:/physics/software/t2k/ND280/v11r31/tfbApplyCalib/v3r43/Linux-x86_64:/physics/software/t2k/ND280/v11r31/fgdRawData/v2r3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaUnpack/v3r3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaSandSimulation/v0r3p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/atmPitSim/v1r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaCosmicTrigger/v1r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/nd280mc/v5r3p5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaApplyAlign/v2r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaGeometryLookup/v0r1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaGeomInfo/v5r3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaChanInfo/v3r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/ingridUtils/v1r3p1/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaUtility/v5r3p3/Linux-x86_64:/physics/software/t2k/ND280/v11r31/magnetCalib/v4r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/calibTables/v1r19/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaOfflineDatabase/v2r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaSlowControlDatabase/v1r11/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaRuntimeParameters/v0r5/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaRawEvent/v3r25/Linux-x86_64:/physics/software/t2k/ND280/v11r31/oaEvent/v8r9p11/Linux-x86_64:/physics/software/t2k/ND280/v11r31/GEANT/v9r4p04n00/Linux-x86_64/Linux-g++:/physics/software/t2k/ND280/v11r31/GEANT/v9r4p04n00/Linux-x86_64/plists/Linux-g++:/physics/software/t2k/ND280/v11r31/CLHEP/v2r1p1n00/Linux-x86_64/lib:/physics/INSTALLATION/lib:/physics/INSTALLATION/lib64:/usr/lib:/usr/lib64:/usr/local/lib/:/usr/local/lib64:/usr/local2/etc/scripts/linux/X11rdp/lib:/home/other/mhogan/lib:/usr/local/openmpi/1.6.4/x86_64/ib/gnu/lib/./
MYSQL_linkopts=`$(MYSQLROOT)/$(MYSQL_tag)/bin/mysql_config  --libs`
MYSQL_BASE_DIR=/physics/software/t2k/ND280/v11r31/MYSQL/v5r051an01/Linux-x86_64
MYSQL_LIB_DIR=${MYSQL_BASE_DIR}/lib/mysql
MYSQL_INCLUDE_DIR=${MYSQL_BASE_DIR}/include/mysql
MAKFLAGS=--no-print-directory
GSL_tag=$(tag)
GSLROOT=/physics/software/t2k/ND280/v11r31/GSL/v1r15p0n00
GSL_root=/physics/software/t2k/ND280/v11r31/GSL/v1r15p0n00
GSLVERSION=v1r15p0n00
GSL_cmtpath=/physics/software/t2k/ND280/v11r31
GSL_project=ND280
GSL_project_release=v11r31
DYLD_LIBRARY_PATH=/physics/software/t2k/ND280/v11r31/ROOT/v5r34p09n04/Linux-x86_64/lib/root
GSL_cppflags=`$(GSLROOT)/$(GSL_tag)/bin/gsl-config --cflags`
GSL_linkopts=`$(GSLROOT)/$(GSL_tag)/bin/gsl-config --libs`
GSL_BASE_DIR=/physics/software/t2k/ND280/v11r31/GSL/v1r15p0n00/Linux-x86_64
GSL_LIB_DIR=${GSL_BASE_DIR}/lib
GSL_INCLUDE_DIR=${GSL_BASE_DIR}/include
ROOT_PATH=$(ROOTROOT)/$(tag)
ROOT_BIN=$(ROOT_PATH)/bin
ROOT_LIB=$(ROOT_PATH)/lib/root
ROOTSYS=/physics/software/t2k/ND280/v11r31/ROOT/v5r34p09n04/Linux-x86_64
PYTHONPATH=/physics/software/t2k/ND280/v11r31/nd280AnalysisTools/v1r9p3/macros:/physics/software/t2k/ND280/v11r31/soffTasks/v1r23/Linux-x86_64:/physics/software/t2k/ND280/v11r31/reconUtils/v1r17p1/scripts:/physics/software/t2k/ND280/v11r31/reconUtils/v1r17p1/macros:/physics/software/t2k/ND280/v11r31/oaDataQuality/v1r21/src:/physics/software/t2k/ND280/v11r31/nd280Control/v1r67/modules:./:/physics/INSTALLATION/bin:/physics/home/mhogan/software/macros/root:${ROOTSYS}/lib/root
ROOT_linkopts=`$(ROOT_BIN)/root-config --glibs` -lEG -lGeom -lMinuit -lFumili -lXMLIO
ROOT_cppflags=-DROOT_USED `$(ROOT_BIN)/root-config --cflags` -Wno-long-long
ROOT_stamps=$(ROOTROOT)/$(ROOTCONFIG)/ROOT.stamp
dict=../dict/
rootcint=$(ROOT_BIN)/rootcint
PRODUCTIONDIR=prod6B
oaAnalysisReader_ccppflags=-DOAANALYSISREADER_USED -fPIC
oaAnalysisReader_linkopts=-L$(OAANALYSISREADERROOT)/$(oaAnalysisReader_tag) -loaAnalysisReader
oaAnalysisReader_stamps= $(OAANALYSISREADERROOT)/$(oaAnalysisReader_tag)/oaAnalysisReader.stamp
includes=-I$(OAANALYSISREADERROOT)/src/$(PRODUCTIONDIR) $(ppcmd)"$(srcdir)" $(use_includes)
tag=Linux-x86_64
package=oaAnalysisReader
version=v2r10
PACKAGE_ROOT=$(OAANALYSISREADERROOT)
srcdir=../src
bin=../$(oaAnalysisReader_tag)/
BIN=/physics/home/mhogan/software/t2k-software-clone/highland2/oaAnalysisReader/v2r10/$(oaAnalysisReader_tag)/
javabin=../classes/
mgrdir=cmt
project=software
cmt_installarea_paths= $(cmt_installarea_prefix)/$(CMTCONFIG)/bin
use_linkopts= $(cmt_installarea_linkopts)   $(oaAnalysisReader_linkopts)  $(ROOT_linkopts)  $(MYSQL_linkopts)  $(GSL_linkopts)  $(EXTERN_linkopts) 
ND280_installarea_prefix=$(cmt_installarea_prefix)
ND280_installarea_prefix_remove=$(ND280_installarea_prefix)
software_installarea_prefix=$(cmt_installarea_prefix)
software_installarea_prefix_remove=$(software_installarea_prefix)
use_requirements=requirements $(CMTROOT)/mgr/requirements $(ROOTROOT)/cmt/requirements $(MYSQLROOT)/cmt/requirements $(GSLROOT)/cmt/requirements $(EXTERNROOT)/cmt/requirements 
use_includes= $(ppcmd)"/physics/software/t2k/ND280/v11r31/ROOT/v5r34p09n04/Linux-x86_64/include/root" $(ppcmd)"none" $(ppcmd)"/physics/software/t2k/ND280/v11r31/MYSQL/v5r051an01/Linux-x86_64/include/mysql" $(ppcmd)"/physics/software/t2k/ND280/v11r31/GSL/v1r15p0n00/Linux-x86_64/include" $(ppcmd)"$(EXTERN_root)/src" 
use_fincludes= $(use_includes)
use_stamps= $(oaAnalysisReader_stamps)  $(ROOT_stamps)  $(MYSQL_stamps)  $(GSL_stamps)  $(EXTERN_stamps) 
use_cflags=  $(oaAnalysisReader_cflags)  $(ROOT_cflags)  $(MYSQL_cflags)  $(GSL_cflags)  $(EXTERN_cflags) 
use_pp_cflags=  $(oaAnalysisReader_pp_cflags)  $(ROOT_pp_cflags)  $(MYSQL_pp_cflags)  $(GSL_pp_cflags)  $(EXTERN_pp_cflags) 
use_cppflags=  $(oaAnalysisReader_cppflags)  $(ROOT_cppflags)  $(MYSQL_cppflags)  $(GSL_cppflags)  $(EXTERN_cppflags) 
use_pp_cppflags=  $(oaAnalysisReader_pp_cppflags)  $(ROOT_pp_cppflags)  $(MYSQL_pp_cppflags)  $(GSL_pp_cppflags)  $(EXTERN_pp_cppflags) 
use_fflags=  $(oaAnalysisReader_fflags)  $(ROOT_fflags)  $(MYSQL_fflags)  $(GSL_fflags)  $(EXTERN_fflags) 
use_pp_fflags=  $(oaAnalysisReader_pp_fflags)  $(ROOT_pp_fflags)  $(MYSQL_pp_fflags)  $(GSL_pp_fflags)  $(EXTERN_pp_fflags) 
use_libraries= $(ROOT_libraries)  $(MYSQL_libraries)  $(GSL_libraries)  $(EXTERN_libraries) 
fincludes= $(includes)
constituents= oaAnalysisReader 
all_constituents= $(constituents)
constituentsclean= oaAnalysisReaderclean 
all_constituentsclean= $(constituentsclean)
cmt_actions_constituents= make 
cmt_actions_constituentsclean= makeclean 
