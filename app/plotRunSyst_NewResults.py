#!/usr/bin/env python2.7
"""
A script that makes histogram of the resulting TTree from
   RunSyst_New.exe
"""

from os import getenv
from os.path import join
import ROOT
from ROOT import TH1D, TH2D, THStack, TCanvas, TLegend, gSystem  # TChain
import ROOTChain
from ROOTHStack import ROOTHStack
import RunName as RN
import sys

# toggle these to draw particular variables
DRAW_ENU = 0
DRAW_PMU = 1
DRAW_COSTHETAMU = 0
DRAW_PMU_TN328 = 0
DRAW_COSTHETAMU_TN328 = 0
DRAW_THETAMU = 1
DRAW_P0DX = 0
DRAW_P0DY = 0
DRAW_P0DZ = 0

# use the TN-208 runs
TN208_ANALYSIS = 0

# cut the measured momentum
USE_MOMENTUM_CUT = 1
MOMENTUM_CUT_VALUE = '5000.'

# Which selection to run
P0DNUMUCCSELECTION = -1
P0DNUMUBARCCSELECTION = -1
SELECTION = -1
SELECTIONSTR = str()
SELECTIONDICT = dict()

# P0DBANFFInterface class to make plots pretty
STACK_COLORS = list()
BLACK = int(1)

# Classes from P0DBANFF library
INTERFACE = None


