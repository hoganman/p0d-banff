'''
MakeParameterPlotsForComparison
Produces plots that compare the two similar BANFF fits
    Should use the same number of parameters
    Should use the same parameter ordering also
Usage:
    python MakeParameterPlotsForComparison \
            postfitFileWithAllParameters1 \ ---> BANFF processed fit file 1
            title1                        \ ---> Legend label for file 1
            postfitFileWithAllParameters2 \ ---> BANFF processed fit file 2
            title2                        \ ---> Legend label for file 2
            outputfile                    \ ---> PDF name of output file
'''
#!/usr/bin/python
from ROOT import *
import sys
import math
import numpy as np
from array import array

gStyle.SetOptStat(0)
gStyle.SetLabelSize(0.055)
gStyle.SetPadBottomMargin(0.3)

gROOT.SetBatch(kTRUE)

# Check number of arguments
if (len(sys.argv) != 6):
    print 'Usage: python MakeParameterPlotsForComparison \\'
    print '\tpostfitFileWithAllParameters1 \\'
    print '\ttitle1 \\'
    print '\tpostfitFileWithAllParameters2 \\'
    print '\ttitle2 \\'
    print '\toutputfile.pdf'
    exit()
else:
    print 'Processing BANFF files'

# Load input files
infile1 = TFile(sys.argv[1])
infile2 = TFile(sys.argv[3])
title1  = str(sys.argv[2])
title2  = str(sys.argv[4])

# Get first set of BANFF prefit/postfit parameters, covariance, and errors
pref_1     = infile1.Get('prefit_params')
post_1     = infile1.Get('postfit_params')
pref_cov_1 = infile1.Get('prefit_cov')
post_cov_1 = infile1.Get('postfit_cov')
params_1   = infile1.Get('param_list')
pref_err_1 = TVectorD(pref_1.GetNrows())
post_err_1 = TVectorD(post_1.GetNrows())

# Get second set of BANFF prefit/postfit parameters, covariance, and errors
pref_2     = infile2.Get('prefit_params')
post_2     = infile2.Get('postfit_params')
pref_cov_2 = infile2.Get('prefit_cov')
post_cov_2 = infile2.Get('postfit_cov')
params_2   = infile2.Get('param_list')
pref_err_2 = TVectorD(pref_2.GetNrows())
post_err_2 = TVectorD(post_2.GetNrows())

# Perform a couple checks to make sure parameter order is the same 
# and number of parameters is equal
if (pref_1.GetNrows() != pref_2.GetNrows()):
    print 'There are a different number of parameters in each file'
    exit(1)
for name in range(0, pref_1.GetNrows()):
    if (params_1.At(name).GetString().Data() != params_2.At(name).GetString().Data()):
        print 'Parameter order is different'
        exit(1)
    else:
        continue

# Filling the BANFF error vectors
for i in xrange(0, pref_err_1.GetNrows()):
    pref_err_1[i] = np.sqrt(pref_cov_1[i][i])
    post_err_1[i] = np.sqrt(post_cov_1[i][i])
    pref_err_2[i] = np.sqrt(pref_cov_2[i][i])
    post_err_2[i] = np.sqrt(post_cov_2[i][i])

# Find parameter locations
# Terms followed by 'LOC' are used for paramRange
# Other terms are used for setting bin contents
FSI      = -1
BERPA    = -1
CCDIS    = -1
FLUXLOC  = -1
OBSLOC   = -1
FSILOC   = -1
MAQELOC  = -1
BERPALOC = -1
CA5LOC   = -1
NCOTHLOC = -1
for i in xrange(0, params_1.GetEntriesFast()):
    if (params_1[i] == 'NDNuModeNumu0'):
        FLUXLOC = i
    elif (params_1[i] == 'OBSNORM_0'):
        OBSLOC = i
    elif (params_1[i] == 'FSI_INEL_LO') or (params_1[i] == 'FEFQE'):
        FSILOC = i
        FSI    = i+1
    elif (params_1[i] == 'MAQE'):
        MAQELOC = i
    elif (params_1[i] == 'BeRPA_A'):
        BERPALOC = i
        BERPA    = i+1
    elif (params_1[i] == 'CA5'):
        CA5LOC = i
    elif (params_1[i] == 'CC_DIS'):
        CCDIS = i+1
    elif (params_1[i] == 'NC_other_far'):
        NCOTHLOC = i
    else:
        continue

