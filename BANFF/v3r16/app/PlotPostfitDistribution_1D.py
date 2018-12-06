#!/usr/bin/python
import os
import sys
import math
from ROOT import *

'''
Makes plots for comparing 1D kinematic distributions
Current setup is for comparing Psyche v1 and v3, but should be an easy
    adjustment for doing just one or the other
Usage:
    python PlotPostfitDistribution_1D.py inputFile1 inputFile2
'''

if (len(sys.argv) != 3):
    print 'Usage:\n'
    print 'PlotPostfitDistribution_1D.py inputFile1 inputFile2'
    print '\tinputFile1 ---> raw output from v3 BANFF fit'
    print '\tinputFile2 ---> raw output from v1 BANFF fit'
    exit()

# Loop through BANFF fit outputs and plot prefit/postfit/data for the 2 fitters
gROOT.SetBatch(kTRUE)
gROOT.ProcessLine('gErrorIgnoreLevel = kError')
gStyle.SetErrorX(0.0000001)

sampleNames_New = ['FGD1_numuCC_0pi', 'FGD1_numuCC_1pi', 'FGD1_numuCC_other',
                   'FGD2_numuCC_0pi', 'FGD2_numuCC_1pi', 'FGD2_numuCC_other',
                   'FGD1_anti-numuCC_QE', 'FGD1_anti-numuCC_nQE',
                   'FGD2_anti-numuCC_1_track', 'FGD2_anti-numuCC_N_tracks',
                   'FGD1_NuMuBkg_CCQE_in_AntiNu_Mode_', 'FGD1_NuMuBkg_CCnQE_in_AntiNu_Mode',
                   'FGD2_NuMuBkg_CCQE_in_AntiNu_Mode_', 'FGD2_NuMuBkg_CCnQE_in_AntiNu_Mode']

sampleNames_Old = ['fgd1cc0pi', 'fgd1cc1pi', 'fgd1ccOth',
                   'fgd2cc0pi', 'fgd2cc1pi', 'fgd2ccOth',
                   'fgd1anucc1tr', 'fgd1anuccntr',
                   'fgd2anucc1tr', 'fgd2anuccntr',
                   'fgd1nucc1tr', 'fgd1nuccntr',
                   'fgd2nucc1tr', 'fgd2nuccntr']

# Set up some colors for making plots
color1 = TColor(1001, 228./255.,  26./255.,  28./255., 'color1')
color2 = TColor(1002,  55./255., 126./255., 184./255., 'color2')
color3 = TColor(1003,  77./255., 175./255.,  74./255., 'color3')
color4 = TColor(1004, 152./255.,  78./255., 163./255., 'color4')
color5 = TColor(1005, 255./255., 127./255.,   0./255., 'color5')
color6 = TColor(1006, 255./255., 255./255.,  51./255., 'color6')
color7 = TColor(1007, 166./255.,  86./255.,  40./255., 'color7')

firstHisto   = True
firstHistot  = True
firstRatio   = True
firstRatiot  = True
firstWeight  = True
firstWeightt = True
firstRatiop  = True
firstRatioc  = True

inputFile1 = TFile(sys.argv[1])
inputFile2 = TFile(sys.argv[2])