def main(argv):
    """main"""
    helpstatement = "plotRunSyst_NewResults.py (no args)"
    p0dbanffroot = getenv('P0DBANFFROOT')
    if len(argv) > 0:
        print helpstatement
    engine = ROOT.TXMLEngine()
    gSystem.Load("libP0DBANFF")
    global INTERFACE
    try:
        INTERFACE = ROOT.P0DBANFFInterface()
        INTERFACE.SetBatch(True)
        INTERFACE.GetThisStyle().SetOptStat(0000)
    except Exception as exc:
        print type(exc)
        print "unable to load libP0DBANFF.so"
        sys.exit(1)

    global P0DNUMUCCSELECTION, P0DNUMUBARCCSELECTION, SELECTION, SELECTIONSTR, SELECTIONDICT
    sampleIds = ROOT.SampleId()
    if P0DNUMUCCSELECTION == -1:
        P0DNUMUCCSELECTION = sampleIds.GetP0DNuMuCC()
    if P0DNUMUBARCCSELECTION == -1:
        P0DNUMUBARCCSELECTION = sampleIds.GetP0DNuMuBarCC()
    del sampleIds
    SELECTIONDICT = {
            P0DNUMUCCSELECTION: '#mu^{-} Selection,',
            P0DNUMUBARCCSELECTION: '#mu^{+} Selection,'
    }

    # P0D_SAMPLE_BINS = None
    # if SELECTION == P0DNUMUCCSELECTION:
    #     P0D_SAMPLE_BINS = ROOT.Samples('P0DNumuFHC', '%s/config/SampleBinning.xml' % (p0dbanffroot))
    # if SELECTION == P0DNUMUBARCCSELECTION:
    #     P0D_SAMPLE_BINS = ROOT.Samples('P0DNumubarFHC', '%s/config/SampleBinning.xml' % (p0dbanffroot))

    xml = ROOT.XMLTools()
    # # A ROOT.AnalysisBins class
    # pMu_AnaBins = P0D_SAMPLE_BINS.GetAnalysisMomentumBins()
    # cosTheta_AnaBins = P0D_SAMPLE_BINS.GetAnalysiscosThetaBins()
    binningLocation = '%s/config/Binning.xml' % p0dbanffroot

    Enu_AnaBins = ROOT.AnalysisBins('NeutrinoEnergy', binningLocation, xml)
    pMu_TN328_AnaBins = ROOT.AnalysisBins('TN328Momentum', binningLocation, xml)
    cosThetaMu_TN328_AnaBins = ROOT.AnalysisBins('TN328CosTheta', binningLocation, xml)
    pMu_AnaBins = ROOT.AnalysisBins('Momentum', binningLocation, xml)
    cosThetaMu_AnaBins = ROOT.AnalysisBins('CosTheta', binningLocation, xml)
    thetaMu_AnaBins = ROOT.AnalysisBins('Theta', binningLocation, xml)
    p0dZ_AnaBins = ROOT.AnalysisBins('P0DuleCoarseZ', binningLocation, xml)
    p0dX_AnaBins = ROOT.AnalysisBins('P0DPositionX', binningLocation, xml)
    p0dY_AnaBins = ROOT.AnalysisBins('P0DPositionY', binningLocation, xml)

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

    selections_tuple = (P0DNUMUCCSELECTION, P0DNUMUBARCCSELECTION)

    for current_sel in selections_tuple:
        SELECTION = current_sel
        SELECTIONSTR = 'numubarCCInc' if (SELECTION == P0DNUMUBARCCSELECTION) else 'numuCCInc'

        # These store the Chains for numu_FHC numu_RHC, numubar_FHC, and numubar_RHC
        mc_samples_dict = GetMonteCarloSamples()
        data_samples_dict = GetDATAsamples()
        mc_data_sample_dict = dict()
        mc_data_sample_dict_MC_key = 'MC'
        mc_data_sample_dict_DATA_key = 'DATA'
        for sample_name in mc_samples_dict.keys():
            if TN208_ANALYSIS and 'Air' in sample_name:
                continue
            if TN208_ANALYSIS and SELECTION == P0DNUMUBARCCSELECTION and 'FHC' in sample_name:
                continue
            if TN208_ANALYSIS and SELECTION == P0DNUMUCCSELECTION and 'RHC' in sample_name:
                continue
            mc_data_sample_dict[sample_name] =\
                {
                mc_data_sample_dict_MC_key: mc_samples_dict[sample_name],
                mc_data_sample_dict_DATA_key: data_samples_dict[sample_name]
                }

        # these store how to break down the samples by particles or other grouping
        neutrino_selections = GetNeutrinoSelectionList()
        particle_selections = GetLeptonCandidateSelectionList()
        evts_p_bin = 'Events / bin'

        # loop over sample classes
        for smpls in mc_data_sample_dict.values():
            mc_sample = smpls[mc_data_sample_dict_MC_key]['Magnet']
            data_pot_exponent = INTERFACE.GetExponentBase10(mc_sample.data_pot)
            data_pot_mantissa = INTERFACE.GetMantissaBase10(mc_sample.data_pot,
                                                            data_pot_exponent)
            pot_str = '%.2f #times 10^{%d} PoT' % (data_pot_mantissa,
                                                   data_pot_exponent)
            evts_p_bin_p_pot = '%s / %s ' % (evts_p_bin, pot_str)

            # neutrino energy
            if DRAW_ENU:
                histstack_Enu = ROOTHStack()
                histstack_Enu.plot_var = 'TrueEnuNom'
                histstack_Enu.x_title = 'True Neutrino Energy'
                histstack_Enu.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_Enu, Enu_AnaBins, pot_str)
                make_mc_stack(mc_sample, neutrino_selections, Enu_AnaBins,
                              histstack_Enu, 'trueE_nu')

            # TN-328 Momentum
            if DRAW_PMU_TN328:
                histstack_pMu_TN328 = ROOTHStack()
                histstack_pMu_TN328.plot_var = 'LeptonMomNom'
                histstack_pMu_TN328.x_title = 'Lepton Candidate Momentum'
                histstack_pMu_TN328.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_pMu_TN328, pMu_TN328_AnaBins, pot_str)
                make_stack(smpls, particle_selections, pMu_TN328_AnaBins,
                           histstack_pMu_TN328, 'recoP_mu_TN328')

            # lepton candidate momentum
            if DRAW_PMU:
                histstack_pMu = ROOTHStack()
                histstack_pMu.plot_var = 'LeptonMomNom'
                histstack_pMu.x_title = 'Lepton Candidate Momentum'
                histstack_pMu.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_pMu, pMu_AnaBins, pot_str)
                make_stack(smpls, particle_selections, pMu_AnaBins,
                           histstack_pMu, 'recoP_mu_uniform')

            # TN-328 cos(theta)
            if DRAW_COSTHETAMU_TN328:
                histstack_cosThetaMu_TN328 = ROOTHStack()
                histstack_cosThetaMu_TN328.plot_var = 'LeptonCosNom'
                histstack_cosThetaMu_TN328.x_title = 'Lepton Candidate Angle'
                histstack_cosThetaMu_TN328.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_cosThetaMu_TN328,
                                    cosThetaMu_TN328_AnaBins, pot_str)
                make_stack(smpls, particle_selections, cosThetaMu_TN328_AnaBins,
                           histstack_cosThetaMu_TN328, 'recocosq_mu_TN328_uniform')

            # lepton candidate cos(theta)
            if DRAW_COSTHETAMU:
                histstack_cosThetaMu = ROOTHStack()
                histstack_cosThetaMu.plot_var = 'LeptonCosNom'
                histstack_cosThetaMu.x_title = 'Lepton Candidate Track Angle'
                histstack_cosThetaMu.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_cosThetaMu, cosThetaMu_AnaBins, pot_str)
                make_stack(smpls, particle_selections, cosThetaMu_AnaBins,
                           histstack_cosThetaMu, 'recocosq_mu_uniform')

            # lepton candidate thetaMu
            if DRAW_THETAMU:
                histstack_thetaMu = ROOTHStack()
                histstack_thetaMu.plot_var = 'TMath::ACos(LeptonCosNom)*TMath::RadToDeg()'
                histstack_thetaMu.x_title = 'Lepton Candidate Track Angle'
                histstack_thetaMu.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_thetaMu, thetaMu_AnaBins, pot_str)
                make_stack(smpls, particle_selections, thetaMu_AnaBins,
                           histstack_thetaMu, 'recothetaMu_mu_uniform')

            # lepton p0d position Z
            if DRAW_P0DZ:
                histstack_p0dZ = ROOTHStack()
                histstack_p0dZ.plot_var = 'vtxZ'
                histstack_p0dZ.x_title = 'Vertex Z'
                histstack_p0dZ.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_p0dZ, p0dZ_AnaBins, pot_str)
                make_stack(smpls, particle_selections, p0dZ_AnaBins,
                           histstack_p0dZ, 'recoZ_mu')

            # lepton p0d position X
            if DRAW_P0DX:
                histstack_p0dX = ROOTHStack()
                histstack_p0dX.plot_var = 'vtxX'
                histstack_p0dX.x_title = 'Vertex X'
                histstack_p0dX.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_p0dX, p0dX_AnaBins, pot_str)
                make_stack(smpls, particle_selections, p0dX_AnaBins,
                           histstack_p0dX, 'recoX_mu')

            # lepton p0d position Y
            if DRAW_P0DY:
                histstack_p0dY = ROOTHStack()
                histstack_p0dY.plot_var = 'vtxY'
                histstack_p0dY.x_title = 'Vertex Y'
                histstack_p0dY.y_title = evts_p_bin_p_pot
                ConfigureROOTHStack(histstack_p0dY, p0dY_AnaBins, pot_str)
                make_stack(smpls, particle_selections, p0dY_AnaBins,
                           histstack_p0dY, 'recoY_mu')
    del engine


