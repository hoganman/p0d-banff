#!/usr/bin/env python2

from __future__ import print_function

from math import sqrt
import matplotlib.pyplot as plt
import numpy as np
import optparse
import pandas as pd
import sys
from scipy.optimize import curve_fit


def gauss(x, *p):
    """Create a function which returns a Gaussian"""
    norm, mean, sigma = p
    y = norm * np.exp(-0.5 * (x - mean) * (x - mean) / (sigma * sigma))
    return y


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

    # minMomentum = 0.200
    # maxMomentum = 10.0
    minMomentum = 0.200
    maxMomentum = 1.200
    binSpacing = 0.05
    recoMomBins = np.arange(minMomentum, maxMomentum, binSpacing)
    gauss_p0 = [50., 1e-8, 0.1]

    dataset = pd.read_csv(options.file)
    # errors of the dist mean and std
    bootstrapped_mean = np.array([], dtype=np.float)
    bootstrapped_meanstd = np.array([], dtype=np.float)
    bootstrapped_std = np.array([], dtype=np.float)
    bootstrapped_stdstd = np.array([], dtype=np.float)
    fit_mean = np.array([], dtype=np.float)
    fit_meanstd = np.array([], dtype=np.float)
    fit_std = np.array([], dtype=np.float)
    fit_stdstd = np.array([], dtype=np.float)

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

        # histogram the resolution to get a gauss fit
        bin_heights, bin_borders, _ = plt.hist(resol, bins=50, range=(-0.5, 0.5), label='histogram')
        bin_centers = bin_borders[:-1] + np.diff(bin_borders) / 2
        errors = np.array([sqrt(x)+1e-8 for x in bin_heights.tolist()])
        popt, pcov = curve_fit(gauss, bin_centers, bin_heights, p0=gauss_p0, sigma=errors)
        fit_mean = np.append(fit_mean, popt[1])
        fit_std = np.append(fit_std, popt[2])
        fit_meanstd = np.append(fit_meanstd, sqrt(pcov[1, 1]))
        fit_stdstd = np.append(fit_stdstd, sqrt(pcov[2, 2]+1e-8))

        # now examine bootstrapping
        bs_mean = np.array([], dtype=np.float)
        bs_std = np.array([], dtype=np.float)
        # iterate of many times and get a distribution of mean and std
        for bsIter in range(int(1e5)):
            sample_idx = np.random.choice(len(resol), size=len(resol), replace=True)
            array_bootstrapped = resol.values
            array_bootstrapped = array_bootstrapped[sample_idx]
            bs_mean = np.append(bs_mean, np.mean(array_bootstrapped))
            bs_std = np.append(bootstrapped_std, np.std(array_bootstrapped))
        # store the mean, std, std of mean, and std of std
        bootstrapped_mean = np.append(bootstrapped_mean, np.mean(bs_mean))
        bootstrapped_meanstd = np.append(bootstrapped_meanstd, np.std(bs_mean))
        bootstrapped_std = np.append(bootstrapped_std, np.mean(bs_std))
        bootstrapped_stdstd = np.append(bootstrapped_stdstd, np.std(bs_std))

    print('fit_mean', fit_mean)
    print('fit_meanstd', fit_meanstd)
    print('fit_std', fit_std)
    print('fit_stdstd', fit_stdstd)
    print(bootstrapped_mean)
    print(bootstrapped_meanstd)
    print(bootstrapped_std)
    print(bootstrapped_stdstd)
    fig, axes = plt.subplots(1, 2, sharey=True)
    axes[0].errorbar(recoMomBins[:-1], fit_mean, xerr=0.5*binSpacing, yerr=fit_meanstd, ls='none')
    axes[0].errorbar(recoMomBins[:-1], fit_std, xerr=0.5*binSpacing, yerr=fit_stdstd, ls='none')
    axes[1].errorbar(recoMomBins[:-1], bootstrapped_mean, xerr=0.5*binSpacing, yerr=bootstrapped_meanstd, ls='none')
    axes[1].errorbar(recoMomBins[:-1], bootstrapped_std, xerr=0.5*binSpacing, yerr=bootstrapped_stdstd, ls='none')
    # x_interval_for_fit = np.linspace(bin_borders[0], bin_borders[-1], 10000)
    # axes.plot(x_interval_for_fit, gauss(x_interval_for_fit, *popt), label='fit')
    # fig.legend()
    fig.show()
    fig.savefig('test.png')


if __name__ == '__main__':
    main(sys.argv)
