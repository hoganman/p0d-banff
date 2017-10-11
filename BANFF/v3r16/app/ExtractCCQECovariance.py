#!/usr/bin/python
from ROOT import TMatrixDSym, TFile, TObjArray, TObjString,TVectorD
import sys

#Format is:
#1st argument: input file.
#2nd argument: output file.

#Content of output file is:
#covariance matrix (ccqe_cov)
#TObjArray with parameter names.

inputFile = TFile(sys.argv[1])

fullCov = inputFile.Get("FittedCovariance0")
fullParams = inputFile.Get("FittedParameters0")
paramNames = inputFile.Get("parameterNames")

#The CCQE covariance runs from parameter 106 to 112
#Only want MAQE, pF_C, and MEC_C, 106, 107, 108.
outputFile = TFile(sys.argv[2],"RECREATE")
ccqeCov = TMatrixDSym(3)
ccqeParams = TVectorD(3)
outParamNames = TObjArray(3)

for i in xrange(106,109):
    outParamNames.AddAt(paramNames.At(i),i-106)
    ccqeParams[i-106] = 1.0 + fullParams[i]

    for j in xrange(106, 109):

        ccqeCov[i-106][j-106] = fullCov[i][j]


#With everything filled, print it to the output file.
outputFile.cd()
ccqeCov.Write("ccqe_cov")
ccqeParams.Write("ccqe_params")
outParamNames.Write("param_names",outParamNames.kSingleKey)
