#!/usr/bin/env python
"""A script that makes histogram of the resulting TTree from
   RunSyst_New.exe """

from ROOTFile import ROOTFile
import ROOT
import sys
# import os
from os.path import join

# P0DBANFFInterface class to make plots pretty
INTFACE = None
BLACK = 0
RED = 0
BLUE = 0
SKY = 0
GREEN = 0
STACK_COLORS = []

X1 = 0.64196
Y1 = 0.606272
X2 = 0.894472
Y2 = 0.892765

POT_SCALE = 1.0e+21
RUN2_WTR_MCPOT = 1.203412e+21
RUN2_AIR_MCPOT = 9.239371e+20
RUN3B_AIR_MCPOT = 4.478639e+20
RUN3C_AIR_MCPOT = 2.632266e+21
RUN4_AIR_MCPOT = 3.499604e+21
RUN4_WTR_MCPOT = 3.497117e+21
RUN5C_WTR_MCPOT = 2.277663e+21
RUN6B_AIR_MCPOT = 1.417404e+21
RUN6C_AIR_MCPOT = 5.275622e+20
RUN6D_AIR_MCPOT = 6.884002e+20
# RUN6E_AIR_MCPOT = 8.594394e+20
# RUN7B_WTR_MCPOT = 3.370465e+21

FHC_WTR_MCPOT = RUN2_WTR_MCPOT + RUN4_WTR_MCPOT
FHC_AIR_MCPOT = RUN2_AIR_MCPOT + RUN3B_AIR_MCPOT + RUN3C_AIR_MCPOT \
          + RUN4_AIR_MCPOT
RHC_WTR_MCPOT = RUN5C_WTR_MCPOT  \
        # + RUN7B_WTR_MCPOT
RHC_AIR_MCPOT = RUN6B_AIR_MCPOT + RUN6C_AIR_MCPOT + RUN6D_AIR_MCPOT  \
        # + RUN6E_AIR_MCPOT


class sample(object):
    """a way to store sample titles/labels"""

    def __init__(self, chain, plot_title, save_title):
        """set titles and input chain. plot_title put in legend
           save_title used when saved to file"""
        self.chain = chain
        self.plot_title = plot_title
        self.save_title = save_title
        self.pot_scale = 1

    def __str__(self):
        return self.plot_title

    def set_pot_scale(self, scale):
        """Normalize plots by POT"""
        self.pot_scale = scale

    def make_H1D(self, hist_save_title, var, n_bins, low, high, cuts=''):
        """create TH1 and return it"""
        if len(var) < 1:
            return None
        hist_name = 'h1d%s_%s' % (hist_save_title, self.save_title)
        hist = ROOT.TH1D(hist_name, '', n_bins, low, high)
        self.chain.Draw('%s>>%s' % (var, hist_name), cuts, 'goff')
        if self.pot_scale != 1:
            hist.Scale(self.pot_scale)
        return hist

    def make_H2D(self, hist_save_title, varX, n_binsX, lowX, highX,
                 varY, n_binsY, lowY, highY, cuts=''):
        """create TH2 and return it"""
        if len(varX) < 1 or len(varY) < 1:
            return None
        hist_name = 'h2d%s_%s' % (hist_save_title, self.save_title)
        hist = ROOT.TH2D(hist_name, '', n_binsX, lowX, highX,
                         n_binsY, lowY, highY)
        self.chain.Draw('%s:%s>>%s' % (varY, varX, hist_name), cuts, 'goff')
        if self.pot_scale != 1:
            hist.Scale(self.pot_scale)
        return hist


class selection_info(object):
    """store the name, cuts, and legend labels for a selection"""

    def __init__(self, name, cuts, legend_label):
        self.name = name
        self.cuts = cuts
        self.legend_label = legend_label

    def __str__(self):
        return self.cuts


class stack_info(object):
    """store the save name, plot var and binning, and axes labels"""

    def __init__(self, plot_var, n_bins, low_val, high_val, x_title, y_title):
        self.plot_var = plot_var
        self.n_bins = n_bins
        self.low_X = low_val
        self.high_X = high_val
        self.x_title = x_title
        self.y_title = y_title
        self.minimum = -1
        self.maximum = -1
        self.log_y = False

    def set_log_y(self, log_y=True):
        """should log_y also be shown"""
        self.log_y = log_y

    def set_min(self, minimum=1):
        """define the minimum for the stack"""
        self.minimum = minimum

    def set_max(self, maximum):
        """define the maximum for the stack"""
        self.maximum = maximum


