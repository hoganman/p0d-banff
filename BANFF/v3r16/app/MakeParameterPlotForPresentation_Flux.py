#!/usr/bin/python
from ROOT import *
import sys
import math

gStyle.SetOptStat(0)
gStyle.SetLabelSize(0.035)
#gStyle.SetPadBottomMargin(0.3)
gROOT.SetBatch(kTRUE);

#Useage is python MakeParameterPlots.py postfitFileWithAllParameters outputFileName
inFile = TFile(sys.argv[1])
inFile2 = TFile(sys.argv[2])

prefit_params = inFile.Get("postfit_params")
prefit_cov = inFile.Get("postfit_cov")
prefit_err = TVectorD(prefit_params.GetNrows())

postfit_params = inFile2.Get("postfit_params")
postfit_cov = inFile2.Get("postfit_cov")
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

#BeRPA_A
prefitHist.SetBinContent(115,prefitHist.GetBinContent(115)*1.69491525424)
postfitHist.SetBinContent(115,postfitHist.GetBinContent(115)*1.69491525424)
prefitHist.SetBinError(115,prefitHist.GetBinError(115)*1.69491525424)
postfitHist.SetBinError(115,postfitHist.GetBinError(115)*1.69491525424)
#BeRPA_B
prefitHist.SetBinContent(116,prefitHist.GetBinContent(116)*0.95238095238)
postfitHist.SetBinContent(116,postfitHist.GetBinContent(116)*0.95238095238)
prefitHist.SetBinError(116,prefitHist.GetBinError(116)*0.95238095238)
postfitHist.SetBinError(116,postfitHist.GetBinError(116)*0.95238095238)
#BeRPA_D
prefitHist.SetBinContent(117,prefitHist.GetBinContent(117)*0.88495575221)
postfitHist.SetBinContent(117,postfitHist.GetBinContent(117)*0.88495575221)
prefitHist.SetBinError(117,prefitHist.GetBinError(117)*0.88495575221)
postfitHist.SetBinError(117,postfitHist.GetBinError(117)*0.88495575221)
#BeRPA_E
prefitHist.SetBinContent(118,prefitHist.GetBinContent(118)*1.13636363636)
postfitHist.SetBinContent(118,postfitHist.GetBinContent(118)*1.13636363636)
prefitHist.SetBinError(118,prefitHist.GetBinError(118)*1.13636363636)
postfitHist.SetBinError(118,postfitHist.GetBinError(118)*1.13636363636)
#BeRPA_U
prefitHist.SetBinContent(119,prefitHist.GetBinContent(119)*0.83333333333)
postfitHist.SetBinContent(119,postfitHist.GetBinContent(119)*0.83333333333)
prefitHist.SetBinError(119,prefitHist.GetBinError(119)*0.83333333333)
postfitHist.SetBinError(119,postfitHist.GetBinError(119)*0.83333333333)
#CCDIS
prefitHist.SetBinContent(125,prefitHist.GetBinContent(125)+1)
postfitHist.SetBinContent(125,postfitHist.GetBinContent(125)+1)
#FSI
prefitHist.SetBinContent(101,prefitHist.GetBinContent(101)+1)
postfitHist.SetBinContent(101,postfitHist.GetBinContent(101)+1)
prefitHist.SetBinContent(102,prefitHist.GetBinContent(102)+1)
postfitHist.SetBinContent(102,postfitHist.GetBinContent(102)+1)
prefitHist.SetBinContent(103,prefitHist.GetBinContent(103)+1)
postfitHist.SetBinContent(103,postfitHist.GetBinContent(103)+1)
prefitHist.SetBinContent(104,prefitHist.GetBinContent(104)+1)
postfitHist.SetBinContent(104,postfitHist.GetBinContent(104)+1)
prefitHist.SetBinContent(105,prefitHist.GetBinContent(105)+1)
postfitHist.SetBinContent(105,postfitHist.GetBinContent(105)+1)
prefitHist.SetBinContent(106,prefitHist.GetBinContent(106)+1)
postfitHist.SetBinContent(106,postfitHist.GetBinContent(106)+1)

histoTypes = ["ND280 NuMode Flux", "ND280 ANuMode Flux", "SK NuMode Flux","SK ANuMode Flux","FSI","XSec"]
paramRange = [[0,24],[25,49],[50,74],[75,99],[100,105],[106,130]]
yRange = [[0.80,1.2],[0.80,1.2],[0.80,1.2],[0.80,1.2],[-1.0,1.2],[0,2.05]]
binLabels = []
for i in xrange(0, postfit_params.GetNrows()):
    binLabels.append("")

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

for i in xrange(100,131):
    binLabels[i] = param_list.At(i).GetString().Data()


