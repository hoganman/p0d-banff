#!/usr/bin/env python2.7
"""
A script that makes histogram of the resulting TTree from
   RunSyst_New.exe
"""

from os import getenv
from os.path import join
import ROOT
from ROOT import THStack, TCanvas, TLegend, gSystem  # TChain
from ROOT import TPad, TGaxis, TLine
import ROOTChain
from ROOTHStack import ROOTHStack
import sys

# toggle these to draw particular variables
DRAW_ENU = 0
DRAW_PMU = 1
DRAW_THETAMU = 1
DRAW_COSTHETAMU = 0
DRAW_PMU_TN328 = 0
DRAW_COSTHETAMU_TN328 = 0
DRAW_P0DX = 0
DRAW_P0DY = 0
DRAW_P0DZ = 0

# Display the ratio of Data/MC below histogram
SHOW_RATIO_PLOT_BELOW = True

# use the TN-208 runs
TN208_ANALYSIS = False

# cut the measured momentum
USE_MOMENTUM_CUT = 1
MOMENTUM_CUT_VALUE = '5000.'

# if the user wants to apply an addition set of cuts
ADDITIONAL_CUTS = None

# Which selection to run
RUNP0DWATERNUMUCCSELECTION = True
RUNP0DWATERNUMUBARINANTINUMODECCSELECTION = False
RUNP0DWATERNUMUBKGINANTINUMODECCSELECTION = False
RUNP0DAIRNUMUCCSELECTION = True
RUNP0DAIRNUMUBARINANTINUMODECCSELECTION = False
RUNP0DAIRNUMUBKGINANTINUMODECCSELECTION = False

# This sets the SampleId throughout the macro
SELECTION = int(0)
SELECTIONSAVENAME = str()
SELECTIONSAVENAMEDICT = dict()
SELECTIONLABELSDICT = dict()

# P0DBANFFInterface class to make plots pretty
STACK_COLORS = list()
BLACK = int(1)

# Classes from P0DBANFF library
INTERFACE = None
SAMPLEIDS = None


