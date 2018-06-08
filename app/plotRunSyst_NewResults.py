#!/usr/bin/env python2.7
"""
A script that makes histogram of the resulting TTree from
   RunSyst_New.exe
"""

import ROOT
from ROOT import TXMLEngine, TH1D, TH2D, THStack, TCanvas, TLegend, gSystem  # TChain
import ROOTChain
from ROOTHStack import ROOTHStack
import sys
from os import getenv
from os.path import join
import RunName as RN

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
# For PoT numbers
T2K = None
TN80 = None

# legend coordinates
X1 = 0.55  # 0.64196
Y1 = 0.5  # 0.606272
X2 = 0.894472
Y2 = 0.892765


def main(argv):
    """main"""
    helpstatement = "plotRunSyst_NewResults.py (no args)"
    p0dbanffroot = getenv('P0DBANFFROOT')
    if len(argv) > 0:
        print helpstatement
    engine = TXMLEngine()
    gSystem.Load("libP0DBANFF")
    global INTERFACE, T2K, TN80
    try:
        INTERFACE = ROOT.P0DBANFFInterface()
        INTERFACE.SetBatch(True)
        INTERFACE.GetThisStyle().SetOptStat(0000)
        T2K = ROOT.TotalPOT()
        TN80 = ROOT.TN80POT()
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
    if SELECTION == -1:
        # SELECTION = P0DNUMUBARCCSELECTION
        SELECTION = P0DNUMUCCSELECTION
    SELECTIONSTR = 'numubarCCInc' if (SELECTION == P0DNUMUBARCCSELECTION) else 'numuCCInc'
    SELECTIONDICT = {
            P0DNUMUCCSELECTION: '#mu^{-} Selection,',
            P0DNUMUBARCCSELECTION: '#mu^{+} Selection,'
    }
    del sampleIds

    # P0D_SAMPLE_BINS = None
    # if SELECTION == P0DNUMUCCSELECTION:
    #     P0D_SAMPLE_BINS = ROOT.Samples('P0DNumuFHC', '%s/config/SampleBinning.xml' % (p0dbanffroot))
    # if SELECTION == P0DNUMUBARCCSELECTION:
    #     P0D_SAMPLE_BINS = ROOT.Samples('P0DNumubarFHC', '%s/config/SampleBinning.xml' % (p0dbanffroot))

    xml = ROOT.XMLTools()
    # # A ROOT.AnalysisBins class
    # Pmu_AnaBins = P0D_SAMPLE_BINS.GetAnalysisMomentumBins()
    # CosTheta_AnaBins = P0D_SAMPLE_BINS.GetAnalysisCosThetaBins()
    Pmu_AnaBins = ROOT.AnalysisBins('Momentum',
                                    '%s/config/Binning.xml' % p0dbanffroot,
                                    xml)
    CosTheta_AnaBins = ROOT.AnalysisBins('CosTheta',
                                         '%s/config/Binning.xml' % p0dbanffroot,
                                         xml)
    Theta_AnaBins = ROOT.AnalysisBins('Theta',
                                      '%s/config/Binning.xml' % p0dbanffroot,
                                      xml)
    Enu_AnaBins = ROOT.AnalysisBins('NeutrinoEnergy',
                                    '%s/config/Binning.xml' % p0dbanffroot,
                                    xml)
    p0dZ_AnaBins = ROOT.AnalysisBins('P0DuleCoarseZ',
                                     '%s/config/Binning.xml' % p0dbanffroot,
                                     xml)
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

    # These store the Chains for numu_FHC numu_RHC, numubar_FHC, and numubar_RHC
    mc_samples_dict = GetMonteCarloSamples()
    data_samples_dict = GetDATAsamples()
    mc_data_sample_dict = dict()
    mc_data_sample_dict_MC_key = 'MC'
    mc_data_sample_dict_DATA_key = 'DATA'
    for key in mc_samples_dict.keys():
        mc_data_sample_dict[key] =\
            {
            mc_data_sample_dict_MC_key: mc_samples_dict[key],
            mc_data_sample_dict_DATA_key: data_samples_dict[key]
            }

    # these store how to break down the samples by particles or other grouping
    neutrino_selections = GetNeutrinoSelectionList()
    particle_selections = GetLeptonCandidateSelectionList()
    evts_p_bin = 'Events / bin'

    # loop over sample classes
    for smpls in mc_data_sample_dict.values():
        mc_sample = smpls[mc_data_sample_dict_MC_key]
        data_pot_exponent = INTERFACE.GetExponentBase10(mc_sample.data_pot)
        data_pot_mantissa = INTERFACE.GetMantissaBase10(mc_sample.data_pot,
                                                        data_pot_exponent)
        evts_p_bin_p_pot = '%s / %.2f #times 10^{%d} PoT' % (evts_p_bin,
                                                             data_pot_mantissa,
                                                             data_pot_exponent)

        # neutrino energy
        histstack_Enu = ROOTHStack()
        histstack_Enu.plot_var = 'TrueEnuNom*1e-3'
        histstack_Enu.x_title = 'True Neutrino Energy [GeV]'
        histstack_Enu.y_title = evts_p_bin_p_pot
        make_mc_stack(mc_sample, neutrino_selections, Enu_AnaBins,
                      histstack_Enu, 'trueE_nu')
        Enu_AnaBins.Reset()

        # lepton candidate momentum
        histstack_Pmu = ROOTHStack()
        histstack_Pmu.plot_var = 'LeptonMomNom*1e-3'
        histstack_Pmu.x_title = 'Lepton Candidate Momentum [GeV/c]'
        histstack_Pmu.y_title = evts_p_bin_p_pot
        # histstack_Pmu.set_log_y(True)
        make_stack(smpls, particle_selections, Pmu_AnaBins,
                   histstack_Pmu, 'recoP_mu_uniform')
        Pmu_AnaBins.Reset()

        # lepton candidate cos(theta)
        histstack_cosTheta = ROOTHStack()
        histstack_cosTheta.plot_var = 'LeptonCosNom'
        histstack_cosTheta.x_title = 'Lepton Candidate cos(#theta)'
        histstack_cosTheta.y_title = evts_p_bin_p_pot
        # hist_cosq.set_log_y(True)
        make_stack(smpls, particle_selections, CosTheta_AnaBins,
                   histstack_cosTheta, 'recocosq_mu_uniform')
        CosTheta_AnaBins.Reset()

        # lepton candidate theta
        histstack_theta = ROOTHStack()
        histstack_theta.plot_var = 'TMath::ACos(LeptonCosNom)*TMath::RadToDeg()'
        histstack_theta.x_title = 'Lepton Candidate #theta_{ND280}'
        histstack_theta.y_title = evts_p_bin_p_pot
        # hist_cosq.set_log_y(True)
        make_stack(smpls, particle_selections, Theta_AnaBins,
                   histstack_theta, 'recotheta_mu_uniform')
        CosTheta_AnaBins.Reset()

        # lepton p0d position
        histstack_p0dZ = ROOTHStack()
        histstack_p0dZ.plot_var = 'vtxZ'
        histstack_p0dZ.x_title = 'Track Start Z (mm)'
        histstack_p0dZ.y_title = evts_p_bin_p_pot
        make_stack(smpls, particle_selections, p0dZ_AnaBins,
                   histstack_p0dZ, 'recoZ_mu')
        p0dZ_AnaBins.Reset()
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
    """Take evt_sample (sample) and separate it by
    true_selections (selection_info). The histogram is stored in
    anaBins (ROOT.AnaysisBins) with the histogram labels in hstack (ROOTHStack)
    Saved as "save_title".root  and .png (str)
    """

    mc_sample = evt_sample['MC']
    data_sample = evt_sample['DATA']

    save_as = '%s_%s_%s' % (SELECTIONSTR, save_title, mc_sample.save_title)
    canvas = TCanvas("canvas", "", 800, 600)
    legend = TLegend(X1, Y1, X2, Y2, mc_sample.plot_title)
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
    data_hist = anaBins.GetTH1DClone('h1d_%s_%s' % (tmp_save_name, data_sample.save_title))
    INTERFACE.PrettyUpTH1(data_hist, hstack.x_title, hstack.y_title,
                          BLACK, BLACK)
    if anaBins.GetShowOverflow():
        legend.AddEntry(data_hist, 'DATA (Overflow shown)', 'LE')
    else:
        legend.AddEntry(data_hist, 'DATA', 'LE')
    anaBins.Reset()
    anaBins.Sumw2(False)
    data_stats = TLegend(0.5, 0.9, 0.7, 0.95, 'Integral %d' % data_hist.Integral())
    data_stats.SetBorderSize(1)
    data_stats.SetFillColor(0)
    data_stats.SetMargin(0.1)

    # MC loop over true_selections to create stack,
    #    first entry is full selection
    for index in range(1, len(true_selections)):
        a_selection = true_selections[index]
        tmp_save_name = '%s_%s' % (save_title, a_selection.name)
        nEntries = mc_sample.getTChain().Draw(plot_var, a_selection.cuts, 'goff')
        v1 = mc_sample.chain.GetV1()
        for entry_in_draw in range(nEntries):
            anaBins.Fill(v1[entry_in_draw])
        a_hist = anaBins.GetTH1DClone('h1d_%s_%s' % (tmp_save_name,
                                                     mc_sample.save_title))
        a_hist.Scale(mc_sample.scale)
        INTERFACE.PrettyUpTH1(a_hist, hstack.x_title, hstack.y_title, BLACK,
                              STACK_COLORS[index])
        legend.AddEntry(a_hist, a_selection.legend_label, 'f')
        mc_hists.append(a_hist)
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
    data_hist.Draw('SAME E1')
    legend.Draw()
    data_stats.Draw()
    mc_stats.Draw()

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
    data_hist.Delete()
    legend.Delete()
    canvas.Close()
    return


