#!/usr/bin/env python2.7
"""
A program to run a similar analysis as TN-246: checking the consistency
between two T2K runs
"""

from os import getenv
from os.path import join
import ROOT
from ROOT import gSystem, TCanvas, TLegend
import ROOTChain
import RunName as RN
import sys
from plotRunSyst_NewResults import sample

# Which selection to run
P0DNUMUCCSELECTION = -1
P0DNUMUBARCCSELECTION = -1
SELECTION = -1
SELECTIONDICT = dict()

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

    global P0DNUMUCCSELECTION, P0DNUMUBARCCSELECTION, SELECTION, SELECTIONDICT
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
    cuts = ROOT.DefineCuts()
    coords = ROOT.CanvasCoordinates()
    canvas = TCanvas("canvas", "", 800, 600)
    canvas.cd()
    binningLocation = '%s/config/Binning.xml' % p0dbanffroot

    # Enu_AnaBins = ROOT.AnalysisBins('NeutrinoEnergy', binningLocation, xml)
    # pMu_TN328_AnaBins = ROOT.AnalysisBins('TN328Momentum', binningLocation, xml)
    # cosThetaMu_TN328_AnaBins = ROOT.AnalysisBins('TN328CosTheta', binningLocation, xml)

    # pMu_AnaBins = ROOT.AnalysisBins('Momentum', binningLocation, xml)
    # pMu_AnaBins.Sumw2()

    # cosThetaMu_AnaBins = ROOT.AnalysisBins('CosTheta', binningLocation, xml)
    thetaMu_AnaBins = ROOT.AnalysisBins('Theta', binningLocation, xml)
    thetaMu_AnaBins.Sumw2()

    # p0dZ_AnaBins = ROOT.AnalysisBins('P0DuleCoarseZ', binningLocation, xml)
    # p0dZ_AnaBins = ROOT.AnalysisBins('P0DuleFineZ', binningLocation, xml)
    # p0dZ_AnaBins.Sumw2()

    # p0dX_AnaBins = ROOT.AnalysisBins('P0DPositionX', binningLocation, xml)
    # p0dY_AnaBins = ROOT.AnalysisBins('P0DPositionY', binningLocation, xml)
    data_samples = GetDATAsamples()
    # run2water = data_samples[RN.RUN2WDATA.low()]
    run4water = data_samples[RN.RUN4WDATA.low()]

    plot_var = 'TMath::ACos(LeptonCosNom)*TMath::RadToDeg()'
    x_title = 'Lepton Candidate Angle'
    y_title = 'Events'
    save_as = 'run4w_theta'
    legend = TLegend(coords.Legend_RHS_X1, coords.Legend_RHS_Y1,
                     coords.Legend_RHS_X2, coords.Legend_RHS_Y2, '')

    # n_entries_run2w = run2water.getTChain().Draw(plot_var, cuts.muMinusSelection, 'goff')
    # run2w_v1 = run2water.getTChain().GetV1()
    # for index in range(n_entries_run2w):
    #     pMu_AnaBins.Fill(run2w_v1[index])
    # run2w_hist = pMu_AnaBins.GetTH1DClone('run2w_hist')
    # pMu_AnaBins.Reset()
    # run2w_hist.SetMarkerStyle(20)
    # INTERFACE.PrettyUpTH1(run2w_hist, x_title, y_title, INTERFACE.kcbBlack)
    # run2w_hist.Scale(1./n_entries_run2w)
    # legend.AddEntry(run2w_hist, RN.RUN2WDATA.proper_name(), 'LPE')

    # p0dZ_AnaBins.Sumw2()
    n_entries_run4w = run4water.getTChain().Draw(plot_var, cuts.AndTCuts(cuts.muMinusSelection, ROOT.TCut('-2962.<=vtxZ&&vtxZ<=-1274.')), 'goff')
    run4w_v1 = run4water.getTChain().GetV1()
    for index in range(n_entries_run4w):
        thetaMu_AnaBins.Fill(run4w_v1[index])
    run4w_hist = thetaMu_AnaBins.GetTH1DClone('run4w_hist')
    run4w_hist.SetMarkerStyle(2)
    run4w_hist.SetMarkerColor(INTERFACE.kcbBlue)
    INTERFACE.PrettyUpTH1(run4w_hist, x_title, y_title, INTERFACE.kcbBlue)
    # run4w_hist.Scale(1./n_entries_run4w)
    legend.AddEntry(run4w_hist, RN.RUN4WDATA.proper_name(), 'LPE')

    # run2w_hist.Draw("E0")
    run4w_hist.Draw("E0")
    # legend.Draw()
    INTERFACE.SaveCanvasAs(canvas, join('plots', save_as))

    del engine


