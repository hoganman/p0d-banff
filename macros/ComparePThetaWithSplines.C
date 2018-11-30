void ComparePThetaWithSplines()
{
    const UInt_t nTweaks = 7;
    const UInt_t noTweakIndex = 3;

    TXMLEngine eng();
    AnalysisBins2D* ptheta = new AnalysisBins2D("P0DFitFHCMomentum", "P0DFitFHCCosTheta", TString::Format("%s/config/Binning.xml", getenv("P0DBANFFROOT")));
    MakeClSampleSummary* mcSampleSum = new MakeClSampleSummary("/home/mhogan/Downloads/Run4_Water_splines.root");

    TH2D* fsi_inel_lo_tweaks[nTweaks];
    TH2D* fsi_inel_lo_nominal_ratios[nTweaks];
    Double_t tweakValues[nTweaks] = {-0.75, -0.5, -0.25, 0., 0.25, 0.5, 0.75};
    for(UInt_t index = 0; index < nTweaks; ++index)
    {
        fsi_inel_lo_tweaks[index] = ptheta->GetTH2DClone(TString::Format("fsi_inel_lo_tweak_%i", index));
        fsi_inel_lo_nominal_ratios[index] = ptheta->GetTH2DClone(TString::Format("fsi_inel_lo_nominal_ratios_%i", index));
    }

    //for(Long64_t entry = 0; entry < mcSampleSum->fChain->GetEntries(); ++entry)
    for(Long64_t entry = 0; entry < 1e5; ++entry)
    {
        mcSampleSum->GetEntry(entry);
        Int_t& SelectedSample = mcSampleSum->SelectedSample;
        if(SelectedSample != 89 || SelectedSample != 93)
            continue;
        TGraph* graph = mcSampleSum->GetSelectedSampleFSI_INEL_LOGraph();
        Double_t& Pmu = mcSampleSum->Pmu[SelectedSample];
        Double_t& CosThetamu = mcSampleSum->CosThetamu[SelectedSample];
        if(graph && graph->GetN() != 1)
        {
            for(UInt_t index = 0; index < nTweaks; ++index)
            {
                TSpline3* spline = new TSpline3("spline", graph);
                fsi_inel_lo_tweaks[index]->Fill(Pmu*1e-3, CosThetamu, spline->Eval(tweakValues[index]));
            }
        }
        else
        {
            for(UInt_t index = 0; index < nTweaks; ++index)
            {
                fsi_inel_lo_tweaks[index]->Fill(Pmu*1e-3, CosThetamu);
            }

        }
    }
    TH2D* nominal = fsi_inel_lo_tweaks[noTweakIndex];
    for(UInt_t index = 0; index < nTweaks; ++index)
    {
        fsi_inel_lo_nominal_ratios[index]->Add(fsi_inel_lo_tweaks[index]);
        fsi_inel_lo_nominal_ratios[index]->Divide(nominal);
    }
    TCanvas* canvas = new TCanvas("canvas", "", 800, 600);
    canvas->cd();
    for(UInt_t index = 0; index < nTweaks; ++index)
    {
        fsi_inel_lo_nominal_ratios[index]->Draw("COLZ");
        interface.SaveCanvasAs(canvas, TString::Format("FSI_INEL_LO_Tweak_%d", index));
    }
}
