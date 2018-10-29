#!/usr/bin/env python2.7
"""
A script that makes histogram of the resulting TTree from
   RunSyst_New.exe
"""

from os import getenv
from os.path import join
import ROOT
from ROOT import TCut, TLegend, gSystem
from ROOTHStack import ROOTHStack
import sys

# toggle these to draw particular variables
DRAW_ENU = 0
DRAW_PMU = 1
DRAW_THETAMU = 0
DRAW_COSTHETAMU = 0
DRAW_PMU_TN328 = 0
DRAW_COSTHETAMU_TN328 = 0
DRAW_P0DX = 0
DRAW_P0DY = 0
DRAW_P0DZ = 0

# Apply event + flux weight
APPLY_FLUX_WEIGHTS = 0
APPLY_EVENT_WEIGHTS = 0

PLOTLEPTONCANDIDATETRUEPDG = 1
PLOTNEUTNUREACTIONCODES = 0
PLOTNEUTANTINUREACTIONCODES = 0
PLOTTOPOLOGY = 0

# Display the ratio of Data/MC below histogram
SHOW_RATIO_PLOT_BELOW = 1

# use the TN-208 runs
TN208_ANALYSIS = 0

# cut the measured momentum
USE_MOMENTUM_CUT = 0
MOMENTUM_CUT_VALUE = '5000.'

# if the user wants to apply an addition set of cuts
ADDITIONAL_CUTS = None

# Which selection to run
RUNP0DWATERNUMUCCSELECTION = 0
RUNP0DWATERNUMUBARINANTINUMODECCSELECTION = 0
RUNP0DWATERNUMUBKGINANTINUMODECCSELECTION = 0
RUNP0DAIRNUMUCCSELECTION = 1
RUNP0DAIRNUMUBARINANTINUMODECCSELECTION = 0
RUNP0DAIRNUMUBKGINANTINUMODECCSELECTION = 0

# This sets the SampleId throughout the macro
SELECTIONSAVENAME = str()
SELECTIONSAVENAMEDICT = dict()
SELECTIONLABELSDICT = dict()

# P0DBANFFInterface class to make plots pretty
STACK_COLORS = None
BLACK = int(1)

# Classes from P0DBANFF library
ENGINE = None
INTERFACE = None
SAMPLEIDS = None
T2KPOT = None
T2KDATAMC = None
XML = None
HEPCONSTANTS = None
CUTS = None