#print FLUXLOC, OBSLOC, FSILOC, MAQELOC, BERPALOC, CA5LOC, NCOTHLOC

# We only need to make one TH1D per file and fit type
# Then, change the range as need to get the desired parameter subset
pref_hist_1 = TH1D(pref_1)
post_hist_1 = TH1D(post_1)
pref_hist_2 = TH1D(pref_2)
post_hist_2 = TH1D(post_2)

# Set histogram errors
for i in xrange(1, pref_err_1.GetNrows()+1):
    pref_hist_1.SetBinError(i, pref_err_1[i-1])
    post_hist_1.SetBinError(i, post_err_1[i-1])
    pref_hist_2.SetBinError(i, pref_err_2[i-1])
    post_hist_2.SetBinError(i, post_err_2[i-1])

# Modify some of the parameter values
# BeRPA_A
pref_hist_1.SetBinContent(BERPA, pref_hist_1.GetBinContent(BERPA)*1.69491525424)
post_hist_1.SetBinContent(BERPA, post_hist_1.GetBinContent(BERPA)*1.69491525424)
pref_hist_2.SetBinContent(BERPA, pref_hist_2.GetBinContent(BERPA)*1.69491525424)
post_hist_2.SetBinContent(BERPA, post_hist_2.GetBinContent(BERPA)*1.69491525424)
pref_hist_1.SetBinError(BERPA, pref_hist_1.GetBinError(BERPA)*1.69491525424)
post_hist_1.SetBinError(BERPA, post_hist_1.GetBinError(BERPA)*1.69491525424)
pref_hist_2.SetBinError(BERPA, pref_hist_2.GetBinError(BERPA)*1.69491525424)
post_hist_2.SetBinError(BERPA, post_hist_2.GetBinError(BERPA)*1.69491525424)

# BeRPA_B
pref_hist_1.SetBinContent(BERPA+1, pref_hist_1.GetBinContent(BERPA+1)*0.95238095238)
post_hist_1.SetBinContent(BERPA+1, post_hist_1.GetBinContent(BERPA+1)*0.95238095238)
pref_hist_2.SetBinContent(BERPA+1, pref_hist_2.GetBinContent(BERPA+1)*0.95238095238)
post_hist_2.SetBinContent(BERPA+1, post_hist_2.GetBinContent(BERPA+1)*0.95238095238)
pref_hist_1.SetBinError(BERPA+1, pref_hist_1.GetBinError(BERPA+1)*0.95238095238)
post_hist_1.SetBinError(BERPA+1, post_hist_1.GetBinError(BERPA+1)*0.95238095238)
pref_hist_2.SetBinError(BERPA+1, pref_hist_2.GetBinError(BERPA+1)*0.95238095238)
post_hist_2.SetBinError(BERPA+1, post_hist_2.GetBinError(BERPA+1)*0.95238095238)

# BeRPA_D
pref_hist_1.SetBinContent(BERPA+2, pref_hist_1.GetBinContent(BERPA+2)*0.88495575221)
post_hist_1.SetBinContent(BERPA+2, post_hist_1.GetBinContent(BERPA+2)*0.88495575221)
pref_hist_2.SetBinContent(BERPA+2, pref_hist_2.GetBinContent(BERPA+2)*0.88495575221)
post_hist_2.SetBinContent(BERPA+2, post_hist_2.GetBinContent(BERPA+2)*0.88495575221)
pref_hist_1.SetBinError(BERPA+2, pref_hist_1.GetBinError(BERPA+2)*0.88495575221)
post_hist_1.SetBinError(BERPA+2, post_hist_1.GetBinError(BERPA+2)*0.88495575221)
pref_hist_2.SetBinError(BERPA+2, pref_hist_2.GetBinError(BERPA+2)*0.88495575221)
post_hist_2.SetBinError(BERPA+2, post_hist_2.GetBinError(BERPA+2)*0.88495575221)

