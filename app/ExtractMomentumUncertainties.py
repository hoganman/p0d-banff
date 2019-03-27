#!/usr/bin/env python2

from __future__ import print_function

# from math import sqrt
import matplotlib.pyplot as plt
import numpy as np
import optparse
import pandas as pd
import sys


def main(argv):
    parser = optparse.OptionParser()
    parser.add_option('-f', '--file', dest='file',
                      default='',
                      help='input file name')
    options, args = parser.parse_args()
    if len(options.file) < 1:
        print('ERROR: Missing input')
        parser.print_help()
        return 0

    maxBootstraps = int(1e4)
    binSpacing = 0.05
    minMomentum = 0.2
    maxMomentum = 1.25
    recoMomBins = np.arange(minMomentum, maxMomentum, binSpacing)
    # recoMomBins = np.insert(recoMomBins, 0, 0., axis=0)

    dataset = pd.read_csv(options.file)
    dataset = dataset[dataset.recoP < 30]
    dataset = dataset[dataset.recoP > 0]
    dataset = dataset[dataset.recoP/dataset.trueP < 3]

    # errors of the dist mean and std
    mean = np.array([], dtype=np.float)
    std = np.array([], dtype=np.float)
    bootstrapped_mean = np.array([], dtype=np.float)
    bootstrapped_meanstd = np.array([], dtype=np.float)
    bootstrapped_std = np.array([], dtype=np.float)
    bootstrapped_stdstd = np.array([], dtype=np.float)

    for index in range(len(recoMomBins)-1):
        # get the reco low and high range
        lowMomentum = recoMomBins[index]
        highMomentum = recoMomBins[index+1]
        print('current range (%5.3f,%5.3f)' % (lowMomentum, highMomentum))

        # get the series that correspond to the range
        MomentumInRange = dataset[lowMomentum < dataset.recoP]
        MomentumInRange = MomentumInRange[MomentumInRange.recoP <= highMomentum]
        trueMomentumInRange = MomentumInRange.trueP
        recoMomentumInRange = MomentumInRange.recoP
        resol = (recoMomentumInRange - trueMomentumInRange) / trueMomentumInRange

        mean = np.append(mean, np.mean(resol))
        std = np.append(std, np.std(resol))

        # now examine bootstrapping
        bs_mean = np.array([], dtype=np.float)
        bs_std = np.array([], dtype=np.float)
        # iterate of many times and get a distribution of mean and std
        for bsIter in range(maxBootstraps):
            sample_idx = np.random.choice(len(resol), size=len(resol), replace=True)
            array_bootstrapped = resol.values
            array_bootstrapped = array_bootstrapped[sample_idx]
            bs_mean = np.append(bs_mean, np.mean(array_bootstrapped))
            bs_std = np.append(bs_std, np.std(array_bootstrapped))

        # fig, ax = plt.subplots()
        # ax.hist(bs_std, bins=50, range=(0.9*min(bs_std), max(bs_std)*1.1))
        # fig.show()
        # fig.savefig('bs_std_%5.3f_%5.3f.png' % (lowMomentum, highMomentum))
        if len(bootstrapped_stdstd) > 0 and np.std(bs_std) > 10 * bootstrapped_stdstd[-1]:
            print('previous stdstd', bootstrapped_stdstd[-1])
            print('current stdstd', np.std(array_bootstrapped))
            print(len(bs_std))
            for bsIter in range(10*maxBootstraps):
                sample_idx = np.random.choice(len(resol), size=len(resol), replace=True)
                array_bootstrapped = resol.values
                array_bootstrapped = array_bootstrapped[sample_idx]
                bs_mean = np.append(bs_mean, np.mean(array_bootstrapped))
                bs_std = np.append(bs_std, np.std(array_bootstrapped))
        # store the mean, std, std of mean, and std of std
        bootstrapped_mean = np.append(bootstrapped_mean, np.mean(bs_mean))
        bootstrapped_meanstd = np.append(bootstrapped_meanstd, np.std(bs_mean))
        bootstrapped_std = np.append(bootstrapped_std, np.mean(bs_std))
        bootstrapped_stdstd = np.append(bootstrapped_stdstd, np.std(bs_std))

    print('bootstrapped_mean', bootstrapped_mean)
    print('bootstrapped_meanstd', bootstrapped_meanstd)
    print('bootstrapped_std', bootstrapped_std)
    print('bootstrapped_stdstd', bootstrapped_stdstd)
    fig0, axes0 = plt.subplots()
    fig1, axes1 = plt.subplots()
    fig2, axes2 = plt.subplots()
    fig3, axes3 = plt.subplots()
    recoMomBins = recoMomBins[:-1]
    recoMomBins = recoMomBins + 0.5 * binSpacing
    linear = np.abs(bootstrapped_mean) + 2. * bootstrapped_std
    print('total uncertainty', linear*recoMomBins*1000)
    # fracError_mean = np.abs(bootstrapped_meanstd/(1-bootstrapped_mean))
    # fracError_std = np.abs(bootstrapped_stdstd/bootstrapped_std)
    # fracError_total = np.sqrt(fracError_mean*fracError_mean + fracError_std*fracError_std)
    axes0.errorbar(recoMomBins, bootstrapped_mean*recoMomBins*1000, xerr=0.5*binSpacing, yerr=bootstrapped_meanstd*recoMomBins*1000, ls='none', label='Bias')
    axes0.errorbar(recoMomBins, bootstrapped_std*recoMomBins*1000, xerr=0.5*binSpacing, yerr=bootstrapped_stdstd*recoMomBins*1000, ls='none', label='Resolution')
    # axes0.errorbar(recoMomBins, linear*recoMomBins*1000, xerr=0.5*binSpacing, yerr=np.sqrt(4.*bootstrapped_stdstd*bootstrapped_stdstd+bootstrapped_meanstd*bootstrapped_meanstd)*recoMomBins*1000, ls='none', label='|B| + 2R')
    axes0.set_xlabel('Reco Momentum [GeV/c]')
    axes0.set_ylabel('Momentum Uncertainty [MeV/c]')
    axes0.legend()
    axes1.errorbar(recoMomBins, bootstrapped_mean, xerr=0.5*binSpacing, yerr=bootstrapped_meanstd, ls='none', label='Mean')
    axes1.errorbar(recoMomBins, bootstrapped_std, xerr=0.5*binSpacing, yerr=bootstrapped_stdstd, ls='none', label='stddev')
    # axes1.errorbar(recoMomBins, linear, xerr=0.5*binSpacing, yerr=np.sqrt(4.*bootstrapped_stdstd*bootstrapped_stdstd+bootstrapped_meanstd*bootstrapped_meanstd), ls='none', label='|B| + 2R')
    axes1.set_xlabel('Reco Momentum [GeV]')
    axes1.set_title('Bootstrap')
    axes1.set_ylabel('Resolution')
    axes1.legend()
    axes2.errorbar(recoMomBins, mean, xerr=0.5*binSpacing, ls='none', label='Mean')
    axes2.errorbar(recoMomBins, std, xerr=0.5*binSpacing, ls='none', label='stddev')
    axes2.set_xlabel('Reco Momentum [GeV]')
    axes2.set_title('MC')
    axes2.set_ylabel('Resolution')
    axes2.legend()

    # x_interval_for_fit = np.linspace(bin_borders[0], bin_borders[-1], 10000)
    # axes.plot(x_interval_for_fit, gauss(x_interval_for_fit, *popt), label='fit')
    # fig.legend()
    fig0.show()
    fig0.savefig('Uncertainty.png')
    fig1.show()
    fig1.savefig('Bootstrap.png')
    fig2.show()
    fig2.savefig('MC.png')


if __name__ == '__main__':
    main(sys.argv)