def main(argv):
    """main"""
    helpstatement = "plotRunSyst_NewResults.py (no args)"
    if len(argv) > 0:
        print helpstatement
    p0dbanffroot = getenv('P0DBANFFROOT')
    LoadP0DBANFF()
    LoadSampleIDs()

    xml = ROOT.XMLTools()
    binningLocation = '%s/config/Binning.xml' % p0dbanffroot
    evts_p_bin = 'Events / bin'

    # These store the Chains for numu_FHC numu_RHC, numubar_FHC, and numubar_RHC
    mc_samples_dict = GetMonteCarloSamples()
    data_samples_dict = GetDATAsamples()
    mc_data_sample_dict = dict()
    mc_data_sample_dict_MC_key = 'MC'
    mc_data_sample_dict_DATA_key = 'DATA'

    for current_sel in SELECTIONLABELSDICT.keys:
        global SELECTION, SELECTIONSAVENAME
        SELECTION = current_sel
        if SELECTION == SAMPLEIDS.GetP0DWaterNuMuCC() and not RUNP0DWATERNUMUCCSELECTION:
            continue
        if SELECTION == SAMPLEIDS.GetP0DWaterNuMuBarInAntiNuModeCC() and not RUNP0DWATERNUMUBARINANTINUMODECCSELECTION:
            continue
        if SELECTION == SAMPLEIDS.GetP0DWaterNuMuBkgInAntiNuModeCC() and not RUNP0DWATERNUMUBKGINANTINUMODECCSELECTION:
            continue
        if SELECTION == SAMPLEIDS.GetP0DAirNuMuCC() and not RUNP0DAIRNUMUCCSELECTION:
            continue
        if SELECTION == SAMPLEIDS.GetP0DAirNuMuBarInAntiNuModeCC() and not RUNP0DAIRNUMUBARINANTINUMODECCSELECTION:
            continue
        if SELECTION == SAMPLEIDS.GetP0DAirNuMuBkgInAntiNuModeCC() and not RUNP0DAIRNUMUBKGINANTINUMODECCSELECTION:
            continue

        SELECTIONSAVENAME = SELECTIONSAVENAMEDICT[SELECTION]
        for sample_name in mc_samples_dict.keys():
            if TN208_ANALYSIS and 'Air' in sample_name:
                continue
            if TN208_ANALYSIS and SELECTION == SAMPLEIDS.GetP0DWaterNuMuBarInAntiNuModeCC() and 'FHC' in sample_name:
                continue
            if TN208_ANALYSIS and SELECTION == SAMPLEIDS.GetP0DWaterNuMuCC() and 'RHC' in sample_name:
                continue
            if TN208_ANALYSIS and SELECTION == SAMPLEIDS.GetP0DWaterNuMuBkgInAntiNuModeCC():
                continue
            mc_data_sample_dict[sample_name] =\
                {
                    mc_data_sample_dict_MC_key: mc_samples_dict[sample_name],
                    mc_data_sample_dict_DATA_key: data_samples_dict[sample_name]
                }

        # these store how to break down the samples by particles or other grouping
        neutrino_selections = GetNeutrinoSelectionList()
        particle_selections = GetLeptonCandidateSelectionList()

        # loop over sample classes
        for smpls in mc_data_sample_dict.values():
            mc_sample = smpls[mc_data_sample_dict_MC_key]['Magnet']
            # avoid making empty plots
            if SAMPLEIDS.IsP0DFHCSample(SELECTION) and not mc_sample.CPPCLASS.is_FHC:
                continue
            if SAMPLEIDS.IsP0DRHCSample(SELECTION) and mc_sample.CPPCLASS.is_FHC:
                continue
            data_pot_exponent = INTERFACE.GetExponentBase10(mc_sample.CPPCLASS.data_pot)
            data_pot_mantissa = INTERFACE.GetMantissaBase10(mc_sample.CPPCLASS.data_pot,
                                                            data_pot_exponent)
            pot_str = '%.2f #times 10^{%d} PoT' % (data_pot_mantissa,
                                                   data_pot_exponent)
            evts_p_bin_p_pot = '%s / %s ' % (evts_p_bin, pot_str)

            # neutrino energy
            if DRAW_ENU:
                Enu_AnaBins = ROOT.AnalysisBins('NeutrinoEnergy', binningLocation, xml)
                histstack_Enu = ROOTHStack()
                histstack_Enu.plot_var = 'TrueEnuNom'
                histstack_Enu.x_title = 'True Neutrino Energy'
                histstack_Enu.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_Enu, Enu_AnaBins, pot_str)
                make_mc_only_stack(mc_sample, neutrino_selections, Enu_AnaBins,
                                   histstack_Enu, 'trueE_nu')

            # TN-328 Momentum
            if DRAW_PMU_TN328:
                pMu_TN328_AnaBins = ROOT.AnalysisBins('TN328Momentum', binningLocation, xml)
                histstack_pMu_TN328 = ROOTHStack()
                histstack_pMu_TN328.plot_var = 'LeptonMomNom'
                histstack_pMu_TN328.x_title = 'Lepton Candidate Momentum'
                histstack_pMu_TN328.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_pMu_TN328, pMu_TN328_AnaBins, pot_str)
                make_data_mc_stack(smpls, particle_selections, pMu_TN328_AnaBins,
                                   histstack_pMu_TN328, 'recoP_mu_TN328')

            # lepton candidate momentum
            if DRAW_PMU:
                pMu_AnaBins = ROOT.AnalysisBins('Momentum', binningLocation, xml)
                histstack_pMu = ROOTHStack()
                histstack_pMu.plot_var = 'LeptonMomNom'
                histstack_pMu.x_title = 'Lepton Candidate Momentum'
                histstack_pMu.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_pMu, pMu_AnaBins, pot_str)
                make_data_mc_stack(smpls, particle_selections, pMu_AnaBins,
                                   histstack_pMu, 'recoP_mu_uniform')

            # TN-328 cos(theta)
            if DRAW_COSTHETAMU_TN328:
                cosThetaMu_TN328_AnaBins = ROOT.AnalysisBins('TN328CosTheta', binningLocation, xml)
                histstack_cosThetaMu_TN328 = ROOTHStack()
                histstack_cosThetaMu_TN328.plot_var = 'LeptonCosNom'
                histstack_cosThetaMu_TN328.x_title = 'Lepton Candidate Angle'
                histstack_cosThetaMu_TN328.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_cosThetaMu_TN328,
                                    cosThetaMu_TN328_AnaBins, pot_str)
                make_data_mc_stack(smpls, particle_selections, cosThetaMu_TN328_AnaBins,
                                   histstack_cosThetaMu_TN328, 'recocosq_mu_TN328_uniform')

            # lepton candidate cos(theta)
            if DRAW_COSTHETAMU:
                cosThetaMu_AnaBins = ROOT.AnalysisBins('CosTheta', binningLocation, xml)
                histstack_cosThetaMu = ROOTHStack()
                histstack_cosThetaMu.plot_var = 'LeptonCosNom'
                histstack_cosThetaMu.x_title = 'Lepton Candidate Track Angle'
                histstack_cosThetaMu.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_cosThetaMu, cosThetaMu_AnaBins, pot_str)
                make_data_mc_stack(smpls, particle_selections, cosThetaMu_AnaBins,
                                   histstack_cosThetaMu, 'recocosq_mu_uniform')

            # lepton candidate thetaMu
            if DRAW_THETAMU:
                thetaMu_AnaBins = ROOT.AnalysisBins('Theta', binningLocation, xml)
                histstack_thetaMu = ROOTHStack()
                histstack_thetaMu.plot_var = 'TMath::ACos(LeptonCosNom)*TMath::RadToDeg()'
                histstack_thetaMu.x_title = 'Lepton Candidate Track Angle'
                histstack_thetaMu.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_thetaMu, thetaMu_AnaBins, pot_str)
                make_data_mc_stack(smpls, particle_selections, thetaMu_AnaBins,
                                   histstack_thetaMu, 'recothetaMu_mu_uniform')

            # lepton p0d position Z
            if DRAW_P0DZ:
                p0dZ_AnaBins = ROOT.AnalysisBins('P0DuleCoarseZ', binningLocation, xml)
                histstack_p0dZ = ROOTHStack()
                histstack_p0dZ.plot_var = 'vtxZ'
                histstack_p0dZ.x_title = 'Vertex Z'
                histstack_p0dZ.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_p0dZ, p0dZ_AnaBins, pot_str)
                make_data_mc_stack(smpls, particle_selections, p0dZ_AnaBins,
                                   histstack_p0dZ, 'recoZ_mu')

            # lepton p0d position X
            if DRAW_P0DX:
                p0dX_AnaBins = ROOT.AnalysisBins('P0DPositionX', binningLocation, xml)
                histstack_p0dX = ROOTHStack()
                histstack_p0dX.plot_var = 'vtxX'
                histstack_p0dX.x_title = 'Vertex X'
                histstack_p0dX.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_p0dX, p0dX_AnaBins, pot_str)
                make_data_mc_stack(smpls, particle_selections, p0dX_AnaBins,
                                   histstack_p0dX, 'recoX_mu')

            # lepton p0d position Y
            if DRAW_P0DY:
                p0dY_AnaBins = ROOT.AnalysisBins('P0DPositionY', binningLocation, xml)
                histstack_p0dY = ROOTHStack()
                histstack_p0dY.plot_var = 'vtxY'
                histstack_p0dY.x_title = 'Vertex Y'
                histstack_p0dY.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_p0dY, p0dY_AnaBins, pot_str)
                make_data_mc_stack(smpls, particle_selections, p0dY_AnaBins,
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

    # def make_H1D(self, hist_save_title, var, n_bins, low, high, cuts=''):
    #     """create TH1 and return it"""
    #     if len(var) < 1:
    #         return None
    #     hist_name = 'h1d%s_%s' % (hist_save_title, self.save_title)
    #     hist = TH1D(hist_name, '', n_bins, low, high)
    #     self.chain.Draw('%s>>%s' % (var, hist_name), cuts, 'goff')
    #     if self.CPPCLASS.scale != 1:
    #         hist.Scale(self.CPPCLASS.scale)
    #     return hist

    # def make_H2D(self, hist_save_title, varX, n_binsX, lowX, highX,
    #              varY, n_binsY, lowY, highY, cuts=''):
    #     """create TH2 and return it"""
    #     if len(varX) < 1 or len(varY) < 1:
    #         return None
    #     hist_name = 'h2d%s_%s' % (hist_save_title, self.save_title)
    #     hist = TH2D(hist_name, '', n_binsX, lowX, highX,
    #                 n_binsY, lowY, highY)
    #     self.chain.Draw('%s:%s>>%s' % (varY, varX, hist_name), cuts, 'goff')
    #     if self.CPPCLASS.scale != 1:
    #         hist.Scale(self.CPPCLASS.scale)
    #     return hist


class selection_info(object):
    """store the name, cuts, and legend labels for a selection"""

    def __init__(self, name, cuts, legend_label):
        self.CPPClass = ROOT.PottingSelection_Info(name, cuts, legend_label)

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
    coords = ROOT.CanvasCoordinates()

    save_as = '%s_%s_%s' % (SELECTIONSAVENAME, save_title, mc_sample.save_title)
    canvas = TCanvas("canvas", "", 800, 600)
    legend = TLegend(coords.Legend_RHS_X1, coords.Legend_RHS_Y1,
                     coords.Legend_RHS_X2, coords.Legend_RHS_Y2,
                     mc_sample.plot_title)
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
    tmp_save_name = '%s_%s' % (save_title, full_selection.CPPClass.name)
    nEntries = data_sample.getTChain().Draw(plot_var, full_selection.CPPClass.cuts, 'goff')
    v1 = data_sample.getTChain().GetV1()
    for entry_in_draw in range(nEntries):
        anaBins.Fill(v1[entry_in_draw])
    if anaBins.GetDivideByBinWidth():
        print 'WARNING:', anaBins.GetName(), 'dividing by bin width'
        anaBins.DivideByBinWidth(True)
    data_hist = anaBins.GetTH1DClone('h1d_%s_%s' % (tmp_save_name, data_sample.save_title))
    data_hist.SetMarkerStyle(21)
    INTERFACE.PrettyUpTH1(data_hist, hstack.x_title, hstack.y_title,
                          BLACK, BLACK)
    if anaBins.GetShowOverflow():
        legend.AddEntry(data_hist, 'DATA (Overflow shown)', 'LPE')
    else:
        legend.AddEntry(data_hist, 'DATA', 'LPE')
    anaBins.Reset()
    anaBins.Sumw2(False)
    data_stats = TLegend(coords.PaveStats_Data_X1, coords.PaveStats_Data_Y1,
                         coords.PaveStats_Data_X2, coords.PaveStats_Data_Y2,
                         'Integral %d' % data_hist.Integral())
    data_stats.SetBorderSize(1)
    data_stats.SetFillColor(0)
    data_stats.SetMargin(0.1)

    # MC loop over true_selections to create stack,
    #    first entry is full selection
    for index in range(1, len(true_selections)):
        a_selection = true_selections[index]
        tmp_save_name = '%s_%s' % (save_title, a_selection.name)
        mc_hist = None

        if not ROOT.TString(a_selection.cuts.GetName()).Contains('Sand'):
            mc_nEntries = mc_sample.getTChain().Draw(plot_var, a_selection.cuts, 'goff')
            mc_v1 = mc_sample.getTChain().GetV1()
            for entry_in_draw in range(mc_nEntries):
                anaBins.Fill(mc_v1[entry_in_draw])
            if anaBins.GetDivideByBinWidth():
                anaBins.DivideByBinWidth()
            mc_hist = anaBins.GetTH1DClone('mc_h1d_%s_%s' % (tmp_save_name,
                                                             mc_sample.save_title))
            mc_hist.Scale(mc_sample.CPPCLASS.scale)

        else:
            sand_nEntries = sand_sample.getTChain().Draw(plot_var, a_selection.cuts, 'goff')
            sand_v1 = sand_sample.getTChain().GetV1()
            for entry_in_draw in range(sand_nEntries):
                anaBins.Fill(sand_v1[entry_in_draw])
            if anaBins.GetDivideByBinWidth():
                anaBins.DivideByBinWidth()
            mc_hist = anaBins.GetTH1DClone('sand_h1d_%s_%s' % (tmp_save_name,
                                                               sand_sample.save_title))
            mc_hist.Scale(sand_sample.CPPCLASS.scale)

        INTERFACE.PrettyUpTH1(mc_hist, hstack.x_title, hstack.y_title, BLACK,
                              STACK_COLORS[index])
        mc_hists.append(mc_hist)
        legend.AddEntry(mc_hist, a_selection.legend_label, 'f')
        anaBins.Reset()

    mc_hists.reverse()

    h_stack = THStack('%s_stack' % (save_title), '')
    h_total = anaBins.GetTH1DClone("h_total_%s" % save_title)
    INTERFACE.PrettyUpTH1(h_total, hstack.x_title, hstack.y_title,
                          BLACK)
    h_total.SetLineWidth(5)
    for a_hist in mc_hists:
        h_stack.Add(a_hist)
        h_total.Add(a_hist)

    mc_stats = TLegend(coords.PaveStats_MC_X1, coords.PaveStats_MC_Y1,
                       coords.PaveStats_MC_X2, coords.PaveStats_MC_Y2,
                       'Integral %.2f' % h_total.Integral())
    mc_stats.SetBorderSize(1)
    mc_stats.SetFillColor(0)
    mc_stats.SetMargin(0.1)

    h_stack.Draw()
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
        new_max = 1.1*old_max
        h_stack.SetMaximum(new_max)
        # h_total.SetMaximum(new_max)
        # data_hist.SetMaximum(new_max)

    h_stack.SetMinimum(0)
    if SHOW_RATIO_PLOT_BELOW:
        # move the minimum lower since some weird stuff happens with my style template
        h_stack.SetMinimum(-30*h_stack.GetMaximum()/h_total.GetNdivisions())

    canvas.cd()
    pad1, pad2, axis, line, ratio = None, None, None, None, None
    if SHOW_RATIO_PLOT_BELOW:
        pad1 = TPad("pad1", "pad1", 0, 0.3, 1, 1.0)
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
        # Do not draw the Y axis label on the upper plot and redraw a small
        # axis instead, in order to avoid the first label (0) to be clipped.
        # h_stack.GetYaxis().SetLabelSize(0.)
        axis = TGaxis(-5, 20, -5, 220, 20, 220, 510, "")
        # axis.SetLabelSize(15)
        axis.Draw()

        canvas.cd()  # Go back to the main canvas before defining pad2
        pad2 = TPad("pad2", "pad2", 0, 0.05, 1, 0.3)
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
        ratio.SetMarkerStyle(21)

        line = TLine(ratio.GetXaxis().GetBinLowEdge(1), 1, ratio.GetXaxis().GetBinUpEdge(ratio.GetXaxis().GetNbins()), 1)
        line.SetLineWidth(3)
        line.SetLineStyle(9)
        line.SetLineColor(INTERFACE.kcbBlue)

        ratio.Draw("ep")        # Draw the ratio plot
        line.Draw()
        ratio.Draw("ep same")   # Draw the ratio plot

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
    coords = ROOT.CanvasCoordinates()

    save_as = '%s_%s_%s' % (SELECTIONSAVENAME, save_title, mc_sample.save_title)
    canvas = TCanvas("canvas", "", 800, 600)
    legend = TLegend(coords.Legend_RHS_X1, coords.Legend_RHS_Y1,
                     coords.Legend_RHS_X2, coords.Legend_RHS_Y2,
                     mc_sample.plot_title)
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
        legend.AddEntry(a_hist, a_selection.legend_label, 'f')
        a_hist.Scale(mc_sample.CPPCLASS.scale)
        hists.append(a_hist)
        anaBins.Reset()

    hists.reverse()

    h_stack = THStack('%s_stack' % (save_title), '')
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


def GetMonteCarloSamples():
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
    RunSyst_New_NEUT_TTREE_name = 'all'
    file_path = getenv('SYSTEMATICSROOT')
    NEUTP6B = join(file_path, 'mcp6_Spin_B', 'neut')
    NEUTP6L = join(file_path, 'mcp6_Spin_L', 'neut')
    SAND = join(file_path, 'mcp6_Spin_B', 'sand')
    T2KPOT = ROOT.TotalPOT()

    chn_NEUTRun2Air = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN2A.iter_name(NEUTP6B))
    chn_NEUTRun2Wtr = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN2W.iter_name(NEUTP6B))
    chn_NEUTRun3bAir = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN3B.iter_name(NEUTP6B))
    chn_NEUTRun3cAir = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN3C.iter_name(NEUTP6B))
    chn_NEUTRun4Air = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN4A.iter_name(NEUTP6B))
    chn_NEUTRun4Wtr = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN4W.iter_name(NEUTP6B))
    chn_NEUTRun5cWtr = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN5C.iter_name(NEUTP6B))
    chn_NEUTRun6bAir = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN6B.iter_name(NEUTP6B))
    chn_NEUTRun6cAir = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN6C.iter_name(NEUTP6B))
    chn_NEUTRun6dAir = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN6D.iter_name(NEUTP6B))
    chn_NEUTRun6eAir = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN6E.iter_name(NEUTP6B))
    chn_NEUTRun7bWtr = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.RUN7B.iter_name(NEUTP6L))

    chn_SANDRun3AirFHC = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.SANDFHC.iter_name(SAND))
    chn_SANDRun3AirRHC = ROOTChain.get_all_from_to(RunSyst_New_NEUT_TTREE_name, ROOT.T2KDataMC.SANDRHC.iter_name(SAND))

    # FHC, P0D water-in
    chn_FHC_Wtr = chn_NEUTRun4Wtr
    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONLABELSDICT[SELECTION] + ' Water-In', 'fhc_water')
    FHC_Wtr_Snd = sample(chn_SANDRun3AirFHC, SELECTIONLABELSDICT[SELECTION] + ' Water-In, Sand', 'fhc_water_sand')
    FHC_Wtr_Snd.CPPCLASS.is_FHC = True
    FHC_Wtr.CPPCLASS.is_FHC = True
    if not TN208_ANALYSIS:
        chn_FHC_Wtr.Add(chn_NEUTRun2Wtr)
        FHC_Wtr.CPPCLASS.scale = T2KPOT.GetPOTFHCWaterData()/T2KPOT.GetPOTFHCWaterMC()
        FHC_Wtr.CPPCLASS.data_pot = T2KPOT.GetPOTFHCWaterData()
        FHC_Wtr_Snd.CPPCLASS.scale = T2KPOT.GetPOTFHCWaterData()/T2KPOT.GetPOTFHCAirSandMC()
    else:
        FHC_Wtr.CPPCLASS.scale = T2KPOT.GetPOTRun4WaterData()/T2KPOT.GetPOTRun4WaterMC()
        FHC_Wtr.CPPCLASS.data_pot = T2KPOT.GetPOTRun4WaterData()
        FHC_Wtr_Snd.CPPCLASS.scale = T2KPOT.GetPOTRun4WaterData()/T2KPOT.GetPOTFHCAirSandMC()

    # FHC, P0D water-out
    if not TN208_ANALYSIS:
        chn_FHC_Air = chn_NEUTRun4Air
        FHC_Air = sample(chn_FHC_Air, SELECTIONLABELSDICT[SELECTION] + ' Water-Out', 'fhc_air')
        FHC_Air_Snd = sample(chn_SANDRun3AirFHC, SELECTIONLABELSDICT[SELECTION] + ' Water-Out, Sand', 'fhc_air_sand')
        FHC_Air.CPPCLASS.is_FHC = True
        FHC_Air_Snd.CPPCLASS.is_FHC = True
        chn_FHC_Air.Add(chn_NEUTRun3bAir)
        chn_FHC_Air.Add(chn_NEUTRun3cAir)
        chn_FHC_Air.Add(chn_NEUTRun2Air)
        FHC_Air.CPPCLASS.scale = T2KPOT.GetPOTFHCAirData()/T2KPOT.GetPOTFHCAirMC()
        FHC_Air.CPPCLASS.data_pot = T2KPOT.GetPOTFHCAirData()
        FHC_Air_Snd.CPPCLASS.scale = T2KPOT.GetPOTFHCAirData()/T2KPOT.GetPOTFHCAirSandMC()

    # RHC, P0D water-in
    chn_RHC_Wtr = chn_NEUTRun5cWtr
    RHC_Wtr_Snd = sample(chn_SANDRun3AirRHC, SELECTIONLABELSDICT[SELECTION] + ' Water-In, Sand', 'rhc_water_sand')
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONLABELSDICT[SELECTION] + ' Water-In', 'rhc_water')
    RHC_Wtr.CPPCLASS.is_FHC = False
    if not TN208_ANALYSIS:
        chn_RHC_Wtr.Add(chn_NEUTRun7bWtr)
        RHC_Wtr.CPPCLASS.scale = T2KPOT.GetPOTRHCWaterData() / T2KPOT.GetPOTRHCWaterMC()
        RHC_Wtr.CPPCLASS.data_pot = T2KPOT.GetPOTRHCWaterData()
        RHC_Wtr_Snd.CPPCLASS.scale = T2KPOT.GetPOTRHCWaterData() / T2KPOT.GetPOTRHCAirSandMC()
    else:
        RHC_Wtr.CPPCLASS.scale = T2KPOT.GetPOTRun5cWaterData() / T2KPOT.GetPOTRun5cWaterMC()
        RHC_Wtr.CPPCLASS.data_pot = T2KPOT.GetPOTRun5cWaterData()
        RHC_Wtr_Snd.CPPCLASS.scale = T2KPOT.GetPOTRun5cWaterData() / T2KPOT.GetPOTRHCAirSandMC()

    # RHC, P0D water-out
    if not TN208_ANALYSIS:
        chn_RHC_Air = chn_NEUTRun6bAir
        chn_RHC_Air.Add(chn_NEUTRun6cAir)
        chn_RHC_Air.Add(chn_NEUTRun6dAir)
        chn_RHC_Air.Add(chn_NEUTRun6eAir)
        RHC_Air = sample(chn_RHC_Air, SELECTIONLABELSDICT[SELECTION] + ' Water-Out', 'rhc_air')
        RHC_Air.CPPCLASS.is_FHC = False
        RHC_Air.CPPCLASS.data_pot = T2KPOT.GetPOTRHCAirData()
        RHC_Air.CPPCLASS.scale = T2KPOT.GetPOTRHCAirData()/T2KPOT.GetPOTRHCAirMC()
        RHC_Air_Snd = sample(chn_SANDRun3AirRHC, SELECTIONLABELSDICT[SELECTION] + ' Water-Out, Sand', 'rhc_air_sand')
        RHC_Air_Snd.CPPCLASS.is_FHC = False
        RHC_Air_Snd.CPPCLASS.scale = T2KPOT.GetPOTRHCAirData() / T2KPOT.GetPOTRHCAirSandMC()

    all_samples = {
            'FHC_Wtr': {'Magnet': FHC_Wtr, 'Sand': FHC_Wtr_Snd},
            'RHC_Wtr': {'Magnet': RHC_Wtr, 'Sand': RHC_Wtr_Snd}
    }
    if not TN208_ANALYSIS:
        all_samples['FHC_Air'] = {'Magnet': FHC_Air, 'Sand': FHC_Air_Snd}
        all_samples['RHC_Air'] = {'Magnet': RHC_Air, 'Sand': RHC_Air_Snd}
    return all_samples