# BeRPA_E
pref_hist_1.SetBinContent(BERPA+3, pref_hist_1.GetBinContent(BERPA+3)*1.13636363636)
post_hist_1.SetBinContent(BERPA+3, post_hist_1.GetBinContent(BERPA+3)*1.13636363636)
pref_hist_2.SetBinContent(BERPA+3, pref_hist_2.GetBinContent(BERPA+3)*1.13636363636)
post_hist_2.SetBinContent(BERPA+3, post_hist_2.GetBinContent(BERPA+3)*1.13636363636)
pref_hist_1.SetBinError(BERPA+3, pref_hist_1.GetBinError(BERPA+3)*1.13636363636)
post_hist_1.SetBinError(BERPA+3, post_hist_1.GetBinError(BERPA+3)*1.13636363636)
pref_hist_2.SetBinError(BERPA+3, pref_hist_2.GetBinError(BERPA+3)*1.13636363636)
post_hist_2.SetBinError(BERPA+3, post_hist_2.GetBinError(BERPA+3)*1.13636363636)

# BeRPA_U
pref_hist_1.SetBinContent(BERPA+4, pref_hist_1.GetBinContent(BERPA+4)*0.83333333333)
post_hist_1.SetBinContent(BERPA+4, post_hist_1.GetBinContent(BERPA+4)*0.83333333333)
pref_hist_2.SetBinContent(BERPA+4, pref_hist_2.GetBinContent(BERPA+4)*0.83333333333)
post_hist_2.SetBinContent(BERPA+4, post_hist_2.GetBinContent(BERPA+4)*0.83333333333)
pref_hist_1.SetBinError(BERPA+4, pref_hist_1.GetBinError(BERPA+4)*0.83333333333)
post_hist_1.SetBinError(BERPA+4, post_hist_1.GetBinError(BERPA+4)*0.83333333333)
pref_hist_2.SetBinError(BERPA+4, pref_hist_2.GetBinError(BERPA+4)*0.83333333333)
post_hist_2.SetBinError(BERPA+4, post_hist_2.GetBinError(BERPA+4)*0.83333333333)

# CCDIS
pref_hist_1.SetBinContent(CCDIS, pref_hist_1.GetBinContent(CCDIS)+1)
post_hist_1.SetBinContent(CCDIS, post_hist_1.GetBinContent(CCDIS)+1)
pref_hist_2.SetBinContent(CCDIS, pref_hist_2.GetBinContent(CCDIS)+1)
post_hist_2.SetBinContent(CCDIS, post_hist_2.GetBinContent(CCDIS)+1)

#FSI
pref_hist_1.SetBinContent(FSI,   pref_hist_1.GetBinContent(FSI)   + 1)
pref_hist_1.SetBinContent(FSI+1, pref_hist_1.GetBinContent(FSI+1) + 1)
pref_hist_1.SetBinContent(FSI+2, pref_hist_1.GetBinContent(FSI+2) + 1)
pref_hist_1.SetBinContent(FSI+3, pref_hist_1.GetBinContent(FSI+3) + 1)
pref_hist_1.SetBinContent(FSI+4, pref_hist_1.GetBinContent(FSI+4) + 1)
pref_hist_1.SetBinContent(FSI+5, pref_hist_1.GetBinContent(FSI+5) + 1)
post_hist_1.SetBinContent(FSI,   post_hist_1.GetBinContent(FSI)   + 1)
post_hist_1.SetBinContent(FSI+1, post_hist_1.GetBinContent(FSI+1) + 1)
post_hist_1.SetBinContent(FSI+2, post_hist_1.GetBinContent(FSI+2) + 1)
post_hist_1.SetBinContent(FSI+3, post_hist_1.GetBinContent(FSI+3) + 1)
post_hist_1.SetBinContent(FSI+4, post_hist_1.GetBinContent(FSI+4) + 1)
post_hist_1.SetBinContent(FSI+5, post_hist_1.GetBinContent(FSI+5) + 1)
pref_hist_2.SetBinContent(FSI,   pref_hist_2.GetBinContent(FSI)   + 1)
pref_hist_2.SetBinContent(FSI+1, pref_hist_2.GetBinContent(FSI+1) + 1)
pref_hist_2.SetBinContent(FSI+2, pref_hist_2.GetBinContent(FSI+2) + 1)
pref_hist_2.SetBinContent(FSI+3, pref_hist_2.GetBinContent(FSI+3) + 1)
pref_hist_2.SetBinContent(FSI+4, pref_hist_2.GetBinContent(FSI+4) + 1)
pref_hist_2.SetBinContent(FSI+5, pref_hist_2.GetBinContent(FSI+5) + 1)
post_hist_2.SetBinContent(FSI,   post_hist_2.GetBinContent(FSI)   + 1)
post_hist_2.SetBinContent(FSI+1, post_hist_2.GetBinContent(FSI+1) + 1)
post_hist_2.SetBinContent(FSI+2, post_hist_2.GetBinContent(FSI+2) + 1)
post_hist_2.SetBinContent(FSI+3, post_hist_2.GetBinContent(FSI+3) + 1)
post_hist_2.SetBinContent(FSI+4, post_hist_2.GetBinContent(FSI+4) + 1)
post_hist_2.SetBinContent(FSI+5, post_hist_2.GetBinContent(FSI+5) + 1)

