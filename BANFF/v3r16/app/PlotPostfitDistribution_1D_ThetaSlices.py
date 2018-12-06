#!/usr/bin/python
import os
import sys
import math
from ROOT import *
from array import array

'''
Makes plots for comparing 1D kinematic distributions in slices of theta
Current setup is for comparing Psyche v1 and v3, but should be an easy
    adjustment for doing just one or the other
Usage:
    python PlotPostfitDistribution_1D_ThetaSlices.py inputFile1 inputFile2
'''

if (len(sys.argv) != 3):
    print 'Usage:\n'
    print 'PlotPostfitDistribution_1D_ThetaSlices.py inputFile1 inputFile2'
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

inputFile1 = TFile(sys.argv[1])
inputFile2 = TFile(sys.argv[2])

for index in xrange(0,len(sampleNames_New)):
    # Retrieve plots for first input file
    dataForSlices_1 = (inputFile1.Get(sampleNames_New[index] + '_data')).Projection(1,0)
    dataForSlices_1.SetDirectory(0)

    prefForSlices_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit')).Projection(1,0)
    prefForSlices_1.SetDirectory(0)

    postForSlices_1 = (inputFile1.Get(sampleNames_New[index] + '_prefit')).Projection(1,0)
    postForSlices_1.SetDirectory(0)

    #postForSlices_1 = (inputFile1.Get(sampleNames_New[index] + '_postfit_0_0')).Projection(1,0)
    #postForSlices_1.SetDirectory(0)

    # Retrieve plots for second input file
    dataForSlices_2 = (inputFile2.Get(sampleNames_Old[index] + '_data')).Projection(1,0)
    dataForSlices_2.SetDirectory(0)

    prefForSlices_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit')).Projection(1,0)
    prefForSlices_2.SetDirectory(0)

    postForSlices_2 = (inputFile2.Get(sampleNames_Old[index] + '_prefit')).Projection(1,0)
    postForSlices_2.SetDirectory(0)

    #postForSlices_2 = (inputFile2.Get(sampleNames_Old[index] + '_postfit_0_0')).Projection(1,0)
    #postForSlices_2.SetDirectory(0)

    # Make lists for the cos(theta) slice histograms
    dataSlices_1 = []
    prefSlices_1 = []
    postSlices_1 = []
    dataSlices_2 = []
    prefSlices_2 = []
    postSlices_2 = []

    for iy in range(1, dataForSlices_1.GetNbinsY() + 1):
        thetaLow  = dataForSlices_1.GetYaxis().GetBinLowEdge(iy)
        thetaHigh = dataForSlices_1.GetYaxis().GetBinUpEdge(iy)

        thetaString = str(thetaLow) + ' < cos#theta < ' + str(thetaHigh)

        dataSlices_1.append(TH1D('dataSlice' + sampleNames_New[index] + str(iy),
                                 thetaString, dataForSlices_1.GetXaxis().GetNbins(),
                                 dataForSlices_1.GetXaxis().GetXbins().GetArray()))
        prefSlices_1.append(TH1D('prefSlice' + sampleNames_New[index] + str(iy),
                                 thetaString, prefForSlices_1.GetXaxis().GetNbins(),
                                 prefForSlices_1.GetXaxis().GetXbins().GetArray()))
        postSlices_1.append(TH1D('postSlice' + sampleNames_New[index] + str(iy),
                               thetaString, postForSlices_1.GetXaxis().GetNbins(),
                               postForSlices_1.GetXaxis().GetXbins().GetArray()))

        dataSlices_2.append(TH1D('dataSlice' + sampleNames_Old[index] + str(iy),
                                 thetaString, dataForSlices_2.GetXaxis().GetNbins(),
                                 dataForSlices_2.GetXaxis().GetXbins().GetArray()))
        prefSlices_2.append(TH1D('prefSlice' + sampleNames_Old[index] + str(iy),
                                 thetaString, prefForSlices_2.GetXaxis().GetNbins(),
                                 prefForSlices_2.GetXaxis().GetXbins().GetArray()))
        postSlices_2.append(TH1D('postSlice' + sampleNames_Old[index] + str(iy),
                               thetaString, postForSlices_2.GetXaxis().GetNbins(),
                               postForSlices_2.GetXaxis().GetXbins().GetArray()))

    # Scale projections by momentum bin width
    for ix in range(1, dataForSlices_1.GetNbinsX() + 1):
        for iy in range(1, dataForSlices_1.GetNbinsY() + 1):
            pfxdim = (dataForSlices_1.GetXaxis().GetBinUpEdge(ix) - 
                      dataForSlices_1.GetXaxis().GetBinLowEdge(ix))

            #dataForSlices_1.SetBinContent(ix, iy, dataForSlices_1.GetBinContent(ix,iy) / pfxdim)
            #dataForSlices_1.SetBinError(ix, iy, sqrt(dataForSlices_1.GetBinContent(ix,iy) / pfxdim))

            dataSlices_1[iy-1].SetBinContent(ix, dataForSlices_1.GetBinContent(ix,iy))
            dataSlices_1[iy-1].SetBinError(ix, dataForSlices_1.GetBinError(ix,iy))

            #prefForSlices_1.SetBinContent(ix, iy, prefForSlices_1.GetBinContent(ix,iy) / pfxdim)
            prefSlices_1[iy-1].SetBinContent(ix, prefForSlices_1.GetBinContent(ix,iy))

            #postForSlices_1.SetBinContent(ix, iy, postForSlices_1.GetBinContent(ix,iy) / pfxdim)
            postSlices_1[iy-1].SetBinContent(ix, postForSlices_1.GetBinContent(ix,iy))

            #dataForSlices_2.SetBinContent(ix, iy, dataForSlices_2.GetBinContent(ix,iy) / pfxdim)
            #dataForSlices_2.SetBinError(ix, iy, sqrt(dataForSlices_2.GetBinContent(ix,iy) / pfxdim))

            dataSlices_2[iy-1].SetBinContent(ix, dataForSlices_2.GetBinContent(ix,iy))
            dataSlices_2[iy-1].SetBinError(ix, dataForSlices_2.GetBinError(ix,iy))

            #prefForSlices_2.SetBinContent(ix, iy, prefForSlices_2.GetBinContent(ix,iy) / pfxdim)
            prefSlices_2[iy-1].SetBinContent(ix, prefForSlices_2.GetBinContent(ix,iy))

            #postForSlices_2.SetBinContent(ix, iy, postForSlices_2.GetBinContent(ix,iy) / pfxdim)
            postSlices_2[iy-1].SetBinContent(ix, postForSlices_2.GetBinContent(ix,iy))

    # Set color palette for the ratio plots
    NRGBs = 6
    NCont = 99
    stops = [ 0.00, 0.02, 0.34, 0.51, 0.64, 1.00 ]
    red   = [ 1.00, 0.00, 0.00, 0.87, 1.00, 0.51 ]
    green = [ 1.00, 0.00, 0.81, 1.00, 0.20, 0.00 ]
    blue  = [ 1.00, 0.51, 1.00, 0.12, 0.00, 0.00 ]
    stopsArray = array('d', stops)
    redArray   = array('d', red)
    greenArray = array('d', green)
    blueArray  = array('d', blue)
    TColor.CreateGradientColorTable(NRGBs, stopsArray, redArray, greenArray, blueArray, NCont)
    gStyle.SetNumberContours(NCont)

    # Set up maximum momenta for the different histogram types
    maxMom   = 2000.0
    minTheta = 0.6
    if (index > 5):
        maxMom   = 2000.0
        minTheta =  0.75

    stackSlices = []
    for si in range(0, len(dataSlices_1)):
        stackSlices.append(THStack('stackSlices',
                                   sampleNames_New[index] + ': ' + dataSlices_1[si].GetTitle() +
                                   ';p_{#mu} (MeV/c);Events'))
                                   #';p_{#mu} (MeV/c);Events / (1 MeV/c)'))

        # Data is black
        dataSlices_1[si].SetMarkerStyle(20)
        dataSlices_1[si].SetMarkerColor(1)
        dataSlices_1[si].SetLineColor(1)
        dataSlices_1[si].SetLineWidth(3)
        dataSlices_2[si].SetMarkerStyle(21)
        dataSlices_2[si].SetMarkerColor(1)
        dataSlices_2[si].SetLineColor(1)
        dataSlices_2[si].SetLineWidth(3)

        # Prefit is blue
        prefSlices_1[si].SetLineColor(4)
        prefSlices_1[si].SetLineStyle(1)
        prefSlices_1[si].SetLineWidth(3)
        prefSlices_2[si].SetLineColor(4)
        prefSlices_2[si].SetLineStyle(2)
        prefSlices_2[si].SetLineWidth(3)

        # Postfit is red
        postSlices_1[si].SetLineColor(2)
        postSlices_1[si].SetLineStyle(1)
        postSlices_1[si].SetLineWidth(3)
        postSlices_2[si].SetLineColor(2)
        postSlices_2[si].SetLineStyle(2)
        postSlices_2[si].SetLineWidth(3)

        # Add slices to THStack
        #stackSlices[si].Add(dataSlices_1[si], 'P0E')
        stackSlices[si].Add(prefSlices_1[si], 'HIST')
        #stackSlices[si].Add(postSlices_1[si], 'HIST')
        #stackSlices[si].Add(dataSlices_2[si], 'P0E')
        stackSlices[si].Add(prefSlices_2[si], 'HIST')
        #stackSlices[si].Add(postSlices_2[si], 'HIST')

        c1 = TCanvas()
        c1.cd()
        stackSlices[si].Draw('NOSTACK')
        leg = TLegend(0.68, 0.70, 0.95, 0.95)
        leg.SetFillColor(0)
        #leg.AddEntry(dataSlices_1[si], 'v3 Data', 'LEP')
        leg.AddEntry(prefSlices_1[si], 'v3 Prefit', 'L')
        #leg.AddEntry(postSlices_1[si], 'v3 Postfit', 'L')
        #leg.AddEntry(dataSlices_2[si], 'v1 Data', 'LEP')
        leg.AddEntry(prefSlices_2[si], 'v1 Prefit', 'L')
        #leg.AddEntry(postSlices_2[si], 'v1 Postfit', 'L')
        leg.Draw()
        stackSlices[si].GetXaxis().SetRangeUser(0, maxMom)
        
        if(firstHisto):
            c1.Print('thetaSlices.pdf(')
            firstHisto = False
        else:
            c1.Print('thetaSlices.pdf')