# binLabels[131] = "FGD 1 CC-0#pi, FHC"
# binLabels[174] = "FGD 1 CC-1#pi, FHC"
# binLabels[215] = "FGD 1 CC-Other, FHC"
# binLabels[256] = "FGD 1 #bar{#nu}_{#mu} CC-1Track, RHC"
# binLabels[297] = "FGD 1 #bar{#nu}_{#mu} CC-NTracks, RHC"
# binLabels[333] = "FGD 1 #nu_{#mu} CC-1Track, RHC"
# binLabels[364] = "FGD 1 #nu_{#mu} CC-NTracks, RHC"
# binLabels[405] = "FGD 2 CC-0#pi, FHC"
# binLabels[446] = "FGD 2 CC-1#pi, FHC"
# binLabels[487] = "FGD 2 CC-Other, FHC"
# binLabels[528] = "FGD 2 #bar{#nu}_{#mu} CC-1Track, RHC"
# binLabels[569] = "FGD 2 #bar{#nu}_{#mu} CC-NTracks, RHC"
# binLabels[606] = "FGD 2 #nu_{#mu} CC-1Track, RHC"
# binLabels[647] = "FGD 2 #nu_{#mu} CC-NTracks, RHC"


#Set the fill types for the prefit and postfit histograms.  Will plot them as
#points, but this will propagate to the error bars.

#Prefit is red fill, red point that isn't seen.
prefitHist.SetFillColor(kRed)
prefitHist.SetFillStyle(3002)
prefitHist.SetLineColor(kRed)
prefitHist.SetMarkerColor(kRed)
prefitHist.SetMarkerStyle(7)
prefitHist.SetMarkerSize(0.5)


#Postfit is with blue squares
postfitHist.SetMarkerColor(4)
postfitHist.SetMarkerStyle(21)
postfitHist.SetMarkerSize(0.5)
postfitHist.SetLineWidth(2)

#line at 0
line = TLine(0,0,1000,0)
line.SetLineColor(kWhite)
line.SetLineStyle(2)
c1 = TCanvas()
c1.cd()
c1.Print(sys.argv[3] + "[")

for i in xrange(0, len(histoTypes)):

    stack = THStack("pStack",histoTypes[i])
    stack.Add(prefitHist,"E2P")
    stack.Add(postfitHist,"E1")
    
    c1.cd()
    stack.Draw("NOSTACK")
    line.Draw("SAME")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0],paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.12,0.84,0.32,0.98)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "RHC Multi-track","FEP")
    leg.AddEntry(postfitHist,"RHC Multi-#pi", "ELP")

    leg.Draw()

    for j in xrange(0, prefit_params.GetNrows()):
        stack.GetXaxis().SetBinLabel(j+1,binLabels[j])
    
    stack.GetXaxis().LabelsOption("v")

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[3])


histoTypes = ["FSI parameters","CC0#pi parameters","BeRPA parameters","CC1#pi parameters","Other","Observable normalization "]
paramRange = [[100,105],[106,113],[114,118],[119,121],[122,130],[131,688]]
yRange = [[0.2,1.8],[0,2.1],[0,2.1],[0,2.1],[0,2.1],[0.5,1.5]]

c1.cd()

for i in xrange(0, len(histoTypes)):

    stack = THStack("pStack",histoTypes[i])
    if i == 1:
        stack.Add(prefitHist,"HIST P")
        line = TLine(107,1,114,1)
        line.SetLineColor(kRed)
        line.SetLineStyle(2)
    else:
        stack.Add(prefitHist,"E2P")
    stack.Add(postfitHist,"E1")
    postfitHist.SetMarkerSize(0.5)
    #stack.Add(postfitHist,"E1")
    
    c1.cd()
    stack.Draw("NOSTACK")
    
    if i == 1 or i > 4:
        line.Draw("SAME")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0],paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.12,0.84,0.32,0.98)
    leg.SetFillColor(0)
    leg.AddEntry(prefitHist, "RHC Multi-track","FEP")
    leg.AddEntry(postfitHist,"RHC Multi-#pi", "ELP")

    leg.Draw()

    for j in xrange(100, prefit_params.GetNrows()):
        stack.GetXaxis().SetBinLabel(j+1,binLabels[j])
    
    if i<5:
        stack.GetXaxis().LabelsOption("v")
    else:
        stack.GetXaxis().LabelsOption("")
    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[3])


fluxfile = TFile("/data/lasorak/BANFF/inputs/flux_covariance_banff_13av2.root")
flux_axis = []
flux_axis.append(fluxfile.Get("nd5_numode_numu_bins"))
flux_axis.append(fluxfile.Get("nd5_numode_numub_bins"))
flux_axis.append(fluxfile.Get("nd5_numode_nue_bins"))
flux_axis.append(fluxfile.Get("nd5_numode_nueb_bins"))
flux_axis.append(fluxfile.Get("nd5_anumode_numu_bins"))
flux_axis.append(fluxfile.Get("nd5_anumode_numub_bins"))
flux_axis.append(fluxfile.Get("nd5_anumode_nue_bins"))
flux_axis.append(fluxfile.Get("nd5_anumode_nueb_bins"))
flux_axis.append(fluxfile.Get("sk_numode_numu_bins"))
flux_axis.append(fluxfile.Get("sk_numode_numub_bins"))
flux_axis.append(fluxfile.Get("sk_numode_nue_bins"))
flux_axis.append(fluxfile.Get("sk_numode_nueb_bins"))
flux_axis.append(fluxfile.Get("sk_anumode_numu_bins"))
flux_axis.append(fluxfile.Get("sk_anumode_numub_bins"))
flux_axis.append(fluxfile.Get("sk_anumode_nue_bins"))
flux_axis.append(fluxfile.Get("sk_anumode_nueb_bins"))