# Set hist names, ranges, and bin labels
hist_name = ['ND280 NuMode Flux', 'ND280 ANuMode Flux',
             'SK NuMode Flux', 'SK ANuMode Flux',
             'FSI', 'Cross Section']
param_range = [[FLUXLOC,FLUXLOC+25], [FLUXLOC+25,FLUXLOC+50],
               [FLUXLOC+50,FLUXLOC+75], [FLUXLOC+75,FLUXLOC+100],
               [FSILOC,FSILOC+6], [MAQELOC,NCOTHLOC+1]]
y_range = [[0.8,1.2], [0.8,1.2],
           [0.8,1.2], [0.8,1.2],
           [0.0,2.0], [0.0,2.0]]

bin_labels = []
SKFHCNUMU = -1
for i in xrange(0, params_1.GetEntriesFast()):
    bin_labels.append('')
    if (params_1[i] == 'NDNuModeNumu0'):
        bin_labels[i] = 'ND280 FHC #nu_{#mu}'
    elif (params_1[i] == 'NDNuModeNumub0'):
        bin_labels[i] = 'ND280 FHC #bar{#nu}_{#mu}'
    elif (params_1[i] == 'NDNuModeNue0'):
        bin_labels[i] = 'ND280 FHC #nu_{e}'
    elif (params_1[i] == 'NDNuModeNueb0'):
        bin_labels[i] = 'ND280 FHC #bar{#nu}_{e}'
    elif (params_1[i] == 'NDANuModeNumu0'):
        bin_labels[i] = 'ND280 RHC #nu_{#mu}'
    elif (params_1[i] == 'NDANuModeNumub0'):
        bin_labels[i] = 'ND280 RHC #bar{#nu}_{#mu}'
    elif (params_1[i] == 'NDANuModeNue0'):
        bin_labels[i] = 'ND280 RHC #nu_{e}'
    elif (params_1[i] == 'NDANuModeNueb0'):
        bin_labels[i] = 'ND280 RHC #bar{#nu}_{e}'
    elif (params_1[i] == 'SKNuModeNumu0'):
        SKFHCNUMU = i
        bin_labels[i] = 'SK FHC #nu_{#mu}'
    elif (params_1[i] == 'SKNuModeNumub0'):
        bin_labels[i] = 'SK FHC #bar{#nu}_{#mu}'
    elif (params_1[i] == 'SKNuModeNue0'):
        bin_labels[i] = 'SK FHC #nu_{e}'
    elif (params_1[i] == 'SKNuModeNueb0'):
        bin_labels[i] = 'SK FHC #bar{#nu}_{e}'
    elif (params_1[i] == 'SKANuModeNumu0'):
        bin_labels[i] = 'SK RHC #nu_{#mu}'
    elif (params_1[i] == 'SKANuModeNumub0'):
        bin_labels[i] = 'SK RHC #bar{#nu}_{#mu}'
    elif (params_1[i] == 'SKANuModeNue0'):
        bin_labels[i] = 'SK RHC #nu_{e}'
    elif (params_1[i] == 'SKANuModeNueb0'):
        bin_labels[i] = 'SK RHC #bar{#nu}_{e}'