def main(argv):
    """main"""
    helpstatement = "plotRunSyst_NewResults.py (no args)"
    if len(argv) > 0:
        print helpstatement
    p0dbanffroot = getenv('P0DBANFFROOT')
    LoadP0DBANFF()
    LoadSampleIDs()

    binningLocation = '%s/config/Binning.xml' % p0dbanffroot
    cosThetaMu_AnaBins = ROOT.AnalysisBins('CosTheta', binningLocation, XML)
    # cosThetaMu_AnaBins = ROOT.AnalysisBins('P0DFitFHCCosTheta', binningLocation, XML)
    Enu_AnaBins = ROOT.AnalysisBins('NeutrinoEnergy', binningLocation, XML)
    pMu_TN328_AnaBins = ROOT.AnalysisBins('TN328Momentum', binningLocation, XML)
    pMu_AnaBins = ROOT.AnalysisBins('Momentum', binningLocation, XML)
    # pMu_AnaBins = ROOT.AnalysisBins('P0DFitFHCMomentum', binningLocation, XML)
    cosThetaMu_TN328_AnaBins = ROOT.AnalysisBins('TN328CosTheta', binningLocation, XML)
    thetaMu_AnaBins = ROOT.AnalysisBins('Theta', binningLocation, XML)
    p0dZ_AnaBins = ROOT.AnalysisBins('P0DuleCoarseZ', binningLocation, XML)
    p0dX_AnaBins = ROOT.AnalysisBins('P0DPositionX', binningLocation, XML)
    p0dY_AnaBins = ROOT.AnalysisBins('P0DPositionY', binningLocation, XML)

    evts_p_bin = 'Events / bin'
    mc_data_sample_dict_MC_key = 'MC'
    mc_data_sample_dict_DATA_key = 'DATA'

    for current_sampleID in SELECTIONLABELSDICT.keys():
        global SELECTIONSAVENAME
        if current_sampleID == SAMPLEIDS.GetP0DWaterNuMuCC() and \
                not RUNP0DWATERNUMUCCSELECTION:
            continue
        if current_sampleID == SAMPLEIDS.GetP0DWaterNuMuBarInAntiNuModeCC() \
                and not RUNP0DWATERNUMUBARINANTINUMODECCSELECTION:
            continue
        if current_sampleID == SAMPLEIDS.GetP0DWaterNuMuBkgInAntiNuModeCC() \
                and not RUNP0DWATERNUMUBKGINANTINUMODECCSELECTION:
            continue
        if current_sampleID == SAMPLEIDS.GetP0DAirNuMuCC() and \
                not RUNP0DAIRNUMUCCSELECTION:
            continue
        if current_sampleID == SAMPLEIDS.GetP0DAirNuMuBarInAntiNuModeCC() and \
                not RUNP0DAIRNUMUBARINANTINUMODECCSELECTION:
            continue
        if current_sampleID == SAMPLEIDS.GetP0DAirNuMuBkgInAntiNuModeCC() and \
                not RUNP0DAIRNUMUBKGINANTINUMODECCSELECTION:
            continue

        INTERFACE.Announce(INTERFACE, '-----------------------------------')
        INTERFACE.Announce(INTERFACE, '--- You can ignore these errors ---')
        INTERFACE.Announce(INTERFACE, '-----------------------------------')
        mc_samples_dict = GetMonteCarloSamples(current_sampleID)
        data_samples_dict = GetDATAsamples(current_sampleID)
        INTERFACE.Announce(INTERFACE, '-----------------------------------')
        SELECTIONSAVENAME = SELECTIONSAVENAMEDICT[current_sampleID]

        mc_data_sample_dict = dict()
        for sample_name in mc_samples_dict.keys():
            if TN208_ANALYSIS and \
                    ('Air' in sample_name or 'Water-Out' in sample_name):
                continue
            if TN208_ANALYSIS and current_sampleID \
                    == SAMPLEIDS.GetP0DWaterNuMuBarInAntiNuModeCC() and \
                    'FHC' in sample_name:
                continue
            if TN208_ANALYSIS and \
                    current_sampleID == SAMPLEIDS.GetP0DWaterNuMuCC() and \
                    'RHC' in sample_name:
                continue
            if TN208_ANALYSIS and current_sampleID \
                    == SAMPLEIDS.GetP0DWaterNuMuBkgInAntiNuModeCC():
                continue
            # These store the Chains for numu_FHC numu_RHC, numubar_FHC, and numubar_RHC
            mc_data_sample_dict[sample_name] = \
                {
                    mc_data_sample_dict_MC_key: mc_samples_dict[sample_name],
                    mc_data_sample_dict_DATA_key: data_samples_dict[sample_name]
                }

        # these store how to break down the samples by particles or other grouping
        neutrino_selections = GetNeutrinoSelectionList(current_sampleID)
        particle_selections = GetLeptonCandidateSelectionList(current_sampleID)
        neut_nu_selections = GetNEUTNuSelectionList(current_sampleID)
        neut_antinu_selections = GetNEUTAntiNuSelectionList(current_sampleID)
        topology_selecitons = GetTopologySelectionList(current_sampleID)

        # these are data/mc histograms
        all_selection_sets = [particle_selections, neut_antinu_selections,
                              neut_nu_selections, topology_selecitons]

        # loop over sample classes
        for smpls in mc_data_sample_dict.values():
            mc_sample = smpls[mc_data_sample_dict_MC_key]['Magnet']
            # avoid making empty plots
            if SAMPLEIDS.IsP0DFHCSample(current_sampleID) and \
                    not mc_sample.CPPClass.is_FHC:
                continue
            if SAMPLEIDS.IsP0DRHCSample(current_sampleID) and \
                    mc_sample.CPPClass.is_FHC:
                continue
            if SAMPLEIDS.IsP0DWaterSample(current_sampleID) and \
                    ('Air' in mc_sample.CPPClass.plotTitle or
                        'Water-Out' in mc_sample.CPPClass.plotTitle):
                continue
            if SAMPLEIDS.IsP0DAirSample(current_sampleID) and not \
                    ('Air' in mc_sample.CPPClass.plotTitle or
                        'Water-Out' in mc_sample.CPPClass.plotTitle):
                continue
            print mc_sample.CPPClass.plotTitle
            data_pot_exponent = INTERFACE.\
                GetExponentBase10(mc_sample.CPPClass.data_pot)
            data_pot_mantissa = INTERFACE.\
                GetMantissaBase10(mc_sample.CPPClass.data_pot,
                                  data_pot_exponent)
            pot_str_fmt = '%.2f #times 10^{%d} PoT'
            pot_str = pot_str_fmt % (data_pot_mantissa, data_pot_exponent)
            evts_p_bin_p_pot = '%s / %s ' % (evts_p_bin, pot_str)

            # neutrino energy
            if DRAW_ENU:
                histstack_Enu = ROOTHStack()
                histstack_Enu.plot_var = 'TrueEnuNom'
                histstack_Enu.x_title = 'True Neutrino Energy'
                histstack_Enu.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_Enu, Enu_AnaBins, pot_str)
                make_mc_only_stack(mc_sample, neutrino_selections, Enu_AnaBins,
                                   histstack_Enu, 'trueE_nu')

            for a_selection_set in all_selection_sets:
                selection_name = ROOT.TString(a_selection_set[0].name)
                if selection_name.Contains('LeptonCandidateTruePDG'):
                    if not PLOTLEPTONCANDIDATETRUEPDG:
                        continue
                if selection_name.Contains('NEUTNuReactionCodes'):
                    if not PLOTNEUTNUREACTIONCODES:
                        continue
                    if not (SAMPLEIDS.IsP0DNuMuSample(current_sampleID)
                            or SAMPLEIDS.IsP0DNuMuBkgInAntiNuModeSample(current_sampleID)):
                        continue
                if selection_name.Contains('NEUTAntiNuReactionCodes'):
                    if not PLOTNEUTANTINUREACTIONCODES:
                        continue
                    if not SAMPLEIDS.IsP0DNuMuBarInAntiNuModeSample(current_sampleID):
                        continue
                if selection_name.Contains('Topology'):
                    if not PLOTTOPOLOGY:
                        continue

                # TN-328 Momentum
                if DRAW_PMU_TN328:
                    histstack_pMu_TN328 = ROOTHStack()
                    histstack_pMu_TN328.plot_var = 'LeptonMomNom'
                    histstack_pMu_TN328.x_title = 'Lepton Candidate Momentum'
                    histstack_pMu_TN328.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_pMu_TN328, pMu_TN328_AnaBins,
                                        pot_str)
                    make_data_mc_stack(smpls, a_selection_set,
                                       pMu_TN328_AnaBins, histstack_pMu_TN328,
                                       'recoP_mu_TN328')

                # lepton candidate momentum
                if DRAW_PMU:
                    histstack_pMu = ROOTHStack()
                    histstack_pMu.plot_var = 'LeptonMomNom'
                    histstack_pMu.x_title = 'Lepton Candidate Momentum'
                    histstack_pMu.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_pMu, pMu_AnaBins, pot_str)
                    make_data_mc_stack(smpls, a_selection_set, pMu_AnaBins,
                                       histstack_pMu, 'recoP_mu')

                # TN-328 cos(theta)
                if DRAW_COSTHETAMU_TN328:
                    histstack_cosThetaMu_TN328 = ROOTHStack()
                    histstack_cosThetaMu_TN328.plot_var = 'LeptonCosNom'
                    histstack_cosThetaMu_TN328.x_title = 'Lepton Candidate Angle'
                    histstack_cosThetaMu_TN328.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_cosThetaMu_TN328,
                                        cosThetaMu_TN328_AnaBins, pot_str)
                    make_data_mc_stack(smpls, a_selection_set,
                                       cosThetaMu_TN328_AnaBins,
                                       histstack_cosThetaMu_TN328,
                                       'recocosq_mu_TN328')

                # lepton candidate cos(theta)
                if DRAW_COSTHETAMU:
                    histstack_cosThetaMu = ROOTHStack()
                    histstack_cosThetaMu.plot_var = 'LeptonCosNom'
                    histstack_cosThetaMu.x_title = 'Lepton Candidate Track Angle'
                    histstack_cosThetaMu.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_cosThetaMu, cosThetaMu_AnaBins,
                                        pot_str)
                    make_data_mc_stack(smpls, a_selection_set,
                                       cosThetaMu_AnaBins, histstack_cosThetaMu,
                                       'recocosq_mu')

                # lepton candidate thetaMu
                if DRAW_THETAMU:
                    histstack_thetaMu = ROOTHStack()
                    histstack_thetaMu.plot_var = \
                        'TMath::ACos(LeptonCosNom)*TMath::RadToDeg()'
                    histstack_thetaMu.x_title = 'Lepton Candidate Track Angle'
                    histstack_thetaMu.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_thetaMu, thetaMu_AnaBins, pot_str)
                    make_data_mc_stack(smpls, a_selection_set, thetaMu_AnaBins,
                                       histstack_thetaMu, 'recothetaMu_mu')

                # lepton p0d position Z
                if DRAW_P0DZ:
                    histstack_p0dZ = ROOTHStack()
                    histstack_p0dZ.plot_var = 'vtxZ'
                    histstack_p0dZ.x_title = 'Vertex Z'
                    histstack_p0dZ.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_p0dZ, p0dZ_AnaBins, pot_str)
                    make_data_mc_stack(smpls, a_selection_set, p0dZ_AnaBins,
                                       histstack_p0dZ, 'recoZ_mu')

                # lepton p0d position X
                if DRAW_P0DX:
                    histstack_p0dX = ROOTHStack()
                    histstack_p0dX.plot_var = 'vtxX'
                    histstack_p0dX.x_title = 'Vertex X'
                    histstack_p0dX.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_p0dX, p0dX_AnaBins, pot_str)
                    make_data_mc_stack(smpls, a_selection_set, p0dX_AnaBins,
                                       histstack_p0dX, 'recoX_mu')

                # lepton p0d position Y
                if DRAW_P0DY:
                    histstack_p0dY = ROOTHStack()
                    histstack_p0dY.plot_var = 'vtxY'
                    histstack_p0dY.x_title = 'Vertex Y'
                    histstack_p0dY.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_p0dY, p0dY_AnaBins, pot_str)
                    make_data_mc_stack(smpls, a_selection_set, p0dY_AnaBins,
                                       histstack_p0dY, 'recoY_mu')


