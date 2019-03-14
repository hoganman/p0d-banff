#!/usr/bin/env python2

from __future__ import print_function

import math
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

    degtorad = math.pi/180
    maxBootstraps = int(1e4)
    binSpacing = 5  # degrees
    minAngle = 0
    maxAngle = 50
    recoAngBins = np.arange(minAngle, maxAngle, binSpacing)

    dataset = pd.read_csv(options.file)
    dataset = dataset[dataset.recoTheta < 90]
    dataset = dataset[dataset.recoTheta > 0]
    # dataset = dataset[dataset.recoTheta/dataset.trueTheta < 3]

    # errors of the dist mean and std
    mean = np.array([], dtype=np.float)
    std = np.array([], dtype=np.float)
    bootstrapped_mean = np.array([], dtype=np.float)
    bootstrapped_meanstd = np.array([], dtype=np.float)
    bootstrapped_std = np.array([], dtype=np.float)
    bootstrapped_stdstd = np.array([], dtype=np.float)

    for index in range(len(recoAngBins)):
        # get the reco low and high range
        lowAngle = recoAngBins[index]
        highAngle = lowAngle + binSpacing
        print('current range (%5.3f,%5.3f)' % (lowAngle, highAngle))

        # get the series that correspond to the range
        AngleInRange = dataset[lowAngle <= dataset.recoTheta]
        AngleInRange = AngleInRange[AngleInRange.recoTheta < highAngle]
        trueAngleInRange = AngleInRange.trueTheta

        mean = np.append(mean, np.mean(trueAngleInRange))
        std = np.append(std, np.std(trueAngleInRange))

        # now examine bootstrapping
        bs_mean = np.array([], dtype=np.float)
        bs_std = np.array([], dtype=np.float)
        # iterate of many times and get a distribution of mean and std
        for bsIter in range(maxBootstraps):
            sample_idx = np.random.choice(len(trueAngleInRange), size=len(trueAngleInRange), replace=True)
            array_bootstrapped = trueAngleInRange.values
            array_bootstrapped = array_bootstrapped[sample_idx]
            bs_mean = np.append(bs_mean, np.mean(array_bootstrapped))
            bs_std = np.append(bs_std, np.std(array_bootstrapped))

        # fig, ax = plt.subplots()
        # ax.hist(bs_std, bins=50, range=(0.9*min(bs_std), max(bs_std)*1.1))
        # fig.show()
        # fig.savefig('bs_std_%5.3f_%5.3f.png' % (lowAngle, highAngle))
        if len(bootstrapped_stdstd) > 0 and np.std(bs_std) > 10 * bootstrapped_stdstd[-1]:
            print('previous stdstd', bootstrapped_stdstd[-1])
            print('current stdstd', np.std(array_bootstrapped))
            print(len(bs_std))
            for bsIter in range(10*maxBootstraps):
                sample_idx = np.random.choice(len(trueAngleInRange), size=len(trueAngleInRange), replace=True)
                array_bootstrapped = trueAngleInRange.values
                array_bootstrapped = array_bootstrapped[sample_idx]
                bs_mean = np.append(bs_mean, np.mean(array_bootstrapped))
                bs_std = np.append(bs_std, np.std(array_bootstrapped))
        # store the mean, std, std of mean, and std of std
        bootstrapped_mean = np.append(bootstrapped_mean, np.mean(bs_mean))
        bootstrapped_meanstd = np.append(bootstrapped_meanstd, np.std(bs_mean))
        bootstrapped_std = np.append(bootstrapped_std, np.mean(bs_std))
        bootstrapped_stdstd = np.append(bootstrapped_stdstd, np.std(bs_std))

    recoAngBins = np.flip(recoAngBins)
    bootstrapped_mean = np.flip(bootstrapped_mean)
    bootstrapped_meanstd = np.flip(bootstrapped_meanstd)
    bootstrapped_std = np.flip(bootstrapped_std)
    bootstrapped_stdstd = np.flip(bootstrapped_stdstd)
    mean = np.flip(mean)
    std = np.flip(std)

    print('recoAngBins', recoAngBins)
    print('bootstrapped_mean', bootstrapped_mean)
    print('bootstrapped_meanstd', bootstrapped_meanstd)
    print('bootstrapped_std', bootstrapped_std)
    print('bootstrapped_stdstd', bootstrapped_stdstd)

    recoCosBins = np.cos((recoAngBins+0.5*binSpacing)*degtorad)
    binWidths = np.array([], dtype=np.float)
    for cosbin in range(len(recoAngBins)-1):
        binWidths = np.append(binWidths, (recoCosBins[cosbin+1]-recoCosBins[cosbin])*0.5)

    print('recoCosBins', recoCosBins)
    fig0, axes0 = plt.subplots()
    fig1, axes1 = plt.subplots()
    fig2, axes2 = plt.subplots()
    fig3, axes3 = plt.subplots()
    # linear = np.abs(bootstrapped_mean-recoAngBins) + bootstrapped_std
    axes0.errorbar(recoCosBins[:-1], np.sin((bootstrapped_mean-recoAngBins)*degtorad)[:-1], xerr=binWidths, yerr=np.sin(bootstrapped_meanstd*degtorad)[:-1], ls='none', label='Bias')
    axes0.errorbar(recoCosBins[:-1], np.sin((bootstrapped_std)*degtorad)[:-1], xerr=binWidths, yerr=np.sin(bootstrapped_stdstd*degtorad)[:-1], ls='none', label='Stddev')
    # axes0.errorbar(recoCosBins[:-1], np.sin(linear*degtorad)[:-1], xerr=binWidths, yerr=np.sqrt(np.sin(bootstrapped_stdstd*degtorad)[:-1]**2+np.sin(bootstrapped_meanstd*degtorad)[:-1]**2), ls='none', label='Total')
    axes0.set_xlabel('Reco cos(theta)')
    axes0.set_ylabel('Uncertainty [cos(theta)]')
    axes0.legend()
    recoAngBins = recoAngBins + 0.5 * binSpacing
    axes1.errorbar(recoAngBins, bootstrapped_mean-recoAngBins, xerr=0.5*binSpacing, yerr=bootstrapped_meanstd, ls='none', label='Bias')
    axes1.errorbar(recoAngBins, bootstrapped_std, xerr=0.5*binSpacing, yerr=bootstrapped_stdstd, ls='none', label='stddev')
    # axes1.errorbar(recoAngBins, bootstrapped_std+abs(bootstrapped_mean-recoAngBins), xerr=0.5*binSpacing, yerr=np.sqrt(bootstrapped_stdstd*bootstrapped_stdstd+bootstrapped_meanstd*bootstrapped_meanstd), ls='none', label='Total')
    axes1.set_title('Bootstrap')
    axes1.set_xlabel('Reco theta')
    axes1.set_ylabel('Reco theta')
    axes1.legend()
    axes2.errorbar(recoAngBins, mean-recoAngBins, xerr=0.5*binSpacing, ls='none', label='Mean')
    axes2.errorbar(recoAngBins, std, xerr=0.5*binSpacing, ls='none', label='stddev')
    axes2.set_title('MC')
    axes2.set_xlabel('Reco theta')
    axes2.set_ylabel('Uncertainty theta')
    axes2.legend()

    fig0.show()
    fig0.savefig('Uncertainty.png')
    fig1.show()
    fig1.savefig('Bootstrap.png')
    fig2.show()
    fig2.savefig('MC.png')


if __name__ == '__main__':
    main(sys.argv)
