#!/usr/bin/python
from ROOT import *
import sys
import math

gStyle.SetOptStat(0)

#Useage is python MakeParameterPlots.py postfitFileWithAllParameters outputFileName
inFile = TFile(sys.argv[1])

prefit_params = inFile.Get("prefit_params")
prefit_cov = inFile.Get("prefit_cov")
prefit_err = TVectorD(prefit_params.GetNrows())

postfit_params = inFile.Get("postfit_params")
postfit_cov = inFile.Get("postfit_cov")
postfit_err = TVectorD(postfit_params.GetNrows())

for i in xrange(0, prefit_err.GetNrows()):

    prefit_err[i] = math.sqrt(prefit_cov[i][i])
    postfit_err[i] = math.sqrt(postfit_cov[i][i])

#We just need to make one TH1D for each, then just change the range as needed
#to get the desired parameter subset.
prefitHist = TH1D(prefit_params)
postfitHist = TH1D(postfit_params)

for i in xrange(1, prefit_err.GetNrows() + 1):

    prefitHist.SetBinError(i,prefit_err[i-1])
    postfitHist.SetBinError(i,postfit_err[i-1])


histoTypes = ["ND280 NuMode Flux", "ND280 ANuMode Flux", "SK NuMode Flux","SK ANuMode Flux","FSI","XSec"]
paramRange = [[0,24],[25,49],[50,74],[75,99],[100,105],[106,121]]
yRange = [[0.80,1.30],[0.80,1.30],[0.80,1.30],[0.80,1.30],[-1.0,1.0],[-0.5,2.25]]

#Set the fill types for the prefit and postfit histograms.  Will plot them as
#points, but this will propagate to the error bars.

#Prefit is solid red fill, red point that isn't seen.
prefitHist.SetFillStyle(1001)
prefitHist.SetFillColor(2)
prefitHist.SetMarkerStyle(21)
prefitHist.SetMarkerColor(2)

#Postfit is hatched blue background, with blue squares
postfitHist.SetFillStyle(3244)
postfitHist.SetFillColor(4)
postfitHist.SetMarkerColor(4)
postfitHist.SetMarkerStyle(21)

c1 = TCanvas()
c1.cd()
#Start with the legend.
leg = TLegend(0.05,0.05,0.95,0.95)
leg.SetFillColor(0)
leg.AddEntry(prefitHist, "Prefit","FEP")
leg.AddEntry(postfitHist,"Postfit", "FEP")
leg.Draw()
c1.Print(sys.argv[2] + "(")

for i in xrange(0, len(histoTypes)):

    stack = THStack("pStack",histoTypes[i]+";Parameter ID;")
    stack.Add(prefitHist,"E2P")
    stack.Add(postfitHist,"E2P")
    
    c1.cd()
    stack.Draw("NOSTACK")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0],paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])
    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])


c1.Print(sys.argv[2] + "]")