def GetDATAsamples():
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
    RunSyst_New_DATA_TTREE_name = 'nominal'
    DATAP6M = join(file_path, 'rdp6_Spin_M')
    DATAP6N = join(file_path, 'rdp6_Spin_N')
    T2KPOT = ROOT.TotalPOT()
    chn_DATARun2Air = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN2ADATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun2Wtr = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN2WDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun3bAir = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN3BDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun3cAir = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN3CDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun4Air = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN4ADATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun4Wtr = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN4WDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun5cWtr = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN5CDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun6bAir = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN6BDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun6cAir = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN6CDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun6dAir = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN6DDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun6eAir = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN6EDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun7bWtr = ROOTChain.get_all_from_to(RunSyst_New_DATA_TTREE_name, ROOT.T2KDataMC.RUN7BDATA.iter_name(DATAP6N), 1, 10)

    chn_FHC_Wtr = chn_DATARun4Wtr
    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONLABELSDICT[SELECTION] + ' FHC Water', 'fhc_water')
    if not TN208_ANALYSIS:
        chn_FHC_Wtr.Add(chn_DATARun2Wtr)
        FHC_Wtr.CPPCLASS.data_pot = T2KPOT.GetPOTFHCWaterData()
    else:
        FHC_Wtr.CPPCLASS.data_pot = T2KPOT.GetPOTRun4WaterData()

    if not TN208_ANALYSIS:
        chn_FHC_Air = chn_DATARun4Air
        FHC_Air = sample(chn_FHC_Air, SELECTIONLABELSDICT[SELECTION] + ' FHC Air', 'fhc_air')
        chn_FHC_Air.Add(chn_DATARun3bAir)
        chn_FHC_Air.Add(chn_DATARun3cAir)
        chn_FHC_Air.Add(chn_DATARun2Air)
        FHC_Air.CPPCLASS.data_pot = T2KPOT.GetPOTFHCAirData()

    chn_RHC_Wtr = chn_DATARun5cWtr
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONLABELSDICT[SELECTION] + ' RHC Water', 'rhc_water')
    if not TN208_ANALYSIS:
        chn_RHC_Wtr.Add(chn_DATARun7bWtr)
        RHC_Wtr.CPPCLASS.data_pot = T2KPOT.GetPOTRHCWaterData()
    else:
        RHC_Wtr.CPPCLASS.data_pot = T2KPOT.GetPOTRun5cWaterData()

    if not TN208_ANALYSIS:
        chn_RHC_Air = chn_DATARun6bAir
        chn_RHC_Air.Add(chn_DATARun6cAir)
        chn_RHC_Air.Add(chn_DATARun6dAir)
        chn_RHC_Air.Add(chn_DATARun6eAir)
        RHC_Air = sample(chn_RHC_Air, SELECTIONLABELSDICT[SELECTION] + ' RHC Air', 'rhc_air')
        RHC_Air.CPPCLASS.data_pot = T2KPOT.GetPOTRHCAirData()

    all_samples = {
            'FHC_Wtr': FHC_Wtr,
            'RHC_Wtr': RHC_Wtr,
    }
    if not TN208_ANALYSIS:
        all_samples['FHC_Air'] = FHC_Air
        all_samples['RHC_Air'] = RHC_Air
    return all_samples


