#!/usr/bin/env python
"""A script that makes histogram of the resulting TTree from
   RunSyst_New.exe """

import ROOT
from ROOT import TH1D, TH2D, THStack, TCanvas, TLegend, gSystem, TChain
# import ROOTChain
import ROOTHStack
import sys
from os.path import join

# Which selection to run
P0DNUMUCCSELECTION = 88
P0DNUMUBARCCSELECTION = 89
SELECTION = P0DNUMUCCSELECTION

# P0DBANFFInterface class to make plots pretty
INTFACE = None
STACK_COLORS = []
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


def make_stack(evt_sample, all_selections, hist_labels, save_title):
    """Take sample (evt_sample) and separate it by
    selection (all_selections). The histogram labels are stored in
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

    h_stack = THStack('%s_stack' % (save_title), '')
    h_total = TH1D('%s_total' % (save_title), '', n_bins, low_X, high_X)
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
        INTFACE.SaveCanvasAs(
                canvas, join('plots', '%s_log' % (save_as)))
        canvas.SetLogy(0)

    h_total.Delete()
    h_stack.Delete()
    for index in range(0, len(all_selections)):
        a_hist = hists[index]
        a_hist.Delete()
    legend.Delete()
    canvas.Close()
    return


def main(argv):
    """main"""
    helpstatement = "plotRunSyst_NewResults.py (no args)"
    if len(argv) > 0:
        print helpstatement
    gSystem.Load("P0DBANFF")
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
    global STACK_COLORS, BLACK
    BLACK = INTFACE.kcbBlack
    STACK_COLORS.append(INTFACE.kcbBlack)
    STACK_COLORS.append(INTFACE.kcbBlue)
    STACK_COLORS.append(INTFACE.kcbSky)
    STACK_COLORS.append(INTFACE.kcbRed)

    mc_name = 'all'
    # data_name = 'nominal'
    # file_path = '/Raid/home/mhogan/systematics'
    file_path = '/physics/home/mhogan/systematics'
    Run2Air = 'Run2_Air_hadd.root'
    Run2Wtr = 'Run2_Water_hadd.root'
    Run3bAir = 'Run3b_Air_hadd.root'
    Run3cAir = 'Run3c_Air_hadd.root'
    Run4Air = 'Run4_Air_hadd.root'
    Run4Wtr = 'Run4_Water_hadd.root'
    Run5cWtr = 'Run5c_Water_hadd.root'
    Run6bAir = 'Run6b_Air_hadd.root'
    Run6cAir = 'Run6c_Air_hadd.root'
    Run6dAir = 'Run6d_Air_hadd.root'
    Run6eAir = 'Run6e_Air_hadd.root'
    Run7bWtr = 'Run7b_Water_hadd.root'

    chn_MCRun2Air = TChain(mc_name).Add(join(file_path, Run2Air))
    chn_MCRun2Wtr = TChain(mc_name).Add(join(file_path, Run2Wtr))
    chn_MCRun3bAir = TChain(mc_name).Add(join(file_path, Run3bAir))
    chn_MCRun3cAir = TChain(mc_name).Add(join(file_path, Run3cAir))
    chn_MCRun4Air = TChain(mc_name).Add(join(file_path, Run4Air))
    chn_MCRun4Wtr = TChain(mc_name).Add(join(file_path, Run4Wtr))
    chn_MCRun5cWtr = TChain(mc_name).Add(join(file_path, Run5cWtr))
    chn_MCRun6bAir = TChain(mc_name).Add(join(file_path, Run6bAir))
    chn_MCRun6cAir = TChain(mc_name).Add(join(file_path, Run6cAir))
    chn_MCRun6dAir = TChain(mc_name).Add(join(file_path, Run6dAir))
    chn_MCRun6eAir = TChain(mc_name).Add(join(file_path, Run6eAir))
    chn_MCRun7bWtr = TChain(mc_name).Add(join(file_path, Run7bWtr))

    # chn_MCRun2Air = ROOTChain.get(mc_name, join(file_path, Run2Air))
    # chn_MCRun2Wtr = ROOTChain.get(mc_name, join(file_path, Run2Wtr))
    # chn_MCRun3bAir = ROOTChain.get(mc_name, join(file_path, Run3bAir))
    # chn_MCRun3cAir = ROOTChain.get(mc_name, join(file_path, Run3cAir))
    # chn_MCRun4Air = ROOTChain.get(mc_name, join(file_path, Run4Air))
    # chn_MCRun4Wtr = ROOTChain.get(mc_name, join(file_path, Run4Wtr))
    # chn_MCRun5cWtr = ROOTChain.get(mc_name, join(file_path, Run5cWtr))
    # chn_MCRun6bAir = ROOTChain.get(mc_name, join(file_path, Run6bAir))
    # chn_MCRun6cAir = ROOTChain.get(mc_name, join(file_path, Run6cAir))
    # chn_MCRun6dAir = ROOTChain.get(mc_name, join(file_path, Run6dAir))
    # chn_MCRun6eAir = ROOTChain.get(mc_name, join(file_path, Run6eAir))
    # chn_MCRun7bWtr = ROOTChain.get(mc_name, join(file_path, Run7bWtr))

    chn_FHC_Wtr = chn_MCRun2Wtr
    chn_FHC_Wtr.Add(chn_MCRun4Wtr)

    chn_FHC_Air = chn_MCRun2Air
    chn_FHC_Air.Add(chn_MCRun3bAir)
    chn_FHC_Air.Add(chn_MCRun3cAir)
    chn_FHC_Air.Add(chn_MCRun4Air)

    chn_RHC_Wtr = chn_MCRun5cWtr
    chn_RHC_Wtr.Add(chn_MCRun7bWtr)

    chn_RHC_Air = chn_MCRun6bAir
    chn_RHC_Air.Add(chn_MCRun6cAir)
    chn_RHC_Air.Add(chn_MCRun6dAir)
    chn_RHC_Air.Add(chn_MCRun6eAir)

    FHC_Wtr = sample(chn_FHC_Wtr, 'FHC Water', 'fhc_water')
    RHC_Wtr = sample(chn_RHC_Wtr, 'RHC Water', 'rhc_water')
    FHC_Air = sample(chn_FHC_Air, 'FHC Air', 'fhc_air')
    RHC_Air = sample(chn_RHC_Air, 'RHC Air', 'rhc_air')

    FHC_Wtr.scale = TN80.GetPOTWaterData()/T2K.GetPOTWaterMC()
    FHC_Air.scale = TN80.GetPOTAirData()/T2K.GetFHCAirMC()
    RHC_Wtr.scale = T2K.GetPOTRHCWaterData()/T2K.GetPOTRHCWaterMC()
    RHC_Air.scale = T2K.GetPOTRHCAirData()/T2K.GetPOTRHCAirMC()

    FHC_Wtr.pot_scale = TN80.GetPOTWaterData()
    FHC_Air.pot_scale = TN80.GetPOTAirData()
    RHC_Wtr.pot_scale = T2K.GetPOTRHCWaterData()
    RHC_Air.pot_scale = T2K.GetPOTRHCAirData()

    all_samples = [
            FHC_Wtr,
            RHC_Wtr,
            FHC_Air,
            RHC_Air
    ]

    nom_sel = selection_info('nom_sel', 'SelectionNom==%d' % (SELECTION),
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

    # loop over sample classes
    for smpl in all_samples:
        evts_p_bin_p_pot = '%s / %s PoT' % (evts_p_bin, smpl.pot_scale)

        # # neutrino energy
        hist_Enu = ROOTHStack('TrueEnuNom*1e-3', 50, 0., 5.0,
                              'True Neutrino Energy [GeV]',
                              evts_p_bin_p_pot)
        hist_Enu.set_log_y(True)
        hist_Enu.set_min(1)
        # hist_Enu.set_max(1.1*8e+3)  # hard coded number
        make_stack(smpl, all_selections, hist_Enu, 'trueE_nu')

        # lepton candidate momentum
        hist_Pmu = ROOTHStack('LeptonMomNom*1e-3', 35, 0., 5.0,
                              'Lepton Candidate Momentum [GeV/c]',
                              evts_p_bin_p_pot)
        hist_Pmu.set_log_y(True)
        make_stack(smpl, all_selections, hist_Pmu, 'recoP_mu')

        # lepton candidate cos(theta)
        hist_cosq = ROOTHStack('LeptonCosNom', 25, 0.5, 1.0,
                               'Lepton Candidate cos(#theta)',
                               evts_p_bin_p_pot)
        hist_cosq.set_log_y(True)
        make_stack(smpl, all_selections, hist_cosq, 'recocosq_mu')

        # lepton candidate theta
        hist_q = ROOTHStack('TMath::ACos(LeptonCosNom)*TMath::RadToDeg()',
                            50, 0.0, 90., 'Lepton Candidate #theta',
                            evts_p_bin_p_pot)
        hist_q.set_log_y(True)
        make_stack(smpl, all_selections, hist_q, 'recoq_mu')

        # lepton candidate true Z position
        hist_trueZ = ROOTHStack('tVtxZ*1e-3', 29, -3.2, -1.2,
                                'True Vertex Z Position [m]',
                                evts_p_bin_p_pot)
        hist_trueZ.set_log_y(True)
        make_stack(smpl, all_selections, hist_trueZ, 'trueVtxZ')

        # lepton candidate reco Z position
        hist_recoZ = ROOTHStack('vtxZ*1e-3', 29, -3.2, -1.2,
                                'Reco Vertex Z Position [m]',
                                evts_p_bin_p_pot)
        hist_recoZ.set_log_y(True)
        make_stack(smpl, all_selections, hist_recoZ, 'recoVtxZ')

        # lepton candidate true X position
        hist_trueX = ROOTHStack('tVtxX*1e-3', 25, -1.1, 1.1,
                                'True Vertex X Position [m]',
                                evts_p_bin_p_pot)
        hist_trueX.set_log_y(True)
        make_stack(smpl, all_selections, hist_trueX, 'trueVtxX')

        # lepton candidate reco X position
        hist_recoX = ROOTHStack('vtxX*1e-3', 25, -1.1, 1.1,
                                'Reco Vertex X Position [m]',
                                evts_p_bin_p_pot)
        hist_recoX.set_log_y(True)
        make_stack(smpl, all_selections, hist_recoX, 'recoVtxX')

        # lepton candidate true Y position
        hist_trueY = ROOTHStack('tVtxY*1e-3', 25, -1.1, 1.1,
                                'True Vertex Y Position [m]',
                                evts_p_bin_p_pot)
        hist_trueY.set_log_y(True)
        make_stack(smpl, all_selections, hist_trueY, 'trueVtxY')

        # lepton candidate reco Y position
        hist_recoY = ROOTHStack('vtxY*1e-3', 25, -1.1, 1.1,
                                'Reco Vertex Y Position [m]',
                                evts_p_bin_p_pot)
        hist_recoY.set_log_y(True)
        make_stack(smpl, all_selections, hist_recoY, 'recoVtxY')


if __name__ == "__main__":
    main(sys.argv[1:])
