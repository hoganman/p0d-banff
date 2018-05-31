#!/usr/bin/env python2.7
"""
A script that makes histogram of the resulting TTree from
   RunSyst_New.exe
"""

import ROOT
from ROOT import TXMLEngine, TH1D, TH2D, THStack, TCanvas, TLegend, gSystem, TChain
# import ROOTChain
from ROOTHStack import ROOTHStack
import sys
from os import getenv
from os.path import join
import RunName as RN

# Which selection to run
P0DNUMUCCSELECTION = 88
P0DNUMUBARCCSELECTION = 89
SELECTION = P0DNUMUBARCCSELECTION
SELECTIONDICT = {
        P0DNUMUCCSELECTION: '#nu_{#mu} CCInc.,',
        P0DNUMUBARCCSELECTION: '#bar{#nu}_{#mu} CCInc.,'
}

# P0DBANFFInterface class to make plots pretty
INTFACE = None
STACK_COLORS = list()
BLACK = 1

# Classes from P0DBANFF library for POT numbers
T2K = None
TN80 = None

# legend coordinates
X1 = 0.64196
Y1 = 0.606272
X2 = 0.894472
Y2 = 0.892765


class sample(object):
    """a way to store sample titles/labels"""

    def __init__(self, chain, plot_title, save_title):
        """set titles and input chain. plot_title put in legend
           save_title used when saved to file"""
        self.chain = chain
        self.plot_title = plot_title
        self.save_title = save_title
        self.scale = 1
        self.pot_scale = 'DEFAULT'

    def __str__(self):
        return self.plot_title

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


def make_stack(evt_sample, neutrino_selections, hist_labels, save_title):
    """Take sample (evt_sample) and separate it by
    selection (neutrino_selections). The histogram labels are stored in
    hist_labels and saved as save_title.root """

    save_as = '%s_%s' % (save_title, evt_sample.save_title)
    canvas = TCanvas("canvas", "", 800, 600)
    legend = TLegend(X1, Y1, X2, Y2, evt_sample.plot_title)
    legend.SetFillStyle(0)
    legend.SetLineColor(0)
    legend.SetBorderSize(0)
    canvas.cd()

    plot_var = hist_labels.plot_var
    n_bins = hist_labels.n_bins
    low_X = hist_labels.low_X
    high_X = hist_labels.high_X

    hists = []
    for index in range(0, len(neutrino_selections)):
        a_selection = neutrino_selections[index]
        a_hist = evt_sample.make_H1D('%s_%s' % (save_title, a_selection.name),
                                     plot_var, n_bins, low_X, high_X,
                                     a_selection.cuts)
        INTFACE.PrettyUpTH1(a_hist, hist_labels.x_title, hist_labels.y_title,
                            BLACK, STACK_COLORS[index])
        legend.AddEntry(a_hist, a_selection.legend_label, 'f')
        hists.append(a_hist)

    hists.reverse()

    h_stack = THStack('%s_stack' % (save_title), '')
    h_total = TH1D('%s_total' % (save_title), '', n_bins, low_X, high_X)
    INTFACE.PrettyUpTH1(h_total, hist_labels.x_title, hist_labels.y_title,
                        BLACK)
    for index in range(0, len(neutrino_selections)):
        a_hist = hists[index]
        h_stack.Add(a_hist)
        h_total.Add(a_hist)

    h_stack.Draw()
    INTFACE.PrettyUpTHStack(h_stack, hist_labels.x_title, hist_labels.y_title)
    if hist_labels.minimum > -1:
        new_min = hist_labels.minimum
        h_stack.SetMinimum(new_min)
        h_total.SetMinimum(new_min)
    old_max = h_total.GetMaximum()
    if hist_labels.maximum > -1:
        new_max = hist_labels.maximum
        h_stack.SetMaximum(new_max)
        h_total.SetMaximum(new_max)

    h_total.Draw()
    h_stack.Draw('same')
    legend.Draw()

    INTFACE.SaveCanvasAs(canvas, join('plots', save_as))

    if hist_labels.log_y:
        canvas.SetLogy(1)
        h_stack.SetMinimum(1)
        h_total.SetMinimum(1)
        h_stack.SetMaximum(old_max)
        h_total.SetMaximum(old_max)
        h_total.Draw()
        h_stack.Draw('same')
        INTFACE.SaveCanvasAs(
                canvas, join('plots', '%s_log' % (save_as)))
        canvas.SetLogy(0)

    h_total.Delete()
    h_stack.Delete()
    for index in range(0, len(neutrino_selections)):
        a_hist = hists[index]
        a_hist.Delete()
    legend.Delete()
    canvas.Close()
    return


