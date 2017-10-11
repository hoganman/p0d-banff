
SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(T2KREWEIGHT)/make/Make.include
LIBRARIES += "MoveStats.o"

all: plep_Tuning_Validation_ND280_NIWG.exe q0q3_Tuning_Validation_ND280_NIWG.exe ediff_Tuning_Validation_ND280_NIWG.exe Tuning_Validation_ND280_NIWG.exe weights_Tuning_Validation_ND280_NIWG.exe

MoveStats.o : move_stats.C move_stats.h
	$(CXX) -c $(CXXFLAGS) $< $(INCLUDES) -o $@

#CheckWeights: FORCE
#	@echo -e "\n\n***** Compiling CheckWeights\n"
#	$(CXX) $(CXXFLAGS) -c CheckWeights.cxx $(INCLUDES)
#	$(LD) $(LDFLAGS) CheckWeights.o $(LIBRARIES) -o CheckWeights.exe

Tuning_Validation_ND280_NIWG.exe: Tuning_Validation_ND280_NIWG.cxx MoveStats.o
	@echo -e "\n\n***** Compiling Tuning_Validation_ND280_NIWG\n"
	$(CXX) $(CXXFLAGS) -c Tuning_Validation_ND280_NIWG.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) Tuning_Validation_ND280_NIWG.o $(LIBRARIES) -o Tuning_Validation_ND280_NIWG.exe

plep_Tuning_Validation_ND280_NIWG.exe: plep_Tuning_Validation_ND280_NIWG.cxx MoveStats.o
	@echo -e "\n\n***** Compiling plep_Tuning_Validation_ND280_NIWG\n"
	$(CXX) $(CXXFLAGS) -c plep_Tuning_Validation_ND280_NIWG.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) plep_Tuning_Validation_ND280_NIWG.o $(LIBRARIES) -o plep_Tuning_Validation_ND280_NIWG.exe

q0q3_Tuning_Validation_ND280_NIWG.exe: q0q3_Tuning_Validation_ND280_NIWG.cxx MoveStats.o
	@echo -e "\n\n***** Compiling q0q3_Tuning_Validation_ND280_NIWG\n"
	$(CXX) $(CXXFLAGS) -c q0q3_Tuning_Validation_ND280_NIWG.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) q0q3_Tuning_Validation_ND280_NIWG.o $(LIBRARIES) -o q0q3_Tuning_Validation_ND280_NIWG.exe

ediff_Tuning_Validation_ND280_NIWG.exe: ediff_Tuning_Validation_ND280_NIWG.cxx MoveStats.o
	@echo -e "\n\n***** Compiling ediff_Tuning_Validation_ND280_NIWG\n"
	$(CXX) $(CXXFLAGS) -c ediff_Tuning_Validation_ND280_NIWG.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) ediff_Tuning_Validation_ND280_NIWG.o $(LIBRARIES) -o ediff_Tuning_Validation_ND280_NIWG.exe

weights_Tuning_Validation_ND280_NIWG.exe: weights_Tuning_Validation_ND280_NIWG.cxx MoveStats.o
	@echo -e "\n\n***** Compiling weights_Tuning_Validation_ND280_NIWG\n"
	$(CXX) $(CXXFLAGS) -c weights_Tuning_Validation_ND280_NIWG.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) weights_Tuning_Validation_ND280_NIWG.o $(LIBRARIES) -o weights_Tuning_Validation_ND280_NIWG.exe

purge: FORCE
	$(RM) *.o *~ core 

clean: FORCE
	$(RM) *.o *~ core
	$(RM) *.exe

FORCE:

# DO NOT DELETE