class sample(object):
    """a way to store sample titles/labels"""

    def __init__(self, chain, plot_title, save_title):
        """set titles and input chain. plot_title put in legend
           save_title used when saved to file"""
        self.CPPClass = ROOT.PlottingSample(chain, plot_title, save_title)

    def __str__(self):
        return self.CPPClass.plotTitle

    def getTChain(self):
        """returns the TChain"""
        return self.CPPClass.GetTChain()


class selection_info(object):
    """store the name, cuts, and legend labels for a selection"""

    def __init__(self, name, cuts, legend_label):
        self.CPPClass = ROOT.PlottingSelectionInfo(name, cuts, legend_label)

    def __str__(self):
        return self.CPPClass.cuts


def make_data_mc_stack(evt_sample, true_selections, anaBins, hstack, save_title):
    """
    Take evt_sample (sample) and separate it by
    true_selections (selection_info). The histogram is stored in
    anaBins (ROOT.AnaysisBins) with the histogram labels in hstack (ROOTHStack)
    Saved as "save_title".root  and .png (str)
    """

    mc_sample = evt_sample['MC']['Magnet']
    sand_sample = evt_sample['MC']['Sand']
    data_sample = evt_sample['DATA']

    save_as = '%s_%s_%s_%s' % (SELECTIONSAVENAME, save_title,
                               mc_sample.CPPClass.saveTitle,
                               true_selections[0].name)
    if APPLY_FLUX_WEIGHTS:
        save_as += '_fluxtuned'
    if APPLY_EVENT_WEIGHTS:
        save_as += '_systematicweighted'
    canvas = ROOT.TCanvas("canvas", "", 800, 600)
    legend = TLegend(COORDS.Legend_RHS_X1, COORDS.Legend_RHS_Y1,
                     COORDS.Legend_RHS_X2, COORDS.Legend_RHS_Y2,
                     mc_sample.CPPClass.plotTitle)
    legend.SetFillStyle(0)
    legend.SetLineColor(0)
    legend.SetBorderSize(0)
    canvas.cd()

    plot_var = hstack.plot_var
    mc_hists = list()
    data_hist = None

    # Data hist fill
    anaBins.Sumw2(True)
    full_selection = true_selections[0]
    tmp_save_name = '%s_%s' % (save_title, full_selection.name)
    nEntries = data_sample.getTChain().Draw(plot_var,
                                            full_selection.cuts,
                                            'goff')
    v1 = data_sample.getTChain().GetV1()
    for entry_in_draw in range(nEntries):
        anaBins.Fill(v1[entry_in_draw])
    if anaBins.GetDivideByBinWidth():
        print 'WARNING:', anaBins.GetName(), 'dividing by bin width'
        anaBins.DivideByBinWidth(True)
    data_hist = anaBins.GetTH1DClone('h1d_%s_%s' % (tmp_save_name,
                                     data_sample.CPPClass.saveTitle))
    data_hist.SetMarkerStyle(INTERFACE.kDataMarkerStyle)
    INTERFACE.PrettyUpTH1(data_hist, hstack.x_title, hstack.y_title,
                          BLACK, BLACK)
    if anaBins.GetShowOverflow():
        legend.AddEntry(data_hist, 'DATA (Overflow shown)', 'LPE')
    else:
        legend.AddEntry(data_hist, 'DATA', 'LPE')
    anaBins.Reset()
    anaBins.Sumw2(False)
    data_integral = data_hist.Integral(1, data_hist.GetNbinsX())
    data_stats = TLegend(COORDS.PaveStats_Data_X1, COORDS.PaveStats_Data_Y1,
                         COORDS.PaveStats_Data_X2, COORDS.PaveStats_Data_Y2,
                         'Integral %d' % data_integral)
    data_stats.SetBorderSize(1)
    data_stats.SetFillColor(0)
    data_stats.SetMargin(0.1)

    # MC loop over true_selections to create stack,
    #    first entry is full selection
    for index in range(1, len(true_selections)):
        a_selection = true_selections[index]
        tmp_save_name = '%s_%s' % (save_title, a_selection.name)
        mc_hist = None

        if not ROOT.TString(a_selection.cuts.GetName()).\
                Contains('Sand'):
            mc_nEntries = mc_sample.getTChain().Draw('%s:WeightNom:FluxWeightNom' % plot_var, a_selection.cuts, 'goff')
            mc_var = mc_sample.getTChain().GetV1()
            mc_systematic_weights = mc_sample.getTChain().GetV2()
            mc_flux_weights = mc_sample.getTChain().GetV3()
            for entry_in_draw in range(mc_nEntries):
                weight = 1.0
                if APPLY_FLUX_WEIGHTS:
                    weight *= mc_flux_weights[entry_in_draw]
                if APPLY_EVENT_WEIGHTS:
                    weight *= mc_systematic_weights[entry_in_draw]
                anaBins.Fill(mc_var[entry_in_draw], weight)
            if anaBins.GetDivideByBinWidth():
                anaBins.DivideByBinWidth()
            name_fmt = 'mc_h1d_%s_%s'
            mc_hist = anaBins.GetTH1DClone(
                    name_fmt % (tmp_save_name, mc_sample.CPPClass.saveTitle))
            mc_hist.Scale(mc_sample.CPPClass.scale)

        else:
            sandTChain = sand_sample.getTChain()
            sand_nEntries = sandTChain.Draw('%s:WeightNom:FluxWeightNom' % plot_var,
                                            a_selection.cuts,
                                            'goff')
            sand_var = sand_sample.getTChain().GetV1()
            sand_systematic_weights = sand_sample.getTChain().GetV2()
            sand_flux_weights = sand_sample.getTChain().GetV3()
            for entry_in_draw in range(sand_nEntries):
                weight = 1.0
                if APPLY_FLUX_WEIGHTS:
                    weight *= sand_flux_weights[entry_in_draw]
                if APPLY_EVENT_WEIGHTS:
                    weight *= sand_systematic_weights[entry_in_draw]
                anaBins.Fill(sand_var[entry_in_draw], weight)
            if anaBins.GetDivideByBinWidth():
                anaBins.DivideByBinWidth()
            name_fmt = 'sand_h1d_%s_%s'
            mc_hist = anaBins.GetTH1DClone(
                    name_fmt % (tmp_save_name,
                                sand_sample.CPPClass.saveTitle))
            mc_hist.Scale(sand_sample.CPPClass.scale)

        INTERFACE.PrettyUpTH1(mc_hist, hstack.x_title, hstack.y_title, BLACK,
                              STACK_COLORS[index])
        mc_hists.append(mc_hist)
        legend.AddEntry(mc_hist, a_selection.legendLabel, 'f')
        anaBins.Reset()

    mc_hists.reverse()

    h_stack = ROOT.THStack('%s_stack' % (save_title), '')
    h_total = anaBins.GetTH1DClone("h_total_%s" % save_title)
    INTERFACE.PrettyUpTH1(h_total, hstack.x_title, hstack.y_title,
                          BLACK)
    h_total.SetLineWidth(5)
    for a_hist in mc_hists:
        h_stack.Add(a_hist)
        h_total.Add(a_hist)

    mc_integral = h_total.Integral(1, h_total.GetNbinsX())
    mc_stats = TLegend(COORDS.PaveStats_MC_X1, COORDS.PaveStats_MC_Y1,
                       COORDS.PaveStats_MC_X2, COORDS.PaveStats_MC_Y2,
                       'Integral %.2f' % mc_integral)
    mc_stats.SetBorderSize(1)
    mc_stats.SetFillColor(0)
    mc_stats.SetMargin(0.1)

    h_stack.SetMinimum(0)
    h_stack.Draw()
    # h_stack.GetXaxis().SetTitleSize(1.25*h_stack.GetXaxis().GetTitleSize())
    INTERFACE.PrettyUpTHStack(h_stack, hstack.x_title, hstack.y_title)
    if hstack.minimum > -1:
        new_min = hstack.minimum
        h_stack.SetMinimum(new_min)
        h_total.SetMinimum(new_min)
    old_max = max(h_total.GetMaximum(), data_hist.GetMaximum())

    if hstack.maximum > -1:
        new_max = hstack.maximum
        h_stack.SetMaximum(new_max)
        h_total.SetMaximum(new_max)
    else:
        new_max = 1.1 * old_max
        h_stack.SetMaximum(new_max)
        # h_total.SetMaximum(new_max)
        # data_hist.SetMaximum(new_max)

    canvas.Clear()
    canvas.cd()
    pad1, pad2, axis, line, ratio = None, None, None, None, None
    if SHOW_RATIO_PLOT_BELOW:
        pad1 = ROOT.TPad("pad1", "pad1", 0.0, 0.325, 1.0, 1.0)
        pad1.SetBottomMargin(0)
        pad1.Draw()
        pad1.cd()
    h_stack.Draw()
    h_total.Draw('SAME')
    data_hist.Draw('SAME E0')
    legend.Draw()
    data_stats.Draw()
    mc_stats.Draw()

    if hstack.log_y:
        canvas.SetLogy(1)
        h_stack.SetMinimum(1)
        h_total.SetMinimum(1)
        h_stack.SetMaximum(old_max)
        h_total.SetMaximum(old_max)
        h_total.Draw()
        h_stack.Draw('same')
        INTERFACE.SaveCanvasAs(
                canvas, join('plots', '%s_log' % (save_as)))
        canvas.SetLogy(0)

    if SHOW_RATIO_PLOT_BELOW:
        canvas.cd()  # Go back to the main canvas before defining pad2
        pad2 = ROOT.TPad("pad2", "pad2", 0, 0.0, 1., 0.225)
        pad2.SetTopMargin(0)
        pad2.SetBottomMargin(0.2)
        pad2.SetGridx()  # vertical grid
        pad2.SetGridy()  # horizontal grid
        pad2.Draw()
        pad2.cd()       # pad2 becomes the current pad

        # Define the ratio plot
        ratio = data_hist.Clone("ratio")
        ratio.SetLineColor(BLACK)
        ratio.Divide(h_total)
        ratio_max_bin = ratio.GetMaximumBin()
        ratio_min_bin = ratio.GetMinimumBin()
        ratio.SetMaximum(min(1.5, 1.1*(ratio.GetBinContent(ratio_max_bin)+ratio.GetBinError(ratio_max_bin))))  # range
        ratio.SetMinimum(max(0.5, 0.9*(ratio.GetBinContent(ratio_min_bin)-ratio.GetBinError(ratio_min_bin))))  # range
        ratio.SetStats(0)       # No statistics on lower plot
        ratio.SetMarkerStyle(INTERFACE.kDataMarkerStyle)

        xaxis = ratio.GetXaxis()
        line = ROOT.TLine(xaxis.GetBinLowEdge(1), 1,
                          xaxis.GetBinUpEdge(xaxis.GetNbins()), 1)
        line.SetLineWidth(3)
        line.SetLineStyle(9)
        line.SetLineColor(INTERFACE.kcbBlue)

        ratio.Draw("EP ")        # Draw the ratio plot
        line.Draw()
        ratio.Draw("EP SAME")   # Draw the ratio plot

        # Ratio plot (ratio) settings
        ratio.SetTitle("")  # Remove the ratio title

        # Y axis ratio plot settings
        ratio.GetYaxis().SetTitle('Data/MC')
        ratio.GetYaxis().SetNdivisions(505)
        ratio.GetYaxis().SetLabelSize(3*h_stack.GetYaxis().GetLabelSize())
        ratio.GetYaxis().SetTitleOffset(0.2)
        ratio.GetYaxis().SetTitleSize(0.2)

        # X axis ratio plot settings
        ratio.GetXaxis().SetTitle('')
        ratio.GetXaxis().SetTitleOffset(h_stack.GetXaxis().GetTitleOffset())
        ratio.GetXaxis().SetLabelSize(3*h_stack.GetXaxis().GetLabelSize())
        # ratio.GetXaxis().SetLabelSize(15)

    canvas.Update()
    canvas.Draw()
    INTERFACE.SaveCanvasAs(canvas, join('plots', save_as))

    anaBins.Reset()
    h_total.Delete()
    h_stack.Delete()
    for a_hist in mc_hists:
        a_hist.Delete()
    if ratio:
        ratio.Delete()
    if line:
        line.Delete()
    if axis:
        axis.Delete()
    if pad1:
        pad1.Close()
    if pad2:
        pad2.Close()
    data_hist.Delete()
    legend.Delete()
    canvas.Close()