for index in xrange(0,len(sampleNames_New)):
    # Retrieve plots for first input file
    dataMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_data')).Projection(0)
    dataMomProj_1.SetDirectory(0)

    prefMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit')).Projection(0)
    prefMomProj_1.SetDirectory(0)

    postMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_postfit_0_0')).Projection(0)
    postMomProj_1.SetDirectory(0)

    #postMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit')).Projection(0)
    #postMomProj_1.SetDirectory(0)

    noneMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNoWeights')).Projection(0)
    noneMomProj_1.SetDirectory(0)

    potsMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withPOTWeights')).Projection(0)
    potsMomProj_1.SetDirectory(0)

    fluxMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomFluxWeights')).Projection(0)
    fluxMomProj_1.SetDirectory(0)

    xsecMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomXSecWeights')).Projection(0)
    xsecMomProj_1.SetDirectory(0)

    detsMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomDetWeights')).Projection(0)
    detsMomProj_1.SetDirectory(0)

    flxsMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomFluxAndXSecWeights')).Projection(0)
    flxsMomProj_1.SetDirectory(0)

    fldeMomProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomFluxAndDetWeights')).Projection(0)
    fldeMomProj_1.SetDirectory(0)

    dataThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_data')).Projection(1)
    dataThetaProj_1.SetDirectory(0)

    prefThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit')).Projection(1)
    prefThetaProj_1.SetDirectory(0)

    postThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_postfit_0_0')).Projection(1)
    postThetaProj_1.SetDirectory(0)

    #postThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit')).Projection(1)
    #postThetaProj_1.SetDirectory(0)

    noneThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNoWeights')).Projection(1)
    noneThetaProj_1.SetDirectory(0)

    potsThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withPOTWeights')).Projection(1)
    potsThetaProj_1.SetDirectory(0)

    fluxThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomFluxWeights')).Projection(1)
    fluxThetaProj_1.SetDirectory(0)

    xsecThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomXSecWeights')).Projection(1)
    xsecThetaProj_1.SetDirectory(0)

    detsThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomDetWeights')).Projection(1)
    detsThetaProj_1.SetDirectory(0)

    flxsThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomFluxAndXSecWeights')).Projection(1)
    flxsThetaProj_1.SetDirectory(0)

    fldeThetaProj_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit_withNomFluxAndDetWeights')).Projection(1)
    fldeThetaProj_1.SetDirectory(0)

    # Retrieve plots for second input file
    dataMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_data')).Projection(0)
    dataMomProj_2.SetDirectory(0)

    prefMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit')).Projection(0)
    prefMomProj_2.SetDirectory(0)

    postMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_postfit_0_0')).Projection(0)
    postMomProj_2.SetDirectory(0)

    #postMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit')).Projection(0)
    #postMomProj_2.SetDirectory(0)

    noneMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNoWeights')).Projection(0)
    noneMomProj_2.SetDirectory(0)

    potsMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withPOTWeights')).Projection(0)
    potsMomProj_2.SetDirectory(0)

    fluxMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomFluxWeights')).Projection(0)
    fluxMomProj_2.SetDirectory(0)

    xsecMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomXSecWeights')).Projection(0)
    xsecMomProj_2.SetDirectory(0)

    detsMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomDetWeights')).Projection(0)
    detsMomProj_2.SetDirectory(0)

    flxsMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomFluxAndXSecWeights')).Projection(0)
    flxsMomProj_2.SetDirectory(0)

    fldeMomProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomFluxAndDetWeights')).Projection(0)
    fldeMomProj_2.SetDirectory(0)

    dataThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_data')).Projection(1)
    dataThetaProj_2.SetDirectory(0)

    prefThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit')).Projection(1)
    prefThetaProj_2.SetDirectory(0)

    postThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_postfit_0_0')).Projection(1)
    postThetaProj_2.SetDirectory(0)

    #postThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit')).Projection(1)
    #postThetaProj_2.SetDirectory(0)

    noneThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNoWeights')).Projection(1)
    noneThetaProj_2.SetDirectory(0)

    potsThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withPOTWeights')).Projection(1)
    potsThetaProj_2.SetDirectory(0)

    fluxThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomFluxWeights')).Projection(1)
    fluxThetaProj_2.SetDirectory(0)

    xsecThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomXSecWeights')).Projection(1)
    xsecThetaProj_2.SetDirectory(0)

    detsThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomDetWeights')).Projection(1)
    detsThetaProj_2.SetDirectory(0)

    flxsThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomFluxAndXSecWeights')).Projection(1)
    flxsThetaProj_2.SetDirectory(0)

    fldeThetaProj_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit_withNomFluxAndDetWeights')).Projection(1)
    fldeThetaProj_2.SetDirectory(0)

    # Make sure the binning is the same before moving on
    if (dataMomProj_1.GetXaxis().GetXbins() == dataMomProj_2.GetXaxis().GetXbins()):
        print 'Data Momentum Projections have different binnings'
        exit()
    if (prefMomProj_1.GetXaxis().GetXbins() == prefMomProj_2.GetXaxis().GetXbins()):
        print 'Prefit Momentum Projections have different binnings'
        exit()
    if (postMomProj_1.GetXaxis().GetXbins() == postMomProj_2.GetXaxis().GetXbins()):
        print 'Postfit Momentum Projections have different binnings'
        exit()
    if (dataThetaProj_1.GetXaxis().GetXbins() == dataThetaProj_2.GetXaxis().GetXbins()):
        print 'Data Momentum Projections have different binnings'
        exit()
    if (prefThetaProj_1.GetXaxis().GetXbins() == prefThetaProj_2.GetXaxis().GetXbins()):
        print 'Prefit Momentum Projections have different binnings'
        exit()
    if (postThetaProj_1.GetXaxis().GetXbins() == postThetaProj_2.GetXaxis().GetXbins()):
        print 'Postfit Momentum Projections have different binnings'
        exit()

    # Scale projections by bin width
    for j in xrange(1, dataMomProj_1.GetNbinsX()+1):
        width = float(dataMomProj_1.GetXaxis().GetBinUpEdge(j) - dataMomProj_1.GetXaxis().GetBinLowEdge(j))
        dataMomProj_1.SetBinContent(j,dataMomProj_1.GetBinContent(j) / width)
        dataMomProj_1.SetBinError(j, dataMomProj_1.GetBinError(j) / width)
        prefMomProj_1.SetBinContent(j,prefMomProj_1.GetBinContent(j) / width)
        prefMomProj_1.SetBinError(j,prefMomProj_1.GetBinError(j) / width)
        postMomProj_1.SetBinContent(j,postMomProj_1.GetBinContent(j) / width)
        postMomProj_1.SetBinError(j,postMomProj_1.GetBinError(j) / width)
        noneMomProj_1.SetBinContent(j,noneMomProj_1.GetBinContent(j) / width)
        noneMomProj_1.SetBinError(j,noneMomProj_1.GetBinError(j) / width)
        potsMomProj_1.SetBinContent(j,potsMomProj_1.GetBinContent(j) / width)
        potsMomProj_1.SetBinError(j,potsMomProj_1.GetBinError(j) / width)
        fluxMomProj_1.SetBinContent(j,fluxMomProj_1.GetBinContent(j) / width)
        fluxMomProj_1.SetBinError(j,fluxMomProj_1.GetBinError(j) / width)
        xsecMomProj_1.SetBinContent(j,xsecMomProj_1.GetBinContent(j) / width)
        xsecMomProj_1.SetBinError(j,xsecMomProj_1.GetBinError(j) / width)
        detsMomProj_1.SetBinContent(j,detsMomProj_1.GetBinContent(j) / width)
        detsMomProj_1.SetBinError(j,detsMomProj_1.GetBinError(j) / width)
        flxsMomProj_1.SetBinContent(j,flxsMomProj_1.GetBinContent(j) / width)
        flxsMomProj_1.SetBinError(j,flxsMomProj_1.GetBinError(j) / width)
        fldeMomProj_1.SetBinContent(j,fldeMomProj_1.GetBinContent(j) / width)
        fldeMomProj_1.SetBinError(j,fldeMomProj_1.GetBinError(j) / width)

        dataMomProj_2.SetBinContent(j,dataMomProj_2.GetBinContent(j) / width)
        dataMomProj_2.SetBinError(j, dataMomProj_2.GetBinError(j) / width)
        prefMomProj_2.SetBinContent(j,prefMomProj_2.GetBinContent(j) / width)
        prefMomProj_2.SetBinError(j,prefMomProj_2.GetBinError(j) / width)
        postMomProj_2.SetBinContent(j,postMomProj_2.GetBinContent(j) / width)
        postMomProj_2.SetBinError(j,postMomProj_2.GetBinError(j) / width)
        noneMomProj_2.SetBinContent(j,noneMomProj_2.GetBinContent(j) / width)
        noneMomProj_2.SetBinError(j,noneMomProj_2.GetBinError(j) / width)
        potsMomProj_2.SetBinContent(j,potsMomProj_2.GetBinContent(j) / width)
        potsMomProj_2.SetBinError(j,potsMomProj_2.GetBinError(j) / width)
        fluxMomProj_2.SetBinContent(j,fluxMomProj_2.GetBinContent(j) / width)
        fluxMomProj_2.SetBinError(j,fluxMomProj_2.GetBinError(j) / width)
        xsecMomProj_2.SetBinContent(j,xsecMomProj_2.GetBinContent(j) / width)
        xsecMomProj_2.SetBinError(j,xsecMomProj_2.GetBinError(j) / width)
        detsMomProj_2.SetBinContent(j,detsMomProj_2.GetBinContent(j) / width)
        detsMomProj_2.SetBinError(j,detsMomProj_2.GetBinError(j) / width)
        flxsMomProj_2.SetBinContent(j,flxsMomProj_2.GetBinContent(j) / width)
        flxsMomProj_2.SetBinError(j,flxsMomProj_2.GetBinError(j) / width)
        fldeMomProj_2.SetBinContent(j,fldeMomProj_2.GetBinContent(j) / width)
        fldeMomProj_2.SetBinError(j,fldeMomProj_2.GetBinError(j) / width)

    for j in xrange(1, dataThetaProj_1.GetNbinsX()+1):
        width = float(dataThetaProj_1.GetXaxis().GetBinUpEdge(j) - dataThetaProj_1.GetXaxis().GetBinLowEdge(j))
        dataThetaProj_1.SetBinContent(j,dataThetaProj_1.GetBinContent(j) / width)
        dataThetaProj_1.SetBinError(j, dataThetaProj_1.GetBinError(j) / width)
        prefThetaProj_1.SetBinContent(j,prefThetaProj_1.GetBinContent(j) / width)
        prefThetaProj_1.SetBinError(j,prefThetaProj_1.GetBinError(j) / width)
        postThetaProj_1.SetBinContent(j,postThetaProj_1.GetBinContent(j) / width)
        postThetaProj_1.SetBinError(j,postThetaProj_1.GetBinError(j) / width)
        noneThetaProj_1.SetBinContent(j,noneThetaProj_1.GetBinContent(j) / width)
        noneThetaProj_1.SetBinError(j,noneThetaProj_1.GetBinError(j) / width)
        potsThetaProj_1.SetBinContent(j,potsThetaProj_1.GetBinContent(j) / width)
        potsThetaProj_1.SetBinError(j,potsThetaProj_1.GetBinError(j) / width)
        fluxThetaProj_1.SetBinContent(j,fluxThetaProj_1.GetBinContent(j) / width)
        fluxThetaProj_1.SetBinError(j,fluxThetaProj_1.GetBinError(j) / width)
        xsecThetaProj_1.SetBinContent(j,xsecThetaProj_1.GetBinContent(j) / width)
        xsecThetaProj_1.SetBinError(j,xsecThetaProj_1.GetBinError(j) / width)
        detsThetaProj_1.SetBinContent(j,detsThetaProj_1.GetBinContent(j) / width)
        detsThetaProj_1.SetBinError(j,detsThetaProj_1.GetBinError(j) / width)
        flxsThetaProj_1.SetBinContent(j,flxsThetaProj_1.GetBinContent(j) / width)
        flxsThetaProj_1.SetBinError(j,flxsThetaProj_1.GetBinError(j) / width)
        fldeThetaProj_1.SetBinContent(j,fldeThetaProj_1.GetBinContent(j) / width)
        fldeThetaProj_1.SetBinError(j,fldeThetaProj_1.GetBinError(j) / width)

        dataThetaProj_2.SetBinContent(j,dataThetaProj_2.GetBinContent(j) / width)
        dataThetaProj_2.SetBinError(j, dataThetaProj_2.GetBinError(j) / width)
        prefThetaProj_2.SetBinContent(j,prefThetaProj_2.GetBinContent(j) / width)
        prefThetaProj_2.SetBinError(j,prefThetaProj_2.GetBinError(j) / width)
        postThetaProj_2.SetBinContent(j,postThetaProj_2.GetBinContent(j) / width)
        postThetaProj_2.SetBinError(j,postThetaProj_2.GetBinError(j) / width)
        noneThetaProj_2.SetBinContent(j,noneThetaProj_2.GetBinContent(j) / width)
        noneThetaProj_2.SetBinError(j,noneThetaProj_2.GetBinError(j) / width)
        potsThetaProj_2.SetBinContent(j,potsThetaProj_2.GetBinContent(j) / width)
        potsThetaProj_2.SetBinError(j,potsThetaProj_2.GetBinError(j) / width)
        fluxThetaProj_2.SetBinContent(j,fluxThetaProj_2.GetBinContent(j) / width)
        fluxThetaProj_2.SetBinError(j,fluxThetaProj_2.GetBinError(j) / width)
        xsecThetaProj_2.SetBinContent(j,xsecThetaProj_2.GetBinContent(j) / width)
        xsecThetaProj_2.SetBinError(j,xsecThetaProj_2.GetBinError(j) / width)
        detsThetaProj_2.SetBinContent(j,detsThetaProj_2.GetBinContent(j) / width)
        detsThetaProj_2.SetBinError(j,detsThetaProj_2.GetBinError(j) / width)
        flxsThetaProj_2.SetBinContent(j,flxsThetaProj_2.GetBinContent(j) / width)
        flxsThetaProj_2.SetBinError(j,flxsThetaProj_2.GetBinError(j) / width)
        fldeThetaProj_2.SetBinContent(j,fldeThetaProj_2.GetBinContent(j) / width)
        fldeThetaProj_2.SetBinError(j,fldeThetaProj_2.GetBinError(j) / width)

    # Retrieve plots for ratios
    dataMom_dataMom = dataMomProj_1.Clone()
    prefMom_prefMom = prefMomProj_1.Clone()
    postMom_postMom = postMomProj_1.Clone()
    noneMom_noneMom = noneMomProj_1.Clone()
    potsMom_potsMom = potsMomProj_1.Clone()
    fluxMom_fluxMom = fluxMomProj_1.Clone()
    xsecMom_xsecMom = xsecMomProj_1.Clone()
    detsMom_detsMom = detsMomProj_1.Clone()
    flxsMom_flxsMom = flxsMomProj_1.Clone()
    fldeMom_fldeMom = fldeMomProj_1.Clone()

    dataMom_prefMom_1 = dataMomProj_1.Clone()
    dataMom_postMom_1 = dataMomProj_1.Clone()
    prefMom_postMom_1 = prefMomProj_1.Clone()
    dataMom_prefMom_2 = dataMomProj_2.Clone()
    dataMom_postMom_2 = dataMomProj_2.Clone()
    prefMom_postMom_2 = prefMomProj_2.Clone()

    dataTheta_dataTheta = dataThetaProj_1.Clone()
    prefTheta_prefTheta = prefThetaProj_1.Clone()
    postTheta_postTheta = postThetaProj_1.Clone()
    noneTheta_noneTheta = noneThetaProj_1.Clone()
    potsTheta_potsTheta = potsThetaProj_1.Clone()
    fluxTheta_fluxTheta = fluxThetaProj_1.Clone()
    xsecTheta_xsecTheta = xsecThetaProj_1.Clone()
    detsTheta_detsTheta = detsThetaProj_1.Clone()
    flxsTheta_flxsTheta = flxsThetaProj_1.Clone()
    fldeTheta_fldeTheta = fldeThetaProj_1.Clone()

    dataTheta_prefTheta_1 = dataThetaProj_1.Clone()
    dataTheta_postTheta_1 = dataThetaProj_1.Clone()
    prefTheta_postTheta_1 = prefThetaProj_1.Clone()
    dataTheta_prefTheta_2 = dataThetaProj_2.Clone()
    dataTheta_postTheta_2 = dataThetaProj_2.Clone()
    prefTheta_postTheta_2 = prefThetaProj_2.Clone()

    # Make the ratios
    dataMom_dataMom.Divide(dataMomProj_2)
    prefMom_prefMom.Divide(prefMomProj_2)
    postMom_postMom.Divide(postMomProj_2)
    noneMom_noneMom.Divide(noneMomProj_2)
    potsMom_potsMom.Divide(potsMomProj_2)
    fluxMom_fluxMom.Divide(fluxMomProj_2)
    xsecMom_xsecMom.Divide(xsecMomProj_2)
    detsMom_detsMom.Divide(detsMomProj_2)
    flxsMom_flxsMom.Divide(flxsMomProj_2)
    fldeMom_fldeMom.Divide(fldeMomProj_2)

    dataMom_prefMom_1.Divide(prefMomProj_1)
    dataMom_postMom_1.Divide(postMomProj_1)
    prefMom_postMom_1.Divide(postMomProj_1)
    dataMom_prefMom_2.Divide(prefMomProj_2)
    dataMom_postMom_2.Divide(postMomProj_2)
    prefMom_postMom_2.Divide(postMomProj_2)

    dataTheta_dataTheta.Divide(dataThetaProj_2)
    prefTheta_prefTheta.Divide(prefThetaProj_2)
    postTheta_postTheta.Divide(postThetaProj_2)
    noneTheta_noneTheta.Divide(noneThetaProj_2)
    potsTheta_potsTheta.Divide(potsThetaProj_2)
    fluxTheta_fluxTheta.Divide(fluxThetaProj_2)
    xsecTheta_xsecTheta.Divide(xsecThetaProj_2)
    detsTheta_detsTheta.Divide(detsThetaProj_2)
    flxsTheta_flxsTheta.Divide(flxsThetaProj_2)
    fldeTheta_fldeTheta.Divide(fldeThetaProj_2)

    dataTheta_prefTheta_1.Divide(prefThetaProj_1)
    dataTheta_postTheta_1.Divide(postThetaProj_1)
    prefTheta_postTheta_1.Divide(postThetaProj_1)
    dataTheta_prefTheta_2.Divide(prefThetaProj_2)
    dataTheta_postTheta_2.Divide(postThetaProj_2)
    prefTheta_postTheta_2.Divide(postThetaProj_2)

    # Set Marker traits and Line traits
    # Set the data to black
    dataMomProj_1.SetMarkerStyle(20)
    dataMomProj_1.SetMarkerColor(1)
    dataMomProj_1.SetLineColor(1)
    dataMomProj_1.SetLineWidth(3)

    dataMomProj_2.SetMarkerStyle(3)
    dataMomProj_2.SetMarkerColor(1)
    dataMomProj_2.SetLineColor(1)
    dataMomProj_2.SetLineWidth(3)

    dataThetaProj_1.SetMarkerStyle(20)
    dataThetaProj_1.SetMarkerColor(1)
    dataThetaProj_1.SetLineColor(1)
    dataThetaProj_1.SetLineWidth(3)

    dataThetaProj_2.SetMarkerStyle(3)
    dataThetaProj_2.SetMarkerColor(1)
    dataThetaProj_2.SetLineColor(1)
    dataThetaProj_2.SetLineWidth(3)

    # Set the prefit to blue
    prefMomProj_1.SetLineColor(4)
    prefMomProj_1.SetLineStyle(1)
    prefMomProj_1.SetLineWidth(3)

    prefMomProj_2.SetLineColor(4)
    prefMomProj_2.SetLineStyle(9)
    prefMomProj_2.SetLineWidth(3)

    prefThetaProj_1.SetLineColor(4)
    prefThetaProj_1.SetLineStyle(1)
    prefThetaProj_1.SetLineWidth(3)
    
    prefThetaProj_2.SetLineColor(4)
    prefThetaProj_2.SetLineStyle(9)
    prefThetaProj_2.SetLineWidth(3)
    
    # Postfit is red
    postMomProj_1.SetLineColor(2)
    postMomProj_1.SetLineStyle(1)
    postMomProj_1.SetLineWidth(3)

    postMomProj_2.SetLineColor(2)
    postMomProj_2.SetLineStyle(9)
    postMomProj_2.SetLineWidth(3)

    postThetaProj_1.SetLineColor(2)
    postThetaProj_1.SetLineStyle(1)
    postThetaProj_1.SetLineWidth(3)

    postThetaProj_2.SetLineColor(2)
    postThetaProj_2.SetLineStyle(9)
    postThetaProj_2.SetLineWidth(3)

    # Weights have different colors
    noneMomProj_1.SetLineColor(1001)
    noneMomProj_1.SetLineStyle(1)
    noneMomProj_1.SetLineWidth(3)
    potsMomProj_1.SetLineColor(1002)
    potsMomProj_1.SetLineStyle(1)
    potsMomProj_1.SetLineWidth(3)
    fluxMomProj_1.SetLineColor(1003)
    fluxMomProj_1.SetLineStyle(1)
    fluxMomProj_1.SetLineWidth(3)
    xsecMomProj_1.SetLineColor(1004)
    xsecMomProj_1.SetLineStyle(1)
    xsecMomProj_1.SetLineWidth(3)
    detsMomProj_1.SetLineColor(1005)
    detsMomProj_1.SetLineStyle(1)
    detsMomProj_1.SetLineWidth(3)
    flxsMomProj_1.SetLineColor(1006)
    flxsMomProj_1.SetLineStyle(1)
    flxsMomProj_1.SetLineWidth(3)
    fldeMomProj_1.SetLineColor(1007)
    fldeMomProj_1.SetLineStyle(1)
    fldeMomProj_1.SetLineWidth(3)

    noneMomProj_2.SetLineColor(1001)
    noneMomProj_2.SetLineStyle(9)
    noneMomProj_2.SetLineWidth(3)
    potsMomProj_2.SetLineColor(1002)
    potsMomProj_2.SetLineStyle(9)
    potsMomProj_2.SetLineWidth(3)
    fluxMomProj_2.SetLineColor(1003)
    fluxMomProj_2.SetLineStyle(9)
    fluxMomProj_2.SetLineWidth(3)
    xsecMomProj_2.SetLineColor(1004)
    xsecMomProj_2.SetLineStyle(9)
    xsecMomProj_2.SetLineWidth(3)
    detsMomProj_2.SetLineColor(1005)
    detsMomProj_2.SetLineStyle(9)
    detsMomProj_2.SetLineWidth(3)
    flxsMomProj_2.SetLineColor(1006)
    flxsMomProj_2.SetLineStyle(9)
    flxsMomProj_2.SetLineWidth(3)
    fldeMomProj_2.SetLineColor(1007)
    fldeMomProj_2.SetLineStyle(9)
    fldeMomProj_2.SetLineWidth(3)

    noneThetaProj_1.SetLineColor(1001)
    noneThetaProj_1.SetLineStyle(1)
    noneThetaProj_1.SetLineWidth(3)
    potsThetaProj_1.SetLineColor(1002)
    potsThetaProj_1.SetLineStyle(1)
    potsThetaProj_1.SetLineWidth(3)
    fluxThetaProj_1.SetLineColor(1003)
    fluxThetaProj_1.SetLineStyle(1)
    fluxThetaProj_1.SetLineWidth(3)
    xsecThetaProj_1.SetLineColor(1004)
    xsecThetaProj_1.SetLineStyle(1)
    xsecThetaProj_1.SetLineWidth(3)
    detsThetaProj_1.SetLineColor(1005)
    detsThetaProj_1.SetLineStyle(1)
    detsThetaProj_1.SetLineWidth(3)
    flxsThetaProj_1.SetLineColor(1006)
    flxsThetaProj_1.SetLineStyle(1)
    flxsThetaProj_1.SetLineWidth(3)
    fldeThetaProj_1.SetLineColor(1007)
    fldeThetaProj_1.SetLineStyle(1)
    fldeThetaProj_1.SetLineWidth(3)

    noneThetaProj_2.SetLineColor(1001)
    noneThetaProj_2.SetLineStyle(9)
    noneThetaProj_2.SetLineWidth(3)
    potsThetaProj_2.SetLineColor(1002)
    potsThetaProj_2.SetLineStyle(9)
    potsThetaProj_2.SetLineWidth(3)
    fluxThetaProj_2.SetLineColor(1003)
    fluxThetaProj_2.SetLineStyle(9)
    fluxThetaProj_2.SetLineWidth(3)
    xsecThetaProj_2.SetLineColor(1004)
    xsecThetaProj_2.SetLineStyle(9)
    xsecThetaProj_2.SetLineWidth(3)
    detsThetaProj_2.SetLineColor(1005)
    detsThetaProj_2.SetLineStyle(9)
    detsThetaProj_2.SetLineWidth(3)
    flxsThetaProj_2.SetLineColor(1006)
    flxsThetaProj_2.SetLineStyle(9)
    flxsThetaProj_2.SetLineWidth(3)
    fldeThetaProj_2.SetLineColor(1007)
    fldeThetaProj_2.SetLineStyle(9)
    fldeThetaProj_2.SetLineWidth(3)

    # Set the data ratio to black
    dataMom_dataMom.SetLineColor(1)
    dataMom_dataMom.SetLineWidth(3)

    dataTheta_dataTheta.SetLineColor(1)
    dataTheta_dataTheta.SetLineWidth(3)

    # Set the prefit to blue
    prefMom_prefMom.SetLineColor(4)
    prefMom_prefMom.SetLineWidth(3)

    prefTheta_prefTheta.SetLineColor(4)
    prefTheta_prefTheta.SetLineWidth(3)

    # Set the postfit to red
    postMom_postMom.SetLineColor(2)
    postMom_postMom.SetLineWidth(3)

    postTheta_postTheta.SetLineColor(2)
    postTheta_postTheta.SetLineWidth(3)

    # Weight ratios have different colors
    noneMom_noneMom.SetLineColor(1001)
    noneMom_noneMom.SetLineWidth(3)
    potsMom_potsMom.SetLineColor(1002)
    potsMom_potsMom.SetLineWidth(3)
    fluxMom_fluxMom.SetLineColor(1003)
    fluxMom_fluxMom.SetLineWidth(3)
    xsecMom_xsecMom.SetLineColor(1004)
    xsecMom_xsecMom.SetLineWidth(3)
    detsMom_detsMom.SetLineColor(1005)
    detsMom_detsMom.SetLineWidth(3)
    flxsMom_flxsMom.SetLineColor(1006)
    flxsMom_flxsMom.SetLineWidth(3)
    fldeMom_fldeMom.SetLineColor(1007)
    fldeMom_fldeMom.SetLineWidth(3)

    noneTheta_noneTheta.SetLineColor(1001)
    noneTheta_noneTheta.SetLineWidth(3)
    potsTheta_potsTheta.SetLineColor(1002)
    potsTheta_potsTheta.SetLineWidth(3)
    fluxTheta_fluxTheta.SetLineColor(1003)
    fluxTheta_fluxTheta.SetLineWidth(3)
    xsecTheta_xsecTheta.SetLineColor(1004)
    xsecTheta_xsecTheta.SetLineWidth(3)
    detsTheta_detsTheta.SetLineColor(1005)
    detsTheta_detsTheta.SetLineWidth(3)
    flxsTheta_flxsTheta.SetLineColor(1006)
    flxsTheta_flxsTheta.SetLineWidth(3)
    fldeTheta_fldeTheta.SetLineColor(1007)
    fldeTheta_fldeTheta.SetLineWidth(3)

    # Set up maximum momenta for the different histogram types
    maxMom   = 2000.0
    minTheta = 0.6
    if (index > 5):
        maxMom   = 2000.0
        minTheta =  0.75

    # Do a stack of the momentum projections
    # Add histograms to stack
    mpStack  = THStack('mpStack', sampleNames_New[index] + ': ' + ';p_{#mu} (MeV/c);Events/(1 MeV/c)')
    mpStack.Add(dataMomProj_1, 'P0E')
    mpStack.Add(prefMomProj_1, 'HIST')
    mpStack.Add(postMomProj_1, 'HIST')
    mpStack.Add(dataMomProj_2, 'P0E')
    mpStack.Add(prefMomProj_2, 'HIST')
    mpStack.Add(postMomProj_2, 'HIST')

    cProj = TCanvas()
    cProj.cd()
    mpStack.Draw('NOSTACK')
    mpStack.GetXaxis().SetRangeUser(0, maxMom)

    cProj.Modified()
    cProj.Update()

    if (firstHisto):
        cProj0 = TCanvas()
        cProj0.cd()

        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Momentum Projections','C')
        leg.SetFillColor(0)
        #leg.SetHeader('1D Momentum Projections','C')
        #leg.AddEntry(dataMomProj_1, 'NoDetNomWeights Data'   , 'LEP')
        #leg.AddEntry(prefMomProj_1, 'NoDetNomWeights Prefit' , 'L')
        #leg.AddEntry(postMomProj_1, 'NoDetNomWeights Postfit', 'L')
        #leg.AddEntry(dataMomProj_2, 'With Det Weights Data'   , 'LEP')
        #leg.AddEntry(prefMomProj_2, 'With Det Weights Prefit' , 'L')
        #leg.AddEntry(postMomProj_2, 'With Det Weights Postfit', 'L')
        leg.AddEntry(dataMomProj_1, 'v3 Data'   , 'LEP')
        leg.AddEntry(prefMomProj_1, 'v3 Prefit' , 'L')
        leg.AddEntry(postMomProj_1, 'v3 Postfit', 'L')
        leg.AddEntry(dataMomProj_2, 'v1 Data'   , 'LEP')
        leg.AddEntry(prefMomProj_2, 'v1 Prefit' , 'L')
        leg.AddEntry(postMomProj_2, 'v1 Postfit', 'L')
        leg.Draw()

        cProj0.Print('momentumProjections.pdf(')
        cProj.Print('momentumProjections.pdf')
        cProj.Clear()
        firstHisto = False
    else:
        cProj.Print('momentumProjections.pdf')
        cProj.Clear()

    # Make stacks of the momentum ratios
    rmStack = THStack('rmStack', sampleNames_New[index] + ' Ratio: ' + ';p_{#mu} (MeV/c);Ratio')
    rmStack.Add(dataMom_dataMom, 'HIST')
    rmStack.Add(prefMom_prefMom, 'HIST')
    rmStack.Add(postMom_postMom, 'HIST')

    cProj = TCanvas()
    cProj.cd()
    rmStack.Draw('NOSTACK')
    rmStack.GetXaxis().SetRangeUser(0, maxMom)
    rmStack.SetMinimum(0.8)
    rmStack.SetMaximum(1.2)

    cProj.Modified()
    cProj.Update()

    if (firstRatio):
        cProj0 = TCanvas()
        cProj0.cd()

        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Momentum Ratios','C')
        leg.SetFillColor(0)
        #leg.SetHeader('1D Momentum Ratios','C')
        #leg.AddEntry(dataMom_dataMom, 'NoDetNomWeights Data / With Det Weights Data'      , 'L')
        #leg.AddEntry(prefMom_prefMom, 'NoDetNomWeights Prefit / With Det Weights Prefit'  , 'L')
        #leg.AddEntry(postMom_postMom, 'NoDetNomWeights Postfit / With Det Weights Postfit', 'L')
        leg.AddEntry(dataMom_dataMom, 'v3 Data / v1 Data'      , 'L')
        leg.AddEntry(prefMom_prefMom, 'v3 Prefit / v1 Prefit'  , 'L')
        leg.AddEntry(postMom_postMom, 'v3 Postfit / v1 Postfit', 'L')
        leg.Draw()

        cProj0.Print('momentumRatios.pdf(')
        cProj.Print('momentumRatios.pdf')
        cProj.Clear()
        firstRatio = False
    else:
        cProj.Print('momentumRatios.pdf')
        cProj.Clear()

    # Make stacks for the theta projections
    # Add histograms to stack
    tpStack = THStack('tpStack', sampleNames_New[index] + ': ' + ';cos#theta_{#mu};Events/(0.1)')
    tpStack.Add(dataThetaProj_1, 'P0E')
    tpStack.Add(prefThetaProj_1, 'HIST')
    tpStack.Add(postThetaProj_1, 'HIST')
    tpStack.Add(dataThetaProj_2, 'P0E')
    tpStack.Add(prefThetaProj_2, 'HIST')
    tpStack.Add(postThetaProj_2, 'HIST')

    cProj = TCanvas()
    cProj.cd()
    tpStack.Draw('NOSTACK')
    tpStack.GetXaxis().SetRangeUser(minTheta, 1)

    cProj.Modified()
    cProj.Update()

    if (firstHistot):
        cProj0 = TCanvas()
        cProj0.cd()

        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Theta Projections','C')
        leg.SetFillColor(0)
        #leg.SetHeader('1D Theta Projections','C')
        #leg.AddEntry(dataThetaProj_1, 'NoDetNomWeights Data'   , 'LEP')
        #leg.AddEntry(prefThetaProj_1, 'NoDetNomWeights Prefit' , 'L')
        #leg.AddEntry(postThetaProj_1, 'NoDetNomWeights Postfit', 'L')
        #leg.AddEntry(dataThetaProj_2, 'With Det Weights Data'   , 'LEP')
        #leg.AddEntry(prefThetaProj_2, 'With Det Weights Prefit' , 'L')
        #leg.AddEntry(postThetaProj_2, 'With Det Weights Postfit', 'L')
        leg.AddEntry(dataThetaProj_1, 'v3 Data'   , 'LEP')
        leg.AddEntry(prefThetaProj_1, 'v3 Prefit' , 'L')
        leg.AddEntry(postThetaProj_1, 'v3 Postfit', 'L')
        leg.AddEntry(dataThetaProj_2, 'v1 Data'   , 'LEP')
        leg.AddEntry(prefThetaProj_2, 'v1 Prefit' , 'L')
        leg.AddEntry(postThetaProj_2, 'v1 Postfit', 'L')
        leg.Draw()

        cProj0.Print('thetaProjections.pdf(')
        cProj.Print('thetaProjections.pdf')
        firstHistot = False
    else:
        cProj.Print('thetaProjections.pdf')

    # Make stacks of the theta ratios
    rtStack = THStack('rtStack', sampleNames_New[index] + ' Ratio: ' + ';cos#theta_{#mu};Ratio')
    rtStack.Add(dataTheta_dataTheta, 'HIST')
    rtStack.Add(prefTheta_prefTheta, 'HIST')
    rtStack.Add(postTheta_postTheta, 'HIST')

    cProj = TCanvas()
    cProj.cd()
    rtStack.Draw('NOSTACK')
    rtStack.GetXaxis().SetRangeUser(minTheta, 1)
    rtStack.SetMinimum(0.8)
    rtStack.SetMaximum(1.2)

    cProj.Modified()
    cProj.Update()

    if (firstRatiot):
        cProj0 = TCanvas()
        cProj0.cd()

        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Theta Ratios','C')
        leg.SetFillColor(0)
        #leg.SetHeader('1D Theta Ratios','C')
        #leg.AddEntry(dataTheta_dataTheta, 'NoDetNomWeights Data / With Det Weights Data'      , 'L')
        #leg.AddEntry(prefTheta_prefTheta, 'NoDetNomWeights Prefit / With Det Weights Prefit'  , 'L')
        #leg.AddEntry(postTheta_postTheta, 'NoDetNomWeights Postfit / With Det Weights Postfit', 'L')
        leg.AddEntry(dataTheta_dataTheta, 'v3 Data / v1 Data'      , 'L')
        leg.AddEntry(prefTheta_prefTheta, 'v3 Prefit / v1 Prefit'  , 'L')
        leg.AddEntry(postTheta_postTheta, 'v3 Postfit / v3 Postfit', 'L')
        leg.Draw()
        
        cProj0.Print('thetaRatios.pdf(')
        cProj.Print('thetaRatios.pdf')
        cProj.Clear()
        firstRatiot = False
    else:
        cProj.Print('thetaRatios.pdf')
        cProj.Clear()

    # Make a stack of the weight momentum projections
    wpStack = THStack('wpStack', sampleNames_New[index] + ': ' + ';p_{#mu} (MeV/c);Events/(1 MeV/c)')
    wpStack.Add(potsMomProj_1, 'HIST')
    wpStack.Add(fluxMomProj_1, 'HIST')
    wpStack.Add(xsecMomProj_1, 'HIST')
    wpStack.Add(detsMomProj_1, 'HIST')
    wpStack.Add(flxsMomProj_1, 'HIST')
    wpStack.Add(fldeMomProj_1, 'HIST')
    wpStack.Add(potsMomProj_2, 'HIST')
    wpStack.Add(fluxMomProj_2, 'HIST')
    wpStack.Add(xsecMomProj_2, 'HIST')
    wpStack.Add(detsMomProj_2, 'HIST')
    wpStack.Add(flxsMomProj_2, 'HIST')
    wpStack.Add(fldeMomProj_2, 'HIST')

    cProj = TCanvas()
    cProj.cd()
    wpStack.Draw('NOSTACK')
    wpStack.GetXaxis().SetRangeUser(0, maxMom)

    cProj.Modified()
    cProj.Update()

    if (firstWeight):
        cProj0 = TCanvas()
        cProj0.cd()

        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Weight Momentum Projections','C')
        leg.SetFillColor(0)
        #leg.SetHeader('1D Weight Momentum Projections','C')
        #leg.AddEntry(potsMomProj_1, 'NoDetNomWeights POT'          , 'L')
        #leg.AddEntry(fluxMomProj_1, 'NoDetNomWeights POT+flux'     , 'L')
        #leg.AddEntry(xsecMomProj_1, 'NoDetNomWeights POT+xsec'     , 'L')
        #leg.AddEntry(detsMomProj_1, 'NoDetNomWeights POT+det'      , 'L')
        #leg.AddEntry(flxsMomProj_1, 'NoDetNomWeights POT+flux+xsec', 'L')
        #leg.AddEntry(fldeMomProj_1, 'NoDetNomWeights POT+flux+det' , 'L')
        #leg.AddEntry(potsMomProj_2, 'With Det Weights POT'          , 'L')
        #leg.AddEntry(fluxMomProj_2, 'With Det Weights POT+flux'     , 'L')
        #leg.AddEntry(xsecMomProj_2, 'With Det Weights POT+xsec'     , 'L')
        #leg.AddEntry(detsMomProj_2, 'With Det Weights POT+det'      , 'L')
        #leg.AddEntry(flxsMomProj_2, 'With Det Weights POT+flux+xsec', 'L')
        #leg.AddEntry(fldeMomProj_2, 'With Det Weights POT+flux+det' , 'L')
        leg.AddEntry(potsMomProj_1, 'v3 POT'          , 'L')
        leg.AddEntry(fluxMomProj_1, 'v3 POT+flux'     , 'L')
        leg.AddEntry(xsecMomProj_1, 'v3 POT+xsec'     , 'L')
        leg.AddEntry(detsMomProj_1, 'v3 POT+det'      , 'L')
        leg.AddEntry(flxsMomProj_1, 'v3 POT+flux+xsec', 'L')
        leg.AddEntry(fldeMomProj_1, 'v3 POT+flux+det' , 'L')
        leg.AddEntry(potsMomProj_2, 'v1 POT'          , 'L')
        leg.AddEntry(fluxMomProj_2, 'v1 POT+flux'     , 'L')
        leg.AddEntry(xsecMomProj_2, 'v1 POT+xsec'     , 'L')
        leg.AddEntry(detsMomProj_2, 'v1 POT+det'      , 'L')
        leg.AddEntry(flxsMomProj_2, 'v1 POT+flux+xsec', 'L')
        leg.AddEntry(fldeMomProj_2, 'v1 POT+flux+det' , 'L')
        leg.Draw()

        cProj0.Print('weightMomentum.pdf(')
        cProj.Print('weightMomentum.pdf')
        cProj.Clear()
        firstWeight = False
    else:
        cProj.Print('weightMomentum.pdf')
        cProj.Clear()

    # Make a stack of the weight theta projections
    wtStack = THStack('wtStack', sampleNames_New[index] + ': ' + ';cos#theta_{#mu};Events/0.01')
    wtStack.Add(potsThetaProj_1, 'HIST')
    wtStack.Add(fluxThetaProj_1, 'HIST')
    wtStack.Add(xsecThetaProj_1, 'HIST')
    wtStack.Add(detsThetaProj_1, 'HIST')
    wtStack.Add(flxsThetaProj_1, 'HIST')
    wtStack.Add(fldeThetaProj_1, 'HIST')
    wtStack.Add(potsThetaProj_2, 'HIST')
    wtStack.Add(fluxThetaProj_2, 'HIST')
    wtStack.Add(xsecThetaProj_2, 'HIST')
    wtStack.Add(detsThetaProj_2, 'HIST')
    wtStack.Add(flxsThetaProj_2, 'HIST')
    wtStack.Add(fldeThetaProj_2, 'HIST')

    cProj = TCanvas()
    cProj.cd()
    wtStack.Draw('NOSTACK')
    wtStack.GetXaxis().SetRangeUser(minTheta, 1.0)

    cProj.Modified()
    cProj.Update()

    if (firstWeightt):
        cProj0 = TCanvas()
        cProj0.cd()

        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Weight Theta Projections','C')
        leg.SetFillColor(0)
        #leg.SetHeader('1D Weight Theta Projections','C')
        #leg.AddEntry(potsThetaProj_1, 'NoDetNomWeights POT'          , 'L')
        #leg.AddEntry(fluxThetaProj_1, 'NoDetNomWeights POT+flux'     , 'L')
        #leg.AddEntry(xsecThetaProj_1, 'NoDetNomWeights POT+xsec'     , 'L')
        #leg.AddEntry(detsThetaProj_1, 'NoDetNomWeights POT+det'      , 'L')
        #leg.AddEntry(flxsThetaProj_1, 'NoDetNomWeights POT+flux+xsec', 'L')
        #leg.AddEntry(fldeThetaProj_1, 'NoDetNomWeights POT+flux+det' , 'L')
        #leg.AddEntry(potsThetaProj_2, 'With Det Weights POT'          , 'L')
        #leg.AddEntry(fluxThetaProj_2, 'With Det Weights POT+flux'     , 'L')
        #leg.AddEntry(xsecThetaProj_2, 'With Det Weights POT+xsec'     , 'L')
        #leg.AddEntry(detsThetaProj_2, 'With Det Weights POT+det'      , 'L')
        #leg.AddEntry(flxsThetaProj_2, 'With Det Weights POT+flux+xsec', 'L')
        #leg.AddEntry(fldeThetaProj_2, 'With Det Weights POT+flux+det' , 'L')
        leg.AddEntry(potsThetaProj_1, 'v3 POT'          , 'L')
        leg.AddEntry(fluxThetaProj_1, 'v3 POT+flux'     , 'L')
        leg.AddEntry(xsecThetaProj_1, 'v3 POT+xsec'     , 'L')
        leg.AddEntry(detsThetaProj_1, 'v3 POT+det'      , 'L')
        leg.AddEntry(flxsThetaProj_1, 'v3 POT+flux+xsec', 'L')
        leg.AddEntry(fldeThetaProj_1, 'v3 POT+flux+det' , 'L')
        leg.AddEntry(potsThetaProj_2, 'v1 POT'          , 'L')
        leg.AddEntry(fluxThetaProj_2, 'v1 POT+flux'     , 'L')
        leg.AddEntry(xsecThetaProj_2, 'v1 POT+xsec'     , 'L')
        leg.AddEntry(detsThetaProj_2, 'v1 POT+det'      , 'L')
        leg.AddEntry(flxsThetaProj_2, 'v1 POT+flux+xsec', 'L')
        leg.AddEntry(fldeThetaProj_2, 'v1 POT+flux+det' , 'L')
        leg.Draw()

        cProj0.Print('weightTheta.pdf(')
        cProj.Print('weightTheta.pdf')
        cProj.Clear()
        firstWeightt = False
    else:
        cProj.Print('weightTheta.pdf')
        cProj.Clear()

    # Make a stack of the weight momentum ratios
    rpStack = THStack('rpStack', sampleNames_New[index] + ' Ratio: ' + ';p_{#mu} (MeV/c);Events/(1 MeV/c)')
    rpStack.Add(potsMom_potsMom, 'HIST')
    rpStack.Add(fluxMom_fluxMom, 'HIST')
    rpStack.Add(xsecMom_xsecMom, 'HIST')
    rpStack.Add(detsMom_detsMom, 'HIST')
    rpStack.Add(flxsMom_flxsMom, 'HIST')
    rpStack.Add(fldeMom_fldeMom, 'HIST')

    cProj = TCanvas()
    cProj.cd()
    rpStack.Draw('NOSTACK')
    rpStack.GetXaxis().SetRangeUser(0, maxMom)
    rpStack.SetMinimum(0.8)
    rpStack.SetMaximum(1.2)

    cProj.Modified()
    cProj.Update()

    if (firstRatiop):
        cProj0 = TCanvas()
        cProj0.cd()

        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Weight Momentum Ratios','C')
        leg.SetFillColor(0)
        #leg.SetHeader('1D Weight Momentum Ratios','C')
        #leg.AddEntry(potsMom_potsMom, 'NoDetNomWeights POT / With Det Weights POT'                     , 'L')
        #leg.AddEntry(fluxMom_fluxMom, 'NoDetNomWeights POT+flux / With Det Weights POT+flux'           , 'L')
        #leg.AddEntry(xsecMom_xsecMom, 'NoDetNomWeights POT+xsec / With Det Weights POT+xsec'           , 'L')
        #leg.AddEntry(detsMom_detsMom, 'NoDetNomWeights POT+det / With Det Weights POT+det'             , 'L')
        #leg.AddEntry(flxsMom_flxsMom, 'NoDetNomWeights POT+flux+xsec / With Det Weights POT+flux+xsec' , 'L')
        #leg.AddEntry(fldeMom_fldeMom, 'NoDetNomWeights POT+flux+det / With Det Weights POT+flux+det'   , 'L')
        leg.AddEntry(potsMom_potsMom, 'v3 POT / v1 POT'                     , 'L')
        leg.AddEntry(fluxMom_fluxMom, 'v3 POT+flux / v1 POT+flux'           , 'L')
        leg.AddEntry(xsecMom_xsecMom, 'v3 POT+xsec / v1 POT+xsec'           , 'L')
        leg.AddEntry(detsMom_detsMom, 'v3 POT+det / v1 POT+det'             , 'L')
        leg.AddEntry(flxsMom_flxsMom, 'v3 POT+flux+xsec / v1 POT+flux+xsec' , 'L')
        leg.AddEntry(fldeMom_fldeMom, 'v3 POT+flux+det / v1 POT+flux+det'   , 'L')
        leg.Draw()

        cProj0.Print('weightMomentumRatios.pdf(')
        cProj.Print('weightMomentumRatios.pdf')
        cProj.Clear()
        firstRatiop = False
    else:
        cProj.Print('weightMomentumRatios.pdf')
        cProj.Clear()

    # Make a stack of the weight theta ratios
    rcStack = THStack('rcStack', sampleNames_New[index] + ' Ratio: ' + ';cos#theta_{#mu};Events/0.01')
    rcStack.Add(potsTheta_potsTheta, 'HIST')
    rcStack.Add(fluxTheta_fluxTheta, 'HIST')
    rcStack.Add(xsecTheta_xsecTheta, 'HIST')
    rcStack.Add(detsTheta_detsTheta, 'HIST')
    rcStack.Add(flxsTheta_flxsTheta, 'HIST')
    rcStack.Add(fldeTheta_fldeTheta, 'HIST')

    cProj = TCanvas()
    cProj.cd()
    rcStack.Draw('NOSTACK')
    rcStack.GetXaxis().SetRangeUser(minTheta, 1)
    rcStack.SetMinimum(0.8)
    rcStack.SetMaximum(1.2)

    cProj.Modified()
    cProj.Update()

    if (firstRatioc):
        cProj0 = TCanvas()
        cProj0.cd()

        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Weight Theta Ratios','C')
        leg.SetFillColor(0)
        #leg.SetHeader('1D Weight Theta Ratios','C')
        #leg.AddEntry(potsTheta_potsTheta, 'NoDetNomWeights POT / With Det Weights POT'                     , 'L')
        #leg.AddEntry(fluxTheta_fluxTheta, 'NoDetNomWeights POT+flux / With Det Weights POT+flux'           , 'L')
        #leg.AddEntry(xsecTheta_xsecTheta, 'NoDetNomWeights POT+xsec / With Det Weights POT+xsec'           , 'L')
        #leg.AddEntry(detsTheta_detsTheta, 'NoDetNomWeights POT+det / With Det Weights POT+det'             , 'L')
        #leg.AddEntry(flxsTheta_flxsTheta, 'NoDetNomWeights POT+flux+xsec / With Det Weights POT+flux+xsec' , 'L')
        #leg.AddEntry(fldeTheta_fldeTheta, 'NoDetNomWeights POT+flux+det / With Det Weights POT+flux+det'   , 'L')
        leg.AddEntry(potsTheta_potsTheta, 'v3 POT / v1 POT'                     , 'L')
        leg.AddEntry(fluxTheta_fluxTheta, 'v3 POT+flux / v1 POT+flux'           , 'L')
        leg.AddEntry(xsecTheta_xsecTheta, 'v3 POT+xsec / v1 POT+xsec'           , 'L')
        leg.AddEntry(detsTheta_detsTheta, 'v3 POT+det / v1 POT+det'             , 'L')
        leg.AddEntry(flxsTheta_flxsTheta, 'v3 POT+flux+xsec / v1 POT+flux+xsec' , 'L')
        leg.AddEntry(fldeTheta_fldeTheta, 'v3 POT+flux+det / v1 POT+flux+det'   , 'L')
        leg.Draw()

        cProj0.Print('weightThetaRatios.pdf(')
        cProj.Print('weightThetaRatios.pdf')
        cProj.Clear()
        firstRatioc = False
    else:
        cProj.Print('weightThetaRatios.pdf')
        cProj.Clear()

c1 = TCanvas()
c1.cd()
c1.Print('momentumProjections.pdf]')
c1.Print('thetaProjections.pdf]')
c1.Print('momentumRatios.pdf]')
c1.Print('thetaRatios.pdf]')
c1.Print('weightMomentum.pdf]')
c1.Print('weightTheta.pdf]')
c1.Print('weightMomentumRatios.pdf]')
c1.Print('weightThetaRatios.pdf]')