def GetNeutrinoSelectionList():
    """
    Get a list of the selections (selection_info) for the neutrino species
    """

    cut = ROOT.DefineCuts()
    sampleID = ROOT.SampleId()
    all_nom_sel_cut = None

    if SELECTION == sampleID.GetP0DNuMuCC():
        all_nom_sel_cut = cut.muMinusSelection
    elif SELECTION == sampleID.GetP0DNuMuBkgInAntiNuModeCC():
        all_nom_sel_cut = cut.muMinusBkgInRHCSelection
    elif SELECTION == sampleID.GetP0DNuMuBarInAntiNuModeCC():
        all_nom_sel_cut = cut.muPlusInRHCSelection
    else:
        print 'ERROR: unable to determine sample in GetNeutrinoSelectionList'
        sys.exit(1)

    if ADDITIONAL_CUTS:
        all_nom_sel_cut = cut.AndTCuts(all_nom_sel_cut, ADDITIONAL_CUTS)

    if TN208_ANALYSIS:
        all_nom_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.FVTN208)

    if USE_MOMENTUM_CUT:
        all_nom_sel_cut = cut.AndTCuts(all_nom_sel_cut, 'LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    # all neutrinos
    all_nom_sel = selection_info('nom_sel', all_nom_sel_cut,
                                 'P0D numuCC-inclusive')

    # muon neutrinos
    numu_cc_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tParNuMu)
    if TN208_ANALYSIS:
        numu_cc_sel_cut = cut.AndTCuts(numu_cc_sel_cut, cut.tFVTN208)
    numu_cc_sel = selection_info('numu_cc_sel', numu_cc_sel_cut,
                                 '#nu_{#mu}')

    # muon antineutrinos
    numubar_cc_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tParNuMubar)
    if TN208_ANALYSIS:
        numubar_cc_sel_cut = cut.AndTCuts(numubar_cc_sel_cut, cut.tFVTN208)
    numubar_cc_sel = selection_info('numubar_cc_sel', numubar_cc_sel_cut,
                                    '#bar{#nu}_{#mu}')

    # nue and nuebars
    nuenuebar_cc_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tParNuEs)
    if TN208_ANALYSIS:
        nuenuebar_cc_sel_cut = cut.AndTCuts(nuenuebar_cc_sel_cut, cut.tFVTN208)
    nuenuebar_cc_sel = selection_info('nuenuebar_cc_sel', nuenuebar_cc_sel_cut,
                                      '#nu_{e} + #bar{#nu}_{e}')

    # other particles
    other_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tParOther)
    if TN208_ANALYSIS:
        other_sel_cut = cut.AndTCuts(other_sel_cut, cut.tFVTN208)
    other_sel = selection_info('other_sel', other_sel_cut, 'other')

    # OOFV but NOT sand
    offv_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tOOFV)
    if TN208_ANALYSIS:
        offv_sel_cut = cut.AndTCuts(offv_sel_cut, cut.tOOFVTN208)
    offv_sel = selection_info('offv_sel', offv_sel_cut, 'OOFV')

    # Sand neutrino
    sand_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tParNuSand)
    sand_sel = selection_info('sand_sel', sand_sel_cut, 'Sand')

    neutrino_selections = [all_nom_sel, numu_cc_sel, numubar_cc_sel,
                           nuenuebar_cc_sel, other_sel, offv_sel, sand_sel]

    return neutrino_selections


