#!/usr/bin/python
from ROOT import *
import sys
import math


#Goes through a LLH scan output file from BANFF (Minuit2LikelihoodFit::ScanParameters()) and plot the Likelihood values for each parameters.

gStyle.SetOptStat(0)
gStyle.SetPadBottomMargin(0.3)
gStyle.SetLabelSize(0.055)

gROOT.SetBatch(kTRUE);

inFile = TFile(sys.argv[1])
outfile = "scan_out.pdf"

c1 = TCanvas()
c1.cd()
c1.Print(outfile + "[")

def rescaleaxis(g,scale=50e-12/1e-9):
    """This function rescales the x-axis on a TGraph."""
    N = g.GetN()
    x = g.GetX()
    for i in range(N):
        x[i] *= scale
    g.GetHistogram().Delete()
    g.SetHistogram(0)
    return
    
#parameters = ["MAQE_scan","pF_C_scan","pF_O_scan","2p2h_shape_C_nu_scan","2p2h_shape_C_nubar_scan","2p2h_shape_O_nu_scan", "2p2h_shape_O_nubar_scan","BeRPA_A_scan","BeRPA_B_scan","BeRPA_D_scan","BeRPA_E_scan","CA5_scan","MARES_scan","ISO_BKG_scan","CC_DIS_scan"]

parameters = ["FSI_INEL_LO_scan" , "FSI_INEL_HI_scan" , "FSI_PI_PROD_scan" , "FSI_PI_ABS_scan" , "FSI_CEX_LO_scan" , "FSI_CEX_HI_scan" , "MAQE_scan" , "pF_C_scan" , "pF_O_scan" , "2p2h_norm_nu_scan" , "2p2h_norm_nubar_scan" , "2p2h_normCtoO_scan" , "2p2h_normCtoO_scan" , "2p2h_shape_C_scan" , "2p2h_shape_O_scan" , "BeRPA_A_scan" , "BeRPA_B_scan" , "BeRPA_D_scan" , "BeRPA_E_scan" , "BeRPA_U_scan" , "CA5_scan" , "MARES_scan" , "ISO_BKG_scan" , "nue_numu_scan" , "nuebar_numubar_scan" , "CC_DIS_scan" , "CC_Coh_C_scan" , "CC_Coh_O_scan" , "NC_Coh_scan" , "NC_1gamma_scan" , "NC_other_near_scan" , "NC_other_far_scan" , "SF_RFG_scan" , "RPA_C_scan" , "COH_BS_scan"]

neutNom = []

for i in range(len(parameters)):
    neutNom.append(1)
    
"""neutNom[6] = 1.2 #MAQE
neutNom[7] = 217 #pF_C
neutNom[8] = 225 #pF_O 
neutNom[19] = 1.01 #CA5
neutNom[20] = 0.95 #MARES
neutNom[21] = 1.30 #BgSclRes"""

for i in range(len(parameters)):
    scan = inFile.Get(parameters[i])
    #rescaleaxis(scan,neutNom[i])
    scan.SetTitle(parameters[i])
    scan.SetLineColor(kBlue)
    scan.SetMarkerColor(kRed)
        
    scan.Draw("AC*")
    c1.Modified()
    c1.Update()
    c1.Print(outfile)

c1.Print(outfile + "]")
