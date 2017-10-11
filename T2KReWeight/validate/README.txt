Validation code for T2KReWeight.

CheckWeights.exe tests NEUT, NIWG, GEANT and GENIE reweight by varying each systematic parameter independently and calculating the weights for an input file. For each parameter, weights are evaluated at -3,-1,+1,+3 sigma from its nominal value.
  To compile, just do "make" after setting up the environment variables necessary to run T2K reweight.
  Usage: CheckWeights.exe -i [input file name] -o [output weight file name]
Input file can be SK(NEUT) or oaAnalysis(NEUT/GENIE) files. Put an option "--noSI" in case you don't want to run GEANTReWeight on SK files.

ROOT script WeightCheck.cc analyzes the output weight files from above and counts the number of events with extreme weight values for each parameter variation, and outputs the result as 1d histograms. Each bin of the histograms corresponds one systematic parameter variation; see command line output for which parameter variation corresponds to which bin. When two input files are specified, consistency of weights between the two is also checked(for consistency check of different T2KReWeight versions).
