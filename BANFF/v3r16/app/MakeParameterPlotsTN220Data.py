#!/usr/bin/python
from ROOT import *
import sys
import math

gStyle.SetOptStat(0)
gStyle.SetPadBottomMargin(0.3)

#gStyle.SetPadLeftMargin(0.25)
gStyle.SetLabelSize(0.045)


#Useage is python MakeParameterPlots.py postfitFileWithAllParameters outputFileName
inFile = TFile(sys.argv[1])

prefit_params = inFile.Get("prefit_params")
prefit_cov = inFile.Get("prefit_cov")
prefit_err = TVectorD(prefit_params.GetNrows())

postfit_params = inFile.Get("postfit_params")
postfit_cov = inFile.Get("postfit_cov")
postfit_err = TVectorD(postfit_params.GetNrows())
param_list = inFile.Get("param_list")

for i in xrange(0, prefit_err.GetNrows()):

    prefit_err[i] = math.sqrt(prefit_cov[i][i])
    postfit_err[i] = math.sqrt(postfit_cov[i][i])


#histoTypes = ["ND280 NuMode Flux", "ND280 ANuMode Flux", "SK NuMode Flux","SK ANuMode Flux","FSI","XSec"]
#paramRange = [[0,24],[25,49],[50,74],[75,99],[100,105],[106,121]]
#yRange = [[0.80,1.35],[0.80,1.35],[0.80,1.35],[0.80,1.35],[-1.0,1.2],[-1.0,2.25]]
#histoTypes = ["Flux","XSec","ObsNorm"]
#paramRange = [[0,99],[100,121],[122,postfit_params.GetNrows()-1]]
#yRange = [[0.80,1.35],[-1.0,2.25],[0.5,2.0]]
#Asher's y ranges.
#Something that makes more sense for this XSec plot.
#yRange = [[0.8,1.2],[-1.0,2.5],[0.5,1.5]]
histoTypes = ["XSec"]
paramRange = [[100,121]]
yRange = [[-1.0,2.5]]


binLabels = []
binLabels_neworder = []
for i in xrange(0, postfit_params.GetNrows()):
    binLabels.append("")
    binLabels_neworder.append("")

binLabels[0] = "ND280 #nu_{#mu}, FHC"
binLabels[11] = "ND280 #bar{#nu}_{#mu}, FHC"
binLabels[16] = "ND280 #nu_{e}, FHC"
binLabels[23] = "ND280 #bar{#nu}_{e}, FHC"
binLabels[25] = "ND280 #nu_{#mu}, RHC"
binLabels[30] = "ND280 #bar{#nu}_{#mu}, RHC"
binLabels[41] = "ND280 #nu_{e}, RHC"
binLabels[43] = "ND280 #bar{#nu}_{e}, RHC"
binLabels[50] = "SK #nu_{#mu}, FHC"
binLabels[61] = "SK #bar{#nu}_{#mu}, FHC"
binLabels[66] = "SK #nu_{e}, FHC"
binLabels[73] = "SK #bar{#nu}_{e}, FHC"
binLabels[75] = "SK #nu_{#mu}, RHC"
binLabels[80] = "SK #bar{#nu}_{#mu}, RHC"
binLabels[91] = "SK #nu_{e}, RHC"
binLabels[93] = "SK #bar{#nu}_{e}, RHC"

for i in xrange(100,122):
    binLabels[i] = param_list.At(i).GetString().Data()

binLabels[122] = "CC-0#pi, FHC"
binLabels[192] = "CC-1#pi, FHC"
binLabels[262] = "CC-Other, FHC"
binLabels[332] = "#bar{#nu}_{#mu} CC-1Track, RHC"
binLabels[352] = "#bar{#nu}_{#mu} CC-NTracks, RHC"
binLabels[372] = "#nu_{#mu} CC-1Track, RHC"
binLabels[392] = "#nu_{#mu} CC-NTracks, RHC"


#Now re-order everything to the same order as Asher for plotting.
prefit_params_neworder = TVectorD(prefit_params.GetNrows())
postfit_params_neworder = TVectorD(prefit_params.GetNrows())
prefit_err_neworder = TVectorD(prefit_params.GetNrows())
postfit_err_neworder = TVectorD(prefit_params.GetNrows())