for i in xrange(FSILOC,NCOTHLOC+1):
    bin_labels[i] = params_1.At(i).GetString().Data()
    #print bin_labels[i]

# Set the fill types for each of the histograms
# File 1 is red
pref_hist_1.SetFillColorAlpha(kRed, 0.35)
pref_hist_1.SetFillStyle(1001)
pref_hist_1.SetLineColor(kRed)
pref_hist_1.SetMarkerColor(kRed)
pref_hist_1.SetMarkerStyle(7)
pref_hist_1.SetMarkerSize(0)

post_hist_1.SetFillColor(kRed)
post_hist_1.SetFillStyle(3002)
post_hist_1.SetLineColor(kRed)
post_hist_1.SetMarkerColor(kRed)
post_hist_1.SetMarkerStyle(7)
post_hist_1.SetMarkerSize(0.5)

# File 2 is blue
pref_hist_2.SetFillColorAlpha(kBlue, 0.35)
pref_hist_2.SetFillStyle(1001)
pref_hist_2.SetLineColor(kBlue)
pref_hist_2.SetMarkerColor(kBlue)
pref_hist_2.SetMarkerStyle(7)
pref_hist_2.SetMarkerSize(0)

post_hist_2.SetFillColor(kBlue)
post_hist_2.SetFillStyle(3002)
post_hist_2.SetLineColor(kBlue)
post_hist_2.SetMarkerColor(kBlue)
post_hist_2.SetMarkerStyle(7)
post_hist_2.SetMarkerSize(0.5)

c1 = TCanvas()
c1.cd()
c1.Print(sys.argv[5] + '[')

for hist in xrange(0, len(hist_name)):
    stack = THStack('pStack', hist_name[hist])
    stack.Add(pref_hist_1, 'E2')
    stack.Add(pref_hist_2, 'E2')
    stack.Add(post_hist_1, 'E1')
    stack.Add(post_hist_2, 'E1')

    c1.cd()
    stack.Draw('NOSTACK')

    # Set the range to the desired parameters
    stack.GetXaxis().SetRangeUser(param_range[hist][0], param_range[hist][1])
    stack.SetMinimum(y_range[hist][0])
    stack.SetMaximum(y_range[hist][1])

    leg = TLegend(0.68,0.84,0.88,0.98)
    leg.SetFillColor(0)
    leg.AddEntry(pref_hist_1, title1 + ' Prefit' , 'FE')
    leg.AddEntry(post_hist_1, title1 + ' Postfit', 'ELP')
    leg.AddEntry(pref_hist_2, title2 + ' Prefit' , 'FE')
    leg.AddEntry(post_hist_2, title2 + ' Postfit', 'ELP')
    leg.Draw()

    for label in xrange(param_range[hist][0], param_range[hist][1]):
        #print bin_labels[label]
        stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])

    stack.GetXaxis().LabelsOption('v')

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[5])

# v3 ordering of samples
hist_name = ['SK FHC #nu_{#mu} Flux',
             'FGD1 FHC CC0#pi', 'FGD1 FHC CC1#pi', 'FGD1 FHC CCOther',
             'FGD2 FHC CC0#pi', 'FGD2 FHC CC1#pi', 'FGD2 FHC CCOther',
             'FGD1 RHC #bar{#nu}_{#mu} CC1Track', 'FGD1 RHC #bar{#nu}_{#mu} CCNTracks',
             'FGD2 RHC #bar{#nu}_{#mu} CC1Track', 'FGD2 RHC #bar{#nu}_{#mu} CCNTracks',
             'FGD1 RHC #nu_{#mu} CC1Track', 'FGD1 RHC #nu_{#mu} CCNTracks',
             'FGD2 RHC #nu_{#mu} CC1Track', 'FGD2 RHC #nu_{#mu} CCNTracks',
             'FSI Parameters', 'CC0#pi Parameters', 'BeRPA Parameters',
             'CC1#pi Parameters']
