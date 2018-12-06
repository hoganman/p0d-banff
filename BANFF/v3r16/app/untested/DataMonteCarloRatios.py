#!/usr/bin/python
from ROOT import *
import sys
import math
import os

gROOT.SetBatch(kTRUE);
gROOT.ProcessLine(".L macro/SetT2KStyle.cxx");
SetT2KStyle(1)
#Loop through the plots associated with the fit and produce:
#data/nominal MC, data/tunedMC(i.e. fit result) plots
#Make these TH2Ds.
sampleNames = ["fgd1cc0pi","fgd1cc1pi","fgd1ccOth","fgd1anucc1tr","fgd1anuccntr","fgd1nucc1tr","fgd1nuccntr","fgd2cc0pi","fgd2cc1pi","fgd2ccOth","fgd2anucc1tr","fgd2anuccntr","fgd2nucc1tr","fgd2nuccntr"]
dataPreFitRatio = []
dataPostFitRatio = []
gStyle.SetErrorX(0.0000001)
inputFile = TFile(sys.argv[1])

firstHisto = True
firstPrefit = True
firstPostfit = True
firstRatio = True

os.mkdir(sys.argv[2])
os.chdir(sys.argv[2])

outputFile = TFile(sys.argv[3],"RECREATE")

for i in xrange(0,len(sampleNames)):

    dataMomProj = (inputFile.Get(sampleNames[i] + "_data")).Projection(0) 
    dataMomProj.SetDirectory(0)
    
    prefitMomProj = (inputFile.Get(sampleNames[i] + "_prefit")).Projection(0)
    prefitMomProj.SetDirectory(0)

    postfitMomProj = (inputFile.Get(sampleNames[i] + "_postfit_0_0")).Projection(0)
    postfitMomProj.SetDirectory(0)

    data1 = (inputFile.Get(sampleNames[i] + "_data")).Projection(1,0)
    data1.SetDirectory(0)
    
    prefit = (inputFile.Get(sampleNames[i] + "_prefit")).Projection(1,0)
    prefit.SetDirectory(0)
    

    prefit.Scale(data1.Integral()/prefit.Integral())
    data1.Divide(prefit)

    data2 = (inputFile.Get(sampleNames[i] + "_data")).Projection(1,0)
    data2.SetDirectory(0)
    
    postfit = (inputFile.Get(sampleNames[i] + "_postfit_0_0")).Projection(1,0)
    postfit.SetDirectory(0)
    
    data2.Divide(postfit)

    dataForResiduals = (inputFile.Get(sampleNames[i] + "_data")).Projection(1,0)
    dataForResiduals.SetDirectory(0)

    residuals = dataForResiduals.Clone()
    prefitResiduals = dataForResiduals.Clone()

    prefitForResiduals = (inputFile.Get(sampleNames[i] + "_prefit")).Projection(1,0)
    prefitForResiduals.SetDirectory(0)


    postfitForResiduals = (inputFile.Get(sampleNames[i] + "_postfit_0_0")).Projection(1,0)
    postfitForResiduals.SetDirectory(0)

    dataForAreaNorm = (inputFile.Get(sampleNames[i] + "_data")).Projection(1,0)
    dataForAreaNorm.SetDirectory(0)
    
    prefitForAreaNorm  = (inputFile.Get(sampleNames[i] + "_prefit")).Projection(1,0)
    prefitForAreaNorm.SetDirectory(0)
   
    postfitForAreaNorm = (inputFile.Get(sampleNames[i] + "_postfit_0_0")).Projection(1,0)
    postfitForAreaNorm.SetDirectory(0)

    dataForSlices = (inputFile.Get(sampleNames[i] + "_data")).Projection(1,0)
    dataForSlices.SetDirectory(0)
    
    prefitForSlices  = (inputFile.Get(sampleNames[i] + "_prefit")).Projection(1,0)
    prefitForSlices.SetDirectory(0)
   
    postfitForSlices = (inputFile.Get(sampleNames[i] + "_postfit_0_0")).Projection(1,0)
    postfitForSlices.SetDirectory(0)

    dataForChi2 = (inputFile.Get(sampleNames[i] + "_data")).Projection(1,0)
    dataForChi2.SetDirectory(0)

    postfitForChi2 =  (inputFile.Get(sampleNames[i] + "_postfit_0_0")).Projection(1,0)
    postfitForChi2.SetDirectory(0)


    data_ratio = (inputFile.Get(sampleNames[i] + "_data")).Projection(0)
    data_ratio.SetDirectory(0)
    
    postfit_ratio = (inputFile.Get(sampleNames[i] + "_postfit_0_0")).Projection(0)
    postfit_ratio.SetDirectory(0)
    
    data_ratio.Divide(postfit_ratio)
    
    data_res = (inputFile.Get(sampleNames[i] + "_data")).Projection(0)
    data_res.SetDirectory(0)
    
    postfit_res = (inputFile.Get(sampleNames[i] + "_postfit_0_0")).Projection(0)
    postfit_res.SetDirectory(0)
    
    gStyle.SetOptStat(0)
    c2 = TCanvas()
    #c2.SetTicks(1,1)
    if(firstRatio):
        c2.Print("dataMCRatiosWithUnc.pdf[")
        firstRatio = False
    
    data_ratio.SetMarkerColor(1)
    data_ratio.SetMarkerStyle(21)
    data_ratio.SetMarkerSize(0.7)
    data_ratio.SetLineWidth(2)
    data_ratio.Draw()
    data_ratio.SetTitle(sampleNames[i])
    data_ratio.GetXaxis().SetTitle("Muon momentum (MeV/c)")
    data_ratio.GetYaxis().SetTitle("Data/MC")
    data_ratio.SetMaximum(1.5)
    data_ratio.SetMinimum(0.5)
    xMax = data_ratio.GetXaxis().GetXmax()
    c2.SetLogx() #Use logarithmic x axis for momentum plots.
    c2.Modified()
    c2.Update()
    
    #In log scale, need to do this instead of a line.
    #Actually, this should work in both scales.
    lineAtOne = TF1("lineAtOne","1",0,xMax)
    lineAtOne.SetLineWidth(3)
    lineAtOne.SetLineColor(kBlue)
    lineAtOne.Draw("same")

    c2.Modified()
    c2.Update()
    c2.Print("dataMCRatiosWithUnc.pdf")
    
    #Make lists for the costheta slice histograms.
    dataSlices = []
    prefitSlices = []
    postfitSlices = []

    #Set up maximum momenta for the different histogram types.
    maxMom = 5000.0
    minTheta = 0.6
    if (i > 2 and i<7) or i>9:
        maxMom = 5000.0


    #Set up one for each theta bin.
    for iby in xrange(1,dataForSlices.GetNbinsY() + 1):

            thetaLow = dataForSlices.GetYaxis().GetBinLowEdge(iby)
            thetaHigh = dataForSlices.GetYaxis().GetBinUpEdge(iby)

            thetaString = " < cos#theta < "
            thetaString = str(thetaLow) + thetaString + str(thetaHigh)

            dataSlices.append(TH1D("dataSlice"+sampleNames[i]+str(iby),
                thetaString, dataForSlices.GetXaxis().GetNbins(),
                dataForSlices.GetXaxis().GetXbins().GetArray()))
            prefitSlices.append(TH1D("prefitSlice"+sampleNames[i]+str(iby),
                thetaString, prefitForSlices.GetXaxis().GetNbins(),
                prefitForSlices.GetXaxis().GetXbins().GetArray()))
            postfitSlices.append(TH1D("postfitSlice"+sampleNames[i]+str(iby),
                thetaString, postfitForSlices.GetXaxis().GetNbins(),
                postfitForSlices.GetXaxis().GetXbins().GetArray()))
   
    
    for ibx in xrange(1, residuals.GetNbinsX() + 1):

        #Do the momentum projections here:
        mpdim = (dataMomProj.GetXaxis().GetBinUpEdge(ibx) -
                    dataMomProj.GetXaxis().GetBinLowEdge(ibx))/100.0
        dataMomProj.SetBinContent(ibx,dataMomProj.GetBinContent(ibx)/mpdim)
        dataMomProj.SetBinError(ibx,math.sqrt(dataMomProj.GetBinContent(ibx))/mpdim)

        prefitMomProj.SetBinContent(ibx, prefitMomProj.GetBinContent(ibx)/mpdim)
        postfitMomProj.SetBinContent(ibx, postfitMomProj.GetBinContent(ibx)/mpdim)
        postfitMomProj.SetBinError(ibx,0.0)


        for iby in xrange(1, residuals.GetNbinsY() + 1):

            dC = dataForResiduals.GetBinContent(ibx,iby)
            #if dC == 0:
            #    dC = 1
            #    print "Warning: " + sampleNames[i] + " has empty bin " + str(ibx) + " " + str(iby)
            pC = postfitForResiduals.GetBinContent(ibx,iby)
            preC = prefitForResiduals.GetBinContent(ibx,iby)
            dErr = math.sqrt(pC)
            preErr = math.sqrt(preC)
            residuals.SetBinContent(ibx,iby,(dC-pC))
            prefitResiduals.SetBinContent(ibx,iby,(dC-preC))
            
            #Divide momentum by 100 so it's per 100 MeV/c and
            #cos(theta) by 0.01 so it's per 0.01
            pfxdim = (postfitForAreaNorm.GetXaxis().GetBinUpEdge(ibx) -
                    postfitForAreaNorm.GetXaxis().GetBinLowEdge(ibx))/100.0
            pfydim = (postfitForAreaNorm.GetYaxis().GetBinUpEdge(iby) -
                    postfitForAreaNorm.GetYaxis().GetBinLowEdge(iby))/0.01

            prefitForAreaNorm.SetBinContent(ibx, iby, prefitForAreaNorm.GetBinContent(ibx,iby)/(pfxdim*pfydim))
            postfitForAreaNorm.SetBinContent(ibx, iby, postfitForAreaNorm.GetBinContent(ibx,iby)/(pfxdim*pfydim))
            
            dataForAreaNormError = math.sqrt(dataForAreaNorm.GetBinContent(ibx,iby))/(pfxdim*pfydim)
            dataForAreaNorm.SetBinContent(ibx,iby,dataForAreaNorm.GetBinContent(ibx,iby)/(pfxdim*pfydim))
            dataForAreaNorm.SetBinError(ibx, iby, dataForAreaNormError)


       
            #For the slices, only divide by the momentum dimension.
            dataForSlices.SetBinError(ibx,iby,math.sqrt(dataForSlices.GetBinContent(ibx,iby))/pfxdim)
            dataForSlices.SetBinContent(ibx,iby,dataForSlices.GetBinContent(ibx,iby)/(pfxdim))
            dataSlices[iby-1].SetBinContent(ibx,dataForSlices.GetBinContent(ibx,iby))
            dataSlices[iby-1].SetBinError(ibx,dataForSlices.GetBinError(ibx,iby))
            
            prefitForSlices.SetBinContent(ibx,iby,prefitForSlices.GetBinContent(ibx,iby)/(pfxdim))
            prefitSlices[iby-1].SetBinContent(ibx,prefitForSlices.GetBinContent(ibx,iby))

            postfitForSlices.SetBinContent(ibx,iby,postfitForSlices.GetBinContent(ibx,iby)/(pfxdim))
            postfitSlices[iby-1].SetBinContent(ibx,postfitForSlices.GetBinContent(ibx,iby))

            #Replicate the chi squared calculation per bin here.
            dataVal = dataForChi2.GetBinContent(ibx,iby)
            predVal = postfitForChi2.GetBinContent(ibx,iby)

            if (predVal > 0.0) and (dataVal > 0.0):
                dataForChi2.SetBinContent(ibx,iby, 2.0*(predVal - dataVal + dataVal*math.log( dataVal/predVal) ))
            elif predVal > 0.0:
                dataForChi2.SetBinContent(ibx,iby,2.0*predVal)


    outputFile.cd()
            
            
    #Now set up canvases to save these to PDF files.
    gStyle.SetOptStat(0)

    #Figure out what the data, prefit, and postfit z axis max and min values
    #need to be.
    dataMin = dataForAreaNorm.GetMinimum()
    dataMax = dataForAreaNorm.GetMaximum()
    prefitMin = prefitForAreaNorm.GetMinimum()
    prefitMax = prefitForAreaNorm.GetMaximum()
    postfitMin = postfitForAreaNorm.GetMinimum()
    postfitMax = postfitForAreaNorm.GetMaximum()

    zMax = max(dataMax,prefitMax,postfitMax)
    zMin = min(dataMin,prefitMin,postfitMin)


    cPre = TCanvas()
    cPre.Divide(2,2)
    cPre.cd(1)
    dataForAreaNorm.SetTitle(sampleNames[i] + ": Data (Events/(100 MeV/c)/0.01)")
    dataForAreaNorm.GetXaxis().SetTitle("p_{#mu} (MeV/c)")
    dataForAreaNorm.GetYaxis().SetTitle("cos#theta_{#mu}")
    dataForAreaNorm.Draw("ZCOL")
    dataForAreaNorm.GetXaxis().SetRangeUser(0.0,maxMom)
    dataForAreaNorm.GetYaxis().SetRangeUser(minTheta,1.0)
    dataForAreaNorm.GetZaxis().SetRangeUser(zMin,zMax)

    cPre.cd(2)
    prefitForAreaNorm.SetTitle(sampleNames[i] + ": Prefit (Events/(100MeV/c)/0.01)")
    prefitForAreaNorm.GetXaxis().SetTitle("p_{#mu} (MeV/c)")
    prefitForAreaNorm.GetYaxis().SetTitle("cos#theta_{#mu}")
    prefitForAreaNorm.Draw("ZCOL")
    prefitForAreaNorm.GetXaxis().SetRangeUser(0.0,maxMom)
    prefitForAreaNorm.GetYaxis().SetRangeUser(minTheta,1.0)
    prefitForAreaNorm.GetZaxis().SetRangeUser(zMin,zMax)

    """for j in xrange(data1.GetNbinsX()*data1.GetNbinsX()+2):
        if data1.GetBinContent(j) < 0.9:
            data1.SetBinContent(j,0.9)
        elif data1.GetBinContent(j) >1.1:
            data1.SetBinContent(j,1.1)"""
    cPre.cd(3)
    data1.SetTitle(sampleNames[i] + ": Data/Prefit")
    data1.GetXaxis().SetTitle("p_{#mu} (MeV/c)")
    data1.GetYaxis().SetTitle("cos#theta_{#mu}")
    data1.Draw("ZCOL")
    data1.GetXaxis().SetRangeUser(0.0,maxMom)
    data1.GetYaxis().SetRangeUser(minTheta,1.0)
    data1.GetZaxis().SetRangeUser(0.0,2.0)

    cPre.cd(4)
    prefitResiduals.SetTitle(sampleNames[i] + ": Absolute Residuals (data - prefit)")
    prefitResiduals.GetXaxis().SetTitle("p_{#mu} (MeV/c)")
    prefitResiduals.GetYaxis().SetTitle("cos#theta_{#mu}")
    prefitResiduals.Draw("ZCOL")
    prefitResiduals.GetXaxis().SetRangeUser(0.0,maxMom)
    prefitResiduals.GetYaxis().SetRangeUser(minTheta,1.0)
    #prefitResiduals.GetZaxis().SetRangeUser(-5.0,5.0)

    cPre.Modified()
    cPre.Update()
    if(firstPrefit):
        cPre.Print("prefitDataMC.pdf(")
    else:
        cPre.Print("prefitDataMC.pdf")

    cPost = TCanvas()
    cPost.Divide(2,2)
    cPost.cd(1)
    dataForAreaNorm.SetTitle(sampleNames[i] + ": Data (Events/(100 MeV/c)/0.01)")
    dataForAreaNorm.GetXaxis().SetTitle("p_{#mu} (MeV/c)")
    dataForAreaNorm.GetYaxis().SetTitle("cos#theta_{#mu}")
    dataForAreaNorm.Draw("ZCOL")
    dataForAreaNorm.GetXaxis().SetRangeUser(0.0,maxMom)
    dataForAreaNorm.GetYaxis().SetRangeUser(minTheta,1.0)
    dataForAreaNorm.GetZaxis().SetRangeUser(zMin,zMax)

    cPost.cd(2)
    postfitForAreaNorm.SetTitle(sampleNames[i] + ": Postfit (Events/(100MeV/c)/0.01)")
    postfitForAreaNorm.GetXaxis().SetTitle("p_{#mu} (MeV/c)")
    postfitForAreaNorm.GetYaxis().SetTitle("cos#theta_{#mu}")
    postfitForAreaNorm.Draw("ZCOL")
    postfitForAreaNorm.GetXaxis().SetRangeUser(0.0,maxMom)
    postfitForAreaNorm.GetYaxis().SetRangeUser(minTheta,1.0)
    postfitForAreaNorm.GetZaxis().SetRangeUser(zMin,zMax)

    cPost.cd(3)
    data2.SetTitle(sampleNames[i] + ": Data/Postfit")
    data2.GetXaxis().SetTitle("p_{#mu} (MeV/c)")
    data2.GetYaxis().SetTitle("cos#theta_{#mu}")
    data2.Draw("ZCOL")
    data2.GetXaxis().SetRangeUser(0.0,5000.0)
    data2.GetYaxis().SetRangeUser(0.6,1.0)
    data2.GetZaxis().SetRangeUser(0.0,2.0)

    cPost.cd(4)
    residuals.SetTitle(sampleNames[i] + ": Residuals (data - postfit)")
    residuals.GetXaxis().SetTitle("p_{#mu} (MeV/c)")
    residuals.GetYaxis().SetTitle("cos#theta_{#mu}")
    residuals.Draw("ZCOL")
    residuals.GetXaxis().SetRangeUser(0.0,maxMom)
    residuals.GetYaxis().SetRangeUser(minTheta,1.0)
    #residuals.GetZaxis().SetRangeUser(-5.0,5.0)

    cPost.Modified()
    cPost.Update()
    if(firstPostfit):
        cPost.Print("postfitDataMC.pdf(")
    else:
        cPost.Print("postfitDataMC.pdf")
 

    #Do a stack of the momentum projections.
    mpStack = THStack("mpStack",sampleNames[i] + ": " + ";p_{#mu} (MeV/c);Events/(100 MeV/c)")
        
    #Set the data to black.
    dataMomProj.SetMarkerStyle(20)
    dataMomProj.SetMarkerColor(1)
    dataMomProj.SetLineColor(1)
    dataMomProj.SetLineWidth(2)
        
    #Draw points and error bars for this plot.
    mpStack.Add(dataMomProj,"P0E")
        
    #Set the pre-fit to blue
    prefitMomProj.SetLineColor(4)
    prefitMomProj.SetLineWidth(2)
    mpStack.Add(prefitMomProj)

    #Postfit is black.
    postfitMomProj.SetLineColor(1)
    postfitMomProj.SetLineWidth(2)
    mpStack.Add(postfitMomProj)

    cProj = TCanvas()
    cProj.Divide(1,2)
    cProj.cd(1)
    
    mpStack.Draw("NOSTACK")
        
    #Now, set the x-axis maximum range.
    mpStack.GetXaxis().SetRangeUser(0,maxMom)
    leg = TLegend(0.68,0.70,0.95,0.99)
    leg.SetFillColor(0)
    leg.AddEntry(dataMomProj, "Data","LEP")
    leg.AddEntry(prefitMomProj,"Prefit", "L")
    leg.AddEntry(postfitMomProj,"Postfit","L")
    leg.Draw()
    cProj.Modified()
    cProj.Update()

    cProj.cd(2)
    data_ratio.SetMarkerColor(4)
    data_ratio.SetMarkerStyle(21)
    data_ratio.SetMarkerSize(0.7)
    data_ratio.SetLineColor(kBlue)
    data_ratio.SetLineWidth(2)
    data_ratio.Draw()
    data_ratio.SetTitle(sampleNames[i])
    data_ratio.GetXaxis().SetTitle("Muon momentum (MeV/c)")
    data_ratio.GetYaxis().SetTitle("Data/MC")
    data_ratio.SetMaximum(1.25)
    data_ratio.SetMinimum(0.75)
    data_ratio.GetXaxis().SetRangeUser(0,maxMom)
    #In log scale, need to do this instead of a line.
    #Actually, this should work in both scales.
    lineAtOne = TF1("lineAtOne","1",0,xMax)
    lineAtOne.SetLineWidth(3)
    lineAtOne.SetLineColor(kBlack)
    lineAtOne.Draw("same")
    cProj.Modified()
    cProj.Update()
    
    if(firstHisto):
        cProj0 = TCanvas()
        cProj0.cd()
        leg = TLegend(0.05,0.05,0.95,0.95)
        leg.SetFillColor(0)
        leg.AddEntry(dataMomProj, "Data","LEP")
        leg.AddEntry(prefitMomProj,"Prefit", "L")
        leg.AddEntry(postfitMomProj,"Postfit","L")
        leg.Draw()
        cProj0.Print("momentumProjections.pdf(")
        cProj.Print("momentumProjections.pdf")
    else:
        cProj.Print("momentumProjections.pdf")
    
    
    #Want a stack for the data/prefit/postfit comparisons.
    stackSlices = []
    for si in xrange(0,len(dataSlices)):
        stackSlices.append(THStack("stackSlices",sampleNames[i] + ": " +
            dataSlices[si].GetTitle() + ";p_{#mu} (MeV/c);Events/(100 MeV/c)"))
        
        #Set the data to black.
        dataSlices[si].SetMarkerStyle(20)
        dataSlices[si].SetMarkerColor(1)
        dataSlices[si].SetLineColor(1)
        dataSlices[si].SetLineWidth(2)
        
        #Draw points and error bars for this plot.
        stackSlices[si].Add(dataSlices[si],"P0E")
        
        #Set the pre-fit to blue
        prefitSlices[si].SetLineColor(4)
        prefitSlices[si].SetLineWidth(2)
        stackSlices[si].Add(prefitSlices[si])

        #Postfit is black.
        postfitSlices[si].SetLineColor(1)
        postfitSlices[si].SetLineWidth(2)
        stackSlices[si].Add(postfitSlices[si])

        c1 = TCanvas()
        c1.cd()
        stackSlices[si].Draw("NOSTACK")
        leg = TLegend(0.68,0.70,0.95,0.95)
        leg.SetFillColor(0)
        leg.AddEntry(dataMomProj, "Data","LEP")
        leg.AddEntry(prefitMomProj,"Prefit", "L")
        leg.AddEntry(postfitMomProj,"Postfit","L")
        leg.Draw()
        #Now, set the x-axis maximum range.
        stackSlices[si].GetXaxis().SetRangeUser(0,maxMom)

        c1.Modified()
        c1.Update()
        if(firstHisto):
            c0 = TCanvas()
            c0.cd()
            leg = TLegend(0.05,0.05,0.95,0.95)
            leg.SetFillColor(0)
            leg.AddEntry(dataSlices[si], "Data","LEP")
            leg.AddEntry(prefitSlices[si],"Prefit", "L")
            leg.AddEntry(postfitSlices[si],"Postfit","L")
            leg.Draw()
            c0.Print("thetaSlices.pdf(")
            c1.Print("thetaSlices.pdf")
            firstHisto = False
        else:
            c1.Print("thetaSlices.pdf")
        
    data1.Write(sampleNames[i] + "_data_prefit_ratio")
    data2.Write(sampleNames[i] + "_data_postfit_ratio")
    residuals.Write(sampleNames[i] + "_residuals")
    postfitForAreaNorm.Write(sampleNames[i] + "_areaNormalizedPostfit")
    dataForChi2.Write(sampleNames[i] + "_chi2Contrib")

    unrolledPrefit = TH1F(sampleNames[i] +
            "_data_prefit_ratio_unrolled",sampleNames[i] +
            "_data_prefit_ratio_unrolled",
            data1.GetNbinsX()*data1.GetNbinsY(),0.0,data1.GetNbinsX()*data1.GetNbinsY())

    #Now unroll these for plotting.
    currentBin = 1
    for j in xrange(1,data1.GetNbinsX() + 1):

        for k in xrange(1,data1.GetNbinsY() + 1):

            unrolledPrefit.SetBinContent(currentBin, data1.GetBinContent(j,k))
            currentBin += 1

    unrolledPrefit.Write(sampleNames[i] + "_data_prefit_ratio_unrolled")


    unrolledPostfit = TH1F(sampleNames[i] +
            "_data_postfit_ratio_unrolled",sampleNames[i] +
            "_data_postfit_ratio_unrolled",
            data2.GetNbinsX()*data2.GetNbinsY(),0.0,data2.GetNbinsX()*data2.GetNbinsY())

    #Now unroll these for plotting.
    currentBin = 1
    for j in xrange(1,data2.GetNbinsX() + 1):

        for k in xrange(1,data2.GetNbinsY() + 1):

            unrolledPostfit.SetBinContent(currentBin, data2.GetBinContent(j,k))
            currentBin += 1

    unrolledPostfit.Write(sampleNames[i] + "_data_postfit_ratio_unrolled")
    
    

c1 = TCanvas()
c1.cd()
c1.Print("thetaSlices.pdf]")
c1.Print("prefitDataMC.pdf]")
c1.Print("postfitDataMC.pdf]")
c1.Print("momentumProjections.pdf]")
c1.Print("dataMCRatiosWithUnc.pdf]")

#Now that we're done with the file, close it.
outputFile.Close()

os.chdir("../")