def make_mc_only_stack(mc_sample, true_selections, anaBins, hstack, save_title):
    """Take samples dictionary (mc_sand_sample) and separate it by
    selection (true_selections). The histogram is stored in
    anaBins (AnaysisBins) with the histogram labels in hstack (ROOTHStack)
    Saved as save_title.root
    """
    save_as = '%s_%s_%s' % (SELECTIONSAVENAME, save_title, mc_sample.CPPClass.saveTitle)
    canvas = ROOT.TCanvas("canvas", "", 800, 600)
    legend = TLegend(COORDS.Legend_RHS_X1, COORDS.Legend_RHS_Y1,
                     COORDS.Legend_RHS_X2, COORDS.Legend_RHS_Y2,
                     mc_sample.CPPClass.plotTitle)
    legend.SetFillStyle(0)
    legend.SetLineColor(0)
    legend.SetBorderSize(0)
    canvas.cd()

    plot_var = hstack.plot_var

    hists = list()
    for index in range(1, len(true_selections)):
        a_selection = true_selections[index]
        tmp_save_name = '%s_%s' % (save_title, a_selection.CPPClass.name)
        nEntries = mc_sample.getTChain().Draw(plot_var, a_selection.CPPClass.cuts, 'goff')
        v1 = mc_sample.getTChain().GetV1()
        for entry_in_draw in range(nEntries):
            anaBins.Fill(v1[entry_in_draw])
        a_hist = anaBins.GetTH1DClone('h1d_%s_%s' % (tmp_save_name, mc_sample.CPPClass.saveTitle))
        INTERFACE.PrettyUpTH1(a_hist, hstack.x_title, hstack.y_title,
                              BLACK, STACK_COLORS[index])
        legend.AddEntry(a_hist, a_selection.CPPClass.legendLabel, 'f')
        a_hist.Scale(mc_sample.CPPClass.scale)
        hists.append(a_hist)
        anaBins.Reset()

    hists.reverse()

    h_stack = ROOT.THStack('%s_stack' % (save_title), '')
    h_total = anaBins.GetTH1DClone("h_total_%s" % save_title)
    INTERFACE.PrettyUpTH1(h_total, hstack.x_title, hstack.y_title,
                          BLACK)
    for a_hist in hists:
        h_stack.Add(a_hist)
        h_total.Add(a_hist)
    mc_stats = TLegend(0.7, 0.9, 0.9, 0.95, 'Integral %.2f' % h_total.Integral())
    mc_stats.SetBorderSize(1)
    mc_stats.SetFillColor(0)
    mc_stats.SetMargin(0.1)

    h_stack.Draw()
    INTERFACE.PrettyUpTHStack(h_stack, hstack.x_title, hstack.y_title)
    if hstack.minimum > -1:
        new_min = hstack.minimum
        h_stack.SetMinimum(new_min)
        h_total.SetMinimum(new_min)
    old_max = h_total.GetMaximum()
    if hstack.maximum > -1:
        new_max = hstack.maximum
        h_stack.SetMaximum(new_max)
        h_total.SetMaximum(new_max)

    h_total.Draw()
    h_stack.Draw('same')
    legend.Draw()
    mc_stats.Draw()
    canvas.cd()
    canvas.SetFillColor(0)
    if(ROOT.gPad.GetPrimitive('TFrame')):
        ROOT.gPad.GetPrimitive('TFrame').SetFillColor(0)

    INTERFACE.SaveCanvasAs(canvas, join('plots', save_as))

    if hstack.log_y:
        canvas.SetLogy(1)
        h_stack.SetMinimum(1)
        h_total.SetMinimum(1)
        h_stack.SetMaximum(old_max)
        h_total.SetMaximum(old_max)
        h_total.Draw()
        h_stack.Draw('same')
        INTERFACE.SaveCanvasAs(
                canvas, join('plots', '%s_log' % (save_as)))
        canvas.SetLogy(0)

    h_total.Delete()
    h_stack.Delete()
    for a_hist in hists:
        a_hist.Delete()
    legend.Delete()
    canvas.Close()
    return