#The new order moves FSI (originally 100-105) after the other XSec paramreters
#106-121 inclusive)
#So 106-121 become 100 to 115
#and 100 to 105 become 116 to 121.
for i in xrange(0,prefit_params.GetNrows()):

    #If it's a flux parameter, do nothing.
    if i < 100:
        prefit_params_neworder[i] = prefit_params[i]
        postfit_params_neworder[i] = postfit_params[i]
        prefit_err_neworder[i] = prefit_err[i]
        postfit_err_neworder[i] = postfit_err[i]
        binLabels_neworder[i] = binLabels[i]

    #Otherwise, if it's an FSI parameter, add 16 to get the new slot.
    elif i < 106:
        prefit_params_neworder[i+16] = prefit_params[i]
        postfit_params_neworder[i+16] = postfit_params[i]
        prefit_err_neworder[i+16] = prefit_err[i]
        postfit_err_neworder[i+16] = postfit_err[i]
        binLabels_neworder[i+16] = binLabels[i]

    #Otherwise, if it's another XSec parameter, subtract 6 to get the new slot.
    elif i < 122:
        prefit_params_neworder[i-6] = prefit_params[i]
        postfit_params_neworder[i-6] = postfit_params[i]
        prefit_err_neworder[i-6] = prefit_err[i]
        postfit_err_neworder[i-6] = postfit_err[i]
        binLabels_neworder[i-6] = binLabels[i]

    #Otherwise, it's an ObsNorm parameter, so copy it over into the same slot.
    else:
        prefit_params_neworder[i] = prefit_params[i]
        postfit_params_neworder[i] = postfit_params[i]
        prefit_err_neworder[i] = prefit_err[i]
        postfit_err_neworder[i] = postfit_err[i]
        binLabels_neworder[i] = binLabels[i]


#We just need to make one TH1D for each, then just change the range as needed
#to get the desired parameter subset.
prefitHist = TH1D(prefit_params_neworder)
postfitHist = TH1D(postfit_params_neworder)

for i in xrange(1, prefit_err_neworder.GetNrows() + 1):

    prefitHist.SetBinError(i,prefit_err_neworder[i-1])
    postfitHist.SetBinError(i,postfit_err_neworder[i-1])


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

#c1 = TCanvas("c1","c1",500,500)
c1 = TCanvas()
c1.cd()
c1.SetTicks(1,1)
#Start with the legend.
#leg = TLegend(0.05,0.05,0.95,0.95)
#leg.SetFillColor(0)
#leg.AddEntry(prefitHist, "Prefit","FEP")
#leg.AddEntry(postfitHist,"Postfit", "FEP")
#leg.AddEntry(prefitHist, "All targets for MEC_C","FEP")
#leg.AddEntry(postfitHist,"Carbon targets only for MEC_C", "FEP")
#leg.AddEntry(prefitHist, "Released fit results","FEP")
#leg.AddEntry(postfitHist,"EB_O, pF_O, and CCCOH_O_0 fixed", "FEP")
#leg.Draw()
c1.Print(sys.argv[2] + "[")

for i in xrange(0, len(histoTypes)):

    #stack = THStack("pStack",histoTypes[i]+";Parameter ID;")
    stack = THStack("pStack",";;Parameter Value")
    stack.Add(prefitHist,"E2P")
    stack.Add(postfitHist,"E2P")
    
    c1.cd()
    stack.Draw("NOSTACK")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0],paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.15,0.86,0.35,0.995)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "Prefit","FEP")
    leg.AddEntry(postfitHist,"Postfit", "FEP")
    leg.Draw()

    for j in xrange(0, prefit_params.GetNrows()):
        stack.GetXaxis().SetBinLabel(j+1,binLabels_neworder[j])
    
    
    stack.GetXaxis().LabelsOption("v")
    #stack.GetYaxis().SetTitleOffset(1.4)
    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[2])


c1.Print(sys.argv[2] + "]")






