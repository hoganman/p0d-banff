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
from ROOTH2 import ROOTH2
import sys

P0DBANFFROOT = getenv('P0DBANFFROOT')

# This sets the SampleId throughout the macro
SELECTIONSAVENAME = str()
SELECTIONSAVENAMEDICT = dict()
SELECTIONLABELSDICT = dict()

ADDITIONAL_CUTS = None
MOMENTUM_CUT_VALUE = '0'
USE_MOMENTUM_CUT = False
TN208_ANALYSIS = False
PLOTLEPTONCANDIDATETRUEPDG = False
PLOTNEUTNUREACTIONCODES = False
PLOTNEUTANTINUREACTIONCODES = False
PLOTTOPOLOGY = False
SHOW_LOGZ = False
PLOTDATA = False

# P0DBANFFInterface class to make plots pretty
STACK_COLORS = None
BLACK = int(1)

# Classes from P0DBANFF library
CONFIGURATION = None
INTERFACE = None


def main(argv):
    """main"""
    helpstatement = "Usage: plotRunSyst_NewResults.py config_file.xml"
    if len(argv) == 0 or len(argv) > 1:
        print helpstatement
        return
    LoadP0DBANFF()
    LoadSampleIDs()
    configFile = argv[0]
    LoadGlobalConfigurations(configFile)

    binningLocation = '%s/config/Binning.xml' % P0DBANFFROOT
    xmlTools = ROOT.XMLTools()
    cosThetaMu_AnaBins = ROOT.AnalysisBins('CosTheta', binningLocation, xmlTools)
    Enu_AnaBins = ROOT.AnalysisBins('NeutrinoEnergy', binningLocation, xmlTools)
    pMu_TN328_AnaBins = ROOT.AnalysisBins('TN328Momentum', binningLocation, xmlTools)
    pMu_AnaBins = ROOT.AnalysisBins('Momentum', binningLocation, xmlTools)
    cosThetaMu_TN328_AnaBins = ROOT.AnalysisBins('TN328CosTheta', binningLocation, xmlTools)
    thetaMu_AnaBins = ROOT.AnalysisBins('Theta', binningLocation, xmlTools)
    p0dZ_AnaBins = ROOT.AnalysisBins('P0DuleCoarseZ', binningLocation, xmlTools)
    p0dX_AnaBins = ROOT.AnalysisBins('P0DPositionX', binningLocation, xmlTools)
    p0dY_AnaBins = ROOT.AnalysisBins('P0DPositionY', binningLocation, xmlTools)

    P0DFitFHC_cosThetaMu_AnaBins = ROOT.AnalysisBins('P0DFitFHCCosTheta', binningLocation, xmlTools)
    P0DFitFHC_pMu_AnaBins = ROOT.AnalysisBins('P0DFitFHCMomentum', binningLocation, xmlTools)
    P0DFitFHC_pMu_cosThetaMu_AnaBins2D = ROOT.AnalysisBins2D(P0DFitFHC_pMu_AnaBins,
                                                             P0DFitFHC_cosThetaMu_AnaBins)
    # P0DCovFHC_cosThetaMu_AnaBins = ROOT.AnalysisBins('P0DCovFHCCosTheta', binningLocation, xmlTools)
    # P0DCovFHC_pMu_AnaBins = ROOT.AnalysisBins('P0DCovFHCMomentum', binningLocation, xmlTools)
    # P0DCovFHC_pMu_cosThetaMu_AnaBins2D = ROOT.AnalysisBins2D(P0DCovFHC_pMu_AnaBins,
    #                                                          P0DCovFHC_cosThetaMu_AnaBins)

    evts_p_bin = 'Events / bin'
    mc_data_sample_dict_MC_key = 'MC'
    mc_data_sample_dict_DATA_key = 'DATA'

    sampleIds = ROOT.SampleId()

    for current_sampleID in SELECTIONLABELSDICT.keys():
        global SELECTIONSAVENAME
        if not RunWithCurrentSample(current_sampleID):
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
            if TN208_ANALYSIS:
                # Must be CC-Inc
                if not (current_sampleID == sampleIds.GetP0DWaterNuMuCC() or
                        current_sampleID == sampleIds.GetP0DWaterNuMuBarInAntiNuModeCC()):
                    continue
                # Catch if running with water-out mode
                if 'Air' in sample_name or 'Water-Out' in sample_name:
                    continue
                # Must be numubar in RHC
                if (current_sampleID == sampleIds.GetP0DWaterNuMuBarInAntiNuModeCC() and
                        'FHC' in sample_name):
                    continue
                # Must be numu in FHC
                if (current_sampleID == sampleIds.GetP0DWaterNuMuCC() and
                        'RHC' in sample_name):
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
            if sampleIds.IsP0DFHCSample(current_sampleID) and \
                    not mc_sample.CPPClass.is_FHC:
                continue
            if sampleIds.IsP0DRHCSample(current_sampleID) and \
                    mc_sample.CPPClass.is_FHC:
                continue
            if sampleIds.IsP0DWaterSample(current_sampleID) and \
                    ('Air' in mc_sample.CPPClass.plotTitle or
                        'Water-Out' in mc_sample.CPPClass.plotTitle):
                continue
            if sampleIds.IsP0DAirSample(current_sampleID) and not \
                    ('Air' in mc_sample.CPPClass.plotTitle or
                        'Water-Out' in mc_sample.CPPClass.plotTitle):
                continue
            data_pot_exponent = INTERFACE.\
                GetExponentBase10(mc_sample.CPPClass.data_pot)
            data_pot_mantissa = INTERFACE.\
                GetMantissaBase10(mc_sample.CPPClass.data_pot,
                                  data_pot_exponent)
            pot_str_fmt = '%.2f #times 10^{%d} PoT'
            pot_str = pot_str_fmt % (data_pot_mantissa, data_pot_exponent)
            evts_p_bin_p_pot = '%s / %s ' % (evts_p_bin, pot_str)

            if CONFIGURATION.GetAttribBool('DRAW_P0DFITFHC_PMU_COSTHETAMU'):
                hist2D_pmu_costhetamu = ROOTH2()
                hist2D_pmu_costhetamu.varX = 'LeptonMomNom'
                hist2D_pmu_costhetamu.varY = 'LeptonCosNom'
                hist2D_pmu_costhetamu.x_title = 'Lepton Candidate Momentum'
                hist2D_pmu_costhetamu.y_title = 'Lepton Candidate Angle'
                ConfigureROOTH2(hist2D_pmu_costhetamu, P0DFitFHC_pMu_cosThetaMu_AnaBins2D, pot_str)
                make_mc_only_H2D(smpls, all_selection_sets[0], P0DFitFHC_pMu_cosThetaMu_AnaBins2D,
                                 hist2D_pmu_costhetamu, 'recoPmu_recocosq_mu_MC_only')
                del hist2D_pmu_costhetamu
            # if CONFIGURATION.GetAttribBool('DRAW_P0DCOVFHC_PMU_COSTHETAMU'):
            #     hist2D_pmu_costhetamu = ROOTH2()
            #     hist2D_pmu_costhetamu.varX = 'LeptonMomNom'
            #     hist2D_pmu_costhetamu.varY = 'LeptonCosNom'
            #     hist2D_pmu_costhetamu.x_title = 'Lepton Candidate Momentum'
            #     hist2D_pmu_costhetamu.y_title = 'Lepton Candidate Angle'
            #     ConfigureROOTH2(hist2D_pmu_costhetamu, P0DCovFHC_pMu_cosThetaMu_AnaBins2D, pot_str)
            #     make_mc_only_H2D(smpls, all_selection_sets[0], P0DCovFHC_pMu_cosThetaMu_AnaBins2D,
            #                      hist2D_pmu_costhetamu, 'recoPmu_recocosq_mu_Cov')

            for a_selection_set in all_selection_sets:
                selection_name = ROOT.TString(a_selection_set[0].name)
                if selection_name.Contains('LeptonCandidateTruePDG'):
                    if not PLOTLEPTONCANDIDATETRUEPDG:
                        continue
                if selection_name.Contains('NEUTNuReactionCodes'):
                    if not PLOTNEUTNUREACTIONCODES:
                        continue
                    if not (sampleIds.IsP0DNuMuSample(current_sampleID)
                            or sampleIds.IsP0DNuMuBkgInAntiNuModeSample(current_sampleID)):
                        continue
                if selection_name.Contains('NEUTAntiNuReactionCodes'):
                    if not PLOTNEUTANTINUREACTIONCODES:
                        continue
                    if not sampleIds.IsP0DNuMuBarInAntiNuModeSample(current_sampleID):
                        continue
                if selection_name.Contains('Topology'):
                    if not PLOTTOPOLOGY:
                        continue
                print 'Plotting', selection_name

                # neutrino energy
                if CONFIGURATION.GetAttribBool('DRAW_ENU'):
                    histstack_Enu = ROOTHStack()
                    histstack_Enu.plot_var = 'TrueEnuNom'
                    histstack_Enu.x_title = 'True Neutrino Energy'
                    histstack_Enu.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_Enu, Enu_AnaBins, pot_str)
                    make_mc_only_stack(mc_sample, neutrino_selections, Enu_AnaBins,
                                       histstack_Enu, 'trueE_nu')

                # TN-328 Momentum
                if CONFIGURATION.GetAttribBool('DRAW_PMU_TN328'):
                    histstack_pMu_TN328 = ROOTHStack()
                    histstack_pMu_TN328.plot_var = 'LeptonMomNom'
                    histstack_pMu_TN328.x_title = 'Lepton Candidate Momentum'
                    histstack_pMu_TN328.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_pMu_TN328, pMu_TN328_AnaBins,
                                        pot_str)
                    if PLOTDATA:
                        make_data_mc_stack(smpls, a_selection_set,
                                           pMu_TN328_AnaBins,
                                           histstack_pMu_TN328,
                                           'recoP_mu_TN328')
                    else:
                        make_mc_only_stack(smpls, a_selection_set,
                                           pMu_TN328_AnaBins,
                                           histstack_pMu_TN328,
                                           'recoP_mu_TN328')

                # lepton candidate momentum
                if CONFIGURATION.GetAttribBool('DRAW_PMU'):
                    histstack_pMu = ROOTHStack()
                    histstack_pMu.plot_var = 'LeptonMomNom'
                    histstack_pMu.x_title = 'Lepton Candidate Momentum'
                    histstack_pMu.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_pMu, pMu_AnaBins, pot_str)
                    if PLOTDATA:
                        make_data_mc_stack(smpls, a_selection_set, pMu_AnaBins,
                                           histstack_pMu, 'recoP_mu')
                    else:
                        make_mc_only_stack(smpls, a_selection_set, pMu_AnaBins,
                                           histstack_pMu, 'recoP_mu')

                # TN-328 cos(theta)
                if CONFIGURATION.GetAttribBool('DRAW_COSTHETAMU_TN328'):
                    histstack_cosThetaMu_TN328 = ROOTHStack()
                    histstack_cosThetaMu_TN328.plot_var = 'LeptonCosNom'
                    histstack_cosThetaMu_TN328.x_title = 'Lepton Candidate Angle'
                    histstack_cosThetaMu_TN328.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_cosThetaMu_TN328,
                                        cosThetaMu_TN328_AnaBins, pot_str)
                    if PLOTDATA:
                        make_data_mc_stack(smpls, a_selection_set,
                                           cosThetaMu_TN328_AnaBins,
                                           histstack_cosThetaMu_TN328,
                                           'recocosq_mu_TN328')
                    else:
                        make_mc_only_stack(smpls, a_selection_set,
                                           cosThetaMu_TN328_AnaBins,
                                           histstack_cosThetaMu_TN328,
                                           'recocosq_mu_TN328')

                # lepton candidate cos(theta)
                if CONFIGURATION.GetAttribBool('DRAW_COSTHETAMU'):
                    histstack_cosThetaMu = ROOTHStack()
                    histstack_cosThetaMu.plot_var = 'LeptonCosNom'
                    histstack_cosThetaMu.x_title = 'Lepton Candidate Track Angle'
                    histstack_cosThetaMu.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_cosThetaMu, cosThetaMu_AnaBins,
                                        pot_str)
                    if PLOTDATA:
                        make_data_mc_stack(smpls, a_selection_set,
                                           cosThetaMu_AnaBins, histstack_cosThetaMu,
                                           'recocosq_mu')
                    else:
                        make_mc_only_stack(smpls, a_selection_set,
                                           cosThetaMu_AnaBins, histstack_cosThetaMu,
                                           'recocosq_mu')

                # lepton candidate thetaMu
                if CONFIGURATION.GetAttribBool('DRAW_THETAMU'):
                    histstack_thetaMu = ROOTHStack()
                    histstack_thetaMu.plot_var = \
                        'TMath::ACos(LeptonCosNom)*TMath::RadToDeg()'
                    histstack_thetaMu.x_title = 'Lepton Candidate Track Angle'
                    histstack_thetaMu.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_thetaMu, thetaMu_AnaBins, pot_str)
                    if PLOTDATA:
                        make_data_mc_stack(smpls, a_selection_set, thetaMu_AnaBins,
                                           histstack_thetaMu, 'recothetaMu_mu')
                    else:
                        make_mc_only_stack(smpls, a_selection_set, thetaMu_AnaBins,
                                           histstack_thetaMu, 'recothetaMu_mu')

                # lepton p0d position Z
                if CONFIGURATION.GetAttribBool('DRAW_P0DZ'):
                    histstack_p0dZ = ROOTHStack()
                    histstack_p0dZ.plot_var = 'vtxZ'
                    histstack_p0dZ.x_title = 'Vertex Z'
                    histstack_p0dZ.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_p0dZ, p0dZ_AnaBins, pot_str)
                    if PLOTDATA:
                        make_data_mc_stack(smpls, a_selection_set, p0dZ_AnaBins,
                                           histstack_p0dZ, 'recoZ_mu')
                    else:
                        make_mc_only_stack(smpls, a_selection_set, p0dZ_AnaBins,
                                           histstack_p0dZ, 'recoZ_mu')

                # lepton p0d position X
                if CONFIGURATION.GetAttribBool('DRAW_P0DX'):
                    histstack_p0dX = ROOTHStack()
                    histstack_p0dX.plot_var = 'vtxX'
                    histstack_p0dX.x_title = 'Vertex X'
                    histstack_p0dX.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_p0dX, p0dX_AnaBins, pot_str)
                    if PLOTDATA:
                        make_data_mc_stack(smpls, a_selection_set, p0dX_AnaBins,
                                           histstack_p0dX, 'recoX_mu')
                    else:
                        make_mc_only_stack(smpls, a_selection_set, p0dX_AnaBins,
                                           histstack_p0dX, 'recoX_mu')

                # lepton p0d position Y
                if CONFIGURATION.GetAttribBool('DRAW_P0DY'):
                    histstack_p0dY = ROOTHStack()
                    histstack_p0dY.plot_var = 'vtxY'
                    histstack_p0dY.x_title = 'Vertex Y'
                    histstack_p0dY.y_title = evts_p_bin_p_pot
                    ConfigureROOTHStack(histstack_p0dY, p0dY_AnaBins, pot_str)
                    if PLOTDATA:
                        make_data_mc_stack(smpls, a_selection_set, p0dY_AnaBins,
                                           histstack_p0dY, 'recoY_mu')
                    else:
                        make_mc_only_stack(smpls, a_selection_set, p0dY_AnaBins,
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

    coordinates = ROOT.CanvasCoordinates()
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
    coordinates = ROOT.CanvasCoordinates()
    canvas = ROOT.TCanvas("canvas", "", 800, 600)
    legend = TLegend(coordinates.Legend_RHS_X1, coordinates.Legend_RHS_Y1,
                     coordinates.Legend_RHS_X2, coordinates.Legend_RHS_Y2,
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
    data_stats = TLegend(coordinates.PaveStats_Data_X1, coordinates.PaveStats_Data_Y1,
                         coordinates.PaveStats_Data_X2, coordinates.PaveStats_Data_Y2,
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
    mc_stats = TLegend(coordinates.PaveStats_MC_X1, coordinates.PaveStats_MC_Y1,
                       coordinates.PaveStats_MC_X2, coordinates.PaveStats_MC_Y2,
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


def make_mc_only_stack(evt_sample, true_selections, anaBins, hstack, save_title):
    """Take samples dictionary (mc_sand_sample) and separate it by
    selection (true_selections). The histogram is stored in
    anaBins (AnaysisBins) with the histogram labels in hstack (ROOTHStack)
    Saved as save_title.root
    """

    coordinates = ROOT.CanvasCoordinates()
    mc_sample = evt_sample['MC']['Magnet']
    sand_sample = evt_sample['MC']['Sand']

    save_as = '%s_%s_MC_only_%s_%s' % (SELECTIONSAVENAME, save_title, true_selections[0].name, mc_sample.CPPClass.saveTitle)
    canvas = ROOT.TCanvas("canvas", "", 800, 600)
    legend = TLegend(coordinates.Legend_RHS_X1, coordinates.Legend_RHS_Y1,
                     coordinates.Legend_RHS_X2, coordinates.Legend_RHS_Y2,
                     mc_sample.CPPClass.plotTitle)
    legend.SetFillStyle(0)
    legend.SetLineColor(0)
    legend.SetBorderSize(0)
    canvas.cd()

    plot_var = hstack.plot_var

    mc_hists = list()
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
    for a_hist in mc_hists:
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
    for a_hist in mc_hists:
        a_hist.Delete()
    legend.Delete()
    canvas.Close()
    return


def make_mc_only_H2D(evt_sample, true_selections, anaBins2D, hist2D, save_title):
    """Take samples dictionary (mc_sand_sample) and separate it by
    selection (true_selections). The histogram is stored in
    anaBins2D (AnaysisBins2D) with the histogram labels in h2 (ROOTH2)
    Saved as save_title.root
    """

    mc_sample = evt_sample['MC']['Magnet']
    sand_sample = evt_sample['MC']['Sand']

    save_as = '%s_2D_%s_%s' % (SELECTIONSAVENAME, save_title, mc_sample.CPPClass.saveTitle)
    canvas = ROOT.TCanvas("canvas", "", 800, 600)
    canvas.cd()

    varX = hist2D.varX
    varY = hist2D.varY

    mc_hists = list()
    # MC loop over true_selections to create stack,
    #    first entry is full selection
    for index in range(len(true_selections)):
        a_selection = true_selections[index]
        tmp_save_name = '%s_%s' % (save_title, a_selection.name)
        mc_hist = None
        # the entire selection include all but sand
        if index == 0:
            mcTChain = mc_sample.getTChain()
            mc_nEntries = mcTChain.Draw('%s:%s:WeightNom:FluxWeightNom' % (varX, varY), a_selection.cuts, 'goff')
            mc_varX = mcTChain.GetV1()
            mc_varY = mcTChain.GetV2()
            mc_systematic_weights = mcTChain.GetV3()
            mc_flux_weights = mcTChain.GetV4()
            for entry_in_draw in range(mc_nEntries):
                weight = 1.0
                if APPLY_FLUX_WEIGHTS:
                    weight *= mc_flux_weights[entry_in_draw]
                if APPLY_EVENT_WEIGHTS:
                    weight *= mc_systematic_weights[entry_in_draw]
                anaBins2D.Fill(mc_varX[entry_in_draw], mc_varY[entry_in_draw], weight)
            name_fmt = 'mc_h2d_%s_%s'
            if anaBins2D.GetDivideByBinWidth():
                anaBins2D.DivideByBinWidth()
            mc_hist = anaBins2D.GetTH2DClone(
                    name_fmt % (tmp_save_name, mc_sample.CPPClass.saveTitle))
            mc_hist.Scale(mc_sample.CPPClass.scale)
            anaBins2D.Reset()

        elif ROOT.TString(a_selection.cuts.GetName()).Contains('Sand'):
            sandTChain = sand_sample.getTChain()
            sand_nEntries = sandTChain.Draw('%s:%s:WeightNom:FluxWeightNom' % (varX, varY), a_selection.cuts, 'goff')
            sand_varX = sandTChain.GetV1()
            sand_varY = sandTChain.GetV2()
            sand_systematic_weights = sandTChain.GetV3()
            sand_flux_weights = sandTChain.GetV4()
            for entry_in_draw in range(sand_nEntries):
                weight = 1.0
                if APPLY_FLUX_WEIGHTS:
                    weight *= sand_flux_weights[entry_in_draw]
                if APPLY_EVENT_WEIGHTS:
                    weight *= sand_systematic_weights[entry_in_draw]
                anaBins2D.Fill(sand_varX[entry_in_draw], sand_varY[entry_in_draw], weight)
            name_fmt = 'sand_h2d_%s_%s'
            mc_hist = anaBins2D.GetTH2DClone(
                    name_fmt % (tmp_save_name,
                                sand_sample.CPPClass.saveTitle))
            mc_hist.Scale(sand_sample.CPPClass.scale)
            anaBins2D.Reset()
        else:
            continue
        # INTERFACE.PrettyUpTH2(mc_hist, hist2D.x_title, hist2D.y_title)
        mc_hists.append(mc_hist)

    h_total = anaBins2D.GetTH2DClone("h_total_%s" % save_title)
    INTERFACE.PrettyUpTH2(h_total, hist2D.x_title, hist2D.y_title)
    new_title = '%s MC: %s' % (mc_sample.CPPClass.plotTitle, hist2D.z_title)
    h_total.SetTitle(new_title)

    for a_hist in mc_hists:
        h_total.Add(a_hist)
    mc_stats = TLegend(0.7, 0.9, 0.9, 0.95, 'Integral %.2f' % h_total.Integral())
    mc_stats.SetBorderSize(1)
    mc_stats.SetFillColor(0)
    mc_stats.SetMargin(0.1)
    plot_title = TLegend(0.100251, 0.910683, 0.689223, 0.980736, new_title)
    plot_title.SetBorderSize(1)
    plot_title.SetFillColor(0)
    plot_title.SetMargin(0.1)

    canvas.cd()
    h_total.Draw('COLZ')
    mc_stats.Draw()
    plot_title.Draw()
    if SHOW_LOGZ:
        canvas.SetLogz(1)
    INTERFACE.SaveCanvasAs(canvas, join('plots', save_as))

    h_total.Delete()
    for a_hist in mc_hists:
        a_hist.Delete()
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
    t2kPOT = ROOT.TotalPOT()
    RunSyst_New_TTree = 'all'
    file_path = getenv('SYSTEMATICSROOT')

    t2kDataMC = ROOT.T2KDataMC()
    chn_SANDRun3AirFHC = t2kDataMC.SANDFHC.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_SANDRun3AirRHC = t2kDataMC.SANDRHC.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun2Air = t2kDataMC.RUN2A.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun2Wtr = t2kDataMC.RUN2W.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun3bAir = t2kDataMC.RUN3B.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun3cAir = t2kDataMC.RUN3C.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun4Air = t2kDataMC.RUN4A.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun4Wtr = t2kDataMC.RUN4W.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun5cWtr = t2kDataMC.RUN5C.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun6bAir = t2kDataMC.RUN6B.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun6cAir = t2kDataMC.RUN6C.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun6dAir = t2kDataMC.RUN6D.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun6eAir = t2kDataMC.RUN6E.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun7bWtr = t2kDataMC.RUN7B.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun8Wtr = t2kDataMC.RUN8W.GetAllChainsFrom(RunSyst_New_TTree, file_path)
    chn_NEUTRun8Air = t2kDataMC.RUN8A.GetAllChainsFrom(RunSyst_New_TTree, file_path)

    # FHC, P0D water-in
    chn_FHC_Wtr = chn_NEUTRun4Wtr
    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONLABELSDICT[sampleID] + ' Water-In', 'fhc_water')
    FHC_Wtr_Snd = sample(chn_SANDRun3AirFHC, SELECTIONLABELSDICT[sampleID] + ' Water-In, Sand', 'fhc_water_sand')
    FHC_Wtr_Snd.CPPClass.is_FHC = True
    FHC_Wtr.CPPClass.is_FHC = True
    if not TN208_ANALYSIS:
        chn_FHC_Wtr.Add(chn_NEUTRun2Wtr)
        chn_FHC_Wtr.Add(chn_NEUTRun8Wtr)
        FHC_Wtr.CPPClass.scale = t2kPOT.GetPOTFHCWaterData()/t2kPOT.GetPOTFHCWaterMC()
        FHC_Wtr.CPPClass.data_pot = t2kPOT.GetPOTFHCWaterData()
        FHC_Wtr_Snd.CPPClass.scale = t2kPOT.GetPOTFHCWaterData()/t2kPOT.GetPOTFHCAirSandMC()
    else:
        FHC_Wtr.CPPClass.scale = t2kPOT.GetPOTRun4WaterData()/t2kPOT.GetPOTRun4WaterMC()
        FHC_Wtr.CPPClass.data_pot = t2kPOT.GetPOTRun4WaterData()
        FHC_Wtr_Snd.CPPClass.scale = t2kPOT.GetPOTRun4WaterData()/t2kPOT.GetPOTFHCAirSandMC()

    # FHC, P0D water-out
    if not TN208_ANALYSIS:
        chn_FHC_Air = chn_NEUTRun4Air
        FHC_Air = sample(chn_FHC_Air, SELECTIONLABELSDICT[sampleID] + ' Water-Out', 'fhc_air')
        FHC_Air_Snd = sample(chn_SANDRun3AirFHC, SELECTIONLABELSDICT[sampleID] + ' Water-Out, Sand', 'fhc_air_sand')
        FHC_Air.CPPClass.is_FHC = True
        FHC_Air_Snd.CPPClass.is_FHC = True
        chn_FHC_Air.Add(chn_NEUTRun2Air)
        chn_FHC_Air.Add(chn_NEUTRun3bAir)
        chn_FHC_Air.Add(chn_NEUTRun3cAir)
        chn_FHC_Air.Add(chn_NEUTRun8Air)
        FHC_Air.CPPClass.scale = t2kPOT.GetPOTFHCAirData()/t2kPOT.GetPOTFHCAirMC()
        FHC_Air.CPPClass.data_pot = t2kPOT.GetPOTFHCAirData()
        FHC_Air_Snd.CPPClass.scale = t2kPOT.GetPOTFHCAirData()/t2kPOT.GetPOTFHCAirSandMC()

    # RHC, P0D water-in
    chn_RHC_Wtr = chn_NEUTRun5cWtr
    RHC_Wtr_Snd = sample(chn_SANDRun3AirRHC, SELECTIONLABELSDICT[sampleID] + ' Water-In, Sand', 'rhc_water_sand')
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONLABELSDICT[sampleID] + ' Water-In', 'rhc_water')
    RHC_Wtr.CPPClass.is_FHC = False
    if not TN208_ANALYSIS:
        chn_RHC_Wtr.Add(chn_NEUTRun7bWtr)
        RHC_Wtr.CPPClass.scale = t2kPOT.GetPOTRHCWaterData() / t2kPOT.GetPOTRHCWaterMC()
        RHC_Wtr.CPPClass.data_pot = t2kPOT.GetPOTRHCWaterData()
        RHC_Wtr_Snd.CPPClass.scale = t2kPOT.GetPOTRHCWaterData() / t2kPOT.GetPOTRHCAirSandMC()
    else:
        RHC_Wtr.CPPClass.scale = t2kPOT.GetPOTRun5cWaterData() / t2kPOT.GetPOTRun5cWaterMC()
        RHC_Wtr.CPPClass.data_pot = t2kPOT.GetPOTRun5cWaterData()
        RHC_Wtr_Snd.CPPClass.scale = t2kPOT.GetPOTRun5cWaterData() / t2kPOT.GetPOTRHCAirSandMC()

    # RHC, P0D water-out
    if not TN208_ANALYSIS:
        chn_RHC_Air = chn_NEUTRun6bAir
        chn_RHC_Air.Add(chn_NEUTRun6cAir)
        chn_RHC_Air.Add(chn_NEUTRun6dAir)
        chn_RHC_Air.Add(chn_NEUTRun6eAir)
        RHC_Air = sample(chn_RHC_Air, SELECTIONLABELSDICT[sampleID] + ' Water-Out', 'rhc_air')
        RHC_Air.CPPClass.is_FHC = False
        RHC_Air.CPPClass.data_pot = t2kPOT.GetPOTRHCAirData()
        RHC_Air.CPPClass.scale = t2kPOT.GetPOTRHCAirData()/t2kPOT.GetPOTRHCAirMC()
        RHC_Air_Snd = sample(chn_SANDRun3AirRHC, SELECTIONLABELSDICT[sampleID] + ' Water-Out, Sand', 'rhc_air_sand')
        RHC_Air_Snd.CPPClass.is_FHC = False
        RHC_Air_Snd.CPPClass.scale = t2kPOT.GetPOTRHCAirData() / t2kPOT.GetPOTRHCAirSandMC()

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
    t2kPOT = ROOT.TotalPOT()
    t2kDataMC = ROOT.T2KDataMC()
    file_path = getenv('SYSTEMATICSROOT')
    RunSyst_New_DATA_TTree = 'nominal'
    chn_DATARun2Air = t2kDataMC.RUN2ADATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun2Wtr = t2kDataMC.RUN2WDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun3bAir = t2kDataMC.RUN3BDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun3cAir = t2kDataMC.RUN3CDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun4Air = t2kDataMC.RUN4ADATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun4Wtr = t2kDataMC.RUN4WDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun5cWtr = t2kDataMC.RUN5CDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun6bAir = t2kDataMC.RUN6BDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun6cAir = t2kDataMC.RUN6CDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun6dAir = t2kDataMC.RUN6DDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun6eAir = t2kDataMC.RUN6EDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun7bWtr = t2kDataMC.RUN7BDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun8Wtr = t2kDataMC.RUN8WDATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)
    chn_DATARun8Air = t2kDataMC.RUN8ADATA.GetAllChainsFrom(RunSyst_New_DATA_TTree, file_path)

    chn_FHC_Wtr = chn_DATARun4Wtr
    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONLABELSDICT[sampleID] + ' Water-In', 'fhc_water')
    FHC_Wtr.CPPClass.is_FHC = True
    if not TN208_ANALYSIS:
        chn_FHC_Wtr.Add(chn_DATARun2Wtr)
        chn_FHC_Wtr.Add(chn_DATARun8Wtr)
        FHC_Wtr.CPPClass.data_pot = t2kPOT.GetPOTFHCWaterData()
    else:
        FHC_Wtr.CPPClass.data_pot = t2kPOT.GetPOTRun4WaterData()

    if not TN208_ANALYSIS:
        chn_FHC_Air = chn_DATARun4Air
        FHC_Air = sample(chn_FHC_Air, SELECTIONLABELSDICT[sampleID] + ' Water-Out', 'fhc_air')
        FHC_Air.CPPClass.data_pot = t2kPOT.GetPOTFHCAirData()
        FHC_Air.CPPClass.is_FHC = True
        chn_FHC_Air.Add(chn_DATARun2Air)
        chn_FHC_Air.Add(chn_DATARun3bAir)
        chn_FHC_Air.Add(chn_DATARun3cAir)
        chn_FHC_Air.Add(chn_DATARun8Air)

    chn_RHC_Wtr = chn_DATARun5cWtr
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONLABELSDICT[sampleID] + ' Water-In', 'rhc_water')
    RHC_Wtr.CPPClass.is_FHC = False
    if not TN208_ANALYSIS:
        chn_RHC_Wtr.Add(chn_DATARun7bWtr)
        RHC_Wtr.CPPClass.data_pot = t2kPOT.GetPOTRHCWaterData()
    else:
        RHC_Wtr.CPPClass.data_pot = t2kPOT.GetPOTRun5cWaterData()

    if not TN208_ANALYSIS:
        chn_RHC_Air = chn_DATARun6bAir
        chn_RHC_Air.Add(chn_DATARun6cAir)
        chn_RHC_Air.Add(chn_DATARun6dAir)
        chn_RHC_Air.Add(chn_DATARun6eAir)
        RHC_Air = sample(chn_RHC_Air, SELECTIONLABELSDICT[sampleID] + ' Water-Out', 'rhc_air')
        RHC_Air.CPPClass.data_pot = t2kPOT.GetPOTRHCAirData()
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
    cuts = ROOT.DefineCuts()
    if ADDITIONAL_CUTS and type(ADDITIONAL_CUTS) == TCut:
        more_cuts = ADDITIONAL_CUTS

    if TN208_ANALYSIS:
        more_cuts += cuts.FVTN208

    if USE_MOMENTUM_CUT:
        more_cuts += TCut('LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    sampleIds = ROOT.SampleId()
    cuts.FillNeutrinoSelections('TrueNuPDG', sampleIds.GetLabelName(sampleID), sampleID, more_cuts)
    neutrino_selections = list()
    for index in range(0, cuts.NMAXNEUTRINOSELECTIONS):
        neutrino_selections.append(cuts.GetNeutrinoSelection(index))
    return neutrino_selections


def GetLeptonCandidateSelectionList(sampleID):
    """Make a list of lepton candidate cuts by particle"""
    more_cuts = TCut()
    cuts = ROOT.DefineCuts()
    if ADDITIONAL_CUTS and type(ADDITIONAL_CUTS) == TCut:
        more_cuts = ADDITIONAL_CUTS

    if TN208_ANALYSIS:
        more_cuts += cuts.FVTN208

    if USE_MOMENTUM_CUT:
        more_cuts += TCut('LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    sampleIds = ROOT.SampleId()
    cuts.FillParticleSelections('LeptonCandidateTruePDG', sampleIds.GetLabelName(sampleID), sampleID, more_cuts)
    particle_selections = list()
    for index in range(cuts.NMAXPARTICLESELECTIONS):
        particle_selections.append(cuts.GetParticleSelection(index))
    return particle_selections


def GetNEUTNuSelectionList(sampleID):
    """Make a list of true reaction code cuts (Nu)"""
    more_cuts = TCut()
    cuts = ROOT.DefineCuts()
    if ADDITIONAL_CUTS and type(ADDITIONAL_CUTS) == TCut:
        more_cuts = ADDITIONAL_CUTS

    if TN208_ANALYSIS:
        more_cuts += cuts.FVTN208

    if USE_MOMENTUM_CUT:
        more_cuts += TCut('LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    sampleIds = ROOT.SampleId()
    cuts.FillNEUTNuSelections('NEUTNuReactionCodes', sampleIds.GetLabelName(sampleID), sampleID, more_cuts)
    neut_nu_selections = list()
    for index in range(cuts.NMAXNEUTSELECTIONS):
        neut_nu_selections.append(cuts.GetNEUTNuSelection(index))
    return neut_nu_selections


def GetNEUTAntiNuSelectionList(sampleID):
    """Make a list of true reaction code cuts (Anti-nu)"""
    more_cuts = TCut()
    cuts = ROOT.DefineCuts()
    if ADDITIONAL_CUTS and type(ADDITIONAL_CUTS) == TCut:
        more_cuts = ADDITIONAL_CUTS

    if TN208_ANALYSIS:
        more_cuts += cuts.FVTN208

    if USE_MOMENTUM_CUT:
        more_cuts += TCut('LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    sampleIds = ROOT.SampleId()
    cuts.FillNEUTAntiNuSelections('NEUTAntiNuReactionCodes', sampleIds.GetLabelName(sampleID), sampleID, more_cuts)
    neut_antinu_selections = list()
    for index in range(cuts.NMAXNEUTSELECTIONS):
        neut_antinu_selections.append(cuts.GetNEUTAntiNuSelection(index))
    return neut_antinu_selections


def GetTopologySelectionList(sampleID):
    """Make a list of true topology cuts """
    more_cuts = TCut()
    cuts = ROOT.DefineCuts()
    if ADDITIONAL_CUTS and type(ADDITIONAL_CUTS) == TCut:
        more_cuts = ADDITIONAL_CUTS

    if TN208_ANALYSIS:
        more_cuts += cuts.FVTN208

    if USE_MOMENTUM_CUT:
        more_cuts += TCut('LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    sampleIds = ROOT.SampleId()
    cuts.FillTopologySelections('TrueTopology', sampleIds.GetLabelName(sampleID), sampleID, more_cuts)
    topology_selections = list()
    for index in range(cuts.NMAXTOPOLOGYSELECTIONS):
        topology_selections.append(cuts.GetTopologySelection(index))
    return topology_selections


def ConfigureROOTHStack(hstack, anaBins, pot_str):
    """
    For the hstack (ROOTHStack), get options from anaBins (AnalysisBins),
    the pot_str (str) is just a string for the y_title
    """
    if len(anaBins.GetUnits()) < 1:
        return
    units = anaBins.GetUnits()
    hepConstants = ROOT.HEPConstants()
    hstack.plot_var += '*%f' % hepConstants.Convert(units)
    hstack.x_title += ' [%s]' % units
    if anaBins.GetDivideByBinWidth():
        y_title = 'Events / %s / (%s)'
        y_title_tuple = (units, pot_str)
        hstack.y_title = y_title % y_title_tuple


def ConfigureROOTH2(hist2D, anaBins2D, pot_str):
    """
    For the hist2D (ROOTH2), get options from anaBins2D (AnalysisBins2D)
    """
    unitsX, unitsY = None, None
    hepConstants = ROOT.HEPConstants()
    if len(anaBins2D.anaBinsX.GetUnits()) > 1:
        unitsX = anaBins2D.anaBinsX.GetUnits()
        hist2D.varX += '*%f' % hepConstants.Convert(unitsX)
        hist2D.x_title += ' [%s]' % unitsX
    if len(anaBins2D.anaBinsY.GetUnits()) > 1:
        unitsY = anaBins2D.anaBinsY.GetUnits()
        hist2D.varY += '*%f' % hepConstants.Convert(unitsY)
        hist2D.y_title += ' [%s]' % unitsY
    if unitsX and unitsY:
        print 'anaBins2D.GetDivideByBinWidth() =', anaBins2D.GetDivideByBinWidth()
        if anaBins2D.GetDivideByBinWidth():
            hist2D.z_title = 'Events / ( %s ) / ( %s ) / ( %s )' % (unitsX, unitsY, pot_str)
        else:
            hist2D.z_title = 'Events / bin / (%s)' % (pot_str)
    print 'hist2D.z_title =', hist2D.z_title


def LoadP0DBANFF():
    """Load in the necessary classes"""
    if len(P0DBANFFROOT) <= 0:
        print 'P0DBANFFROOT NOT exported. Please export it now'
        sys.exit(1)
    engine = ROOT.TXMLEngine()
    loadStatus = gSystem.Load("libP0DBANFF")
    if not (loadStatus == 1 or loadStatus == 0):
        print "Unable to load libP0DBANFF.so. gSystem.Load(\"libP0DBANFF\") returned", loadStatus
        sys.exit(1)
    global INTERFACE
    try:
        INTERFACE = ROOT.P0DBANFFInterface()
        INTERFACE.SetBatch(True)
        ROOT.gStyle.SetOptStat(0000)
        INTERFACE.LoadColorBlindPalette()
    except Exception as exc:
        print type(exc)
        print "unable to load libP0DBANFF.so"
        sys.exit(1)
    global STACK_COLORS, BLACK
    BLACK = INTERFACE.kcbBlack
    STACK_COLORS = INTERFACE.GetStackColors()
    del engine


def LoadSampleIDs():
    """Using the SampleIds class, load the samples set at the top of file"""
    global SELECTIONLABELSDICT, SELECTIONSAVENAMEDICT
    sampleIds = ROOT.SampleId()
    # SELECTIONLABELSDICT[sampleIds.GetP0DWaterNuMuCC()] = '#nu_{#mu} CCInc in FHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DWaterNuMuCC1Track()] = '#nu_{#mu} CC1Track in FHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DWaterNuMuCCNTracks()] = '#nu_{#mu} CCNTracks in FHC'

    # SELECTIONLABELSDICT[sampleIds.GetP0DWaterNuMuBarInAntiNuModeCC()] = '#bar{#nu}_{#mu} CCInc in RHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DWaterNuMuBarInAntiNuModeCC1Track()] = '#bar{#nu}_{#mu} CC1Track in RHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DWaterNuMuBarInAntiNuModeCCNTracks()] = '#bar{#nu}_{#mu} CCNTracks in RHC'

    # SELECTIONLABELSDICT[sampleIds.GetP0DWaterNuMuBkgInAntiNuModeCC()] = '#nu_{#mu} Bkg CCInc in RHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DWaterNuMuBkgInAntiNuModeCC1Track()] = '#nu_{#mu} Bkg CC1Tracks in RHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DWaterNuMuBkgInAntiNuModeCCNTracks()] = '#nu_{#mu} Bkg CCNTracks in RHC'

    # SELECTIONLABELSDICT[sampleIds.GetP0DAirNuMuCC()] = '#nu_{#mu} CCInc in FHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DAirNuMuCC1Track()] = '#nu_{#mu} CC1Track in FHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DAirNuMuCCNTracks()] = '#nu_{#mu} CCNTracks in FHC'

    # SELECTIONLABELSDICT[sampleIds.GetP0DAirNuMuBarInAntiNuModeCC()] = '#bar{#nu}_{#mu} CCInc in RHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DAirNuMuBarInAntiNuModeCC1Track()] = '#bar{#nu}_{#mu} CC1Track in RHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DAirNuMuBarInAntiNuModeCCNTracks()] = '#bar{#nu}_{#mu} CCNTracks in RHC'

    # SELECTIONLABELSDICT[sampleIds.GetP0DAirNuMuBkgInAntiNuModeCC()] = '#nu_{#mu} CCInc Bkg in RHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DAirNuMuBkgInAntiNuModeCC1Track()] = '#nu_{#mu} CC1Track Bkg in RHC'
    SELECTIONLABELSDICT[sampleIds.GetP0DAirNuMuBkgInAntiNuModeCCNTracks()] = '#nu_{#mu} CCNTracks Bkg in RHC'

    # SELECTIONSAVENAMEDICT[sampleIds.GetP0DWaterNuMuCC()] = 'numuCCIncWaterIn'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DWaterNuMuCC1Track()] = 'numuCC1TrackWaterIn'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DWaterNuMuCCNTracks()] = 'numuCCNTracksWaterIn'

    # SELECTIONSAVENAMEDICT[sampleIds.GetP0DWaterNuMuBarInAntiNuModeCC()] = 'numubarRHCCCIncWaterIn'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DWaterNuMuBarInAntiNuModeCC1Track()] = 'numubarRHCC1TrackCWaterIn'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DWaterNuMuBarInAntiNuModeCCNTracks()] = 'numubarRHCCNTracksCWaterIn'

    # SELECTIONSAVENAMEDICT[sampleIds.GetP0DWaterNuMuBkgInAntiNuModeCC()] = 'numubkgRHCCCIncWaterIn'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DWaterNuMuBkgInAntiNuModeCC1Track()] = 'numubkgRHCC1TrackCWaterIn'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DWaterNuMuBkgInAntiNuModeCCNTracks()] = 'numubkgRHCCNTracksCWaterIn'

    # SELECTIONSAVENAMEDICT[sampleIds.GetP0DAirNuMuCC()] = 'numuCCIncWaterOut'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DAirNuMuCC1Track()] = 'numuCC1TrackWaterOut'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DAirNuMuCCNTracks()] = 'numuCCNTracksWaterOut'

    # SELECTIONSAVENAMEDICT[sampleIds.GetP0DAirNuMuBarInAntiNuModeCC()] = 'numubarRHCCCIncWaterOut'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DAirNuMuBarInAntiNuModeCC1Track()] = 'numubarRHCC1TrackCWaterOut'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DAirNuMuBarInAntiNuModeCCNTracks()] = 'numubarRHCCNTracksCWaterOut'

    # SELECTIONSAVENAMEDICT[sampleIds.GetP0DAirNuMuBkgInAntiNuModeCC()] = 'numubkgRHCCCIncWaterOut'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DAirNuMuBkgInAntiNuModeCC1Track()] = 'numubkgRHCC1TrackCWaterOut'
    SELECTIONSAVENAMEDICT[sampleIds.GetP0DAirNuMuBkgInAntiNuModeCCNTracks()] = 'numubkgRHCCNTracksCWaterOut'


def RunWithCurrentSample(sampleId):
    """
    For all the various samples, check if the sampleId (int)
    will be plotted
    """

    sampleIds = ROOT.SampleId()
    if sampleId == sampleIds.GetP0DWaterNuMuCC():
        return CONFIGURATION.GetAttribBool('RUNP0DWATERNUMUCCSELECTION')

    if sampleId == sampleIds.GetP0DWaterNuMuCC1Track():
        return CONFIGURATION.GetAttribBool('RUNP0DWATERNUMUCC1TRACKSELECTION')

    if sampleId == sampleIds.GetP0DWaterNuMuCCNTracks():
        return CONFIGURATION.GetAttribBool('RUNP0DWATERNUMUCCNTRACKSSELECTION')

    if sampleId == sampleIds.GetP0DWaterNuMuBarInAntiNuModeCC():
        return CONFIGURATION.GetAttribBool('RUNP0DWATERNUMUBARINANTINUMODECCSELECTION')

    if sampleId == sampleIds.GetP0DWaterNuMuBarInAntiNuModeCC1Track():
        return CONFIGURATION.GetAttribBool('RUNP0DWATERNUMUBARINANTINUMODECC1TRACKSELECTION')

    if sampleId == sampleIds.GetP0DWaterNuMuBarInAntiNuModeCCNTracks():
        return CONFIGURATION.GetAttribBool('RUNP0DWATERNUMUBARINANTINUMODECCNTRACKSSELECTION')

    if sampleId == sampleIds.GetP0DWaterNuMuBkgInAntiNuModeCC():
        return CONFIGURATION.GetAttribBool('RUNP0DWATERNUMUBKGINANTINUMODECCSELECTION')

    if sampleId == sampleIds.GetP0DWaterNuMuBkgInAntiNuModeCC1Track():
        return CONFIGURATION.GetAttribBool('RUNP0DWATERNUMUBKGINANTINUMODECC1TRACKSELECTION')

    if sampleId == sampleIds.GetP0DWaterNuMuBkgInAntiNuModeCCNTracks():
        return CONFIGURATION.GetAttribBool('RUNP0DWATERNUMUBKGINANTINUMODECCNTRACKSSELECTION')

    if sampleId == sampleIds.GetP0DAirNuMuCC():
        return CONFIGURATION.GetAttribBool('RUNP0DAIRNUMUCCSELECTION')

    if sampleId == sampleIds.GetP0DAirNuMuCC1Track():
        return CONFIGURATION.GetAttribBool('RUNP0DAIRNUMUCC1TRACKSELECTION')

    if sampleId == sampleIds.GetP0DAirNuMuCCNTracks():
        return CONFIGURATION.GetAttribBool('RUNP0DAIRNUMUCCNTRACKSSELECTION')

    if sampleId == sampleIds.GetP0DAirNuMuBarInAntiNuModeCC():
        return CONFIGURATION.GetAttribBool('RUNP0DAIRNUMUBARINANTINUMODECCSELECTION')

    if sampleId == sampleIds.GetP0DAirNuMuBarInAntiNuModeCC1Track():
        return CONFIGURATION.GetAttribBool('RUNP0DAIRNUMUBARINANTINUMODECC1TRACKSELECTION')

    if sampleId == sampleIds.GetP0DAirNuMuBarInAntiNuModeCCNTracks():
        return CONFIGURATION.GetAttribBool('RUNP0DAIRNUMUBARINANTINUMODECCNTRACKSSELECTION')

    if sampleId == sampleIds.GetP0DAirNuMuBkgInAntiNuModeCC():
        return CONFIGURATION.GetAttribBool('RUNP0DAIRNUMUBKGINANTINUMODECCSELECTION')

    if sampleId == sampleIds.GetP0DAirNuMuBkgInAntiNuModeCC1Track():
        return CONFIGURATION.GetAttribBool('RUNP0DAIRNUMUBKGINANTINUMODECC1TRACKSELECTION')

    if sampleId == sampleIds.GetP0DAirNuMuBkgInAntiNuModeCCNTracks():
        return CONFIGURATION.GetAttribBool('RUNP0DAIRNUMUBKGINANTINUMODECCNTRACKSSELECTION')

    return False


def LoadGlobalConfigurations(configFile):
    """Load the global settings from the XML configuration file"""

    global CONFIGURATION
    xmlTools = ROOT.XMLTools()
    xmlTools.SetFile(configFile)
    # this is a special mode of the AttributeMap class where all the values
    # from the configuration files have been extracted
    CONFIGURATION = xmlTools.GetAllNodeValues()
    additional_cuts = CONFIGURATION.GetAttrib('ADDITIONAL_CUTS')
    if additional_cuts.Length() >= 1:
        global ADDITIONAL_CUTS
        ADDITIONAL_CUTS = TCut(additional_cuts.GetAttrib().Data())

    global USE_MOMENTUM_CUT
    USE_MOMENTUM_CUT = CONFIGURATION.GetAttribBool('USE_MOMENTUM_CUT')

    if USE_MOMENTUM_CUT:
        global MOMENTUM_CUT_VALUE
        MOMENTUM_CUT_VALUE = CONFIGURATION.GetAttrib('MOMENTUM_CUT_VALUE')

    global TN208_ANALYSIS
    TN208_ANALYSIS = CONFIGURATION.GetAttribBool('TN208_ANALYSIS')

    global PLOTLEPTONCANDIDATETRUEPDG, PLOTNEUTNUREACTIONCODES
    PLOTLEPTONCANDIDATETRUEPDG = CONFIGURATION.GetAttribBool('PLOTLEPTONCANDIDATETRUEPDG')
    PLOTNEUTNUREACTIONCODES = CONFIGURATION.GetAttribBool('PLOTNEUTNUREACTIONCODES')

    global PLOTNEUTANTINUREACTIONCODES, PLOTTOPOLOGY
    PLOTNEUTANTINUREACTIONCODES = CONFIGURATION.GetAttribBool('PLOTNEUTANTINUREACTIONCODES')
    PLOTTOPOLOGY = CONFIGURATION.GetAttribBool('PLOTTOPOLOGY')

    global SHOW_LOGZ
    SHOW_LOGZ = CONFIGURATION.GetAttribBool('SHOW_LOGZ')

    global APPLY_FLUX_WEIGHTS
    APPLY_FLUX_WEIGHTS = CONFIGURATION.GetAttribBool('APPLY_FLUX_WEIGHTS')

    global APPLY_EVENT_WEIGHTS
    APPLY_EVENT_WEIGHTS = CONFIGURATION.GetAttribBool('APPLY_EVENT_WEIGHTS')

    global PLOTDATA
    PLOTDATA = CONFIGURATION.GetAttribBool('PLOTDATA')

    global SHOW_RATIO_PLOT_BELOW
    SHOW_RATIO_PLOT_BELOW = CONFIGURATION.GetAttribBool('SHOW_RATIO_PLOT_BELOW')


if __name__ == "__main__":
    main(sys.argv[1:])