def GetLeptonCandidateSelectionList():
    """Make a list of lepton candidate cuts by particle"""

    cut = ROOT.DefineCuts()
    sampleID = ROOT.SampleId()
    all_nom_sel_cut = None

    if SELECTION == sampleID.GetP0DNuMuCC():
        all_nom_sel_cut = cut.muMinusSelection
    elif SELECTION == sampleID.GetP0DNuMuBkgInAntiNuModeCC():
        all_nom_sel_cut = cut.muMinusBkgInRHCSelection
    elif SELECTION == sampleID.GetP0DNuMuBarInAntiNuModeCC():
        all_nom_sel_cut = cut.muPlusInRHCSelection
    else:
        print 'ERROR: unable to determine sample in GetNeutrinoSelectionList'
        sys.exit(1)

    if ADDITIONAL_CUTS:
        all_nom_sel_cut = cut.AndTCuts(all_nom_sel_cut, ADDITIONAL_CUTS)

    if TN208_ANALYSIS:
        all_nom_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.FVTN208)

    if USE_MOMENTUM_CUT:
        all_nom_sel_cut = cut.AndTCuts(all_nom_sel_cut, 'LeptonMomNom<=%s' % MOMENTUM_CUT_VALUE)

    # all selection events
    all_nom_sel = selection_info('nom_sel', all_nom_sel_cut,
                                 'P0D numuCC-inclusive')
    # mu minuses
    muMinus_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tLepMuMinus)
    if TN208_ANALYSIS:
        muMinus_sel_cut = cut.AndTCuts(muMinus_sel_cut, cut.tFVTN208)
    muMinus_sel = selection_info('muMinus_sel', muMinus_sel_cut, '#mu^{-}')

    # mu plus
    muPlus_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tLepMuPlus)
    if TN208_ANALYSIS:
        muPlus_sel_cut = cut.AndTCuts(muPlus_sel_cut, cut.tFVTN208)
    muPlus_sel = selection_info('muPlus_sel', muPlus_sel_cut, '#mu^{+}')

    # pi minus
    piMinus_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tLepPiMinus)
    if TN208_ANALYSIS:
        piMinus_sel_cut = cut.AndTCuts(piMinus_sel_cut, cut.tFVTN208)
    piMinus_sel = selection_info('piMinus_sel', piMinus_sel_cut, '#pi^{-}')

    # pi plus
    piPlus_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tLepPiPlus)
    if TN208_ANALYSIS:
        piPlus_sel_cut = cut.AndTCuts(piPlus_sel_cut, cut.tFVTN208)
    piPlus_sel = selection_info('piPlus_sel', piPlus_sel_cut, '#pi^{+}')

    # pi plus
    proton_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tLepProton)
    if TN208_ANALYSIS:
        proton_sel_cut = cut.AndTCuts(proton_sel_cut, cut.tFVTN208)
    proton_sel = selection_info('proton_sel', proton_sel_cut, 'p')

    # EM particles
    em_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tLepEMParticle)
    if TN208_ANALYSIS:
        em_sel_cut = cut.AndTCuts(em_sel_cut, cut.tFVTN208)
    em_sel = selection_info('em_sel', em_sel_cut, 'e^{#pm}/#gamma')

    # other particles
    other_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tLepOther)
    if TN208_ANALYSIS:
        other_sel_cut = cut.AndTCuts(other_sel_cut, cut.tFVTN208)
    other_sel = selection_info('other_sel', other_sel_cut, 'other')

    # OOFV
    oofv_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tOOFV)
    if TN208_ANALYSIS:
        oofv_sel_cut = cut.AndTCuts(oofv_sel_cut, cut.tOOFVTN208)
    oofv_sel = selection_info('oofv_sel', oofv_sel_cut, 'OOFV')

    # sand muons
    # sandmu_sel_cut = ROOT.TCut('%s && %s' % (all_nom_sel_cut, cut.tLepSand))
    sandmu_sel_cut = cut.AndTCuts(all_nom_sel_cut, cut.tLepSand)
    sandmu_sel = selection_info('sandmu_sel', sandmu_sel_cut, 'Sand muons')

    particle_selections = [all_nom_sel, muMinus_sel, muPlus_sel,
                           piMinus_sel, piPlus_sel, proton_sel,
                           em_sel, other_sel, oofv_sel, sandmu_sel]
    return particle_selections