class sample(object):
    """a way to store sample titles/labels"""

    def __init__(self, chain, plot_title, save_title):
        """set titles and input chain. plot_title put in legend
           save_title used when saved to file"""
        self.chain = chain
        self.plot_title = plot_title
        self.save_title = save_title
        self.scale = 1
        self.data_pot = 1

    def __str__(self):
        return self.plot_title

    def getTChain(self):
        """returns the TChain"""
        return self.chain

    def make_H1D(self, hist_save_title, var, n_bins, low, high, cuts=''):
        """create TH1 and return it"""
        if len(var) < 1:
            return None
        hist_name = 'h1d%s_%s' % (hist_save_title, self.save_title)
        hist = TH1D(hist_name, '', n_bins, low, high)
        self.chain.Draw('%s>>%s' % (var, hist_name), cuts, 'goff')
        if self.scale != 1:
            hist.Scale(self.scale)
        return hist

    def make_H2D(self, hist_save_title, varX, n_binsX, lowX, highX,
                 varY, n_binsY, lowY, highY, cuts=''):
        """create TH2 and return it"""
        if len(varX) < 1 or len(varY) < 1:
            return None
        hist_name = 'h2d%s_%s' % (hist_save_title, self.save_title)
        hist = TH2D(hist_name, '', n_binsX, lowX, highX,
                    n_binsY, lowY, highY)
        self.chain.Draw('%s:%s>>%s' % (varY, varX, hist_name), cuts, 'goff')
        if self.scale != 1:
            hist.Scale(self.scale)
        return hist


class selection_info(object):
    """store the name, cuts, and legend labels for a selection"""

    def __init__(self, name, cuts, legend_label):
        self.name = name
        self.cuts = cuts
        self.legend_label = legend_label

    def __str__(self):
        return self.cuts


