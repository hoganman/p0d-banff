#!/usr/bin/env python

import matplotlib.pyplot as plt
import math
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


def resolution_error(reco):
    """get the resolution (reco - true)/true error"""
    if reco < 0.4:
        resol_error = 0.2
    else:
        resol_error = 1.0417e-2*reco + 0.1
    return resol_error


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
    recoMomBins = np.arange(0.600, 0.700, 0.05)
    gauss_p0 = [50., 1e-8, 0.1]

    dataset = pd.read_csv(options.file)
    bootstrapped_mean = np.array([], dtype=np.float)
    bootstrapped_std = np.array([], dtype=np.float)
    # fit_mean, fit_std, dist_mean, dist_std

    for index in range(len(recoMomBins)-1):
        # get the reco low and high range
        lowMomentum = recoMomBins[index]
        highMomentum = recoMomBins[index+1]
        # get the series that correspond to the range
        MomentumInRange = dataset[lowMomentum < dataset.recoP]
        MomentumInRange = MomentumInRange[MomentumInRange.recoP <= highMomentum]
        trueMomentumInRange = MomentumInRange.trueP
        recoMomentumInRange = MomentumInRange.recoP
        # histogram the resolution to get a gauss fit
        resol = (recoMomentumInRange - trueMomentumInRange) / trueMomentumInRange
        bin_heights, bin_borders, _ = plt.hist(resol, bins=50, range=(-0.5, 0.5), label='histogram')
        bin_centers = bin_borders[:-1] + np.diff(bin_borders) / 2
        e = np.array([math.sqrt(x)+1e-8 for x in bin_heights.tolist()])
        popt, pcov = curve_fit(gauss, bin_centers, bin_heights, p0=gauss_p0, sigma=e)
        x_interval_for_fit = np.linspace(bin_borders[0], bin_borders[-1], 10000)
        # for bsIter in range(int(1e4)):
        #     sample_idx = np.random.choice(len(resol), size=len(resol), replace=True)
        #     array_bootstrapped = resol.values
        #     array_bootstrapped = array_bootstrapped[sample_idx]
        #     bootstrapped_std = np.append(bootstrapped_std, np.array(np.mean(array_bootstrapped)))
        #     bootstrapped_mean = np.append(bootstrapped_mean, np.array(np.std(array_bootstrapped)))
        fig, axes = plt.subplots(1, 1)
        axes.hist(resol, bins=50, label='hist', range=(-0.5, 0.5))
        axes.plot(x_interval_for_fit, gauss(x_interval_for_fit, *popt), label='fit')
        fig.legend()
        fig.show()
        fig.savefig('test.png')


if __name__ == '__main__':
    main(sys.argv)