def ConfigureROOTHStack(hstack, anaBins, pot_str):
    """
    For the hstack (ROOTHStack), get options from anaBins (AnalysisBins),
    the pot_str (str) is just a string for the y_title
    """
    if len(anaBins.GetUnits()) < 1:
        return
    units = anaBins.GetUnits()
    hepconstants = ROOT.HEPConstants()
    hstack.plot_var += '*%f' % hepconstants.Convert(units)
    hstack.x_title += ' [%s]' % units
    if anaBins.GetDivideByBinWidth():
        y_title = 'Events / %s / %s'
        y_title_tuple = (units, pot_str)
        hstack.y_title = y_title % y_title_tuple


def LoadP0DBANFF():
    """Load in the necessary classes"""
    engine = ROOT.TXMLEngine()
    loadStatus = gSystem.Load("libP0DBANFF")
    if loadStatus != 1:
        print "unable to load libP0DBANFF.so"
        sys.exit(1)
    global INTERFACE, SAMPLEIDS
    try:
        INTERFACE = ROOT.P0DBANFFInterface()
        INTERFACE.SetBatch(True)
        INTERFACE.GetThisStyle().SetOptStat(0000)
        SAMPLEIDS = ROOT.SampleId()
    except Exception as exc:
        print type(exc)
        print "unable to load libP0DBANFF.so"
        sys.exit(1)
    global STACK_COLORS, BLACK
    BLACK = INTERFACE.kcbBlack
    STACK_COLORS.append(INTERFACE.kcbBlack)  # 0
    STACK_COLORS.append(INTERFACE.kcbBlue)  # 1
    STACK_COLORS.append(INTERFACE.kcbSky)  # 2
    STACK_COLORS.append(INTERFACE.kcbRed)  # 3
    STACK_COLORS.append(INTERFACE.kcbOrange)  # 4
    STACK_COLORS.append(INTERFACE.kcbPurple)  # 5
    STACK_COLORS.append(INTERFACE.kcbGreen)  # 6
    STACK_COLORS.append(INTERFACE.kcbYellow)  # 7
    STACK_COLORS.append(INTERFACE.kcbBrightGreen)  # 8
    STACK_COLORS.append(INTERFACE.kcbBrightPurple)  # 9
    STACK_COLORS.append(INTERFACE.kcbBrightGrey)  # 10
    del engine