c0 = TCanvas()
c0.cd()
c0.Print('thetaSlices.pdf]')
    
    
#
#
#    # Do a stack of the momentum projections
#    # Add histograms to stack
#    mpStack  = THStack('mpStack', sampleNames_New[index] + ': ' + ';p_{#mu} (MeV/c);Events/(1 MeV/c)')
#    mpStack.Add(dataMomProj_1, 'P0E')
#    mpStack.Add(prefMomProj_1, 'HIST')
#    mpStack.Add(postMomProj_1, 'HIST')
#    mpStack.Add(dataMomProj_2, 'P0E')
#    mpStack.Add(prefMomProj_2, 'HIST')
#    mpStack.Add(postMomProj_2, 'HIST')
#
#    cProj = TCanvas()
#    cProj.cd()
#    mpStack.Draw('NOSTACK')
#    mpStack.GetXaxis().SetRangeUser(0, maxMom)
#
#    cProj.Modified()
#    cProj.Update()
#
#    if (firstHisto):
#        cProj0 = TCanvas()
#        cProj0.cd()
#
#        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Momentum Projections','C')
#        leg.SetFillColor(0)
#        leg.AddEntry(dataMomProj_1, 'v3 Data'   , 'LEP')
#        leg.AddEntry(prefMomProj_1, 'v3 Prefit' , 'L')
#        leg.AddEntry(postMomProj_1, 'v3 Postfit', 'L')
#        leg.AddEntry(dataMomProj_2, 'v1 Data'   , 'LEP')
#        leg.AddEntry(prefMomProj_2, 'v1 Prefit' , 'L')
#        leg.AddEntry(postMomProj_2, 'v1 Postfit', 'L')
#        leg.Draw()
#
#        cProj0.Print('momentumProjections.pdf(')
#        cProj.Print('momentumProjections.pdf')
#        cProj.Clear()
#        firstHisto = False
#    else:
#        cProj.Print('momentumProjections.pdf')
#        cProj.Clear()
#
#    # Make stacks of the momentum ratios
#    rmStack = THStack('rmStack', sampleNames_New[index] + ' Ratio: ' + ';p_{#mu} (MeV/c);Ratio')
#    rmStack.Add(dataMom_dataMom, 'HIST')
#    rmStack.Add(prefMom_prefMom, 'HIST')
#    rmStack.Add(postMom_postMom, 'HIST')
#
#    cProj = TCanvas()
#    cProj.cd()
#    rmStack.Draw('NOSTACK')
#    rmStack.GetXaxis().SetRangeUser(0, maxMom)
#    rmStack.SetMinimum(0.8)
#    rmStack.SetMaximum(1.2)
#
#    cProj.Modified()
#    cProj.Update()
#
#    if (firstRatio):
#        cProj0 = TCanvas()
#        cProj0.cd()
#
#        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Momentum Ratios','C')
#        leg.SetFillColor(0)
#        leg.AddEntry(dataMom_dataMom, 'v3 Data / v1 Data'      , 'L')
#        leg.AddEntry(prefMom_prefMom, 'v3 Prefit / v1 Prefit'  , 'L')
#        leg.AddEntry(postMom_postMom, 'v3 Postfit / v1 Postfit', 'L')
#        leg.Draw()
#
#        cProj0.Print('momentumRatios.pdf(')
#        cProj.Print('momentumRatios.pdf')
#        cProj.Clear()
#        firstRatio = False
#    else:
#        cProj.Print('momentumRatios.pdf')
#        cProj.Clear()
#
#    # Make stacks for the theta projections
#    # Add histograms to stack
#    tpStack = THStack('tpStack', sampleNames_New[index] + ': ' + ';cos#theta_{#mu};Events/(0.1)')
#    tpStack.Add(dataThetaProj_1, 'P0E')
#    tpStack.Add(prefThetaProj_1, 'HIST')
#    tpStack.Add(postThetaProj_1, 'HIST')
#    tpStack.Add(dataThetaProj_2, 'P0E')
#    tpStack.Add(prefThetaProj_2, 'HIST')
#    tpStack.Add(postThetaProj_2, 'HIST')
#
#    cProj = TCanvas()
#    cProj.cd()
#    tpStack.Draw('NOSTACK')
#    tpStack.GetXaxis().SetRangeUser(minTheta, 1)
#
#    cProj.Modified()
#    cProj.Update()
#
#    if (firstHistot):
#        cProj0 = TCanvas()
#        cProj0.cd()
#
#        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Theta Projections','C')
#        leg.SetFillColor(0)
#        leg.AddEntry(dataThetaProj_1, 'v3 Data'   , 'LEP')
#        leg.AddEntry(prefThetaProj_1, 'v3 Prefit' , 'L')
#        leg.AddEntry(postThetaProj_1, 'v3 Postfit', 'L')
#        leg.AddEntry(dataThetaProj_2, 'v1 Data'   , 'LEP')
#        leg.AddEntry(prefThetaProj_2, 'v1 Prefit' , 'L')
#        leg.AddEntry(postThetaProj_2, 'v1 Postfit', 'L')
#        leg.Draw()
#
#        cProj0.Print('thetaProjections.pdf(')
#        cProj.Print('thetaProjections.pdf')
#        firstHistot = False
#    else:
#        cProj.Print('thetaProjections.pdf')
#
#    # Make stacks of the theta ratios
#    rtStack = THStack('rtStack', sampleNames_New[index] + ' Ratio: ' + ';cos#theta_{#mu};Ratio')
#    rtStack.Add(dataTheta_dataTheta, 'HIST')
#    rtStack.Add(prefTheta_prefTheta, 'HIST')
#    rtStack.Add(postTheta_postTheta, 'HIST')
#
#    cProj = TCanvas()
#    cProj.cd()
#    rtStack.Draw('NOSTACK')
#    rtStack.GetXaxis().SetRangeUser(minTheta, 1)
#    rtStack.SetMinimum(0.8)
#    rtStack.SetMaximum(1.2)
#
#    cProj.Modified()
#    cProj.Update()
#
#    if (firstRatiot):
#        cProj0 = TCanvas()
#        cProj0.cd()
#
#        leg = TLegend(0.05, 0.05, 0.95, 0.95,'1D Theta Ratios','C')
#        leg.SetFillColor(0)
#        leg.AddEntry(dataTheta_dataTheta, 'v3 Data / v1 Data'      , 'L')
#        leg.AddEntry(prefTheta_prefTheta, 'v3 Prefit / v1 Prefit'  , 'L')
#        leg.AddEntry(postTheta_postTheta, 'v3 Postfit / v3 Postfit', 'L')
#        leg.Draw()
#        
#        cProj0.Print('thetaRatios.pdf(')
#        cProj.Print('thetaRatios.pdf')
#        cProj.Clear()
#        firstRatiot = False
#    else:
#        cProj.Print('thetaRatios.pdf')
#        cProj.Clear()
#
#c1 = TCanvas()
#c1.cd()
#c1.Print('momentumProjections.pdf]')
#c1.Print('thetaProjections.pdf]')
#c1.Print('momentumRatios.pdf]')
#c1.Print('thetaRatios.pdf]')