def GetMonteCarloSamples(sampleID):
    """
    Get a dictionary of the NEUT MC samples broken down by horn current and
    P0D water stataus. In addition, the sample scale is set according to POT

    OUTPUT: a dictionary
    all_samples = {
            'FHC_Wtr': {'Magnet': FHC_Wtr, 'Sand': FHC_Wtr_Snd},
            'FHC_Air': {'Magnet': FHC_Air, 'Sand': FHC_Air_Snd},
            'RHC_Wtr': {'Magnet': RHC_Wtr, 'Sand': RHC_Wtr_Snd},
            'RHC_Air': {'Magnet': RHC_Air, 'Sand': RHC_Air_Snd}
    }

    """
    RunSyst_New_TTree = 'all'
    file_path = getenv('SYSTEMATICSROOT')

    chn_SANDRun3AirFHC = T2KDATAMC.SANDFHC.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_SANDRun3AirRHC = T2KDATAMC.SANDRHC.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun2Air = T2KDATAMC.RUN2W.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun2Wtr = T2KDATAMC.RUN2A.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun3bAir = T2KDATAMC.RUN3B.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun3cAir = T2KDATAMC.RUN3C.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun4Air = T2KDATAMC.RUN4A.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun4Wtr = T2KDATAMC.RUN4W.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun5cWtr = T2KDATAMC.RUN5C.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun6bAir = T2KDATAMC.RUN6B.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun6cAir = T2KDATAMC.RUN6C.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun6dAir = T2KDATAMC.RUN6D.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun6eAir = T2KDATAMC.RUN6E.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun7bWtr = T2KDATAMC.RUN7B.GetAllChainsFrom(RunSyst_New_TTree, file_path)

    # FHC, P0D water-in
    chn_FHC_Wtr = chn_NEUTRun4Wtr
    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONLABELSDICT[sampleID] + ' Water-In', 'fhc_water')
    FHC_Wtr_Snd = sample(chn_SANDRun3AirFHC, SELECTIONLABELSDICT[sampleID] + ' Water-In, Sand', 'fhc_water_sand')
    FHC_Wtr_Snd.CPPClass.is_FHC = True
    FHC_Wtr.CPPClass.is_FHC = True
    if not TN208_ANALYSIS:
        chn_FHC_Wtr.Add(chn_NEUTRun2Wtr)
        FHC_Wtr.CPPClass.scale = T2KPOT.GetPOTFHCWaterData()/T2KPOT.GetPOTFHCWaterMC()
        FHC_Wtr.CPPClass.data_pot = T2KPOT.GetPOTFHCWaterData()
        FHC_Wtr_Snd.CPPClass.scale = T2KPOT.GetPOTFHCWaterData()/T2KPOT.GetPOTFHCAirSandMC()
    else:
        FHC_Wtr.CPPClass.scale = T2KPOT.GetPOTRun4WaterData()/T2KPOT.GetPOTRun4WaterMC()
        FHC_Wtr.CPPClass.data_pot = T2KPOT.GetPOTRun4WaterData()
        FHC_Wtr_Snd.CPPClass.scale = T2KPOT.GetPOTRun4WaterData()/T2KPOT.GetPOTFHCAirSandMC()

    # FHC, P0D water-out
    if not TN208_ANALYSIS:
        chn_FHC_Air = chn_NEUTRun4Air
        FHC_Air = sample(chn_FHC_Air, SELECTIONLABELSDICT[sampleID] + ' Water-Out', 'fhc_air')
        FHC_Air_Snd = sample(chn_SANDRun3AirFHC, SELECTIONLABELSDICT[sampleID] + ' Water-Out, Sand', 'fhc_air_sand')
        FHC_Air.CPPClass.is_FHC = True
        FHC_Air_Snd.CPPClass.is_FHC = True
        chn_FHC_Air.Add(chn_NEUTRun3bAir)
        chn_FHC_Air.Add(chn_NEUTRun3cAir)
        chn_FHC_Air.Add(chn_NEUTRun2Air)
        FHC_Air.CPPClass.scale = T2KPOT.GetPOTFHCAirData()/T2KPOT.GetPOTFHCAirMC()
        FHC_Air.CPPClass.data_pot = T2KPOT.GetPOTFHCAirData()
        FHC_Air_Snd.CPPClass.scale = T2KPOT.GetPOTFHCAirData()/T2KPOT.GetPOTFHCAirSandMC()

    # RHC, P0D water-in
    chn_RHC_Wtr = chn_NEUTRun5cWtr
    RHC_Wtr_Snd = sample(chn_SANDRun3AirRHC, SELECTIONLABELSDICT[sampleID] + ' Water-In, Sand', 'rhc_water_sand')
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONLABELSDICT[sampleID] + ' Water-In', 'rhc_water')
    RHC_Wtr.CPPClass.is_FHC = False
    if not TN208_ANALYSIS:
        chn_RHC_Wtr.Add(chn_NEUTRun7bWtr)
        RHC_Wtr.CPPClass.scale = T2KPOT.GetPOTRHCWaterData() / T2KPOT.GetPOTRHCWaterMC()
        RHC_Wtr.CPPClass.data_pot = T2KPOT.GetPOTRHCWaterData()
        RHC_Wtr_Snd.CPPClass.scale = T2KPOT.GetPOTRHCWaterData() / T2KPOT.GetPOTRHCAirSandMC()
    else:
        RHC_Wtr.CPPClass.scale = T2KPOT.GetPOTRun5cWaterData() / T2KPOT.GetPOTRun5cWaterMC()
        RHC_Wtr.CPPClass.data_pot = T2KPOT.GetPOTRun5cWaterData()
        RHC_Wtr_Snd.CPPClass.scale = T2KPOT.GetPOTRun5cWaterData() / T2KPOT.GetPOTRHCAirSandMC()

    # RHC, P0D water-out
    if not TN208_ANALYSIS:
        chn_RHC_Air = chn_NEUTRun6bAir
        chn_RHC_Air.Add(chn_NEUTRun6cAir)
        chn_RHC_Air.Add(chn_NEUTRun6dAir)
        chn_RHC_Air.Add(chn_NEUTRun6eAir)
        RHC_Air = sample(chn_RHC_Air, SELECTIONLABELSDICT[sampleID] + ' Water-Out', 'rhc_air')
        RHC_Air.CPPClass.is_FHC = False
        RHC_Air.CPPClass.data_pot = T2KPOT.GetPOTRHCAirData()
        RHC_Air.CPPClass.scale = T2KPOT.GetPOTRHCAirData()/T2KPOT.GetPOTRHCAirMC()
        RHC_Air_Snd = sample(chn_SANDRun3AirRHC, SELECTIONLABELSDICT[sampleID] + ' Water-Out, Sand', 'rhc_air_sand')
        RHC_Air_Snd.CPPClass.is_FHC = False
        RHC_Air_Snd.CPPClass.scale = T2KPOT.GetPOTRHCAirData() / T2KPOT.GetPOTRHCAirSandMC()

    all_samples = {
            'FHC_Wtr': {'Magnet': FHC_Wtr, 'Sand': FHC_Wtr_Snd},
            'RHC_Wtr': {'Magnet': RHC_Wtr, 'Sand': RHC_Wtr_Snd}
    }
    if not TN208_ANALYSIS:
        all_samples['FHC_Air'] = {'Magnet': FHC_Air, 'Sand': FHC_Air_Snd}
        all_samples['RHC_Air'] = {'Magnet': RHC_Air, 'Sand': RHC_Air_Snd}

    return all_samples