param_range = [[FLUXLOC+50, FLUXLOC+61],
               [OBSLOC    , OBSLOC+ 42], [OBSLOC+ 42, OBSLOC+ 82], [OBSLOC+ 82, OBSLOC+122],
               [OBSLOC+122, OBSLOC+164], [OBSLOC+164, OBSLOC+204], [OBSLOC+204, OBSLOC+244],
               [OBSLOC+244, OBSLOC+284], [OBSLOC+284, OBSLOC+320],
               [OBSLOC+320, OBSLOC+360], [OBSLOC+360, OBSLOC+396],
               [OBSLOC+396, OBSLOC+436], [OBSLOC+436, OBSLOC+476],
               [OBSLOC+476, OBSLOC+516], [OBSLOC+516, OBSLOC+556],
               [FSILOC, FSILOC+6], [MAQELOC, MAQELOC+8], [BERPALOC, BERPALOC+5],
               [CA5LOC, CA5LOC+3]]
# v1 ordering of samples
#hist_name = ['SK FHC #nu_{#mu} Flux',
#             'FGD1 FHC CC0#pi', 'FGD1 FHC CC1#pi', 'FGD1 FHC CCOther',
#             'FGD1 RHC #bar{#nu}_{#mu} CC1Track', 'FGD1 RHC #bar{#nu}_{#mu} CCNTracks',
#             'FGD1 RHC #nu_{#mu} CC1Track', 'FGD1 RHC #nu_{#mu} CCNTracks',
#             'FGD2 FHC CC0#pi', 'FGD2 FHC CC1#pi', 'FGD2 FHC CCOther',
#             'FGD2 RHC #bar{#nu}_{#mu} CC1Track', 'FGD2 RHC #bar{#nu}_{#mu} CCNTracks',
#             'FGD2 RHC #nu_{#mu} CC1Track', 'FGD2 RHC #nu_{#mu} CCNTracks',
#             'FSI Parameters', 'CC0#pi Parameters', 'BeRPA Parameters',
#             'CC1#pi Parameters']
#param_range = [[FLUXLOC+50, FLUXLOC+61],
#               [OBSLOC    , OBSLOC+ 42], [OBSLOC+ 42, OBSLOC+ 82], [OBSLOC+ 82, OBSLOC+122],
#               [OBSLOC+122, OBSLOC+162], [OBSLOC+162, OBSLOC+198],
#               [OBSLOC+198, OBSLOC+238], [OBSLOC+238, OBSLOC+278],
#               [OBSLOC+278, OBSLOC+320], [OBSLOC+320, OBSLOC+360], [OBSLOC+360, OBSLOC+400],
#               [OBSLOC+400, OBSLOC+440], [OBSLOC+440, OBSLOC+476],
#               [OBSLOC+476, OBSLOC+516], [OBSLOC+516, OBSLOC+556],
#               [FSILOC, FSILOC+6], [MAQELOC, MAQELOC+8], [BERPALOC, BERPALOC+5],
#               [CA5LOC, CA5LOC+3]]
y_range = [[0.8,1.2],
           [0.5,1.5], [0.5,1.5], [0.5,1.5],
           [0.5,1.5], [0.5,1.5], [0.5,1.5],
           [0.5,1.5], [0.5,1.5],
           [0.5,1.5], [0.5,1.5],
           [0.5,1.5], [0.5,1.5],
           [0.5,1.5], [0.5,1.5],
           [0.0,2.0], [0.5,2.0], [0.5,2.0],
           [0.5,2.0]]

bin_labels[SKFHCNUMU   ] = '0, 400 MeV'
bin_labels[SKFHCNUMU+ 1] = '400, 500 MeV'
bin_labels[SKFHCNUMU+ 2] = '500, 600 MeV'
bin_labels[SKFHCNUMU+ 3] = '600, 700 MeV'
bin_labels[SKFHCNUMU+ 4] = '700, 1000 MeV'
bin_labels[SKFHCNUMU+ 5] = '1.0, 1.5 GeV'
bin_labels[SKFHCNUMU+ 6] = '1.5, 2.0 GeV'
bin_labels[SKFHCNUMU+ 7] = '2.0, 3.5 GeV'
bin_labels[SKFHCNUMU+ 8] = '3.5, 5.0 GeV'
bin_labels[SKFHCNUMU+ 9] = '5.0, 7.0 GeV'
bin_labels[SKFHCNUMU+10] = '7.0, 30 GeV'