histoTypes2 = ["ND280 FHC #nu_{#mu} Flux","ND280 FHC #bar{#nu}_{#mu} Flux","ND280 FHC #nu_{e} Flux","ND280 FHC #bar{#nu}_{e} Flux","ND280 RHC #nu_{#mu} Flux","ND280 RHC #bar{#nu}_{#mu} Flux","ND280 RHC #nu_{e} Flux","ND280 RHC #bar{#nu}_{e} Flux","SK FHC #nu_{#mu} Flux","SK FHC #bar{#nu}_{#mu} Flux","SK FHC #nu_{e} Flux","SK FHC #bar{#nu}_{e} Flux","SK RHC #nu_{#mu} Flux","SK RHC #bar{#nu}_{#mu} Flux","SK RHC #nu_{e} Flux","SK RHC #bar{#nu}_{e} Flux"]
paramRange2 = [[0,10],[11,15],[16,22],[23,24],[25,29],[30,40],[41,42],[43,49],[50,60],[61,65],[66,72],[73,74],[75,79],[80,90],[91,92],[93,99]]
yRange2 = [[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3],[0.50,1.3]]

c1.cd()

gStyle.SetPadBottomMargin(0)

for i in xrange(0, len(histoTypes2)):
    
    nbin = int(paramRange2[i][1]-paramRange2[i][0])
    prefit_flux = TH1F("","",flux_axis[i].GetNbins(),flux_axis[i].GetBinLowEdge(1),flux_axis[i].GetBinUpEdge(flux_axis[i].GetNbins()))
    postfit_flux = TH1F("","",flux_axis[i].GetNbins(),flux_axis[i].GetBinLowEdge(1),flux_axis[i].GetBinUpEdge(flux_axis[i].GetNbins()))
    prefit_flux.GetXaxis().Set(flux_axis[i].GetNbins(),flux_axis[i].GetXbins().GetArray())
    postfit_flux.GetXaxis().Set(flux_axis[i].GetNbins(),flux_axis[i].GetXbins().GetArray())

    #Prefit is red fill, red point that isn't seen.
    prefit_flux.SetFillColor(kRed)
    prefit_flux.SetFillStyle(3002)
    prefit_flux.SetLineColor(kRed)
    prefit_flux.SetMarkerColor(kRed)
    prefit_flux.SetMarkerStyle(7)
    prefit_flux.SetMarkerSize(0.5)


    #Postfit is with blue squares
    postfit_flux.SetMarkerColor(4)
    postfit_flux.SetMarkerStyle(21)
    postfit_flux.SetMarkerSize(0.5)
    postfit_flux.SetLineWidth(2)

    for j in xrange(0,nbin+2):
        print int(paramRange2[i][0])+j, prefitHist.GetBinContent(int(paramRange2[i][0])+j)
        prefit_flux.SetBinContent(j,prefitHist.GetBinContent(int(paramRange2[i][0])+j))
        prefit_flux.SetBinError(j,prefitHist.GetBinError(int(paramRange2[i][0])+j))
        postfit_flux.SetBinContent(j,postfitHist.GetBinContent(int(paramRange2[i][0])+j))
        postfit_flux.SetBinError(j,postfitHist.GetBinError(int(paramRange2[i][0])+j))
        
    stack = THStack("pStack",histoTypes2[i])
    postfit_flux.GetXaxis().SetTitle("E_{#nu} (GeV)")
    prefit_flux.GetXaxis().SetTitle("E_{#nu} (GeV)")
    stack.Add(prefit_flux,"E2P")
    stack.Add(postfit_flux,"E1")
    postfit_flux.SetMarkerSize(0.5)

    
    c1.SetLogx()
    c1.cd()
    stack.Draw("NOSTACK")
    
    if i == 1 or i > 4:
        line.Draw("SAME")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.SetMinimum(yRange2[i][0])
    stack.SetMaximum(yRange2[i][1])

    leg = TLegend(0.22,0.2,0.8,0.42)
    leg.SetFillColor(0)
    leg.SetBorderSize(0)
    leg.AddEntry(prefit_flux, "With Nominal BANFF","FEP")
    leg.AddEntry(postfit_flux,"With Nominal BANFF+#nu_{e} samples", "ELP")

    leg.Draw()

    pt = TPaveText(0.8,.05,.95,.1);
    pt.AddText("E_{#nu} (GeV)")
    pt.Draw("SAME");
    
    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[3])


c1.Print(sys.argv[3] + "]")