def GetDATAsamples(sampleID):
    """
    Get a dictionary of the data samples broken down by horn current and
    P0D water stataus

    OUTPUT: a dictionary
    all_samples = {
                 'FHC_Wtr': FHC_Wtr,
                 'RHC_Wtr': RHC_Wtr,
                 'FHC_Air': FHC_Air,
                 'RHC_Air': RHC_Air
    }
    """
    file_path = getenv('SYSTEMATICSROOT')
    RunSyst_New_DATA_TTree = 'nominal'
    chn_DATARun2Air = T2KDATAMC.RUN2WDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun2Wtr = T2KDATAMC.RUN2ADATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun3bAir = T2KDATAMC.RUN3BDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun3cAir = T2KDATAMC.RUN3CDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun4Air = T2KDATAMC.RUN4ADATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun4Wtr = T2KDATAMC.RUN4WDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun5cWtr = T2KDATAMC.RUN5CDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun6bAir = T2KDATAMC.RUN6BDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun6cAir = T2KDATAMC.RUN6CDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun6dAir = T2KDATAMC.RUN6DDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun6eAir = T2KDATAMC.RUN6EDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun7bWtr = T2KDATAMC.RUN7BDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)

    chn_FHC_Wtr = chn_DATARun4Wtr
    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONLABELSDICT[sampleID] + ' Water-In', 'fhc_water')
    FHC_Wtr.CPPClass.is_FHC = True
    if not TN208_ANALYSIS:
        chn_FHC_Wtr.Add(chn_DATARun2Wtr)
        FHC_Wtr.CPPClass.data_pot = T2KPOT.GetPOTFHCWaterData()
    else:
        FHC_Wtr.CPPClass.data_pot = T2KPOT.GetPOTRun4WaterData()

    if not TN208_ANALYSIS:
        chn_FHC_Air = chn_DATARun4Air
        FHC_Air = sample(chn_FHC_Air, SELECTIONLABELSDICT[sampleID] + ' Water-Out', 'fhc_air')
        FHC_Air.CPPClass.data_pot = T2KPOT.GetPOTFHCAirData()
        FHC_Air.CPPClass.is_FHC = True
        chn_FHC_Air.Add(chn_DATARun3bAir)
        chn_FHC_Air.Add(chn_DATARun3cAir)
        chn_FHC_Air.Add(chn_DATARun2Air)

    chn_RHC_Wtr = chn_DATARun5cWtr
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONLABELSDICT[sampleID] + ' Water-In', 'rhc_water')
    RHC_Wtr.CPPClass.is_FHC = False
    if not TN208_ANALYSIS:
        chn_RHC_Wtr.Add(chn_DATARun7bWtr)
        RHC_Wtr.CPPClass.data_pot = T2KPOT.GetPOTRHCWaterData()
    else:
        RHC_Wtr.CPPClass.data_pot = T2KPOT.GetPOTRun5cWaterData()

    if not TN208_ANALYSIS:
        chn_RHC_Air = chn_DATARun6bAir
        chn_RHC_Air.Add(chn_DATARun6cAir)
        chn_RHC_Air.Add(chn_DATARun6dAir)
        chn_RHC_Air.Add(chn_DATARun6eAir)
        RHC_Air = sample(chn_RHC_Air, SELECTIONLABELSDICT[sampleID] + ' Water-Out', 'rhc_air')
        RHC_Air.CPPClass.data_pot = T2KPOT.GetPOTRHCAirData()
        RHC_Air.CPPClass.is_FHC = False

    all_samples = {
            'FHC_Wtr': FHC_Wtr,
            'RHC_Wtr': RHC_Wtr,
    }
    if not TN208_ANALYSIS:
        all_samples['FHC_Air'] = FHC_Air
        all_samples['RHC_Air'] = RHC_Air

    return all_samples