#bin_labels[OBSLOC    ] = "FGD1 CC0#pi"
#bin_labels[OBSLOC+ 42] = "FGD1 CC1#pi"
#bin_labels[OBSLOC+ 82] = "FGD1 CCOth"
#bin_labels[OBSLOC+122] = "FGD2 CC0#pi"
#bin_labels[OBSLOC+164] = "FGD2 CC1#pi"
#bin_labels[OBSLOC+204] = "FGD2 CCOth"
#bin_labels[OBSLOC+244] = "FGD1 #bar{#nu}_{#mu} CC1Tr"
#bin_labels[OBSLOC+284] = "FGD1 #bar{#nu}_{#mu} CCNTr"
#bin_labels[OBSLOC+320] = "FGD2 #bar{#nu}_{#mu} CC1Tr"
#bin_labels[OBSLOC+360] = "FGD2 #bar{#nu}_{#mu} CCNTr"
#bin_labels[OBSLOC+396] = "FGD1 #nu_{#mu} CC1Tr"
#bin_labels[OBSLOC+436] = "FGD1 #nu_{#mu} CCNTr"
#bin_labels[OBSLOC+476] = "FGD2 #nu_{#mu} CC1Tr"
#bin_labels[OBSLOC+516] = "FGD2 #nu_{#mu} CCNTr"

cc0pi_mom_labels = ['0.0, 1.0','1.0, 1.25','1.25, 2.0','2.0, 3.0','3.0, 5.0','5.0, 30']
cc0pi_cos_labels = ['-1.0, 0.6','0.6, 0.7','0.7, 0.8','0.8, 0.85','0.85, 0.94','0.94, 0.96','0.96, 1.0']

cc1pi_mom_labels = ['0.0, 0.3', '0.3, 1.25', '1.25, 1.5', '1.5, 5.0', '5.0, 30']
cc1pi_cos_labels = ['-1.0, 0.7', '0.7, 0.85', '0.85, 0.9', '0.9, 0.92', '0.92, 0.96', '0.96, 0.98', '0.98, 0.99', '0.99, 1.0']

ccoth_mom_labels = ['0.0, 1.5', '1.5, 2.0', '2.0, 3.0', '3.0, 5.0', '5.0, 30']
ccoth_cos_labels = ['-1.0, 0.8', '0.8, 0.85', '0.85, 0.9', '0.9, 0.92', '0.92, 0.96', '0.96, 0.98', '0.98, 0.99', '0.99, 1.0']

acc1t_mom_labels = ['0.0, 0.4', '0.4, 0.9', '0.9, 1.1', '1.1, 2.0', '2.0, 10']
acc1t_cos_labels = ['-1.0, 0.6', '0.6, 0.7', '0.7, 0.88', '0.88, 0.95', '0.95, 0.97', '0.97, 0.98', '0.98, 0.99', '0.99, 1.0']

accnt_mom_labels = ['0.0, 0.7', '0.7, 1.2', '1.2, 1.5', '1.5, 2.0', '2.0, 3.0', '3.0, 10']
accnt_cos_labels = ['-1.0, 0.85', '0.85, 0.88', '0.88, 0.93', '0.93, 0.98', '0.98, 0.99', '0.99, 1.0']

ncc1t_mom_labels = ['0.0, 0.4', '0.4, 0.8', '0.8, 1.1', '1.1, 2.0', '2.0, 10']
ncc1t_cos_labels = ['-1.0, 0.7', '0.7, 0.85', '0.85, 0.9', '0.9, 0.93', '0.93, 0.96', '0.96, 0.98', '0.98, 0.99', '0.99, 1.0']

nccnt_mom_labels = ['0.0, 1.0', '1.0, 1.5', '1.5, 2.0', '2.0, 3.0', '3.0, 10']
nccnt_cos_labels = ['-1.0, 0.8', '0.8, 0.9', '0.9, 0.93', '0.93, 0.95', '0.95, 0.96', '0.96, 0.97', '0.97, 0.99', '0.99, 1.0']