def make_mc_stack(mc_sample, true_selections, anaBins, hstack, save_title):
    """Take sample (mc_sample) and separate it by
    selection (true_selections). The histogram is stored in
    anaBins (AnaysisBins) with the histogram labels in hstack (ROOTHStack)
    Saved as save_title.root
    """

    save_as = '%s_%s_%s' % (SELECTIONSTR, save_title, mc_sample.save_title)
    canvas = TCanvas("canvas", "", 800, 600)
    legend = TLegend(X1, Y1, X2, Y2, mc_sample.plot_title)
    legend.SetFillStyle(0)
    legend.SetLineColor(0)
    legend.SetBorderSize(0)
    canvas.cd()

    plot_var = hstack.plot_var

    hists = []
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
                 'FHC_Wtr': FHC_Wtr,
                 'RHC_Wtr': RHC_Wtr,
                 'FHC_Air': FHC_Air,
                 'RHC_Air': RHC_Air
    }
    """
    RunSyst_New_NEUT_TTREE_name = 'all'
    file_path = getenv('SYSTEMATICSROOT')
    NEUTP6B = join(file_path, 'mcp6_Spin_B', 'neut')
    NEUTP6L = join(file_path, 'mcp6_Spin_L', 'neut')
    # chn_NEUTRun2Air = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun2Wtr = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun3bAir = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun3cAir = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun4Air = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun4Wtr = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun5cWtr = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun6bAir = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun6cAir = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun6dAir = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun6eAir = TChain(RunSyst_New_NEUT_TTREE_name)
    # chn_NEUTRun7bWtr = TChain(RunSyst_New_NEUT_TTREE_name)
    # Run2Air = RN.RUN2A.lowupjoin() + '_hadd.root'
    # Run2Wtr = RN.RUN2W.lowupjoin() + '_hadd.root'
    # Run3bAir = RN.RUN3B.lowupjoin() + '_hadd.root'
    # Run3cAir = RN.RUN3C.lowupjoin() + '_hadd.root'
    # Run4Air = RN.RUN4A.lowupjoin() + '_hadd.root'
    # Run4Wtr = RN.RUN4W.lowupjoin() + '_hadd.root'
    # Run5cWtr = RN.RUN5C.lowupjoin() + '_hadd.root'
    # Run6bAir = RN.RUN6B.lowupjoin() + '_hadd.root'
    # Run6cAir = RN.RUN6C.lowupjoin() + '_hadd.root'
    # Run6dAir = RN.RUN6D.lowupjoin() + '_hadd.root'
    # Run6eAir = RN.RUN6E.lowupjoin() + '_hadd.root'
    # Run7bWtr = RN.RUN7B.lowupjoin() + '_hadd.root'
    # chn_NEUTRun2Air.Add(join(file_path, NEUTP6B, Run2Air))
    # chn_NEUTRun2Wtr.Add(join(file_path, NEUTP6B, Run2Wtr))
    # chn_NEUTRun3bAir.Add(join(file_path, NEUTP6B, Run3bAir))
    # chn_NEUTRun3cAir.Add(join(file_path, NEUTP6B, Run3cAir))
    # chn_NEUTRun4Air.Add(join(file_path, NEUTP6B, Run4Air))
    # chn_NEUTRun4Wtr.Add(join(file_path, NEUTP6B, Run4Wtr))
    # chn_NEUTRun5cWtr.Add(join(file_path, NEUTP6B, Run5cWtr))
    # chn_NEUTRun6bAir.Add(join(file_path, NEUTP6B, Run6bAir))
    # chn_NEUTRun6dAir.Add(join(file_path, NEUTP6B, Run6dAir))
    # chn_NEUTRun6cAir.Add(join(file_path, NEUTP6B, Run6cAir))
    # chn_NEUTRun6eAir.Add(join(file_path, NEUTP6B, Run6eAir))
    # chn_NEUTRun7bWtr.Add(join(file_path, NEUTP6L, Run7bWtr))
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

    # FHC, P0D water-in
    chn_FHC_Wtr = chn_NEUTRun2Wtr
    chn_FHC_Wtr.Add(chn_NEUTRun4Wtr)

    # FHC, P0D water-out
    chn_FHC_Air = chn_NEUTRun2Air
    chn_FHC_Air.Add(chn_NEUTRun3bAir)
    chn_FHC_Air.Add(chn_NEUTRun3cAir)
    chn_FHC_Air.Add(chn_NEUTRun4Air)

    # RHC, P0D water-in
    chn_RHC_Wtr = chn_NEUTRun5cWtr
    chn_RHC_Wtr.Add(chn_NEUTRun7bWtr)

    # RHC, P0D water-out
    chn_RHC_Air = chn_NEUTRun6bAir
    chn_RHC_Air.Add(chn_NEUTRun6cAir)
    chn_RHC_Air.Add(chn_NEUTRun6dAir)
    chn_RHC_Air.Add(chn_NEUTRun6eAir)

    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONDICT[SELECTION] + ' FHC, Water-In', 'fhc_water')
    FHC_Air = sample(chn_FHC_Air, SELECTIONDICT[SELECTION] + ' FHC, Water-Out', 'fhc_air')
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONDICT[SELECTION] + ' RHC, Water-In', 'rhc_water')
    RHC_Air = sample(chn_RHC_Air, SELECTIONDICT[SELECTION] + ' RHC, Water-Out', 'rhc_air')

    # FHC_Wtr.scale = TN80.GetPOTWaterDataWitouthRun1()/T2K.GetPOTFHCWaterMC()
    # FHC_Air.scale = TN80.GetPOTAirData()/T2K.GetPOTFHCAirMC()
    FHC_Wtr.scale = T2K.GetPOTFHCWaterData()/T2K.GetPOTFHCWaterMC()
    FHC_Air.scale = T2K.GetPOTFHCAirData()/T2K.GetPOTFHCAirMC()
    RHC_Wtr.scale = T2K.GetPOTRHCWaterData()/T2K.GetPOTRHCWaterMC()
    RHC_Air.scale = T2K.GetPOTRHCAirData()/T2K.GetPOTRHCAirMC()

    # FHC_Wtr.data_pot = TN80.GetPOTWaterData()
    # FHC_Air.data_pot = TN80.GetPOTAirData()
    FHC_Wtr.data_pot = T2K.GetPOTFHCWaterData()
    FHC_Air.data_pot = T2K.GetPOTFHCAirData()
    RHC_Wtr.data_pot = T2K.GetPOTRHCWaterData()
    RHC_Air.data_pot = T2K.GetPOTRHCAirData()

    all_samples = {
            'FHC_Wtr': FHC_Wtr,
            'FHC_Air': FHC_Air,
            'RHC_Wtr': RHC_Wtr,
            'RHC_Air': RHC_Air
    }
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
    # chn_DATARun2Air = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun2Wtr = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun3bAir = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun3cAir = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun4Air = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun4Wtr = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun5cWtr = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun6bAir = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun6cAir = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun6dAir = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun6eAir = TChain(RunSyst_New_DATA_TTREE_name)
    # chn_DATARun7bWtr = TChain(RunSyst_New_DATA_TTREE_name)
    # Run2Air_data = RN.RUN2A.lowupjoin() + '_data_hadd.root'
    # Run2Wtr_data = RN.RUN2W.lowupjoin() + '_data_hadd.root'
    # Run3bAir_data = RN.RUN3B.lowupjoin() + '_data_hadd.root'
    # Run3cAir_data = RN.RUN3C.lowupjoin() + '_data_hadd.root'
    # Run4Air_data = RN.RUN4A.lowupjoin() + '_data_hadd.root'
    # Run4Wtr_data = RN.RUN4W.lowupjoin() + '_data_hadd.root'
    # Run5cWtr_data = RN.RUN5C.lowupjoin() + '_data_hadd.root'
    # Run6bAir_data = RN.RUN6B.lowupjoin() + '_data_hadd.root'
    # Run6cAir_data = RN.RUN6C.lowupjoin() + '_data_hadd.root'
    # Run6dAir_data = RN.RUN6D.lowupjoin() + '_data_hadd.root'
    # Run6eAir_data = RN.RUN6E.lowupjoin() + '_data_hadd.root'
    # Run7bWtr_data = RN.RUN7B.lowupjoin() + '_data_hadd.root'
    # chn_DATARun2Air.Add(join(file_path, DATA6M, Run2Air_data))
    # chn_DATARun2Wtr.Add(join(file_path, DATA6M, Run2Wtr_data))
    # chn_DATARun3bAir.Add(join(file_path, DATA6M, Run3bAir_data))
    # chn_DATARun3cAir.Add(join(file_path, DATA6M, Run3cAir_data))
    # chn_DATARun4Air.Add(join(file_path, DATA6M, Run4Air_data))
    # chn_DATARun4Wtr.Add(join(file_path, DATA6M, Run4Wtr_data))
    # chn_DATARun5cWtr.Add(join(file_path, DATA6M, Run5cWtr_data))
    # chn_DATARun6bAir.Add(join(file_path, DATA6M, Run6bAir_data))
    # chn_DATARun6dAir.Add(join(file_path, DATA6M, Run6dAir_data))
    # chn_DATARun6cAir.Add(join(file_path, DATA6M, Run6cAir_data))
    # chn_DATARun6eAir.Add(join(file_path, DATA6M, Run6eAir_data))
    # chn_DATARun7bWtr.Add(join(file_path, DATA6N, Run7bWtr_data))
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

    chn_FHC_Wtr = chn_DATARun2Wtr
    chn_FHC_Wtr.Add(chn_DATARun4Wtr)

    chn_FHC_Air = chn_DATARun2Air
    chn_FHC_Air.Add(chn_DATARun3bAir)
    chn_FHC_Air.Add(chn_DATARun3cAir)
    chn_FHC_Air.Add(chn_DATARun4Air)

    chn_RHC_Wtr = chn_DATARun5cWtr
    chn_RHC_Wtr.Add(chn_DATARun7bWtr)

    chn_RHC_Air = chn_DATARun6bAir
    chn_RHC_Air.Add(chn_DATARun6cAir)
    chn_RHC_Air.Add(chn_DATARun6dAir)
    chn_RHC_Air.Add(chn_DATARun6eAir)

    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONDICT[SELECTION] + ' FHC Water', 'fhc_water')
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONDICT[SELECTION] + ' RHC Water', 'rhc_water')
    FHC_Air = sample(chn_FHC_Air, SELECTIONDICT[SELECTION] + ' FHC Air', 'fhc_air')
    RHC_Air = sample(chn_RHC_Air, SELECTIONDICT[SELECTION] + ' RHC Air', 'rhc_air')

    # FHC_Wtr.data_pot = TN80.GetPOTWaterDataWitouthRun1()
    # FHC_Air.data_pot = TN80.GetPOTAirData()
    FHC_Wtr.data_pot = T2K.GetPOTFHCWaterData()
    FHC_Air.data_pot = T2K.GetPOTFHCAirData()
    RHC_Wtr.data_pot = T2K.GetPOTRHCWaterData()
    RHC_Air.data_pot = T2K.GetPOTRHCAirData()

    all_samples = {
            'FHC_Wtr': FHC_Wtr,
            'FHC_Air': FHC_Air,
            'RHC_Wtr': RHC_Wtr,
            'RHC_Air': RHC_Air
    }
    return all_samples


def GetNeutrinoSelectionList():
    """
    Get a list of the selections (selection_info) for the neutrino species
    """
    hepconstants = ROOT.HEPConstants()
    # all neutrinos
    all_nom_sel = selection_info('nom_sel', 'SelectionNom==%d' % (SELECTION),
                                 'P0D numuCC-inclusive')

    # muon neutrinos
    numu_cc_sel = selection_info('numu_cc_sel',
                                 '%s && TrueNuPDGNom==%d' % (all_nom_sel, hepconstants.kNuMuPDG),
                                 '#nu_{#mu}')

    # muon antineutrinos
    numubar_cc_sel = selection_info('numubar_cc_sel',
                                    '%s && TrueNuPDGNom==%s' % (all_nom_sel, hepconstants.kNuMuBarPDG),
                                    '#bar{#nu}_{#mu}')

    # nue and nuebars
    nuenuebar_cc_sel = selection_info('nuenuebar_cc_sel',
                                      '%s && abs(TrueNuPDGNom)==%s' % (
                                          all_nom_sel, hepconstants.kNuEPDG),
                                      '#nu_{e} + #bar{#nu}_{e}')

    # other particles
    other_sel_cut = '%s && abs(TrueNuPDGNom)!=%s && abs(TrueNuPDGNom)!=%s'
    other_sel_cut = other_sel_cut % (all_nom_sel, hepconstants.kNuMuPDG,
                                     hepconstants.kNuEPDG)
    other_sel = selection_info('other_sel', other_sel_cut, 'other')

    neutrino_selections = [all_nom_sel, numu_cc_sel, numubar_cc_sel,
                           nuenuebar_cc_sel, other_sel]
    return neutrino_selections


def GetLeptonCandidateSelectionList():
    """"""
    hepconstants = ROOT.HEPConstants()

    # all selection events
    all_nom_sel_cut = 'SelectionNom==%d' % (SELECTION)
    all_nom_sel = selection_info('nom_sel', all_nom_sel_cut,
                                 'P0D numuCC-inclusive')
    # mu minuses
    muMinus_sel_cut = '%s && tLeptonPDG==%d' % (all_nom_sel_cut,
                                                hepconstants.kMuMinusPDG)
    muMinus_sel = selection_info('muMinus_sel', muMinus_sel_cut, '#mu^{-}')

    # mu plus
    muPlus_sel_cut = '%s && tLeptonPDG==%d' % (all_nom_sel_cut,
                                               hepconstants.kMuPlusPDG)
    muPlus_sel = selection_info('muPlus_sel', muPlus_sel_cut, '#mu^{+}')

    # pi minus
    piMinus_sel_cut = '%s && tLeptonPDG==%d' % (all_nom_sel_cut,
                                                hepconstants.kPiMinusPDG)
    piMinus_sel = selection_info('piMinus_sel', piMinus_sel_cut, '#pi^{-}')

    # pi plus
    piPlus_sel_cut = '%s && tLeptonPDG==%d' % (all_nom_sel_cut,
                                               hepconstants.kPiPlusPDG)
    piPlus_sel = selection_info('piPlus_sel', piPlus_sel_cut, '#pi^{+}')

    # pi plus
    proton_sel_cut = '%s && tLeptonPDG==%d' % (all_nom_sel_cut,
                                               hepconstants.kProtonPDG)
    proton_sel = selection_info('proton_sel', proton_sel_cut, 'p')

    # EM particles
    em_sel_cut = all_nom_sel_cut
    em_sel_cut += ' && (tLeptonPDG==%d' % hepconstants.kGammaPDG
    em_sel_cut += ' || abs(tLeptonPDG)==%d)' % hepconstants.kElectronPDG
    em_sel = selection_info('em_sel', em_sel_cut, 'e^{#pm}/#gamma')

    # Ka Plus
    kaon_sel_cut = '%s && abs(tLeptonPDG)==abs(%d)' % (all_nom_sel_cut,
                                                       hepconstants.kKaPlusPDG)
    kaon_sel = selection_info('kaon_sel', kaon_sel_cut, '#kappa^{#pm}')

    # other particles
    other_sel_cut = all_nom_sel_cut
    other_sel_cut += ' && tLeptonPDG!=%s' % hepconstants.kMuMinusPDG
    other_sel_cut += ' && tLeptonPDG!=%s' % hepconstants.kMuPlusPDG
    other_sel_cut += ' && tLeptonPDG!=%s' % hepconstants.kPiMinusPDG
    other_sel_cut += ' && tLeptonPDG!=%s' % hepconstants.kPiPlusPDG
    other_sel_cut += ' && tLeptonPDG!=%s' % hepconstants.kProtonPDG
    other_sel_cut += ' && tLeptonPDG!=%s' % hepconstants.kGammaPDG
    other_sel_cut += ' && abs(tLeptonPDG)!=%s' % hepconstants.kElectronPDG
    other_sel_cut += ' && abs(tLeptonPDG)!=abs(%s)' % hepconstants.kKaPlusPDG
    other_sel = selection_info('other_sel', other_sel_cut, 'other')

    particle_selections = [all_nom_sel, muMinus_sel, muPlus_sel,
                           piMinus_sel, piPlus_sel, proton_sel,
                           em_sel, kaon_sel, other_sel]
    return particle_selections


if __name__ == "__main__":
    main(sys.argv[1:])
