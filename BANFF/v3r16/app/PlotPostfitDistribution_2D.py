#!/usr/bin/python
import os
import sys
import math
from ROOT import *
from array import array

'''
Makes plots for comparing 2D kinematic distributions
Current setup allows for comparing v3 fits for both file1 or file2,
    but if you want to compare v3 to v1, the v1 file must go in file2 location
Usage:
    python PlotPostfitDistribution_2D.py inputFile1 inputTitle1 inputFile2 inputTitle2 isFile2v1
'''

if (len(sys.argv) != 6):
    print 'Usage:\n'
    print 'PlotPostfitDistribution_2D.py inputFile1 inputTitle1 inputFile2 inputTitle2 isFile2v1'
    print '\tinputFile1   ---> raw output from a BANFF fit'
    print '\tintputTitle1 ---> title heading for inputFile1'
    print '\tinputFile2   ---> raw output from BANFF fit'
    print '\tintputTitle2 ---> title heading for inputFile2'
    print '\tisFile2v1    ---> Is inputFile2 a v1 fit (true) or a v3 fit (false)?'
    exit()

gROOT.SetBatch(kTRUE)
gROOT.ProcessLine('gErrorIgnoreLevel = kError')
gStyle.SetErrorX(0.0000001)

sampleNames_New = ['FGD1_numuCC_0pi', 'FGD1_numuCC_1pi', 'FGD1_numuCC_other',
                   'FGD2_numuCC_0pi', 'FGD2_numuCC_1pi', 'FGD2_numuCC_other',
                   'FGD1_anti-numuCC_QE', 'FGD1_anti-numuCC_nQE',
                   'FGD2_anti-numuCC_1_track', 'FGD2_anti-numuCC_N_tracks',
                   'FGD1_NuMuBkg_CCQE_in_AntiNu_Mode_', 'FGD1_NuMuBkg_CCnQE_in_AntiNu_Mode',
                   'FGD2_NuMuBkg_CCQE_in_AntiNu_Mode_', 'FGD2_NuMuBkg_CCnQE_in_AntiNu_Mode']

v1fit = str(sys.argv[5])
sampleNames_Old = []
if (v1fit == 'true'):
    sampleNames_Old = ['fgd1cc0pi', 'fgd1cc1pi', 'fgd1ccOth',
                       'fgd2cc0pi', 'fgd2cc1pi', 'fgd2ccOth',
                       'fgd1anucc1tr', 'fgd1anuccntr',
                       'fgd2anucc1tr', 'fgd2anuccntr',
                       'fgd1nucc1tr', 'fgd1nuccntr',
                       'fgd2nucc1tr', 'fgd2nuccntr']
else:
    sampleNames_Old = ['FGD1_numuCC_0pi', 'FGD1_numuCC_1pi', 'FGD1_numuCC_other',
                       'FGD2_numuCC_0pi', 'FGD2_numuCC_1pi', 'FGD2_numuCC_other',
                       'FGD1_anti-numuCC_QE', 'FGD1_anti-numuCC_nQE',
                       'FGD2_anti-numuCC_1_track', 'FGD2_anti-numuCC_N_tracks',
                       'FGD1_NuMuBkg_CCQE_in_AntiNu_Mode_', 'FGD1_NuMuBkg_CCnQE_in_AntiNu_Mode',
                       'FGD2_NuMuBkg_CCQE_in_AntiNu_Mode_', 'FGD2_NuMuBkg_CCnQE_in_AntiNu_Mode']

dataPrefitRatio  = []
dataPostfitRatio = []

inputFile1 = TFile(sys.argv[1])
title1     = str(sys.argv[2])
inputFile2 = TFile(sys.argv[3])
title2     = str(sys.argv[4])

firstHisto = True
firstRatio = True