def GetNeutrinoSelectionList(sampleID):
    """
    Get a list of the selections (selection_info) for the neutrino species
    """
    more_cuts = TCut()
    if ADDITIONAL_CUTS and type(ADDITIONAL_CUTS) == TCut:
        more_cuts = ADDITIONAL_CUTS

    if TN208_ANALYSIS:
        more_cuts += CUTS.FVTN208

    if USE_MOMENTUM_CUT:
        more_cuts += TCut('LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    CUTS.FillNeutrinoSelections('TrueNuPDG', SAMPLEIDS.GetLabelName(sampleID), sampleID, more_cuts)
    neutrino_selections = list()
    for index in range(0, CUTS.NMAXNEUTRINOSELECTIONS):
        neutrino_selections.append(CUTS.GetNeutrinoSelection(index))
    return neutrino_selections


def GetLeptonCandidateSelectionList(sampleID):
    """Make a list of lepton candidate cuts by particle"""
    more_cuts = TCut()
    if ADDITIONAL_CUTS and type(ADDITIONAL_CUTS) == TCut:
        more_cuts = ADDITIONAL_CUTS

    if TN208_ANALYSIS:
        more_cuts += CUTS.FVTN208

    if USE_MOMENTUM_CUT:
        more_cuts += TCut('LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    CUTS.FillParticleSelections('LeptonCandidateTruePDG', SAMPLEIDS.GetLabelName(sampleID), sampleID, more_cuts)
    particle_selections = list()
    for index in range(0, CUTS.NMAXPARTICLESELECTIONS):
        particle_selections.append(CUTS.GetParticleSelection(index))
    return particle_selections


def GetNEUTNuSelectionList(sampleID):
    """Make a list of true reaction code cuts (Nu)"""
    more_cuts = TCut()
    if ADDITIONAL_CUTS and type(ADDITIONAL_CUTS) == TCut:
        more_cuts = ADDITIONAL_CUTS

    if TN208_ANALYSIS:
        more_cuts += CUTS.FVTN208

    if USE_MOMENTUM_CUT:
        more_cuts += TCut('LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    CUTS.FillNEUTNuSelections('NEUTNuReactionCodes', SAMPLEIDS.GetLabelName(sampleID), sampleID, more_cuts)
    neut_nu_selections = list()
    for index in range(CUTS.NMAXNEUTSELECTIONS):
        neut_nu_selections.append(CUTS.GetNEUTNuSelection(index))
    return neut_nu_selections


def GetNEUTAntiNuSelectionList(sampleID):
    """Make a list of true reaction code cuts (Anti-nu)"""
    more_cuts = TCut()
    if ADDITIONAL_CUTS and type(ADDITIONAL_CUTS) == TCut:
        more_cuts = ADDITIONAL_CUTS

    if TN208_ANALYSIS:
        more_cuts += CUTS.FVTN208

    if USE_MOMENTUM_CUT:
        more_cuts += TCut('LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    CUTS.FillNEUTAntiNuSelections('NEUTAntiNuReactionCodes', SAMPLEIDS.GetLabelName(sampleID), sampleID, more_cuts)
    neut_antinu_selections = list()
    for index in range(CUTS.NMAXNEUTSELECTIONS):
        neut_antinu_selections.append(CUTS.GetNEUTAntiNuSelection(index))
    return neut_antinu_selections


def GetTopologySelectionList(sampleID):
    """Make a list of true topology cuts """
    more_cuts = TCut()
    if ADDITIONAL_CUTS and type(ADDITIONAL_CUTS) == TCut:
        more_cuts = ADDITIONAL_CUTS

    if TN208_ANALYSIS:
        more_cuts += CUTS.FVTN208

    if USE_MOMENTUM_CUT:
        more_cuts += TCut('LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    CUTS.FillTopologySelections('TrueTopology', SAMPLEIDS.GetLabelName(sampleID), sampleID, more_cuts)
    topology_selections = list()
    for index in range(CUTS.NMAXTOPOLOGYSELECTIONS):
        topology_selections.append(CUTS.GetTopologySelection(index))
    return topology_selections


def ConfigureROOTHStack(hstack, anaBins, pot_str):
    """
    For the hstack (ROOTHStack), get options from anaBins (AnalysisBins),
    the pot_str (str) is just a string for the y_title
    """
    if len(anaBins.GetUnits()) < 1:
        return
    units = anaBins.GetUnits()
    hstack.plot_var += '*%f' % HEPCONSTANTS.Convert(units)
    hstack.x_title += ' [%s]' % units
    if anaBins.GetDivideByBinWidth():
        y_title = 'Events / %s / %s'
        y_title_tuple = (units, pot_str)
        hstack.y_title = y_title % y_title_tuple


def LoadP0DBANFF():
    """Load in the necessary classes"""
    global ENGINE
    ENGINE = ROOT.TXMLEngine()
    loadStatus = gSystem.Load("libP0DBANFF")
    if not (loadStatus == 1 or loadStatus == 0):
        print "Unable to load libP0DBANFF.so. gSystem.Load(\"libP0DBANFF\") returned", loadStatus
        sys.exit(1)
    global INTERFACE, SAMPLEIDS, T2KPOT, T2KDATAMC
    global XML, HEPCONSTANTS, CUTS, COORDS
    try:
        INTERFACE = ROOT.P0DBANFFInterface()
        INTERFACE.SetBatch(True)
        INTERFACE.GetThisStyle().SetOptStat(0000)
        SAMPLEIDS = ROOT.SampleId()
        T2KPOT = ROOT.TotalPOT()
        T2KDATAMC = ROOT.T2KDataMC()
        HEPCONSTANTS = ROOT.HEPConstants()
        XML = ROOT.XMLTools()
        CUTS = ROOT.DefineCuts()
        COORDS = ROOT.CanvasCoordinates()
    except Exception as exc:
        print type(exc)
        print "unable to load libP0DBANFF.so"
        sys.exit(1)
    global STACK_COLORS, BLACK
    BLACK = INTERFACE.kcbBlack
    STACK_COLORS = INTERFACE.GetStackColors()


def LoadSampleIDs():
    """Using the SampleIds class, load the samples set at the top of file"""
    global SELECTIONLABELSDICT, SELECTIONSAVENAMEDICT
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DWaterNuMuCC()] = '#nu_{#mu} in FHC,'
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DWaterNuMuBarInAntiNuModeCC()] = '#bar{#nu}_{#mu} in RHC'
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DWaterNuMuBkgInAntiNuModeCC()] = '#nu_{#mu} Bkg in RHC'
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DAirNuMuCC()] = '#nu_{#mu} in FHC,'
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DAirNuMuBarInAntiNuModeCC()] = '#bar{#nu}_{#mu} in RHC'
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DAirNuMuBkgInAntiNuModeCC()] = '#nu_{#mu} Bkg in RHC'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DWaterNuMuCC()] = 'numuCCIncWaterIn'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DWaterNuMuBarInAntiNuModeCC()] = 'numubarRHCCCIncWaterIn'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DWaterNuMuBkgInAntiNuModeCC()] = 'numubkgRHCCCIncWaterIn'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DAirNuMuCC()] = 'numuCCIncWaterOut'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DAirNuMuBarInAntiNuModeCC()] = 'numubarRHCCCIncWaterOut'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DAirNuMuBkgInAntiNuModeCC()] = 'numubkgRHCCCIncWaterOut'


if __name__ == "__main__":
    main(sys.argv[1:])