def make_stack(evt_sample, all_selections, hist_labels, save_title):
    """Take sample (evt_sample) and separate it by
    selection (all_selections). The histogram labels are stored in
    hist_labels and saved as save_title.root """

    save_as = '%s_%s' % (save_title, evt_sample.save_title)
    canvas = ROOT.TCanvas("canvas", "", 800, 600)
    legend = ROOT.TLegend(X1, Y1, X2, Y2, evt_sample.plot_title)
    legend.SetFillStyle(0)
    legend.SetLineColor(0)
    legend.SetBorderSize(0)
    canvas.cd()

    plot_var = hist_labels.plot_var
    n_bins = hist_labels.n_bins
    low_X = hist_labels.low_X
    high_X = hist_labels.high_X

    hists = []
    for index in range(0, len(all_selections)):
        a_selection = all_selections[index]
        a_hist = evt_sample.make_H1D('%s_%s' % (save_title, a_selection.name),
                                     plot_var, n_bins, low_X, high_X,
                                     a_selection.cuts)
        INTFACE.PrettyUpTH1(a_hist, hist_labels.x_title, hist_labels.y_title,
                            BLACK, STACK_COLORS[index])
        legend.AddEntry(a_hist, a_selection.legend_label, 'f')
        hists.append(a_hist)

    hists.reverse()

    h_stack = ROOT.THStack('%s_stack' % (save_title), '')
    h_total = ROOT.TH1D('%s_total' % (save_title), '', n_bins, low_X, high_X)
    INTFACE.PrettyUpTH1(h_total, hist_labels.x_title, hist_labels.y_title,
                        BLACK)
    for index in range(0, len(all_selections)):
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
        INTFACE.SaveCanvasAs(canvas, join('plots',
                                          '%s_log' % (save_as)))
        canvas.SetLogy(0)

    h_total.Delete()
    h_stack.Delete()
    for index in range(0, len(all_selections)):
        a_hist = hists[index]
        a_hist.Delete()
    legend.Delete()
    canvas.Close()
    return


def get_chain_from_files(tree_name, file_names, first_num=1, last_num=100):
    """read in a TChain of from a set of files like /path/to/file%d.root"""
    chain = ROOT.TChain(tree_name)
    for num in range(first_num, last_num+1):
        testFile = ROOTFile(file_names % (num))
        # print testFile.get_file_name()
        if not testFile.valid():
            continue
        chain.Add(testFile.get_file_name())
    return chain


