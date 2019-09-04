#!/bin/env python2

import argparse
import p0dbanff
import ROOT
from math import log
from TextFile import ReadTextFile, AppendTextFile


def main():
    parser = argparse.ArgumentParser(description='Task of analyzing training\
and testing cross validation in BANFF')
    parser.add_argument('-s', '--set',
                        required=True,
                        type=int,
                        help='Cross validation set index',
                        dest='set')
    parser.add_argument('-r', '--regularization',
                        required=True,
                        type=float,
                        help='Tuning parameter in L2 regularization',
                        dest='regularization')
    parser.add_argument('-t', '--training',
                        required=True,
                        help='Training set postfit file',
                        dest='training', default='')
    parser.add_argument('-T', '--testing',
                        required=True,
                        help='Testing set/apply model file',
                        dest='testing')
    parser.add_argument('-o', '--output',
                        required=True,
                        help='Output file to store results',
                        dest='output')
    args = parser.parse_args()
    FitResults(args)


class FitResults(p0dbanff.p0dbanff):
    """A class to read/write the results of training and testing"""

    def __init__(self, args):
        super(FitResults, self).__init__()
        header = '#set,regularization,trainingChi2,testingChi2'
        self.outputFileName = args.output
        testOutputFile = ReadTextFile(self.outputFileName)
        self.outputFile = None
        if not testOutputFile.exists():
            self.outputFile = AppendTextFile(self.outputFileName)
            self.outputFile.write(header+'\n')
        else:
            lines = testOutputFile.get_file_as_list()
            del testOutputFile
            for aLine in lines:
                if aLine[0] == str(args.set):
                    print 'Training and testing set %d in %s already saved!\n' % (args.set, self.outputFileName)
                    return
            self.outputFile = AppendTextFile(self.outputFileName)
        self.nSamples = 0
        self.sampleNames = []
        self.UpdateSampleNames(args.testing)
        self.trainingChi2 = self.CalculateTrainingChi2(args.training)
        self.testingChi2 = self.CalculateTestingChi2(args.testing)
        results = '%d,%f,%f,%f' % (args.set, args.regularization, self.trainingChi2, self.testingChi2)
        print header
        print results
        self.outputFile.write(results+'\n')
        self.outputFile.close()

    def UpdateSampleNames(self, testingFileName):
        rootFile = ROOT.TFile(testingFileName)
        sampleNamesArray = rootFile.Get("sampleNames")
        for index in range(sampleNamesArray.GetSize()):
            if sampleNamesArray.At(index):
                self.sampleNames.append(sampleNamesArray.At(index).GetName())
                self.nSamples += 1
        rootFile.Close()

    def CalculateTrainingChi2(self, trainingFileName):
        """"""
        return self.CalculateChi2(trainingFileName, '_postfit_0_0')

    def CalculateTestingChi2(self, testingFileName):
        """"""
        return self.CalculateChi2(testingFileName, '_predMC')

    def CalculateChi2(self, inputFileName, predMC_postfix):
        """"""
        rootFile = ROOT.TFile(inputFileName)
        chi2 = 0
        for index in range(self.nSamples):
            sampleName = self.sampleNames[index]
            predMC_name = sampleName + predMC_postfix
            data_name = sampleName + "_data"
            data = rootFile.Get(data_name)
            predMC = rootFile.Get(predMC_name)
            predMC_2D = predMC.Projection(1, 0)
            data_2D = data.Projection(1, 0)
            for binX in range(data_2D.GetNbinsX()+2):
                for binY in range(data_2D.GetNbinsY()+2):
                    binNum = data_2D.GetBin(binX, binY)
                    predMC_content = predMC_2D.GetBinContent(binNum)
                    data_content = data_2D.GetBinContent(binNum)
                    chi2 += predMC_content - data_content
                    if predMC_content > 0 and data_content > 0:
                        chi2 += data_content * log(data_content / predMC_content)
            predMC_2D.Delete()
            data_2D.Delete()
        chi2 *= 2.
        return chi2


if __name__ == '__main__':
    main()