def LoadSampleIDs():
    """Using the SampleIds class, load the samples set at the top of file"""
    global SELECTIONLABELSDICT, SELECTIONSAVENAMEDICT
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DWaterNuMuCC()] = '#nu_{#mu} in FHC,'
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DWaterNuMuBarInAntiNuModeCC()] = '#bar{#nu}_{#mu} in RHC Selection'
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DWaterNuMuBkgInAntiNuModeCC()] = '#nu_{#mu} Bkg in RHC Selection'
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DAirNuMuCC()] = '#nu_{#mu} in FHC,'
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DAirNuMuBarInAntiNuModeCC()] = '#bar{#nu}_{#mu} in RHC Selection'
    SELECTIONLABELSDICT[SAMPLEIDS.GetP0DAirNuMuBkgInAntiNuModeCC()] = '#nu_{#mu} Bkg in RHC Selection'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DWaterNuMuCC()] = 'numuCCIncWaterIn'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DWaterNuMuBarInAntiNuModeCC()] = 'numubarRHCCCIncWaterIn'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DWaterNuMuBkgInAntiNuModeCC()] = 'numubkgRHCCCIncWaterIn'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DAirNuMuCC()] = 'numuCCIncWaterOut'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DAirNuMuBarInAntiNuModeCC()] = 'numubarRHCCCIncWaterOut'
    SELECTIONSAVENAMEDICT[SAMPLEIDS.GetP0DAirNuMuBkgInAntiNuModeCC()] = 'numubkgRHCCCIncWaterOut'


if __name__ == "__main__":
    main(sys.argv[1:])
