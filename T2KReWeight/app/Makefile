# Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>

SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(T2KREWEIGHT)/make/Make.include


#all: genWeightsFromNRooTracker_BANFF_2016 genWeightsFromNRooTracker_ND280_NIWG_Validation_PDD.exe genWeightsSKsplines_NIWG.exe
all: genWeightsFromNRooTracker_BANFF_2017 generate_ND280_Systematics_Covariance_2017BinCorr
# Please only add new apps to be compiled by make all if you have checked 
# they still compile with following config:
# ./configure --disable-jnubeam --disable-neut --disable-genie --disable-oaanalysis;
# throwParmsFromJnuBeam 

genWeightsFromNRooTracker_ND280_NIWG_Validation_PDD.exe: genWeightsFromNRooTracker_ND280_NIWG_Validation_PDD.cxx
	@echo -e "\n\n***** Compiling genWeightsFromNRooTracker_ND280_NIWG_Validation_PDD.exe\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromNRooTracker_ND280_NIWG_Validation_PDD.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromNRooTracker_ND280_NIWG_Validation_PDD.o $(LIBRARIES) -o genWeightsFromNRooTracker_ND280_NIWG_Validation_PDD.exe  

genWeights_2015: FORCE
	@echo -e "\n\n***** Compiling genWeights_2015\n"
	$(CXX) $(CXXFLAGS) -c genWeights_2015.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeights_2015.o $(LIBRARIES) -o genWeights_2015.exe

genWeights_2013a: FORCE
	@echo -e "\n\n***** Compiling genWeights_2013a\n"
	$(CXX) $(CXXFLAGS) -c genWeights_2013a.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeights_2013a.o $(LIBRARIES) -o genWeights_2013a.exe

genWeightsFromNRooTracker_BANFFv2_2013: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromNRooTracker_BANFFv2_2013a\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromNRooTracker_BANFFv2_2013.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromNRooTracker_BANFFv2_2013.o $(LIBRARIES) -o genWeightsFromNRooTracker_BANFFv2_2013.exe


genWeights_2012a: FORCE
	@echo -e "\n\n***** Compiling genWeights_2012a\n"
	$(CXX) $(CXXFLAGS) -c genWeights_2012a.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeights_2012a.o $(LIBRARIES) -o genWeights_2012a.exe

genWeightsFromSK_NIWGexample: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromSK_NIWGexample\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromSK_NIWGexample.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromSK_NIWGexample.o $(LIBRARIES) -o genWeightsFromSK_NIWGexample.exe

genWeightsFromNRooTracker_NEUTexample: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromNRooTracker_NEUTexample\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromNRooTracker_NEUTexample.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromNRooTracker_NEUTexample.o $(LIBRARIES) -o genWeightsFromNRooTracker_NEUTexample.exe

genWeightsFromGHEPTree: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromGHEPTree\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromGHEPTree.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromGHEPTree.o $(LIBRARIES) -o genWeightsFromGHEPTree.exe

genWeightsFromGRooTracker_FluxTuningexample: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromGRooTracker_FluxTuningexample\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromGRooTracker_FluxTuningexample.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromGRooTracker_FluxTuningexample.o $(LIBRARIES) -o genWeightsFromGRooTracker_FluxTuningexample.exe

throwParmsFromJnuBeam: FORCE
	@echo -e "\n\n***** Compiling throwParmsFromJnuBeam\n"
	$(CXX) $(CXXFLAGS) -c throwParmsFromJnuBeam.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) throwParmsFromJnuBeam.o $(LIBRARIES) -o throwParmsFromJnuBeam.exe

genWeightsFromNRooTracker_BANFFv2_2014: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromNRooTracker_BANFFv2_2014a\n"
	$(CXX) $(CXXFLAGS) -ggdb -c genWeightsFromNRooTracker_BANFFv2_2014.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromNRooTracker_BANFFv2_2014.o $(LIBRARIES) -o genWeightsFromNRooTracker_BANFFv2_2014.exe  

genWeightsFromNRooTracker_BANFF_2015: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromNRooTracker_BANFF_2015\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromNRooTracker_BANFF_2015.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromNRooTracker_BANFF_2015.o $(LIBRARIES) -o genWeightsFromNRooTracker_BANFF_2015.exe  

genWeightsFromNRooTracker_BANFF_2016: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromNRooTracker_BANFF_2016\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromNRooTracker_BANFF_2016.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromNRooTracker_BANFF_2016.o $(LIBRARIES) -o genWeightsFromNRooTracker_BANFF_2016.exe  

generate_ND280_Systematics_Covariance_2017BinCorr: FORCE
	@echo -e "\n\n***** generate_ND280_Systematics_Covariance_2017BinCorr\n"
	$(CXX) $(CXXFLAGS) -c generate_ND280_Systematics_Covariance_2017BinCorr.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) generate_ND280_Systematics_Covariance_2017BinCorr.o $(LIBRARIES) -o generate_ND280_Systematics_Covariance_2017BinCorr.exe  

genWeightsFromNRooTracker_BANFF_2017: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromNRooTracker_BANFF_2017\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromNRooTracker_BANFF_2017.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromNRooTracker_BANFF_2017.o $(LIBRARIES) -o genWeightsFromNRooTracker_BANFF_2017.exe

genWeightsFromNRooTracker_BANFF_2016v2: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromNRooTracker_BANFF_2016v2\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromNRooTracker_BANFF_2016v2.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromNRooTracker_BANFF_2016v2.o $(LIBRARIES) -o genWeightsFromNRooTracker_BANFF_2016v2.exe  

genWeightsFromNRooTracker_ND280_NIWG_Validation: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromNRooTracker_ND280_NIWG_Validation\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromNRooTracker_ND280_NIWG_Validation.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromNRooTracker_ND280_NIWG_Validation.o $(LIBRARIES) -o genWeightsFromNRooTracker_ND280_NIWG_Validation.exe  

genWeightsFromSK_NIWG2015: FORCE
	@echo -e "\n\n***** Compiling genWeightsFromSK_NIWG2015\n"
	$(CXX) $(CXXFLAGS) -c genWeightsFromSK_NIWG2015.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsFromSK_NIWG2015.o $(LIBRARIES) -o genWeightsFromSK_NIWG2015.exe  

genWeightsSKsplines_NIWG.exe: FORCE
	@echo -e "\n\n***** Compiling genWeightsSKsplines_NIWG2015\n"
	$(CXX) $(CXXFLAGS) -c genWeightsSKsplines_NIWG2015.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsSKsplines_NIWG2015.o $(LIBRARIES) -o genWeightsSKsplines_NIWG2015.exe  

genWeightsSKsplines_NIWG2015: FORCE
	@echo -e "\n\n***** Compiling genWeightsSKsplines_NIWG2015\n"
	$(CXX) $(CXXFLAGS) -c genWeightsSKsplines_NIWG2015.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeightsSKsplines_NIWG2015.o $(LIBRARIES) -o genWeightsSKsplines_NIWG2015.exe  

genWeights_SK_SFRFG_RPA: FORCE
	@echo -e "\n\n***** Compiling genWeights_SK_SFRFG_RPA\n"
	$(CXX) $(CXXFLAGS) -c genWeights_SK_SFRFG_RPA.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) genWeights_SK_SFRFG_RPA.o $(LIBRARIES) -o genWeights_SK_SFRFG_RPA.exe  


purge: FORCE
	$(RM) *.o *~ core 

clean: FORCE
	$(RM) *.o *~ core
	$(RM) *.exe

FORCE:

# DO NOT DELETE