def make_stack(evt_sample, true_selections, anaBins, hstack, save_title):
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

    save_as = '%s_%s_%s' % (SELECTIONSTR, save_title, mc_sample.save_title)
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
    tmp_save_name = '%s_%s' % (save_title, full_selection.name)
    nEntries = data_sample.getTChain().Draw(plot_var, full_selection.cuts, 'goff')
    v1 = data_sample.getTChain().GetV1()
    for entry_in_draw in range(nEntries):
        anaBins.Fill(v1[entry_in_draw])
    if anaBins.GetDivideByBinWidth():
        print 'WARNING:', anaBins.GetName(), 'dividing by bin width'
        anaBins.DivideByBinWidth(True)
    data_hist = anaBins.GetTH1DClone('h1d_%s_%s' % (tmp_save_name, data_sample.save_title))
    data_hist.SetMarkerStyle(20)
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
            mc_hist.Scale(mc_sample.scale)

        else:
            sand_nEntries = sand_sample.getTChain().Draw(plot_var, a_selection.cuts, 'goff')
            sand_v1 = sand_sample.getTChain().GetV1()
            for entry_in_draw in range(sand_nEntries):
                anaBins.Fill(sand_v1[entry_in_draw])
            if anaBins.GetDivideByBinWidth():
                anaBins.DivideByBinWidth()
            mc_hist = anaBins.GetTH1DClone('sand_h1d_%s_%s' % (tmp_save_name,
                                                               sand_sample.save_title))
            mc_hist.Scale(sand_sample.scale)

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

    canvas.cd()
    h_stack.Draw()
    h_total.Draw('SAME')
    data_hist.Draw('SAME E0')
    legend.Draw()
    data_stats.Draw()
    mc_stats.Draw()

    canvas.SetFillColor(0)
    # if(ROOT.gPad.GetPrimitive('TFrame')):
    #     ROOT.gPad.GetPrimitive('TFrame').SetFillColor(0)

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

    anaBins.Reset()
    h_total.Delete()
    h_stack.Delete()
    for a_hist in mc_hists:
        a_hist.Delete()
    data_hist.Delete()
    legend.Delete()
    canvas.Close()
    return