def GetNEUTsamples():
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
    chn_NEUTRun2Air = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun2Wtr = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun3bAir = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun3cAir = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun4Air = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun4Wtr = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun5cWtr = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun6bAir = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun6cAir = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun6dAir = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun6eAir = TChain(RunSyst_New_NEUT_TTREE_name)
    chn_NEUTRun7bWtr = TChain(RunSyst_New_NEUT_TTREE_name)
    Run2Air = RN.RUN2A.lowupjoin() + '_hadd.root'
    Run2Wtr = RN.RUN2W.lowupjoin() + '_hadd.root'
    Run3bAir = RN.RUN3B.lowupjoin() + '_hadd.root'
    Run3cAir = RN.RUN3C.lowupjoin() + '_hadd.root'
    Run4Air = RN.RUN4A.lowupjoin() + '_hadd.root'
    Run4Wtr = RN.RUN4W.lowupjoin() + '_hadd.root'
    Run5cWtr = RN.RUN5C.lowupjoin() + '_hadd.root'
    Run6bAir = RN.RUN6B.lowupjoin() + '_hadd.root'
    Run6cAir = RN.RUN6C.lowupjoin() + '_hadd.root'
    Run6dAir = RN.RUN6D.lowupjoin() + '_hadd.root'
    Run6eAir = RN.RUN6E.lowupjoin() + '_hadd.root'
    Run7bWtr = RN.RUN7B.lowupjoin() + '_hadd.root'
    chn_NEUTRun2Air.Add(join(file_path, NEUTP6B, Run2Air))
    chn_NEUTRun2Wtr.Add(join(file_path, NEUTP6B, Run2Wtr))
    chn_NEUTRun3bAir.Add(join(file_path, NEUTP6B, Run3bAir))
    chn_NEUTRun3cAir.Add(join(file_path, NEUTP6B, Run3cAir))
    chn_NEUTRun4Air.Add(join(file_path, NEUTP6B, Run4Air))
    chn_NEUTRun4Wtr.Add(join(file_path, NEUTP6B, Run4Wtr))
    chn_NEUTRun5cWtr.Add(join(file_path, NEUTP6B, Run5cWtr))
    chn_NEUTRun6bAir.Add(join(file_path, NEUTP6B, Run6bAir))
    chn_NEUTRun6dAir.Add(join(file_path, NEUTP6B, Run6dAir))
    chn_NEUTRun6cAir.Add(join(file_path, NEUTP6B, Run6cAir))
    chn_NEUTRun6eAir.Add(join(file_path, NEUTP6B, Run6eAir))
    chn_NEUTRun7bWtr.Add(join(file_path, NEUTP6L, Run7bWtr))
    # chn_NEUTRun2Air = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN2A.iter_name(P6B))
    # chn_NEUTRun2Wtr = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN2W.iter_name(P6B))
    # chn_NEUTRun3bAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN3B.iter_name(P6B))
    # chn_NEUTRun3cAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN3C.iter_name(P6B))
    # chn_NEUTRun4Air = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN4A.iter_name(P6B))
    # chn_NEUTRun4Wtr = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN4W.iter_name(P6B))
    # chn_NEUTRun5cWtr = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN5C.iter_name(P6B))
    # chn_NEUTRun6bAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN6B.iter_name(P6B))
    # chn_NEUTRun6cAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN6C.iter_name(P6B))
    # chn_NEUTRun6dAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN6D.iter_name(P6B))
    # chn_NEUTRun6eAir = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN6E.iter_name(P6B))
    # chn_NEUTRun7bWtr = ROOTChain.get(RunSyst_New_NEUT_TTREE_name, RN.RUN7B.iter_name(P6L))
    chn_FHC_Wtr = chn_NEUTRun2Wtr
    chn_FHC_Wtr.Add(chn_NEUTRun4Wtr)

    chn_FHC_Air = chn_NEUTRun2Air
    chn_FHC_Air.Add(chn_NEUTRun3bAir)
    chn_FHC_Air.Add(chn_NEUTRun3cAir)
    chn_FHC_Air.Add(chn_NEUTRun4Air)

    chn_RHC_Wtr = chn_NEUTRun5cWtr
    chn_RHC_Wtr.Add(chn_NEUTRun7bWtr)

    chn_RHC_Air = chn_NEUTRun6bAir
    chn_RHC_Air.Add(chn_NEUTRun6cAir)
    chn_RHC_Air.Add(chn_NEUTRun6dAir)
    chn_RHC_Air.Add(chn_NEUTRun6eAir)

    FHC_Wtr = sample(chn_FHC_Wtr, SELECTIONDICT[SELECTION] + ' FHC, P0D Water', 'fhc_water')
    RHC_Wtr = sample(chn_RHC_Wtr, SELECTIONDICT[SELECTION] + ' RHC, P0D Water', 'rhc_water')
    FHC_Air = sample(chn_FHC_Air, SELECTIONDICT[SELECTION] + ' FHC, P0D Air', 'fhc_air')
    RHC_Air = sample(chn_RHC_Air, SELECTIONDICT[SELECTION] + ' RHC, P0D Air', 'rhc_air')

    FHC_Wtr.scale = TN80.GetPOTWaterData()/T2K.GetPOTFHCWaterMC()
    FHC_Air.scale = TN80.GetPOTAirData()/T2K.GetPOTFHCAirMC()
    RHC_Wtr.scale = T2K.GetPOTRHCWaterData()/T2K.GetPOTRHCWaterMC()
    RHC_Air.scale = T2K.GetPOTRHCAirData()/T2K.GetPOTRHCAirMC()

    FHC_Wtr.pot_scale = TN80.GetPOTWaterData()
    FHC_Air.pot_scale = TN80.GetPOTAirData()
    RHC_Wtr.pot_scale = T2K.GetPOTRHCWaterData()
    RHC_Air.pot_scale = T2K.GetPOTRHCAirData()

    all_samples = {
            'FHC_Wtr': FHC_Wtr,
            'RHC_Wtr': RHC_Wtr,
            'FHC_Air': FHC_Air,
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
                                                            }
    """
    file_path = getenv('SYSTEMATICSROOT')
    RunSyst_New_DATA_TTREE_name = 'nominal'
    DATA6M = join(file_path, 'mcp6_Spin_M')
    DATA6N = join(file_path, 'mcp6_Spin_N')
    chn_DATARun2Air = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun2Wtr = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun3bAir = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun3cAir = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun4Air = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun4Wtr = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun5cWtr = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun6bAir = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun6cAir = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun6dAir = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun6eAir = TChain(RunSyst_New_DATA_TTREE_name)
    chn_DATARun7bWtr = TChain(RunSyst_New_DATA_TTREE_name)
    Run2Air_data = 'Run2_Air_data_hadd.root'
    Run2Wtr_data = 'Run2_Water_data_hadd.root'
    Run3bAir_data = 'Run3b_Air_data_hadd.root'
    Run3cAir_data = 'Run3c_Air_data_hadd.root'
    Run4Air_data = 'Run4_Air_data_hadd.root'
    Run4Wtr_data = 'Run4_Water_data_hadd.root'
    Run5cWtr_data = 'Run5c_Water_data_hadd.root'
    Run6bAir_data = 'Run6b_Air_data_hadd.root'
    Run6cAir_data = 'Run6c_Air_data_hadd.root'
    Run6dAir_data = 'Run6d_Air_data_hadd.root'
    Run6eAir_data = 'Run6e_Air_data_hadd.root'
    Run7bWtr_data = 'Run7b_Water_data_hadd.root'
    chn_DATARun2Air.Add(join(file_path, DATA6M, Run2Air_data))
    chn_DATARun2Wtr.Add(join(file_path, DATA6M, Run2Wtr_data))
    chn_DATARun3bAir.Add(join(file_path, DATA6M, Run3bAir_data))
    chn_DATARun3cAir.Add(join(file_path, DATA6M, Run3cAir_data))
    chn_DATARun4Air.Add(join(file_path, DATA6M, Run4Air_data))
    chn_DATARun4Wtr.Add(join(file_path, DATA6M, Run4Wtr_data))
    chn_DATARun5cWtr.Add(join(file_path, DATA6M, Run5cWtr_data))
    chn_DATARun6bAir.Add(join(file_path, DATA6M, Run6bAir_data))
    chn_DATARun6dAir.Add(join(file_path, DATA6M, Run6dAir_data))
    chn_DATARun6cAir.Add(join(file_path, DATA6M, Run6cAir_data))
    chn_DATARun6eAir.Add(join(file_path, DATA6M, Run6eAir_data))
    chn_DATARun7bWtr.Add(join(file_path, DATA6N, Run7bWtr_data))
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

    all_samples = {
            'FHC_Wtr': FHC_Wtr,
            'RHC_Wtr': RHC_Wtr,
            'FHC_Air': FHC_Air,
            'RHC_Air': RHC_Air
    }
    return all_samples


def main(argv):
    """main"""
    helpstatement = "plotRunSyst_NewResults.py (no args)"
    if len(argv) > 0:
        print helpstatement
    engine = TXMLEngine()
    gSystem.Load("libP0DBANFF")
    global INTFACE, T2K, TN80
    try:
        INTFACE = ROOT.P0DBANFFInterface()
        T2K = ROOT.TotalPOT()
        TN80 = ROOT.TN80POT()
    except Exception as exc:
        print type(exc)
        print "unable to load libP0DBANFF.so"
        sys.exit(1)

    INTFACE.SetBatch(True)

    p0dbanffroot = getenv('P0DBANFFROOT')
    xmltools = ROOT.XMLTools()
    xmltools.SetFile('%s/config/SampleBinning.xml' % (p0dbanffroot))
    # P0DNumuFHC = ROOT.Samples('P0DNumuFHC', '%s/config/SampleBinning.xml' % (p0dbanffroot))
    # P0DNumuRHC = ROOT.Samples('P0DNumuRHC', '%s/config/SampleBinning.xml' % (p0dbanffroot))
    # P0DNumubarFHC = ROOT.Samples('P0DNumubarFHC', '%s/config/SampleBinning.xml' % (p0dbanffroot))
    # P0DNumubarRHC = ROOT.Samples('P0DNumubarRHC', '%s/config/SampleBinning.xml' % (p0dbanffroot))

    global STACK_COLORS, BLACK
    BLACK = INTFACE.kcbBlack
    STACK_COLORS.append(INTFACE.kcbBlack)
    STACK_COLORS.append(INTFACE.kcbBlue)
    STACK_COLORS.append(INTFACE.kcbSky)
    STACK_COLORS.append(INTFACE.kcbRed)

    hepconstants = ROOT.HEPConstants()

    neut_samples_dict = GetNEUTsamples()
    neut_samples = neut_samples_dict.values()
    # data_samples_dict = GetDATAsamples()
    # data_samples = data_samples_dict.values()

    all_nom_sel = selection_info('nom_sel', 'SelectionNom==%d' % (SELECTION),
                                 'P0D numuCC-inclusive')
    numu_cc_sel = selection_info('numu_cc_sel',
                                 '%s && TrueNuPDGNom==%d' % (all_nom_sel, hepconstants.kNuMuPDG),
                                 '#nu_{#mu}')
    numubar_cc_sel = selection_info('numubar_cc_sel',
                                    '%s && TrueNuPDGNom==%s' % (all_nom_sel, hepconstants.kNuMuBarPDG),
                                    '#bar{#nu}_{#mu}')
    nuenuebar_cc_sel = selection_info('nuenuebar_cc_sel',
                                      '%s && \
abs(TrueNuPDGNom)==%s' % (all_nom_sel, hepconstants.kNuEPDG), '#nu_{e} + #bar{#nu}_{e}')
    other_sel = selection_info('other_sel',
                               '%s && abs(TrueNuPDGNom)!=%s && \
abs(TrueNuPDGNom)!=%s' % (all_nom_sel, hepconstants.kNuMuPDG, hepconstants.kNuEPDG), 'other')

    neutrino_selections = [
            numu_cc_sel,
            numubar_cc_sel,
            nuenuebar_cc_sel,
            other_sel
    ]
    evts_p_bin = 'Events / bin'

    # loop over sample classes
    for smpl in neut_samples:
        evts_p_bin_p_pot = '%s / %s PoT' % (evts_p_bin, smpl.pot_scale)

        # neutrino energy
        hist_Enu = ROOTHStack('TrueEnuNom*1e-3', 30, 0., 3.0,
                              'True Neutrino Energy [GeV]',
                              evts_p_bin_p_pot)
        hist_Enu.set_log_y(True)
        hist_Enu.set_min(1)
        # hist_Enu.set_max(1.1*8e+3)  # hard coded number
        make_stack(smpl, neutrino_selections, hist_Enu, 'trueE_nu')

        # # lepton candidate momentum
        # hist_Pmu = ROOTHStack('LeptonMomNom*1e-3', 35, 0., 5.0,
        #                       'Lepton Candidate Momentum [GeV/c]',
        #                       evts_p_bin_p_pot)
        # hist_Pmu.set_log_y(True)
        # make_stack(smpl, neutrino_selections, hist_Pmu, 'recoP_mu')

        # # lepton candidate cos(theta)
        # hist_cosq = ROOTHStack('LeptonCosNom', 25, 0.5, 1.0,
        #                        'Lepton Candidate cos(#theta)',
        #                        evts_p_bin_p_pot)
        # hist_cosq.set_log_y(True)
        # make_stack(smpl, neutrino_selections, hist_cosq, 'recocosq_mu')

        # # lepton candidate theta
        # hist_q = ROOTHStack('TMath::ACos(LeptonCosNom)*TMath::RadToDeg()',
        #                     50, 0.0, 90., 'Lepton Candidate #theta',
        #                     evts_p_bin_p_pot)
        # hist_q.set_log_y(True)
        # make_stack(smpl, neutrino_selections, hist_q, 'recoq_mu')

        # # lepton candidate true Z position
        # hist_trueZ = ROOTHStack('tVtxZ*1e-3', 29, -3.2, -1.2,
        #                         'True Vertex Z Position [m]',
        #                         evts_p_bin_p_pot)
        # hist_trueZ.set_log_y(True)
        # make_stack(smpl, neutrino_selections, hist_trueZ, 'trueVtxZ')

        # # lepton candidate reco Z position
        # hist_recoZ = ROOTHStack('vtxZ*1e-3', 29, -3.2, -1.2,
        #                         'Reco Vertex Z Position [m]',
        #                         evts_p_bin_p_pot)
        # hist_recoZ.set_log_y(True)
        # make_stack(smpl, neutrino_selections, hist_recoZ, 'recoVtxZ')

        # # lepton candidate true X position
        # hist_trueX = ROOTHStack('tVtxX*1e-3', 25, -1.1, 1.1,
        #                         'True Vertex X Position [m]',
        #                         evts_p_bin_p_pot)
        # hist_trueX.set_log_y(True)
        # make_stack(smpl, neutrino_selections, hist_trueX, 'trueVtxX')

        # # lepton candidate reco X position
        # hist_recoX = ROOTHStack('vtxX*1e-3', 25, -1.1, 1.1,
        #                         'Reco Vertex X Position [m]',
        #                         evts_p_bin_p_pot)
        # hist_recoX.set_log_y(True)
        # make_stack(smpl, neutrino_selections, hist_recoX, 'recoVtxX')

        # # lepton candidate true Y position
        # hist_trueY = ROOTHStack('tVtxY*1e-3', 25, -1.1, 1.1,
        #                         'True Vertex Y Position [m]',
        #                         evts_p_bin_p_pot)
        # hist_trueY.set_log_y(True)
        # make_stack(smpl, neutrino_selections, hist_trueY, 'trueVtxY')

        # # lepton candidate reco Y position
        # hist_recoY = ROOTHStack('vtxY*1e-3', 25, -1.1, 1.1,
        #                         'Reco Vertex Y Position [m]',
        #                         evts_p_bin_p_pot)
        # hist_recoY.set_log_y(True)
        # make_stack(smpl, neutrino_selections, hist_recoY, 'recoVtxY')
    del engine


if __name__ == "__main__":
    main(sys.argv[1:])