def GetDATAsamples():
    """
    Get a dictionary of the data samples broken down runs

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

    DATARun2Air = sample(chn_DATARun2Air, RN.RUN2ADATA.proper_name(), RN.RUN2ADATA.low())
    DATARun2Wtr = sample(chn_DATARun2Wtr, RN.RUN2WDATA.proper_name(), RN.RUN2WDATA.low())
    DATARun3bAir = sample(chn_DATARun3bAir, RN.RUN3BDATA.proper_name(), RN.RUN3BDATA.low())
    DATARun3cAir = sample(chn_DATARun3cAir, RN.RUN3CDATA.proper_name(), RN.RUN3CDATA.low())
    DATARun4Air = sample(chn_DATARun4Air, RN.RUN4ADATA.proper_name(), RN.RUN4ADATA.low())
    DATARun4Wtr = sample(chn_DATARun4Wtr, RN.RUN4WDATA.proper_name(), RN.RUN4WDATA.low())
    DATARun5cWtr = sample(chn_DATARun5cWtr, RN.RUN5CDATA.proper_name(), RN.RUN5CDATA.low())
    DATARun6bAir = sample(chn_DATARun6bAir, RN.RUN6BDATA.proper_name(), RN.RUN6BDATA.low())
    DATARun6cAir = sample(chn_DATARun6cAir, RN.RUN6CDATA.proper_name(), RN.RUN6CDATA.low())
    DATARun6dAir = sample(chn_DATARun6dAir, RN.RUN6DDATA.proper_name(), RN.RUN6DDATA.low())
    DATARun6eAir = sample(chn_DATARun6eAir, RN.RUN6EDATA.proper_name(), RN.RUN6EDATA.low())
    DATARun7bWtr = sample(chn_DATARun7bWtr, RN.RUN7BDATA.proper_name(), RN.RUN7BDATA.low())

    T2K = ROOT.TotalPOT()
    DATARun2Air.data_pot = T2K.GetPOTRun2AirData()
    DATARun2Wtr.data_pot = T2K.GetPOTRun2WaterData()
    DATARun3bAir.data_pot = T2K.GetPOTRun3bAirData()
    DATARun3cAir.data_pot = T2K.GetPOTRun3cAirData()
    DATARun4Air.data_pot = T2K.GetPOTRun4AirData()
    DATARun4Wtr.data_pot = T2K.GetPOTRun4WaterData()
    DATARun5cWtr.data_pot = T2K.GetPOTRun5cWaterData()
    DATARun6bAir.data_pot = T2K.GetPOTRun6bAirData()
    DATARun6cAir.data_pot = T2K.GetPOTRun6cAirData()
    DATARun6dAir.data_pot = T2K.GetPOTRun6dAirData()
    DATARun6eAir.data_pot = T2K.GetPOTRun6eAirData()
    DATARun7bWtr.data_pot = T2K.GetPOTRun7bWaterData()

    all_samples = {
                   RN.RUN2ADATA.low(): DATARun2Air,
                   RN.RUN2WDATA.low(): DATARun2Wtr,
                   RN.RUN3BDATA.low(): DATARun3bAir,
                   RN.RUN3CDATA.low(): DATARun3cAir,
                   RN.RUN4ADATA.low(): DATARun4Air,
                   RN.RUN4WDATA.low(): DATARun4Wtr,
                   RN.RUN5CDATA.low(): DATARun5cWtr,
                   RN.RUN6BDATA.low(): DATARun6bAir,
                   RN.RUN6CDATA.low(): DATARun6cAir,
                   RN.RUN6DDATA.low(): DATARun6dAir,
                   RN.RUN6EDATA.low(): DATARun6eAir,
                   RN.RUN7BDATA.low(): DATARun7bWtr
    }

    return all_samples


if __name__ == "__main__":
    main(sys.argv[1:])