def make_mc_stack(mc_sample, true_selections, anaBins, hstack, save_title):
    """Take samples dictionary (mc_sand_sample) and separate it by
    selection (true_selections). The histogram is stored in
    anaBins (AnaysisBins) with the histogram labels in hstack (ROOTHStack)
    Saved as save_title.root
    """
    coords = ROOT.CanvasCoordinates()

    save_as = '%s_%s_%s' % (SELECTIONSTR, save_title, mc_sample.save_title)
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
    # full_selection = true_selections[0]
    for index in range(1, len(true_selections)):
        a_selection = true_selections[index]
        tmp_save_name = '%s_%s' % (save_title, a_selection.name)
        nEntries = mc_sample.getTChain().Draw(plot_var, a_selection.cuts, 'goff')
        v1 = mc_sample.getTChain().GetV1()
        for entry_in_draw in range(nEntries):
            anaBins.Fill(v1[entry_in_draw])
        a_hist = anaBins.GetTH1DClone('h1d_%s_%s' % (tmp_save_name, mc_sample.save_title))
        INTERFACE.PrettyUpTH1(a_hist, hstack.x_title, hstack.y_title,
                              BLACK, STACK_COLORS[index])
        legend.AddEntry(a_hist, a_selection.legend_label, 'f')
        a_hist.Scale(mc_sample.scale)
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
    # file_path = join(getenv('SYSTEMATICSROOT'), 'HMNT_or_HMPT_is_HMT')
    NEUTP6B = join(file_path, 'mcp6_Spin_B', 'neut')
    NEUTP6L = join(file_path, 'mcp6_Spin_L', 'neut')
    SAND = join(file_path, 'mcp6_Spin_B', 'sand')
    T2K = ROOT.TotalPOT()

    chn_NEUTRun2Air = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN2A.iter_name(NEUTP6B))
    chn_NEUTRun2Wtr = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN2W.iter_name(NEUTP6B))
    chn_NEUTRun3bAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN3B.iter_name(NEUTP6B))
    chn_NEUTRun3cAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN3C.iter_name(NEUTP6B))
    chn_NEUTRun4Air = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN4A.iter_name(NEUTP6B))
    chn_NEUTRun4Wtr = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN4W.iter_name(NEUTP6B))
    chn_NEUTRun5cWtr = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN5C.iter_name(NEUTP6B))
    chn_NEUTRun6bAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN6B.iter_name(NEUTP6B))
    chn_NEUTRun6cAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN6C.iter_name(NEUTP6B))
    chn_NEUTRun6dAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN6D.iter_name(NEUTP6B))
    chn_NEUTRun6eAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN6E.iter_name(NEUTP6B))
    chn_NEUTRun7bWtr = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN7B.iter_name(NEUTP6L))

    chn_SANDRun3AirFHC = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.SANDFHC.iter_name(SAND))
    chn_SANDRun3AirRHC = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.SANDRHC.iter_name(SAND))

    # FHC, P0D water-in
    chn_FHC_Wtr = chn_NEUTRun4Wtr
    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONDICT[SELECTION] + ' FHC, Water-In', 'fhc_water')
    FHC_Wtr_Snd = sample(chn_SANDRun3AirFHC, SELECTIONDICT[SELECTION] + ' FHC, Water-In, Sand', 'fhc_water_sand')
    if not TN208_ANALYSIS:
        chn_FHC_Wtr.Add(chn_NEUTRun2Wtr)
        FHC_Wtr.scale = T2K.GetPOTFHCWaterData()/T2K.GetPOTFHCWaterMC()
        FHC_Wtr.data_pot = T2K.GetPOTFHCWaterData()
        FHC_Wtr_Snd.scale = T2K.GetPOTFHCWaterData()/T2K.GetPOTFHCAirSandMC()
    else:
        FHC_Wtr.scale = T2K.GetPOTRun4WaterData()/T2K.GetPOTRun4WaterMC()
        FHC_Wtr.data_pot = T2K.GetPOTRun4WaterData()
        FHC_Wtr_Snd.scale = T2K.GetPOTRun4WaterData()/T2K.GetPOTFHCAirSandMC()

    # FHC, P0D water-out
    if not TN208_ANALYSIS:
        chn_FHC_Air = chn_NEUTRun4Air
        FHC_Air = sample(chn_FHC_Air, SELECTIONDICT[SELECTION] + ' FHC, Water-Out', 'fhc_air')
        FHC_Air_Snd = sample(chn_SANDRun3AirFHC, SELECTIONDICT[SELECTION] + ' FHC, Water-Out, Sand', 'fhc_air_sand')
        chn_FHC_Air.Add(chn_NEUTRun3bAir)
        chn_FHC_Air.Add(chn_NEUTRun3cAir)
        chn_FHC_Air.Add(chn_NEUTRun2Air)
        FHC_Air.scale = T2K.GetPOTFHCAirData()/T2K.GetPOTFHCAirMC()
        FHC_Air.data_pot = T2K.GetPOTFHCAirData()
        FHC_Air_Snd.scale = T2K.GetPOTFHCAirData()/T2K.GetPOTFHCAirSandMC()

    # RHC, P0D water-in
    chn_RHC_Wtr = chn_NEUTRun5cWtr
    RHC_Wtr_Snd = sample(chn_SANDRun3AirRHC, SELECTIONDICT[SELECTION] + ' RHC, Water-In, Sand', 'rhc_water_sand')
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONDICT[SELECTION] + ' RHC, Water-In', 'rhc_water')
    if not TN208_ANALYSIS:
        chn_RHC_Wtr.Add(chn_NEUTRun7bWtr)
        RHC_Wtr.scale = T2K.GetPOTRHCWaterData()/T2K.GetPOTRHCWaterMC()
        RHC_Wtr.data_pot = T2K.GetPOTRHCWaterData()
        RHC_Wtr_Snd.scale = T2K.GetPOTRHCWaterData()/T2K.GetPOTRHCAirSandMC()
    else:
        RHC_Wtr.scale = T2K.GetPOTRun5cWaterData()/T2K.GetPOTRun5cWaterMC()
        RHC_Wtr.data_pot = T2K.GetPOTRun5cWaterData()
        RHC_Wtr_Snd.scale = T2K.GetPOTRun5cWaterData()/T2K.GetPOTRHCAirSandMC()

    # RHC, P0D water-out
    if not TN208_ANALYSIS:
        chn_RHC_Air = chn_NEUTRun6bAir
        chn_RHC_Air.Add(chn_NEUTRun6cAir)
        chn_RHC_Air.Add(chn_NEUTRun6dAir)
        chn_RHC_Air.Add(chn_NEUTRun6eAir)
        RHC_Air = sample(chn_RHC_Air, SELECTIONDICT[SELECTION] + ' RHC, Water-Out', 'rhc_air')
        RHC_Air.data_pot = T2K.GetPOTRHCAirData()
        RHC_Air.scale = T2K.GetPOTRHCAirData()/T2K.GetPOTRHCAirMC()
        RHC_Air_Snd = sample(chn_SANDRun3AirRHC, SELECTIONDICT[SELECTION] + ' RHC, Water-Out, Sand', 'rhc_air_sand')
        RHC_Air_Snd.scale = T2K.GetPOTRHCAirData()/T2K.GetPOTRHCAirSandMC()

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
    # file_path = join(getenv('SYSTEMATICSROOT'), 'HMNT_or_HMPT_is_HMT')
    RunSyst_New_DATA_TTREE_name = 'nominal'
    DATAP6M = join(file_path, 'rdp6_Spin_M')
    DATAP6N = join(file_path, 'rdp6_Spin_N')
    T2K = ROOT.TotalPOT()
    chn_DATARun2Air = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN2ADATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun2Wtr = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN2WDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun3bAir = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN3BDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun3cAir = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN3CDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun4Air = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN4ADATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun4Wtr = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN4WDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun5cWtr = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN5CDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun6bAir = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN6BDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun6cAir = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN6CDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun6dAir = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN6DDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun6eAir = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN6EDATA.iter_name(DATAP6M), 1, 10)
    chn_DATARun7bWtr = ROOTChain.get(RunSyst_New_DATA_TTREE_name, RN.RUN7BDATA.iter_name(DATAP6N), 1, 10)

    chn_FHC_Wtr = chn_DATARun4Wtr
    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONDICT[SELECTION] + ' FHC Water', 'fhc_water')
    if not TN208_ANALYSIS:
        chn_FHC_Wtr.Add(chn_DATARun2Wtr)
        FHC_Wtr.data_pot = T2K.GetPOTFHCWaterData()
    else:
        FHC_Wtr.data_pot = T2K.GetPOTRun4WaterData()

    if not TN208_ANALYSIS:
        chn_FHC_Air = chn_DATARun4Air
        FHC_Air = sample(chn_FHC_Air, SELECTIONDICT[SELECTION] + ' FHC Air', 'fhc_air')
        chn_FHC_Air.Add(chn_DATARun3bAir)
        chn_FHC_Air.Add(chn_DATARun3cAir)
        chn_FHC_Air.Add(chn_DATARun2Air)
        FHC_Air.data_pot = T2K.GetPOTFHCAirData()

    chn_RHC_Wtr = chn_DATARun5cWtr
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONDICT[SELECTION] + ' RHC Water', 'rhc_water')
    if not TN208_ANALYSIS:
        chn_RHC_Wtr.Add(chn_DATARun7bWtr)
        RHC_Wtr.data_pot = T2K.GetPOTRHCWaterData()
    else:
        RHC_Wtr.data_pot = T2K.GetPOTRun5cWaterData()

    if not TN208_ANALYSIS:
        chn_RHC_Air = chn_DATARun6bAir
        chn_RHC_Air.Add(chn_DATARun6cAir)
        chn_RHC_Air.Add(chn_DATARun6dAir)
        chn_RHC_Air.Add(chn_DATARun6eAir)
        RHC_Air = sample(chn_RHC_Air, SELECTIONDICT[SELECTION] + ' RHC Air', 'rhc_air')
        RHC_Air.data_pot = T2K.GetPOTRHCAirData()

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
    elif SELECTION == sampleID.GetP0DNuMuBarCC():
        all_nom_sel_cut = cut.muPlusSelection
    else:
        print 'ERROR: unable to determine sample in GetNeutrinoSelectionList'
        sys.exit(1)
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
    elif SELECTION == sampleID.GetP0DNuMuBarCC():
        all_nom_sel_cut = cut.muPlusSelection
    else:
        print 'ERROR: unable to determine sample in GetNeutrinoSelectionList'
        sys.exit(1)

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


if __name__ == "__main__":
    main(sys.argv[1:])