def main(argv):
    """main"""
    helpstatement = "plotRunSyst_NewResults.py (no args)"
    if len(argv) > 0:
        print helpstatement
    ROOT.gSystem.Load("libP0DBANFF")
    ROOT.gROOT.SetBatch(1)
    global INTFACE
    try:
        INTFACE = ROOT.P0DBANFFInterface()
    except:
        print "unable to load libP0DBANFF.so"
        sys.exit(1)

    global BLACK, RED, BLUE, SKY, GREEN
    BLACK = INTFACE.kcbBlack
    BLUE = INTFACE.kcbBlue
    SKY = INTFACE.kcbSky
    GREEN = INTFACE.kcbGreen
    RED = INTFACE.kcbRed
    global STACK_COLORS
    STACK_COLORS.append(BLUE)
    STACK_COLORS.append(SKY)
    STACK_COLORS.append(GREEN)
    STACK_COLORS.append(RED)

    tree_name = 'all'
    file_path = '/physics/home/mhogan/systematics'
    Run2Air = 'Run2_Air_%d.root'
    Run2Wtr = 'Run2_Water_%d.root'
    Run3bAir = 'Run3b_Air_%d.root'
    Run3cAir = 'Run3c_Air_%d.root'
    Run4Air = 'Run4_Air_%d.root'
    Run4Wtr = 'Run4_Water_%d.root'
    Run5cWtr = 'Run5c_Water_%d.root'
    Run6bAir = 'Run6b_Air_%d.root'
    Run6cAir = 'Run6c_Air_%d.root'
    Run6dAir = 'Run6d_Air_%d.root'

    chn_Run2Air = get_chain_from_files(tree_name, join(file_path, Run2Air))
    chn_Run2Wtr = get_chain_from_files(tree_name, join(file_path, Run2Wtr))
    chn_Run3bAir = get_chain_from_files(tree_name, join(file_path, Run3bAir))
    chn_Run3cAir = get_chain_from_files(tree_name, join(file_path, Run3cAir))
    chn_Run4Air = get_chain_from_files(tree_name, join(file_path, Run4Air))
    chn_Run4Wtr = get_chain_from_files(tree_name, join(file_path, Run4Wtr))
    chn_Run5cWtr = get_chain_from_files(tree_name, join(file_path, Run5cWtr))
    chn_Run6bAir = get_chain_from_files(tree_name, join(file_path, Run6bAir))
    chn_Run6cAir = get_chain_from_files(tree_name, join(file_path, Run6cAir))
    chn_Run6dAir = get_chain_from_files(tree_name, join(file_path, Run6dAir))
    # chn_Run6eAir = get_chain_from_files(tree_name, join(file_path, Run6eAir))
    # chn_Run7bWtr = get_chain_from_files(tree_name, join(file_path, Run7bWtr))

    chn_FHC_Wtr = chn_Run2Wtr
    chn_FHC_Wtr.Add(chn_Run4Wtr)

    chn_FHC_Air = chn_Run2Air
    chn_FHC_Air.Add(chn_Run3bAir)
    chn_FHC_Air.Add(chn_Run3cAir)
    chn_FHC_Air.Add(chn_Run4Air)

    chn_RHC_Wtr = chn_Run5cWtr
    # chn_RHC_Wtr.Add(chn_Run7bWtc)

    chn_RHC_Air = chn_Run6bAir
    chn_RHC_Air.Add(chn_Run6cAir)
    chn_RHC_Air.Add(chn_Run6dAir)
    # chn_RHC_Air.Add(chn_Run6eAir)

    FHC_Wtr = sample(chn_FHC_Wtr, 'FHC Water', 'fhc_water')
    RHC_Wtr = sample(chn_RHC_Wtr, 'RHC Water', 'rhc_water')
    FHC_Air = sample(chn_FHC_Air, 'FHC Air', 'fhc_air')
    RHC_Air = sample(chn_RHC_Air, 'RHC Air', 'rhc_air')

    FHC_Wtr.pot_scale = (POT_SCALE)/(RUN2_WTR_MCPOT)
    RHC_Wtr.pot_scale = (POT_SCALE)/(RUN5C_WTR_MCPOT)
    FHC_Air.pot_scale = (POT_SCALE)/(RUN2_AIR_MCPOT)
    RHC_Air.pot_scale = (POT_SCALE)/(RUN6B_AIR_MCPOT)

    all_samples = [
            FHC_Wtr,
            RHC_Wtr,
            FHC_Air,
            RHC_Air
    ]

    nom_sel = selection_info('nom_sel', 'SelectionNom==88',
                             'P0D numuCC-inclusive')
    numu_cc_sel = selection_info('numu_cc_sel',
                                 '%s && TrueNuPDGNom==14' % (nom_sel),
                                 '#nu_{#mu}')
    numubar_cc_sel = selection_info('numubar_cc_sel',
                                    '%s && TrueNuPDGNom==-14' % (nom_sel),
                                    '#bar{#nu}_{#mu}')
    nuenuebar_cc_sel = selection_info('nuenuebar_cc_sel',
                                      '%s && \
abs(TrueNuPDGNom)==12' % (nom_sel), '#nu_{e} + #bar{#nu}_{e}')
    other_sel = selection_info('other_sel',
                               '%s && abs(TrueNuPDGNom)!=12 && \
abs(TrueNuPDGNom)!=14' % (nom_sel), 'other')

    all_selections = [
            numu_cc_sel,
            numubar_cc_sel,
            nuenuebar_cc_sel,
            other_sel
    ]
    evts_p_bin = 'Events / bin'
    evts_p_bin_p_pot = '%s / 10^{21} PoT' % (evts_p_bin)

    # loop over sample classes
    for smpl in all_samples:

        # # neutrino energy
        # hist_Enu = stack_info('TrueEnuNom*1e-3', 25, 0., 5.0,
        #                       'True Neutrino Energy [GeV]',
        #                       evts_p_bin_p_pot)
        # hist_Enu.set_log_y(True)
        # hist_Enu.set_min(1)
        # hist_Enu.set_max(1.1*8e+3)  # hard coded number
        # make_stack(smpl, all_selections, hist_Enu, 'trueE_nu')

        # # lepton candidate momentum
        hist_Pmu = stack_info('LeptonMomNom*1e-3', 25, 0., 5.0,
                              'Lepton Candidate Momentum [GeV/c]',
                              evts_p_bin_p_pot)
        hist_Pmu.set_log_y(True)
        make_stack(smpl, all_selections, hist_Pmu, 'recoP_mu')

        # lepton candidate cos(theta)
        hist_cosq = stack_info('LeptonCosNom', 20, 0.5, 1.0,
                               'Lepton Candidate cos(#theta)',
                               evts_p_bin_p_pot)
        hist_cosq.set_log_y(True)
        make_stack(smpl, all_selections, hist_cosq, 'recocosq_mu')

        # lepton candidate true Z position
        hist_trueZ = stack_info('tVtxZ*1e-3', 40, -3.4, -0.85,
                                'True Vertex Z Position [m]',
                                evts_p_bin_p_pot)
        hist_trueZ.set_log_y(True)
        make_stack(smpl, all_selections, hist_trueZ, 'trueVtxZ')

        # lepton candidate reco Z position
        hist_recoZ = stack_info('vtxZ*1e-3', 40, -3.4, -0.85,
                                'Reco Vertex Z Position [m]',
                                evts_p_bin_p_pot)
        hist_recoZ.set_log_y(True)
        make_stack(smpl, all_selections, hist_recoZ, 'recoVtxZ')

        # lepton candidate true X position
        hist_trueX = stack_info('tVtxX*1e-3', 25, -1.1, 1.1,
                                'True Vertex X Position [m]',
                                evts_p_bin_p_pot)
        hist_trueX.set_log_y(True)
        make_stack(smpl, all_selections, hist_trueX, 'trueVtxX')

        # lepton candidate reco X position
        hist_recoX = stack_info('vtxX*1e-3', 25, -1.1, 1.1,
                                'Reco Vertex X Position [m]',
                                evts_p_bin_p_pot)
        hist_recoX.set_log_y(True)
        make_stack(smpl, all_selections, hist_recoX, 'recoVtxX')

        # lepton candidate true Y position
        hist_trueY = stack_info('tVtxY*1e-3', 25, -1.1, 1.1,
                                'True Vertex Y Position [m]',
                                evts_p_bin_p_pot)
        hist_trueY.set_log_y(True)
        make_stack(smpl, all_selections, hist_trueY, 'trueVtxY')

        # lepton candidate reco Y position
        hist_recoY = stack_info('vtxY*1e-3', 25, -1.1, 1.1,
                                'Reco Vertex Y Position [m]',
                                evts_p_bin_p_pot)
        hist_recoY.set_log_y(True)
        make_stack(smpl, all_selections, hist_recoY, 'recoVtxY')
    # relevant branches
    # Branch("SelectionNom"
    # Branch("TrueEnuNom"
    # Branch("TrueNuPDGNom"
    # Branch("TrueVertexIDNom"
    # Branch("LeptonMomNom"
    # Branch("LeptonCosNom"
    # Branch("WeightNom"
    # Branch("FluxWeightNom",
    # Branch("vtxX" ,&vtxX ,"vtx
    # Branch("vtxY" ,&vtxY ,"vtx
    # Branch("vtxZ" ,&vtxZ ,"vtx
    # Branch("tVtxX",&tVtxX,"tVt
    # Branch("tVtxY",&tVtxY,"tVt
    # Branch("tVtxZ",&tVtxZ,"tVt
    # Branch("onWaterTarget"
    # Branch("nToys"
    # Branch("Toy"
    # Branch("TrueVertexIDToy"
    # Branch("SelectionToy"
    # Branch("TrueEnuToy"
    # Branch("TrueNuPDGToy"
    # Branch("LeptonMomToy"
    # Branch("LeptonCosToy"
    # Branch("WeightToy"
    # Branch("nSyst"
    # Branch("WeightIndToy"
    # Branch("FluxWeightToy"


if __name__ == "__main__":
    main(sys.argv[1:])
