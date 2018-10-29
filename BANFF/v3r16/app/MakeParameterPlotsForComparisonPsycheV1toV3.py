'''
MakeParameterPlotsForComparisonPsycheV1toV3
Produces plots that compare the current BANFF with previous one
    In this case 2017 and 2018
Usage:
    python MakeParameterPlotsForComparisonPsycheV1toV3 \
            postfitFileWithAllParameters \ ---> BANFF processed fit file
            allParamsPreviousBANFF       \ ---> Prev BANFF processed fit file
            outputfile                   \ ---> PDF name of output file
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
if (len(sys.argv) != 4):
    print 'Usage: python MakeParameterPlotsForComparisonMaCh3 \\'
    print '\t postfitFileWithAllParameters \\'
    print '\t allParamsPreviousBANFF \\'
    print '\t outputfile.pdf'
    exit()
else:
    print 'Processing BANFF files'

# v1 ObsNorm parameters have a different order
def v1_reorder_samples(v1_vector):

    obsnorm     = []
    tmp_obsnorm = []
    for i in range(131, v1_vector.GetNrows()):
        obsnorm.append(v1_vector[i])
        tmp_obsnorm.append(-999)

    for oldx in range(0, len(obsnorm)):
        newx = -1
        if (oldx < 122):
            newx = oldx
        elif (oldx >= 122 and oldx < 198):
            newx = oldx + 122
        elif (oldx >= 198 and oldx < 278):
            newx = oldx + 198
        elif (oldx >= 278 and oldx < 400):
            newx = oldx - 156
        elif (oldx >= 400 and oldx < 476):
            newx = oldx - 80
        elif (oldx >= 476):
            newx = oldx

        if newx != -1:
            tmp_obsnorm[newx] = obsnorm[oldx]
        else:
            print 'Could not find the new x position'

    adj_obsnorm = v1_reorder_binning(tmp_obsnorm)

    adjusted_vector = []
    for i in range(0, 131):
        adjusted_vector.append(v1_vector[i])

    for i in range(131, v1_vector.GetNrows()):
        adjusted_vector.append(adj_obsnorm[i-131])

    return adjusted_vector

# Expects v3 ordering of samples
def v1_reorder_binning(v1_array):
    # Split the values up by sample
    v1_by_sample = [[],[],[],[],[],[],[],[],[],[],[],[],[],[]]
    for index in range(0, len(v1_array)):
        if (index < 42):
            v1_by_sample[0].append(v1_array[index])
        elif (index >=  42 and index <  82):
            v1_by_sample[1].append(v1_array[index])
        elif (index >=  82 and index < 122):
            v1_by_sample[2].append(v1_array[index])
        elif (index >= 122 and index < 164):
            v1_by_sample[3].append(v1_array[index])
        elif (index >= 164 and index < 204):
            v1_by_sample[4].append(v1_array[index])
        elif (index >= 204 and index < 244):
            v1_by_sample[5].append(v1_array[index])
        elif (index >= 244 and index < 284):
            v1_by_sample[6].append(v1_array[index])
        elif (index >= 284 and index < 320):
            v1_by_sample[7].append(v1_array[index])
        elif (index >= 320 and index < 360):
            v1_by_sample[8].append(v1_array[index])
        elif (index >= 360 and index < 396):
            v1_by_sample[9].append(v1_array[index])
        elif (index >= 396 and index < 436):
            v1_by_sample[10].append(v1_array[index])
        elif (index >= 436 and index < 476):
            v1_by_sample[11].append(v1_array[index])
        elif (index >= 476 and index < 516):
            v1_by_sample[12].append(v1_array[index])
        elif (index >= 516 and index < 556):
            v1_by_sample[13].append(v1_array[index])

    arranged_samples = []
    for index1 in range(0, len(v1_by_sample)):
        size = len(v1_by_sample[index1])

        nbins_mom = -1
        nbins_the = -1
        output     = []
        bin_output = []

        if (size == 42):
            nbins_mom = 6
            nbins_the = 7
            #output = [[],[],[],[],[],[],[]]
            output = [[],[],[],[],[],[]]
        elif (size == 40):
            nbins_mom = 5
            nbins_the = 8
            #output = [[],[],[],[],[],[],[],[]]
            output = [[],[],[],[],[]]
        elif (size == 36):
            nbins_mom = 6
            nbins_the = 6
            output = [[],[],[],[],[],[]]

        for index2 in range(0, size):
            #find_modulo = index2 % nbins_the
            find_modulo = index2 % nbins_mom
            output[find_modulo].append(v1_by_sample[index1][index2])

        arranged_samples.append(output)

    temporary   = []
    for sample in range(0, len(arranged_samples)):
        tmp_list = np.array(arranged_samples[sample])
        temporary.append(list(tmp_list.flatten()))

    final_list    = flatten_unequal_array(temporary)

    return final_list

def flatten_unequal_array(twod_array):
    count = 0
    oned_array = []
    for index in range(0, len(twod_array)):
        size = len(twod_array[index])
        for item in range(0, size):
            oned_array.append(twod_array[index][item])
            count += 1

    return oned_array

# Load input files
infile1 = TFile(sys.argv[1]) #v3 BANFF
infile2 = TFile(sys.argv[2]) #v1 BANFF

# Get BANFF postfit parameters, covariance, and errors
postfit_params = infile1.Get('postfit_params')
prefit_params  = infile1.Get('prefit_params')
param_list     = infile1.Get('param_list')
postfit_cov    = infile1.Get('postfit_cov')
prefit_cov     = infile1.Get('prefit_cov')
postfit_err    = TVectorD(postfit_params.GetNrows())
prefit_err     = TVectorD(prefit_params.GetNrows())

# Get previous BANFF parameters and errors
prev_params     = infile2.Get('postfit_params')
prev_pre_params = infile2.Get('prefit_params')
prev_list       = infile2.Get('param_list')
prev_cov        = infile2.Get('postfit_cov')
prev_pre_cov    = infile2.Get('prefit_cov')
prev_err        = TVectorD(prev_params.GetNrows())
prev_pre_err    = TVectorD(prev_pre_params.GetNrows())

# Filling the BANFF postfit error vector
for i in xrange(0, postfit_err.GetNrows()):
    postfit_err[i] = np.sqrt(postfit_cov[i][i])
    prefit_err[i]  = np.sqrt(prefit_cov[i][i])

# Filling the old BANFF postfit error vector
for i in xrange(0, prev_err.GetNrows()):
    prev_err[i] = np.sqrt(prev_cov[i][i])
    prev_pre_err[i] = np.sqrt(prev_pre_cov[i][i])

# The number of each type of parameter
NFLUX = 100
NOBS  = 556
NXSEC = 31
NPARAMS = NFLUX + NOBS + NXSEC

# The BANFF parameters are loaded in the order:
#     FLUX, OBSNORM, XSEC
# In order to match MaCH3, this needs to be:
#     FLUX, XSEC, OBSNORM
# Now moving the parameters around
banff_params = TVectorD(NPARAMS)
banff_errors = TVectorD(NPARAMS)
banff_prefit = TVectorD(NPARAMS)
banff_perror = TVectorD(NPARAMS)
banff_names  = TObjArray(NPARAMS)

for i in range(0, NPARAMS):
    if (i < NFLUX):
        j = i
        banff_params[j] = postfit_params[i]
        banff_errors[j] = postfit_err[i]
        banff_prefit[j] = prefit_params[i]
        banff_perror[j] = prefit_err[i]
        banff_names.AddAt(param_list.At(i), j)
    elif (NFLUX <= i) and (i < NFLUX+NOBS):
        j = i + NXSEC
        banff_params[j] = postfit_params[i]
        banff_errors[j] = postfit_err[i]
        banff_prefit[j] = prefit_params[i]
        banff_perror[j] = prefit_err[i]
        banff_names.AddAt(param_list.At(i), j)
    elif (i >= NFLUX + NOBS):
        j = i - NOBS
        banff_params[j] = postfit_params[i]
        banff_errors[j] = postfit_err[i]
        banff_prefit[j] = prefit_params[i]
        banff_perror[j] = prefit_err[i]
        banff_names.AddAt(param_list.At(i), j)

#for i in range(0, NFLUX+NXSEC):
#    print i, banff_names.At(i).GetString().Data(), banff_params[i], banff_errors[i]

# Find BANFF parameter locations
FEFQE    = -1
BERPA    = -1
CCDIS    = -1
FEFQELOC = -1
MAQELOC  = -1
BERPALOC = -1
CA5LOC   = -1
NCOTHLOC = -1
for i in xrange(0, banff_names.GetEntriesFast()):
    if banff_names[i] == 'FSI_INEL_LO':
        FEFQELOC = i
        FEFQE    = i+1
    elif banff_names[i] == 'MAQE':
        MAQELOC = i
    elif banff_names[i] == 'BeRPA_A':
        BERPALOC = i
        BERPA    = i+1
    elif banff_names[i] == 'CA5':
        CA5LOC = i
    elif banff_names[i] == 'CC_DIS':
        CCDIS = i+1
    elif banff_names[i] == 'NC_other_far':
        NCOTHLOC = i
    else:
        continue

#print FEFQELOC, MAQELOC, CA5LOC, NCOTHLOC

#for i in range(100, 131):
#    print banff_names.At(i).GetString().Data(), prev_list.At(i).GetString().Data()

old_params     = TVectorD(NPARAMS)
old_pre_params = TVectorD(NPARAMS)
old_errors     = TVectorD(NPARAMS)
old_pre_errors = TVectorD(NPARAMS)

tmp_prev_params     = v1_reorder_samples(prev_params)
tmp_prev_pre_params = v1_reorder_samples(prev_pre_params)
tmp_prev_err        = v1_reorder_samples(prev_err)
tmp_prev_pre_err    = v1_reorder_samples(prev_pre_err)

for i in range(0, NPARAMS):
    old_params[i]     = tmp_prev_params[i]
    old_pre_params[i] = tmp_prev_pre_params[i]
    old_errors[i]     = tmp_prev_err[i]
    old_pre_errors[i] = tmp_prev_pre_err[i]

# We just need to make one TH1D for each, then change the range as needed
# to get the desired parameter subset
banffHist = TH1D(banff_params)
bpreHist  = TH1D(banff_prefit)
prevHist  = TH1D(old_params)
ppreHist  = TH1D(old_pre_params)

# Set BANFF errors
for i in xrange(1, banff_errors.GetNrows()+1):
    banffHist.SetBinError(i, banff_errors[i-1])
    bpreHist.SetBinError(i, banff_perror[i-1])

# Set old BANFF errors
for i in xrange(1, old_errors.GetNrows()+1):
    prevHist.SetBinError(i, old_errors[i-1])
    ppreHist.SetBinError(i, old_pre_errors[i-1])

# Modify some of the BANFF parameters
#BeRPA_A
banffHist.SetBinContent(BERPA, banffHist.GetBinContent(BERPA)*1.69491525424)
banffHist.SetBinError(BERPA  , banffHist.GetBinError(BERPA)*1.69491525424)
bpreHist.SetBinContent(BERPA , bpreHist.GetBinContent(BERPA)*1.69491525424)
bpreHist.SetBinError(BERPA   , bpreHist.GetBinError(BERPA)*1.69491525424)
prevHist.SetBinContent(BERPA , prevHist.GetBinContent(BERPA)*1.69491525424)
prevHist.SetBinError(BERPA   , prevHist.GetBinError(BERPA)*1.69491525424)
ppreHist.SetBinContent(BERPA , ppreHist.GetBinContent(BERPA)*1.69491525424)
ppreHist.SetBinError(BERPA   , ppreHist.GetBinError(BERPA)*1.69491525424)

#BeRPA_B
banffHist.SetBinContent(BERPA+1, banffHist.GetBinContent(BERPA+1)*0.95238095238)
banffHist.SetBinError(BERPA+1  , banffHist.GetBinError(BERPA+1)*0.95238095238)
bpreHist.SetBinContent(BERPA+1 , bpreHist.GetBinContent(BERPA+1)*0.95238095238)
bpreHist.SetBinError(BERPA+1   , bpreHist.GetBinError(BERPA+1)*0.95238095238)
prevHist.SetBinContent(BERPA+1 , prevHist.GetBinContent(BERPA+1)*0.95238095238)
prevHist.SetBinError(BERPA+1   , prevHist.GetBinError(BERPA+1)*0.95238095238)
ppreHist.SetBinContent(BERPA+1 , ppreHist.GetBinContent(BERPA+1)*0.95238095238)
ppreHist.SetBinError(BERPA+1   , ppreHist.GetBinError(BERPA+1)*0.95238095238)

#BeRPA_D
banffHist.SetBinContent(BERPA+2, banffHist.GetBinContent(BERPA+2)*0.88495575221)
banffHist.SetBinError(BERPA+2  , banffHist.GetBinError(BERPA+2)*0.88495575221)
bpreHist.SetBinContent(BERPA+2 , bpreHist.GetBinContent(BERPA+2)*0.88495575221)
bpreHist.SetBinError(BERPA+2   , bpreHist.GetBinError(BERPA+2)*0.88495575221)
prevHist.SetBinContent(BERPA+2 , prevHist.GetBinContent(BERPA+2)*0.88495575221)
prevHist.SetBinError(BERPA+2   , prevHist.GetBinError(BERPA+2)*0.88495575221)
ppreHist.SetBinContent(BERPA+2 , ppreHist.GetBinContent(BERPA+2)*0.88495575221)
ppreHist.SetBinError(BERPA+2   , ppreHist.GetBinError(BERPA+2)*0.88495575221)

#BeRPA_E
banffHist.SetBinContent(BERPA+3, banffHist.GetBinContent(BERPA+3)*1.13636363636)
banffHist.SetBinError(BERPA+3  , banffHist.GetBinError(BERPA+3)*1.13636363636)
bpreHist.SetBinContent(BERPA+3 , bpreHist.GetBinContent(BERPA+3)*1.13636363636)
bpreHist.SetBinError(BERPA+3   , bpreHist.GetBinError(BERPA+3)*1.13636363636)
prevHist.SetBinContent(BERPA+3 , prevHist.GetBinContent(BERPA+3)*1.13636363636)
prevHist.SetBinError(BERPA+3   , prevHist.GetBinError(BERPA+3)*1.13636363636)
ppreHist.SetBinContent(BERPA+3 , ppreHist.GetBinContent(BERPA+3)*1.13636363636)
ppreHist.SetBinError(BERPA+3   , ppreHist.GetBinError(BERPA+3)*1.13636363636)

#BeRPA_U for both BANFF and MaCH3
banffHist.SetBinContent(BERPA+4, banffHist.GetBinContent(BERPA+4)*0.83333333333)
banffHist.SetBinError(BERPA+4  , banffHist.GetBinError(BERPA+4)*0.83333333333)
bpreHist.SetBinContent(BERPA+4 , bpreHist.GetBinContent(BERPA+4)*0.83333333333)
bpreHist.SetBinError(BERPA+4   , bpreHist.GetBinError(BERPA+4)*0.83333333333)
prevHist.SetBinContent(BERPA+4 , prevHist.GetBinContent(BERPA+4)*0.83333333333)
prevHist.SetBinError(BERPA+4   , prevHist.GetBinError(BERPA+4)*0.83333333333)
ppreHist.SetBinContent(BERPA+4 , ppreHist.GetBinContent(BERPA+4)*0.83333333333)
ppreHist.SetBinError(BERPA+4   , ppreHist.GetBinError(BERPA+4)*0.83333333333)

#CCDIS
banffHist.SetBinContent(CCDIS, banffHist.GetBinContent(CCDIS)+1)
bpreHist.SetBinContent(CCDIS , bpreHist.GetBinContent(CCDIS)+1)
prevHist.SetBinContent(CCDIS , prevHist.GetBinContent(CCDIS)+1)
ppreHist.SetBinContent(CCDIS , ppreHist.GetBinContent(CCDIS)+1)

#FSI
banffHist.SetBinContent(FEFQE,   banffHist.GetBinContent(FEFQE)   + 1)
banffHist.SetBinContent(FEFQE+1, banffHist.GetBinContent(FEFQE+1) + 1)
banffHist.SetBinContent(FEFQE+2, banffHist.GetBinContent(FEFQE+2) + 1)
banffHist.SetBinContent(FEFQE+3, banffHist.GetBinContent(FEFQE+3) + 1)
banffHist.SetBinContent(FEFQE+4, banffHist.GetBinContent(FEFQE+4) + 1)
banffHist.SetBinContent(FEFQE+5, banffHist.GetBinContent(FEFQE+5) + 1)
bpreHist.SetBinContent(FEFQE,   bpreHist.GetBinContent(FEFQE)   + 1)
bpreHist.SetBinContent(FEFQE+1, bpreHist.GetBinContent(FEFQE+1) + 1)
bpreHist.SetBinContent(FEFQE+2, bpreHist.GetBinContent(FEFQE+2) + 1)
bpreHist.SetBinContent(FEFQE+3, bpreHist.GetBinContent(FEFQE+3) + 1)
bpreHist.SetBinContent(FEFQE+4, bpreHist.GetBinContent(FEFQE+4) + 1)
bpreHist.SetBinContent(FEFQE+5, bpreHist.GetBinContent(FEFQE+5) + 1)
prevHist.SetBinContent(FEFQE,   prevHist.GetBinContent(FEFQE)   + 1)
prevHist.SetBinContent(FEFQE+1, prevHist.GetBinContent(FEFQE+1) + 1)
prevHist.SetBinContent(FEFQE+2, prevHist.GetBinContent(FEFQE+2) + 1)
prevHist.SetBinContent(FEFQE+3, prevHist.GetBinContent(FEFQE+3) + 1)
prevHist.SetBinContent(FEFQE+4, prevHist.GetBinContent(FEFQE+4) + 1)
prevHist.SetBinContent(FEFQE+5, prevHist.GetBinContent(FEFQE+5) + 1)
ppreHist.SetBinContent(FEFQE,   ppreHist.GetBinContent(FEFQE)   + 1)
ppreHist.SetBinContent(FEFQE+1, ppreHist.GetBinContent(FEFQE+1) + 1)
ppreHist.SetBinContent(FEFQE+2, ppreHist.GetBinContent(FEFQE+2) + 1)
ppreHist.SetBinContent(FEFQE+3, ppreHist.GetBinContent(FEFQE+3) + 1)
ppreHist.SetBinContent(FEFQE+4, ppreHist.GetBinContent(FEFQE+4) + 1)
ppreHist.SetBinContent(FEFQE+5, ppreHist.GetBinContent(FEFQE+5) + 1)

# Set histo types, ranges, and bin labels
histoTypes = ["ND280 NuMode Flux", "ND280 ANuMode Flux", "SK NuMode Flux", 
              "SK ANuMode Flux", "FSI", "Cross-section"]
paramRange = [[0,24],     [25,49],    [50,74],    [75,99],    [FEFQELOC,FEFQELOC+5], [MAQELOC,NCOTHLOC]]
yRange     = [[0.80,1.2], [0.80,1.2], [0.80,1.2], [0.80,1.2], [0.0,2.1],             [0,2.1],          ]
binLabels  = []

for i in xrange(0, banff_params.GetNrows()):
    binLabels.append("")

binLabels[0]  = "ND280 #nu_{#mu}, FHC"
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

for i in xrange(FEFQELOC,NCOTHLOC+1):
    binLabels[i] = banff_names.At(i).GetString().Data()

#for i in xrange(0, NCOTHLOC+1):
#    print binLabels[i]

# Set the fill types for the MaCH3 and BANFF histograms.  Will plot them as
# points, but this will propagate to the error bars.

#MaCH3 is red fill, red point that isn't seen.
ppreHist.SetFillColor(kRed)
ppreHist.SetFillColorAlpha(kRed, 0.35)
ppreHist.SetFillStyle(1001)
ppreHist.SetLineColor(kRed)
ppreHist.SetMarkerColor(kRed)
ppreHist.SetMarkerStyle(7)
ppreHist.SetMarkerSize(0)

prevHist.SetFillColor(kRed)
prevHist.SetFillStyle(3002)
prevHist.SetLineColor(kRed)
prevHist.SetMarkerColor(kRed)
prevHist.SetMarkerStyle(7)
prevHist.SetMarkerSize(0.5)

#BANFF is with blue squares
bpreHist.SetFillColor(kBlue)
bpreHist.SetFillColorAlpha(kBlue, 0.35)
bpreHist.SetFillStyle(1001)
bpreHist.SetLineColor(kBlue)
bpreHist.SetMarkerColor(kBlue)
bpreHist.SetMarkerStyle(7)
bpreHist.SetMarkerSize(0)

banffHist.SetMarkerColor(4)
banffHist.SetMarkerStyle(21)
banffHist.SetMarkerSize(0.5)
banffHist.SetLineWidth(2)

c1 = TCanvas()
c1.cd()
c1.Print(sys.argv[3] + "[")

for i in xrange(0, len(histoTypes)):

    stack = THStack("pStack", histoTypes[i])
    stack.Add(ppreHist , 'E2')
    stack.Add(bpreHist , 'E2')
    stack.Add(banffHist, 'E1')
    stack.Add(prevHist , 'E1')
    
    c1.cd()
    stack.Draw("NOSTACK")

    #print 'The histo type is: ', histoTypes[i]
    #print 'The range is: ', paramRange[i][0], paramRange[i][1]+1
    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0], paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.68,0.84,0.88,0.98)
    leg.SetFillColor(0)
    leg.AddEntry(ppreHist , 'v1 Prefit' , 'FE')
    leg.AddEntry(prevHist , 'v1 Postfit', 'ELP')
    leg.AddEntry(bpreHist , 'v3 Prefit' , 'FE')
    leg.AddEntry(banffHist, 'v3 Postfit', 'ELP')
    leg.Draw()

    for j in xrange(paramRange[i][0], paramRange[i][1]+1):
        #print binLabels[j]
        stack.GetXaxis().SetBinLabel(j+1, binLabels[j])
    
    stack.GetXaxis().LabelsOption("v")

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[3])

#histoTypes = ["SK FHC #nu_{#mu} Flux", "FHC ObsNorm", "RHC ObsNorm", "FSI parameters",      "CC0#pi parameters", "BeRPA parameters",    "CC1#pi parameters"]
#paramRange = [[50,60],                 [131,375],     [375,686],     [FEFQELOC,FEFQELOC+4], [MAQELOC,MAQELOC+7], [BERPALOC,BERPALOC+4], [CA5LOC,CA5LOC+2]]
#yRange     = [[0.80,1.2],              [0.5,1.5],     [0.5,1.5],     [0.0,2.1],             [0.5,2.1],           [0.5,2.1],             [0.5,2.1]]

histoTypes = ['SK FHC #nu_{#mu} Flux',
              'FGD1 FHC CC0#pi', 'FGD1 FHC CC1#pi', 'FGD1 FHC CCOther',
              'FGD2 FHC CC0#pi', 'FGD2 FHC CC1#pi', 'FGD2 FHC CCOther',
              'FGD1 RHC #bar{#nu}_{#mu} CC1Track', 'FGD1 RHC #bar{#nu}_{#mu} CCNTracks',
              'FGD2 RHC #bar{#nu}_{#mu} CC1Track', 'FGD2 RHC #bar{#nu}_{#mu} CCNTracks',
              'FGD1 RHC #nu_{#mu} CC1Track', 'FGD1 RHC #nu_{#mu} CCNTracks',
              'FGD2 RHC #nu_{#mu} CC1Track', 'FGD2 RHC #nu_{#mu} CCNTracks',
              'FSI Parameters', 'CC0#pi Parameters', 'BeRPA Parameters',
              'CC1#pi Parameters']
paramRange = [[50, 60],
              [131, 172], [173, 212], [213, 252],
              [253, 294], [295, 334], [335, 374],
              [375, 414], [415, 450],
              [451, 490], [491, 526],
              [527, 566], [567, 606],
              [607, 646], [647, 686],
              [FEFQELOC, FEFQELOC+5], [MAQELOC, MAQELOC+7], [BERPALOC, BERPALOC+4],
              [CA5LOC, CA5LOC+3]]
yRange = [[0.8,1.2],
          [0.5,1.5], [0.5,1.5], [0.5,1.5],
          [0.5,1.5], [0.5,1.5], [0.5,1.5],
          [0.5,1.5], [0.5,1.5],
          [0.5,1.5], [0.5,1.5],
          [0.5,1.5], [0.5,1.5],
          [0.5,1.5], [0.5,1.5],
          [0.0,2.0], [0.5,2.0], [0.5,2.0],
          [0.5,2.0]]

binLabels[50] = "0, 400 MeV"
binLabels[51] = "400, 500 Mev"
binLabels[52] = "500, 600 Mev"
binLabels[53] = "600, 700 Mev"
binLabels[54] = "700, 1000 Mev"
binLabels[55] = "1.0, 1.5 Gev"
binLabels[56] = "1.5, 2.0 Gev"
binLabels[57] = "2.0, 3.5 Gev"
binLabels[58] = "3.5, 5.0 Gev"
binLabels[59] = "5.0, 7.0 Gev"
binLabels[60] = "7.0, 30 Gev"

#binLabels[NFLUX+NXSEC+1  ] = "FGD1 CC0#pi"
#binLabels[NFLUX+NXSEC+43 ] = "FGD1 CC1#pi"
#binLabels[NFLUX+NXSEC+83 ] = "FGD1 CCOth"
#binLabels[NFLUX+NXSEC+123] = "FGD2 CC0#pi"
#binLabels[NFLUX+NXSEC+165] = "FGD2 CC1#pi"
#binLabels[NFLUX+NXSEC+205] = "FGD2 CCOth"
#binLabels[NFLUX+NXSEC+245] = "FGD1 #bar{#nu}_{#mu} CC1Tr"
#binLabels[NFLUX+NXSEC+285] = "FGD1 #bar{#nu}_{#mu} CCNTr"
#binLabels[NFLUX+NXSEC+321] = "FGD2 #bar{#nu}_{#mu} CC1Tr"
#binLabels[NFLUX+NXSEC+361] = "FGD2 #bar{#nu}_{#mu} CCNTr"
#binLabels[NFLUX+NXSEC+397] = "FGD1 #nu_{#mu} CC1Tr"
#binLabels[NFLUX+NXSEC+437] = "FGD1 #nu_{#mu} CCNTr"
#binLabels[NFLUX+NXSEC+477] = "FGD2 #nu_{#mu} CC1Tr"
#binLabels[NFLUX+NXSEC+517] = "FGD2 #nu_{#mu} CCNTr"

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

for i in xrange(0, len(histoTypes)):

    stack = THStack("pStack",histoTypes[i])
    stack.Add(ppreHist , 'E2')
    stack.Add(bpreHist , 'E2')
    stack.Add(banffHist, 'E1')
    stack.Add(prevHist , 'E1')
    
    c1.cd()
    stack.Draw("NOSTACK")

    #Set the range to the desired parameters that want (low end to topend + 1)
    stack.GetXaxis().SetRangeUser(paramRange[i][0], paramRange[i][1] + 1)
    stack.SetMinimum(yRange[i][0])
    stack.SetMaximum(yRange[i][1])

    leg = TLegend(0.68,0.84,0.88,0.98)
    leg.SetFillColor(0)
    leg.AddEntry(ppreHist , 'v1 Prefit' , 'FE')
    leg.AddEntry(prevHist , 'v1 Postfit', 'ELP')
    leg.AddEntry(bpreHist , 'v3 Prefit' , 'FE')
    leg.AddEntry(banffHist, 'v3 Postfit', 'ELP')
    leg.Draw()

    if (i >= 1) and (i <= 14):
        index = 0
        for label in xrange(paramRange[i][0], paramRange[i][1]+1):
            count = label - paramRange[i][0]
            if (i == 1) or (i == 4):
                if (count%7 == 0):
                    binLabels[label] = cc0pi_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, binLabels[label])
            elif (i == 2) or (i == 5):
                if (count%8 == 0):
                    binLabels[label] = cc1pi_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, binLabels[label])
            elif (i == 3) or (i == 6):
                if (count%8 == 0):
                    binLabels[label] = ccoth_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, binLabels[label])
            elif (i == 7) or (i == 9):
                if (count%8 == 0):
                    binLabels[label] = acc1t_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, binLabels[label])
            elif (i == 8) or (i == 10):
                if (count%6 == 0):
                    binLabels[label] = accnt_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, binLabels[label])
            elif (i == 11) or (i == 13):
                if (count%8 == 0):
                    binLabels[label] = ncc1t_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, binLabels[label])
            elif (i == 12) or (i == 14):
                if (count%8 == 0):
                    binLabels[label] = nccnt_mom_labels[index] + ' GeV'
                    index += 1
                stack.GetXaxis().SetBinLabel(label+1, binLabels[label])
            else:
                #print binLabels[label]
                stack.GetXaxis().SetBinLabel(label+1, binLabels[label])
    else:
        for label in xrange(paramRange[i][0], paramRange[i][1]+1):
            #print binLabels[label]
            stack.GetXaxis().SetBinLabel(label+1, binLabels[label])

    stack.GetXaxis().LabelsOption("v")

    c1.Modified()
    c1.Update()
    c1.Print(sys.argv[3])

c1.Print(sys.argv[3] + "]")
