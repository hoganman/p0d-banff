#!/usr/bin/python
from ROOT import *
import sys
import math

gStyle.SetOptStat(111111)

#Script to plot the parameter values calculated from toy throwing
#Useage is python PlotToyValues.py toyThrowingOutputFile nToys outputFileName
inFile = TFile(sys.argv[1])

outFile = sys.argv[3]

param_names = ["FEFQE" , "FEFQEH" , "FEFINEL" , "FEFABS" , "FEFCX" , "MAQE" , "pF_C" , "pF_O" , "2p2h_norm_nu" , "2p2h_norm_nubar" , "2p2h_normCtoO" , "2p2h_shape_C" , "2p2h_shape_O" , "BeRPA_A" , "BeRPA_B" , "BeRPA_D" , "BeRPA_E" , "BeRPA_U" , "CA5" , "MARES" , "ISO_BKG" , "CC_norm_nu", "CC_norm_nubar", "nue_numu" , "nuebar_numubar" , "CC_DIS" , "CC_Coh_C" , "CC_Coh_O" , "NC_Coh" , "NC_1gamma" , "NC_other_near" , "NC_other_far" , "SF_RFG" , "RPA_C" , "COH_BS"]

nToys = int(sys.argv[2])

flux_parms_list = []
xsec_parms_list = []
det_parms_list = []
obsNorm_parms_list = []

for i in xrange(0, nToys):
    flux_parms_list.append(inFile.Get("flux_parms_"+str(i)))
    xsec_parms_list.append(inFile.Get("xsec_parms_"+str(i)))
    det_parms_list.append(inFile.Get("det_parms_"+str(i)))
    obsNorm_parms_list.append(inFile.Get("obsNorm_parms_"+str(i)))

flux_file = TFile("/neut/data20/jmgwalker/banff-work/BANFF/inputs/flux_covariance_run1-9_v2_banff.root")
xsec_file = TFile("/neut/data20/jmgwalker/banff-work/BANFF/inputs/xsec_parameters_2018a_NOMINAL_v11_MaCH3.root")
obsNorm_file = TFile("/neut/data20/jmgwalker/banff-work/BANFF/inputs/obsNorm_2018a_v1.root")

flux_nom = []
for i in xrange(0,flux_file.Get("total_flux_cov").GetNrows()):
    flux_nom.append(1.0)
xsec_nom = xsec_file.Get("xsec_param_prior")
obsNorm_nom = obsNorm_file.Get("obsNorm_weights")

flux_hist_list = []
xsec_hist_list = []
det_hist_list = []
obsNorm_hist_list = []

for i in xrange(0,flux_parms_list[0].GetNrows()):
    flux_hist_list.append(TH1D("flux_parm_"+str(i), "flux_parm_"+str(i), 25, -1.0, 1.0))
    for j in xrange(0, nToys):
        flux_hist_list[i].Fill(flux_parms_list[j][i] - flux_nom[i])

for i in xrange(0,xsec_parms_list[0].GetNrows()):
    xsec_hist_list.append(TH1D("xsec_parm_"+str(i), param_names[i], 32, -1.0, 1.0))
    for j in xrange(0, nToys):
        xsec_hist_list[i].Fill(xsec_parms_list[j][i] - xsec_nom[i])

for i in xrange(0,det_parms_list[0].GetNrows()):
    det_hist_list.append(TH1D("det_parm_"+str(i), "det_parm_"+str(i), 50, -1.0, 1.0))
    for j in xrange(0, nToys):
        det_hist_list[i].Fill(det_parms_list[j][i])

for i in xrange(0,obsNorm_parms_list[0].GetNrows()):
    obsNorm_hist_list.append(TH1D("obsNorm_parm_"+str(i), "obsNorm_parm_"+str(i), 25, -1.0, 1.0))
    for j in xrange(0, nToys):
        obsNorm_hist_list[i].Fill(obsNorm_parms_list[j][i] - obsNorm_nom[i])


mean_hist = TH1D("mean", "thrown mean", 25, -0.01, 0.01)
flux_mean_hist = TH1D("flux_mean", "thrown flux mean", 25, -0.01, 0.01)
for hist in flux_hist_list:
    hist.Fit("gaus","Q")
    mean = hist.GetFunction("gaus").GetParameter(1)
    flux_mean_hist.Fill(mean)
    mean_hist.Fill(mean)

obsNorm_mean_hist = TH1D("obsNorm_mean", "thrown obsNorm mean", 25, -0.01, 0.01)
for hist in obsNorm_hist_list:
    hist.Fit("gaus","Q")
    mean = hist.GetFunction("gaus").GetParameter(1)
    obsNorm_mean_hist.Fill(mean)
    mean_hist.Fill(mean)

xsec_mean_hist = TH1D("xsec_mean", "thrown xsec mean", 25, -0.1, 0.1)
for ihist, hist in enumerate(xsec_hist_list):
    hist.Fit("gaus","Q")
    mean = hist.GetFunction("gaus").GetParameter(1)
    if i==2 or i==5 or i==6 or i==7 or i==8 or i==9 or i==11 or i==12 or i==17 or i==23 or i==24 or i==29 or i==31 or i==32 or i==33 or i==34:
        continue
    xsec_mean_hist.Fill(mean)
    mean_hist.Fill(mean)

c0_f = TCanvas()
c0_f.cd()
flux_mean_hist.Draw()
c0_f.Modified()
c0_f.Update()
c0_f.Print(outFile + "_flux_mean.pdf")

c0_o = TCanvas()
c0_o.cd()
obsNorm_mean_hist.Draw()
c0_o.Modified()
c0_o.Update()
c0_o.Print(outFile + "_obsNorm_mean.pdf")

c0_x = TCanvas()
c0_x.cd()
xsec_mean_hist.Draw()
c0_x.Modified()
c0_x.Update()
c0_x.Print(outFile + "_xsec_mean.pdf")

c0_a = TCanvas()
c0_a.cd()
mean_hist.Draw()
c0_a.Modified()
c0_a.Update()
c0_a.Print(outFile + "_mean.pdf")

c1 = TCanvas()
c1.cd()
c1.Print(outFile + "_flux.pdf[")
for hist in flux_hist_list:
    hist.Draw()
    c1.Modified()
    c1.Update()
    c1.Print(outFile + "_flux.pdf")
c1.Print(outFile + "_flux.pdf]")

c2 = TCanvas()
c2.cd()
c2.Print(outFile + "_xsec.pdf[")
for hist in xsec_hist_list:
    hist.Draw()
    c2.Modified()
    c2.Update()
    c2.Print(outFile + "_xsec.pdf")
c2.Print(outFile + "_xsec.pdf]")

c3 = TCanvas()
c3.cd()
c3.Print(outFile + "_det.pdf[")
for hist in det_hist_list:
    hist.Draw()
    c3.Modified()
    c3.Update()
    c3.Print(outFile + "_det.pdf")
c3.Print(outFile + "_det.pdf]")

c4 = TCanvas()
c4.cd()
c4.Print(outFile + "_obsNorm.pdf[")
for hist in obsNorm_hist_list:
    hist.Draw()
    c4.Modified()
    c4.Update()
    c4.Print(outFile + "_obsNorm.pdf")
c4.Print(outFile + "_obsNorm.pdf]")

    