c1.cd()

for hist in xrange(0, len(hist_name)):
    stack = THStack('pStack', hist_name[hist])
    stack.Add(pref_hist_1, 'E2')
    stack.Add(pref_hist_2, 'E2')
    stack.Add(post_hist_1, 'E1')
    stack.Add(post_hist_2, 'E1')

    c1.cd()
    stack.Draw('NOSTACK')

    # Set the range to the desired parameters
    stack.GetXaxis().SetRangeUser(param_range[hist][0], param_range[hist][1])
    stack.SetMinimum(y_range[hist][0])
    stack.SetMaximum(y_range[hist][1])

    leg = TLegend(0.68,0.84,0.88,0.98)
    leg.SetFillColor(0)
    leg.AddEntry(pref_hist_1, title1 + ' Prefit' , 'FE')
    leg.AddEntry(post_hist_1, title1 + ' Postfit', 'ELP')
    leg.AddEntry(pref_hist_2, title2 + ' Prefit' , 'FE')
    leg.AddEntry(post_hist_2, title2 + ' Postfit', 'ELP')
    leg.Draw()

    # v3 sample ordering
    if (hist >= 1) and (hist <= 14):
        index = 0
        for label in xrange(param_range[hist][0], param_range[hist][1]):
            count = label - param_range[hist][0]
            if (hist == 1) or (hist == 4):
                if (count%7 == 0):
                    bin_labels[label] = cc0pi_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
            elif (hist == 2) or (hist == 5):
                if (count%8 == 0):
                    bin_labels[label] = cc1pi_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
            elif (hist == 3) or (hist == 6):
                if (count%8 == 0):
                    bin_labels[label] = ccoth_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
            elif (hist == 7) or (hist == 9):
                if (count%8 == 0):
                    bin_labels[label] = acc1t_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
            elif (hist == 8) or (hist == 10):
                if (count%6 == 0):
                    bin_labels[label] = accnt_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
            elif (hist == 11) or (hist == 13):
                if (count%8 == 0):
                    bin_labels[label] = ncc1t_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
            elif (hist == 12) or (hist == 14):
                if (count%8 == 0):
                    bin_labels[label] = nccnt_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
            else:
                #print bin_labels[label]
                stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
    # v1 sample ordering
    #if (hist >= 1) and (hist <= 14):
    #    index = 0
    #    for label in xrange(param_range[hist][0], param_range[hist][1]):
    #        count = label - param_range[hist][0]
    #        if (hist == 1) or (hist == 8):
    #            if (count%6 == 0):
    #                bin_labels[label] = cc0pi_cos_labels[index]
    #                index += 1
    #            stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
    #        elif (hist == 2) or (hist == 9):
    #            if (count%5 == 0):
    #                bin_labels[label] = cc1pi_cos_labels[index]
    #                index += 1
    #            stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
    #        elif (hist == 3) or (hist == 10):
    #            if (count%5 == 0):
    #                bin_labels[label] = ccoth_cos_labels[index]
    #                index += 1
    #            stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
    #        elif (hist == 4) or (hist == 11):
    #            if (count%5 == 0):
    #                bin_labels[label] = acc1t_cos_labels[index]
    #                index += 1
    #            stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
    #        elif (hist == 5) or (hist == 12):
    #            if (count%6 == 0):
    #                bin_labels[label] = accnt_cos_labels[index]
    #                index += 1
    #            stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
    #        elif (hist == 6) or (hist == 13):
    #            if (count%5 == 0):
    #                bin_labels[label] = ncc1t_cos_labels[index]
    #                index += 1
    #            stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
    #        elif (hist == 7) or (hist == 14):
    #            if (count%5 == 0):
    #                bin_labels[label] = nccnt_cos_labels[index]
    #                index += 1
    #            stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
    #        else:
    #            #print bin_labels[label]
    #            stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])
    else:
        for label in xrange(param_range[hist][0], param_range[hist][1]):
            #print bin_labels[label]
            stack.GetXaxis().SetBinLabel(label+1, bin_labels[label])

    stack.GetXaxis().LabelsOption('v')

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[5])

c1.Print(sys.argv[5] + ']')