for index in xrange(0, len(sampleNames_New)):
    data1 = (inputFile1.Get(sampleNames_New[index] + '_data')).Projection(1,0);                             data1.SetDirectory(0)
    pref1 = (inputFile1.Get(sampleNames_New[index] + '_prefit')).Projection(1,0);                           pref1.SetDirectory(0)
    post1 = (inputFile1.Get(sampleNames_New[index] + '_postfit_0_0')).Projection(1,0);                      post1.SetDirectory(0)
    none1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNoWeights')).Projection(1,0);             none1.SetDirectory(0)
    pots1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withPOTWeights')).Projection(1,0);            pots1.SetDirectory(0)
    flux1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomFluxWeights')).Projection(1,0);        flux1.SetDirectory(0)
    xsec1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomXSecWeights')).Projection(1,0);        xsec1.SetDirectory(0)
    dets1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomDetWeights')).Projection(1,0);         dets1.SetDirectory(0)
    flxs1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomFluxAndXSecWeights')).Projection(1,0); flxs1.SetDirectory(0)
    flde1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomFluxAndDetWeights')).Projection(1,0);  flde1.SetDirectory(0)

    data2 = (inputFile2.Get(sampleNames_Old[index] + '_data')).Projection(1,0);                             data2.SetDirectory(0)
    pref2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit')).Projection(1,0);                           pref2.SetDirectory(0)
    post2 = (inputFile2.Get(sampleNames_Old[index] + '_postfit_0_0')).Projection(1,0);                      post2.SetDirectory(0)
    none2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNoWeights')).Projection(1,0);             none2.SetDirectory(0)
    pots2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withPOTWeights')).Projection(1,0);            pots2.SetDirectory(0)
    flux2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomFluxWeights')).Projection(1,0);        flux2.SetDirectory(0)
    xsec2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomXSecWeights')).Projection(1,0);        xsec2.SetDirectory(0)
    dets2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomDetWeights')).Projection(1,0);         dets2.SetDirectory(0)
    flxs2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomFluxAndXSecWeights')).Projection(1,0); flxs2.SetDirectory(0)
    flde2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomFluxAndDetWeights')).Projection(1,0);  flde2.SetDirectory(0)

    # Set up maximum momenta for the different histogram types
    maxMom   = 2000.0
    minTheta = 0.6
    if (index > 5):
        maxMom   = 2000.0
        minTheta =  0.75

    for ibx in xrange(1, data1.GetNbinsX() + 1):
        for iby in xrange(1, data1.GetNbinsY() + 1):

            # Scale bins by bin area
            pfxdim = (post1.GetXaxis().GetBinUpEdge(ibx) - post1.GetXaxis().GetBinLowEdge(ibx))
            pfydim = (post1.GetYaxis().GetBinUpEdge(iby) - post1.GetYaxis().GetBinLowEdge(iby))

            pref1.SetBinContent(ibx, iby, pref1.GetBinContent(ibx,iby) / (pfxdim*pfydim))
            post1.SetBinContent(ibx, iby, post1.GetBinContent(ibx,iby) / (pfxdim*pfydim))
            none1.SetBinContent(ibx, iby, none1.GetBinContent(ibx,iby) / (pfxdim*pfydim))
            pots1.SetBinContent(ibx, iby, pots1.GetBinContent(ibx,iby) / (pfxdim*pfydim))
            flux1.SetBinContent(ibx, iby, flux1.GetBinContent(ibx,iby) / (pfxdim*pfydim))
            xsec1.SetBinContent(ibx, iby, xsec1.GetBinContent(ibx,iby) / (pfxdim*pfydim))
            dets1.SetBinContent(ibx, iby, dets1.GetBinContent(ibx,iby) / (pfxdim*pfydim))
            flxs1.SetBinContent(ibx, iby, flxs1.GetBinContent(ibx,iby) / (pfxdim*pfydim))
            flde1.SetBinContent(ibx, iby, flde1.GetBinContent(ibx,iby) / (pfxdim*pfydim))
            
            data1.SetBinContent(ibx,iby,data1.GetBinContent(ibx,iby) / (pfxdim*pfydim))

            pfxdim2 = (post2.GetXaxis().GetBinUpEdge(ibx) - post2.GetXaxis().GetBinLowEdge(ibx))
            pfydim2 = (post2.GetYaxis().GetBinUpEdge(iby) - post2.GetYaxis().GetBinLowEdge(iby))

            pref2.SetBinContent(ibx, iby, pref2.GetBinContent(ibx,iby) / (pfxdim2*pfydim2))
            post2.SetBinContent(ibx, iby, post2.GetBinContent(ibx,iby) / (pfxdim2*pfydim2))
            none2.SetBinContent(ibx, iby, none2.GetBinContent(ibx,iby) / (pfxdim2*pfydim2))
            pots2.SetBinContent(ibx, iby, pots2.GetBinContent(ibx,iby) / (pfxdim2*pfydim2))
            flux2.SetBinContent(ibx, iby, flux2.GetBinContent(ibx,iby) / (pfxdim2*pfydim2))
            xsec2.SetBinContent(ibx, iby, xsec2.GetBinContent(ibx,iby) / (pfxdim2*pfydim2))
            dets2.SetBinContent(ibx, iby, dets2.GetBinContent(ibx,iby) / (pfxdim2*pfydim2))
            flxs2.SetBinContent(ibx, iby, flxs2.GetBinContent(ibx,iby) / (pfxdim2*pfydim2))
            flde2.SetBinContent(ibx, iby, flde2.GetBinContent(ibx,iby) / (pfxdim2*pfydim2))
            
            data2Error = sqrt(data2.GetBinContent(ibx,iby))/(pfxdim2*pfydim2)
            data2.SetBinContent(ibx,iby,data2.GetBinContent(ibx,iby) / (pfxdim2*pfydim2))
            data2.SetBinError(ibx, iby, data2Error)

    # Make ratio plots
    data_data = data1.Clone()
    pref_pref = pref1.Clone()
    post_post = post1.Clone()
    none_none = none1.Clone()
    pots_pots = pots1.Clone()
    flux_flux = flux1.Clone()
    xsec_xsec = xsec1.Clone()
    dets_dets = dets1.Clone()
    flxs_flxs = flxs1.Clone()
    flde_flde = flde1.Clone()

    data_pref_1 = data1.Clone()
    data_post_1 = data1.Clone()
    pref_post_1 = pref1.Clone()
    data_pref_2 = data2.Clone()
    data_post_2 = data2.Clone()
    pref_post_2 = pref2.Clone()

    data_data.Divide(data2)
    pref_pref.Divide(pref2)
    post_post.Divide(post2)
    none_none.Divide(none2)
    pots_pots.Divide(pots2)
    flux_flux.Divide(flux2)
    xsec_xsec.Divide(xsec2)
    dets_dets.Divide(dets2)
    flxs_flxs.Divide(flxs2)
    flde_flde.Divide(flde2)

    data_pref_1.Divide(pref1)
    data_post_1.Divide(post1)
    pref_post_1.Divide(post1)
    data_pref_2.Divide(pref2)
    data_post_2.Divide(post2)
    pref_post_2.Divide(post2)

    # Set a few options for plotting purposes
    gStyle.SetOptStat(0)
    gStyle.SetPalette(54)

    #Figure out what the data, prefit, and postfit z axis max and min values need to be.
    dataMin1 = data1.GetMinimum(); dataMax1 = data1.GetMaximum()
    prefMin1 = pref1.GetMinimum(); prefMax1 = pref1.GetMaximum()
    postMin1 = post1.GetMinimum(); postMax1 = post1.GetMaximum()
    noneMin1 = none1.GetMinimum(); noneMax1 = none1.GetMaximum()
    potsMin1 = pots1.GetMinimum(); potsMax1 = pots1.GetMaximum()
    fluxMin1 = flux1.GetMinimum(); fluxMax1 = flux1.GetMaximum()
    xsecMin1 = xsec1.GetMinimum(); xsecMax1 = xsec1.GetMaximum()
    detsMin1 = dets1.GetMinimum(); detsMax1 = dets1.GetMaximum()
    flxsMin1 = flxs1.GetMinimum(); flxsMax1 = flxs1.GetMaximum()
    fldeMin1 = flde1.GetMinimum(); fldeMax1 = flde1.GetMaximum()
    zMax1 = max(dataMax1,prefMax1,postMax1,potsMax1,fluxMax1,xsecMax1,detsMax1,flxsMax1,fldeMax1)
    zMin1 = min(dataMin1,prefMin1,postMin1,potsMin1,fluxMin1,xsecMin1,detsMin1,flxsMin1,fldeMin1)

    dataMin2 = data2.GetMinimum(); dataMax2 = data2.GetMaximum()
    prefMin2 = pref2.GetMinimum(); prefMax2 = pref2.GetMaximum()
    postMin2 = post2.GetMinimum(); postMax2 = post2.GetMaximum()
    noneMin2 = none2.GetMinimum(); noneMax2 = none2.GetMaximum()
    potsMin2 = pots2.GetMinimum(); potsMax2 = pots2.GetMaximum()
    fluxMin2 = flux2.GetMinimum(); fluxMax2 = flux2.GetMaximum()
    xsecMin2 = xsec2.GetMinimum(); xsecMax2 = xsec2.GetMaximum()
    detsMin2 = dets2.GetMinimum(); detsMax2 = dets2.GetMaximum()
    flxsMin2 = flxs2.GetMinimum(); flxsMax2 = flxs2.GetMaximum()
    fldeMin2 = flde2.GetMinimum(); fldeMax2 = flde2.GetMaximum()
    zMax2 = max(dataMax2,prefMax2,postMax2,potsMax2,fluxMax2,xsecMax2,detsMax2,flxsMax2,fldeMax2)
    zMin2 = min(dataMin2,prefMin2,postMin2,potsMin2,fluxMin2,xsecMin2,detsMin2,flxsMin2,fldeMin2)

    # Plot the 2D distributions
    cPre = TCanvas()
    cPre.Divide(2,3)
    cPre.cd(1)
    data1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' Data (Events/(1 MeV/c)/0.1)')
    data1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    data1.GetYaxis().SetTitle('cos#theta_{#mu}')
    data1.Draw('ZCOL')
    data1.GetXaxis().SetRangeUser(0.0, maxMom)
    data1.GetYaxis().SetRangeUser(minTheta, 1.0)
    data1.GetZaxis().SetRangeUser(zMin1, zMax1)

    cPre.cd(2)
    data2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' Data (Events/(1 MeV/c)/0.1)')
    data2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    data2.GetYaxis().SetTitle('cos#theta_{#mu}')
    data2.Draw('ZCOL')
    data2.GetXaxis().SetRangeUser(0.0, maxMom)
    data2.GetYaxis().SetRangeUser(minTheta, 1.0)
    data2.GetZaxis().SetRangeUser(zMin2, zMax2)

    cPre.cd(3)
    pref1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' Prefit (Events/(1 MeV/c)/0.1)')
    pref1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pref1.GetYaxis().SetTitle('cos#theta_{#mu}')
    pref1.Draw('ZCOL')
    pref1.GetXaxis().SetRangeUser(0.0, maxMom)
    pref1.GetYaxis().SetRangeUser(minTheta, 1.0)
    pref1.GetZaxis().SetRangeUser(zMin1, zMax1)

    cPre.cd(4)
    pref2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' Prefit (Events/(1 MeV/c)/0.1)')
    pref2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pref2.GetYaxis().SetTitle('cos#theta_{#mu}')
    pref2.Draw('ZCOL')
    pref2.GetXaxis().SetRangeUser(0.0, maxMom)
    pref2.GetYaxis().SetRangeUser(minTheta, 1.0)
    pref2.GetZaxis().SetRangeUser(zMin2, zMax2)

    cPre.cd(5)
    post1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' Postfit (Events/(1 MeV/c)/0.1)')
    post1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    post1.GetYaxis().SetTitle('cos#theta_{#mu}')
    post1.Draw('ZCOL')
    post1.GetXaxis().SetRangeUser(0.0, maxMom)
    post1.GetYaxis().SetRangeUser(minTheta, 1.0)
    post1.GetZaxis().SetRangeUser(zMin1, zMax1)

    cPre.cd(6)
    post2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' Postfit (Events/(1 MeV/c)/0.1)')
    post2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    post2.GetYaxis().SetTitle('cos#theta_{#mu}')
    post2.Draw('ZCOL')
    post2.GetXaxis().SetRangeUser(0.0, maxMom)
    post2.GetYaxis().SetRangeUser(minTheta, 1.0)
    post2.GetZaxis().SetRangeUser(zMin2, zMax2)

    cPre.Modified()
    cPre.Update()
    if(firstHisto):
        cPre.Print('ptheta_distributions.pdf(')
        firstHisto = False
    else:
        cPre.Print('ptheta_distributions.pdf')

    cWei1 = TCanvas()
    cWei1.Divide(2,4)
    cWei1.cd(1)
    none1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' NoWeights (Events/(1 MeV/c)/0.1)')
    none1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    none1.GetYaxis().SetTitle('cos#theta_{#mu}')
    none1.Draw('ZCOL')
    none1.GetXaxis().SetRangeUser(0.0, maxMom)
    none1.GetYaxis().SetRangeUser(minTheta, 1.0)
    none1.GetZaxis().SetRangeUser(noneMin1, noneMax1)

    cWei1.cd(2)
    none2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' NoWeights (Events/(1 MeV/c)/0.1)')
    none2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    none2.GetYaxis().SetTitle('cos#theta_{#mu}')
    none2.Draw('ZCOL')
    none2.GetXaxis().SetRangeUser(0.0, maxMom)
    none2.GetYaxis().SetRangeUser(minTheta, 1.0)
    none2.GetZaxis().SetRangeUser(noneMin2, noneMax2)

    cWei1.cd(3)
    pots1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' POT Weight (Events/(1 MeV/c)/0.1)')
    pots1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pots1.GetYaxis().SetTitle('cos#theta_{#mu}')
    pots1.Draw('ZCOL')
    pots1.GetXaxis().SetRangeUser(0.0, maxMom)
    pots1.GetYaxis().SetRangeUser(minTheta, 1.0)
    pots1.GetZaxis().SetRangeUser(zMin1, zMax1)

    cWei1.cd(4)
    pots2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' POT Weight (Events/(1 MeV/c)/0.1)')
    pots2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pots2.GetYaxis().SetTitle('cos#theta_{#mu}')
    pots2.Draw('ZCOL')
    pots2.GetXaxis().SetRangeUser(0.0, maxMom)
    pots2.GetYaxis().SetRangeUser(minTheta, 1.0)
    pots2.GetZaxis().SetRangeUser(zMin2, zMax2)

    cWei1.cd(5)
    flux1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' POT+flux Weight (Events/(1 MeV/c)/0.1)')
    flux1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    flux1.GetYaxis().SetTitle('cos#theta_{#mu}')
    flux1.Draw('ZCOL')
    flux1.GetXaxis().SetRangeUser(0.0, maxMom)
    flux1.GetYaxis().SetRangeUser(minTheta, 1.0)
    flux1.GetZaxis().SetRangeUser(zMin1, zMax1)

    cWei1.cd(6)
    flux2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' POT+flux Weight (Events/(1 MeV/c)/0.1)')
    flux2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    flux2.GetYaxis().SetTitle('cos#theta_{#mu}')
    flux2.Draw('ZCOL')
    flux2.GetXaxis().SetRangeUser(0.0, maxMom)
    flux2.GetYaxis().SetRangeUser(minTheta, 1.0)
    flux2.GetZaxis().SetRangeUser(zMin2, zMax2)

    cWei1.cd(7)
    xsec1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' POT+xsec Weight (Events/(1 MeV/c)/0.1)')
    xsec1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    xsec1.GetYaxis().SetTitle('cos#theta_{#mu}')
    xsec1.Draw('ZCOL')
    xsec1.GetXaxis().SetRangeUser(0.0, maxMom)
    xsec1.GetYaxis().SetRangeUser(minTheta, 1.0)
    xsec1.GetZaxis().SetRangeUser(zMin1, zMax1)

    cWei1.cd(8)
    xsec2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' POT+xsec Weight (Events/(1 MeV/c)/0.1)')
    xsec2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    xsec2.GetYaxis().SetTitle('cos#theta_{#mu}')
    xsec2.Draw('ZCOL')
    xsec2.GetXaxis().SetRangeUser(0.0, maxMom)
    xsec2.GetYaxis().SetRangeUser(minTheta, 1.0)
    xsec2.GetZaxis().SetRangeUser(zMin2, zMax2)

    cWei1.Print('ptheta_distributions.pdf')

    cWei2 = TCanvas()
    cWei2.Divide(2,4)
    cWei2.cd(1)
    dets1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' POT+detector (Events/(1 MeV/c)/0.1)')
    dets1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    dets1.GetYaxis().SetTitle('cos#theta_{#mu}')
    dets1.Draw('ZCOL')
    dets1.GetXaxis().SetRangeUser(0.0, maxMom)
    dets1.GetYaxis().SetRangeUser(minTheta, 1.0)
    dets1.GetZaxis().SetRangeUser(zMin1, zMax1)

    cWei2.cd(2)
    dets2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' POT+detector (Events/(1 MeV/c)/0.1)')
    dets2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    dets2.GetYaxis().SetTitle('cos#theta_{#mu}')
    dets2.Draw('ZCOL')
    dets2.GetXaxis().SetRangeUser(0.0, maxMom)
    dets2.GetYaxis().SetRangeUser(minTheta, 1.0)
    dets2.GetZaxis().SetRangeUser(zMin2, zMax2)

    cWei2.cd(3)
    flxs1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' POT+flux+xsec (Events/(1 MeV/c)/0.1)')
    flxs1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    flxs1.GetYaxis().SetTitle('cos#theta_{#mu}')
    flxs1.Draw('ZCOL')
    flxs1.GetXaxis().SetRangeUser(0.0, maxMom)
    flxs1.GetYaxis().SetRangeUser(minTheta, 1.0)
    flxs1.GetZaxis().SetRangeUser(zMin1, zMax1)

    cWei2.cd(4)
    flxs2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' POT+flux+xsec (Events/(1 MeV/c)/0.1)')
    flxs2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    flxs2.GetYaxis().SetTitle('cos#theta_{#mu}')
    flxs2.Draw('ZCOL')
    flxs2.GetXaxis().SetRangeUser(0.0, maxMom)
    flxs2.GetYaxis().SetRangeUser(minTheta, 1.0)
    flxs2.GetZaxis().SetRangeUser(zMin2, zMax2)

    cWei2.cd(5)
    flde1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' POT+flux+det Weight (Events/(1 MeV/c)/0.1)')
    flde1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    flde1.GetYaxis().SetTitle('cos#theta_{#mu}')
    flde1.Draw('ZCOL')
    flde1.GetXaxis().SetRangeUser(0.0, maxMom)
    flde1.GetYaxis().SetRangeUser(minTheta, 1.0)
    flde1.GetZaxis().SetRangeUser(zMin1, zMax1)

    cWei2.cd(6)
    flde2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' POT+flux+det Weight (Events/(1 MeV/c)/0.1)')
    flde2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    flde2.GetYaxis().SetTitle('cos#theta_{#mu}')
    flde2.Draw('ZCOL')
    flde2.GetXaxis().SetRangeUser(0.0, maxMom)
    flde2.GetYaxis().SetRangeUser(minTheta, 1.0)
    flde2.GetZaxis().SetRangeUser(zMin2, zMax2)

    cWei2.cd(7)
    pref1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' Prefit (Events/(1 MeV/c)/0.1)')
    pref1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pref1.GetYaxis().SetTitle('cos#theta_{#mu}')
    pref1.Draw('ZCOL')
    pref1.GetXaxis().SetRangeUser(0.0, maxMom)
    pref1.GetYaxis().SetRangeUser(minTheta, 1.0)
    pref1.GetZaxis().SetRangeUser(zMin1, zMax1)

    cWei2.cd(8)
    pref2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' Prefit (Events/(1 MeV/c)/0.1)')
    pref2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pref2.GetYaxis().SetTitle('cos#theta_{#mu}')
    pref2.Draw('ZCOL')
    pref2.GetXaxis().SetRangeUser(0.0, maxMom)
    pref2.GetYaxis().SetRangeUser(minTheta, 1.0)
    pref2.GetZaxis().SetRangeUser(zMin2, zMax2)

    cWei2.Print('ptheta_distributions.pdf')

    # Set color palette for the ratio plots
    NRGBs = 6
    NCont = 99
    stops = [ 0.00, 0.02, 0.34, 0.51, 0.64, 1.00 ]
    red   = [ 0.87, 0.00, 0.00, 0.86, 1.00, 0.51 ]
    green = [ 1.00, 0.00, 0.81, 0.86, 0.20, 0.00 ]
    blue  = [ 0.12, 0.51, 1.00, 0.86, 0.00, 0.00 ]
    stopsArray = array('d', stops)
    redArray   = array('d', red)
    greenArray = array('d', green)
    blueArray  = array('d', blue)
    TColor.CreateGradientColorTable(NRGBs, stopsArray, redArray, greenArray, blueArray, NCont)
    gStyle.SetNumberContours(NCont)

    cRat = TCanvas()
    cRat.Divide(1,3)
    cRat.cd(1)
    data_data.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' Data')
    data_data.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    data_data.GetYaxis().SetTitle('cos#theta_{#mu}')
    data_data.Draw('ZCOL')
    data_data.GetXaxis().SetRangeUser(0.0, maxMom)
    data_data.GetYaxis().SetRangeUser(minTheta, 1.0)
    data_data.GetZaxis().SetRangeUser(0.5, 1.5)

    cRat.cd(2)
    pref_pref.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' Prefit')
    pref_pref.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pref_pref.GetYaxis().SetTitle('cos#theta_{#mu}')
    pref_pref.Draw('ZCOL')
    pref_pref.GetXaxis().SetRangeUser(0.0, maxMom)
    pref_pref.GetYaxis().SetRangeUser(minTheta, 1.0)
    pref_pref.GetZaxis().SetRangeUser(0.5, 1.5)
    
    cRat.cd(3)
    post_post.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' Postfit')
    post_post.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    post_post.GetYaxis().SetTitle('cos#theta_{#mu}')
    post_post.Draw('ZCOL')
    post_post.GetXaxis().SetRangeUser(0.0, maxMom)
    post_post.GetYaxis().SetRangeUser(minTheta, 1.0)
    post_post.GetZaxis().SetRangeUser(0.5, 1.5)
    
    cRat.Modified()
    cRat.Update()
    if(firstRatio):
        cRat.Print('ptheta_ratio.pdf(')
        firstRatio = False
    else:
        cRat.Print('ptheta_ratio.pdf')

    cCom = TCanvas()
    cCom.Divide(2,3)
    cCom.cd(1)
    data_pref_1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' Data / ' + title1 + ' Prefit')
    data_pref_1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    data_pref_1.GetYaxis().SetTitle('cos#theta_{#mu}')
    data_pref_1.Draw('ZCOL')
    data_pref_1.GetXaxis().SetRangeUser(0.0, maxMom)
    data_pref_1.GetYaxis().SetRangeUser(minTheta, 1.0)
    data_pref_1.GetZaxis().SetRangeUser(0.5, 1.5)

    cCom.cd(2)
    data_pref_2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' Data / ' + title2 + ' Prefit')
    data_pref_2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    data_pref_2.GetYaxis().SetTitle('cos#theta_{#mu}')
    data_pref_2.Draw('ZCOL')
    data_pref_2.GetXaxis().SetRangeUser(0.0, maxMom)
    data_pref_2.GetYaxis().SetRangeUser(minTheta, 1.0)
    data_pref_2.GetZaxis().SetRangeUser(0.5, 1.5)

    cCom.cd(3)
    data_post_1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' Data / ' + title1 + ' Postfit')
    data_post_1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    data_post_1.GetYaxis().SetTitle('cos#theta_{#mu}')
    data_post_1.Draw('ZCOL')
    data_post_1.GetXaxis().SetRangeUser(0.0, maxMom)
    data_post_1.GetYaxis().SetRangeUser(minTheta, 1.0)
    data_post_1.GetZaxis().SetRangeUser(0.5, 1.5)

    cCom.cd(4)
    data_post_2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' Data / ' + title2 + ' Postfit')
    data_post_2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    data_post_2.GetYaxis().SetTitle('cos#theta_{#mu}')
    data_post_2.Draw('ZCOL')
    data_post_2.GetXaxis().SetRangeUser(0.0, maxMom)
    data_post_2.GetYaxis().SetRangeUser(minTheta, 1.0)
    data_post_2.GetZaxis().SetRangeUser(0.5, 1.5)

    cCom.cd(5)
    pref_post_1.SetTitle(sampleNames_New[index] + ': ' + title1 + ' Prefit / ' + title1 + ' Postfit')
    pref_post_1.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pref_post_1.GetYaxis().SetTitle('cos#theta_{#mu}')
    pref_post_1.Draw('ZCOL')
    pref_post_1.GetXaxis().SetRangeUser(0.0, maxMom)
    pref_post_1.GetYaxis().SetRangeUser(minTheta, 1.0)
    pref_post_1.GetZaxis().SetRangeUser(0.5, 1.5)

    cCom.cd(6)
    pref_post_2.SetTitle(sampleNames_New[index] + ': ' + title2 + ' Prefit / ' + title2 + ' Postfit')
    pref_post_2.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pref_post_2.GetYaxis().SetTitle('cos#theta_{#mu}')
    pref_post_2.Draw('ZCOL')
    pref_post_2.GetXaxis().SetRangeUser(0.0, maxMom)
    pref_post_2.GetYaxis().SetRangeUser(minTheta, 1.0)
    pref_post_2.GetZaxis().SetRangeUser(0.5, 1.5)

    cCom.Print('ptheta_ratio.pdf')

    cWer = TCanvas()
    cWer.Divide(2,4)
    cWer.cd(1)
    none_none.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' NoWeights')
    none_none.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    none_none.GetYaxis().SetTitle('cos#theta_{#mu}')
    none_none.Draw('ZCOL')
    none_none.GetXaxis().SetRangeUser(0.0, maxMom)
    none_none.GetYaxis().SetRangeUser(minTheta, 1.0)
    none_none.GetZaxis().SetRangeUser(0.5, 1.5)

    cWer.cd(2)
    pots_pots.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' POT Weight')
    pots_pots.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pots_pots.GetYaxis().SetTitle('cos#theta_{#mu}')
    pots_pots.Draw('ZCOL')
    pots_pots.GetXaxis().SetRangeUser(0.0, maxMom)
    pots_pots.GetYaxis().SetRangeUser(minTheta, 1.0)
    pots_pots.GetZaxis().SetRangeUser(0.5, 1.5)

    cWer.cd(3)
    flux_flux.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' POT+flux')
    flux_flux.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    flux_flux.GetYaxis().SetTitle('cos#theta_{#mu}')
    flux_flux.Draw('ZCOL')
    flux_flux.GetXaxis().SetRangeUser(0.0, maxMom)
    flux_flux.GetYaxis().SetRangeUser(minTheta, 1.0)
    flux_flux.GetZaxis().SetRangeUser(0.5, 1.5)

    cWer.cd(4)
    xsec_xsec.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' POT+xsec')
    xsec_xsec.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    xsec_xsec.GetYaxis().SetTitle('cos#theta_{#mu}')
    xsec_xsec.Draw('ZCOL')
    xsec_xsec.GetXaxis().SetRangeUser(0.0, maxMom)
    xsec_xsec.GetYaxis().SetRangeUser(minTheta, 1.0)
    xsec_xsec.GetZaxis().SetRangeUser(0.5, 1.5)

    cWer.cd(5)
    dets_dets.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' POT+det')
    dets_dets.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    dets_dets.GetYaxis().SetTitle('cos#theta_{#mu}')
    dets_dets.Draw('ZCOL')
    dets_dets.GetXaxis().SetRangeUser(0.0, maxMom)
    dets_dets.GetYaxis().SetRangeUser(minTheta, 1.0)
    dets_dets.GetZaxis().SetRangeUser(0.5, 1.5)

    cWer.cd(6)
    flxs_flxs.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' POT+flux+xsec')
    flxs_flxs.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    flxs_flxs.GetYaxis().SetTitle('cos#theta_{#mu}')
    flxs_flxs.Draw('ZCOL')
    flxs_flxs.GetXaxis().SetRangeUser(0.0, maxMom)
    flxs_flxs.GetYaxis().SetRangeUser(minTheta, 1.0)
    flxs_flxs.GetZaxis().SetRangeUser(0.5, 1.5)

    cWer.cd(7)
    flde_flde.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' POT+flux+det')
    flde_flde.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    flde_flde.GetYaxis().SetTitle('cos#theta_{#mu}')
    flde_flde.Draw('ZCOL')
    flde_flde.GetXaxis().SetRangeUser(0.0, maxMom)
    flde_flde.GetYaxis().SetRangeUser(minTheta, 1.0)
    flde_flde.GetZaxis().SetRangeUser(0.5, 1.5)

    cWer.cd(8)
    pref_pref.SetTitle(sampleNames_New[index] + ': ' + title1 + ' / ' + title2 + ' Prefit')
    pref_pref.GetXaxis().SetTitle('p_{#mu} (MeV/c)')
    pref_pref.GetYaxis().SetTitle('cos#theta_{#mu}')
    pref_pref.Draw('ZCOL')
    pref_pref.GetXaxis().SetRangeUser(0.0, maxMom)
    pref_pref.GetYaxis().SetRangeUser(minTheta, 1.0)
    pref_pref.GetZaxis().SetRangeUser(0.5, 1.5)

    cWer.Print('ptheta_ratio.pdf')

c1 = TCanvas()
c1.cd()
c1.Print('ptheta_distributions.pdf]')
c1.Print('ptheta_ratio.pdf]')
