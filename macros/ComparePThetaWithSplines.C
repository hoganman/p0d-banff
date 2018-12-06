#include "TXMLEngine.h"
#include "src/AnalysisBins2D.hxx"
#include "src/P0DBANFFInterface.hxx"
#include "src/MakeClSampleSummary.hxx"
#include "TH2D.h"
#include "TString.h"
#include "TSpline.h"
#include <cstdlib>

void ComparePThetaWithSplines()
{
    const UInt_t nGraphs = 10;
    const UInt_t nTweaks = 7;
    const UInt_t noTweakIndex = 3;

    TXMLEngine eng();
    TObject* P0DBANFFInterface_ptr;
    if(!gROOT->FindObjectAny("P0DBANFFStyle"))
    {
        P0DBANFFInterface_ptr = static_cast<TObject*>(new P0DBANFFInterface());
        static_cast<P0DBANFFInterface*>(P0DBANFFInterface_ptr)->LoadColorBlindPalette();
    }
    AnalysisBins2D* ptheta = new AnalysisBins2D("P0DFitFHCMomentum", "P0DFitFHCCosTheta", TString::Format("%s/config/Binning.xml", getenv("P0DBANFFROOT")));
    //MakeClSampleSummary* mcSampleSum = new MakeClSampleSummary(TString::Format("%s/mcp6_Spin_B/neut/run4-water/Run4_Water_splines.root", getenv("SPLINEROOT")));
    MakeClSampleSummary* mcSampleSum = new MakeClSampleSummary(TString::Format("%s/P0DFHC_genWeights2017/mcp6_Spin_B/neut/run4-water/Run4_Water_splines.root", getenv("SPLINEROOT")));
    TClonesArray* graphs[nGraphs];              TString graphNames[nGraphs];
    graphs[ 0] = mcSampleSum->BgSclResGraph;    graphNames[ 0] = "BgSclResGraph";
    graphs[ 1] = mcSampleSum->CA5Graph;         graphNames[ 1] = "CA5Graph";
    graphs[ 2] = mcSampleSum->CCNuEGraph;       graphNames[ 2] = "CCNuEGraph";
    graphs[ 3] = mcSampleSum->DISMPiShpGraph;   graphNames[ 3] = "DISMPiShpGraph";
    graphs[ 4] = mcSampleSum->FSI_CEX_HIGraph;  graphNames[ 4] = "FSI_CEX_HIGraph";
    graphs[ 5] = mcSampleSum->FSI_CEX_LOGraph;  graphNames[ 5] = "FSI_CEX_LOGraph";
    graphs[ 6] = mcSampleSum->FSI_INEL_HIGraph; graphNames[ 6] = "FSI_INEL_HIGraph";
    graphs[ 7] = mcSampleSum->FSI_INEL_LOGraph; graphNames[ 7] = "FSI_INEL_LOGraph";
    graphs[ 8] = mcSampleSum->FSI_PI_ABSGraph;  graphNames[ 8] = "FSI_PI_ABSGraph";
    graphs[ 9] = mcSampleSum->FSI_PI_PRODGraph; graphNames[ 9] = "FSI_PI_PRODGraph";
    //graphs[10] = mcSampleSum->MAQEGraph;        graphNames[10] = "MAQEGraph";
    //graphs[11] = mcSampleSum->MARESGraph;       graphNames[11] = "MARESGraph";
    //graphs[12] = mcSampleSum->MEC_CGraph;       graphNames[12] = "MEC_CGraph";
    //graphs[13] = mcSampleSum->MEC_OGraph;       graphNames[13] = "MEC_OGraph";
    //graphs[14] = mcSampleSum->PDD_CGraph;       graphNames[14] = "PDD_CGraph";
    //graphs[15] = mcSampleSum->PDD_OGraph;       graphNames[15] = "PDD_OGraph";
    //graphs[16] = mcSampleSum->PF_CGraph;        graphNames[16] = "PF_CGraph";
    //graphs[17] = mcSampleSum->PF_OGraph;        graphNames[17] = "PF_OGraph";
    //graphs[18] = mcSampleSum->Q3CUTGraph;       graphNames[18] = "Q3CUTGraph";
    //graphs[19] = mcSampleSum->RPAGraph;         graphNames[19] = "RPAGraph";
    //graphs[20] = mcSampleSum->SF_RFGGraph;      graphNames[20] = "SF_RFGGraph";
    //const Double_t tweakValues[nTweaks] = {-0.45, -0.30, -0.15, 0., 0.15, 0.30, 0.45};
    const Double_t tweakValues[nTweaks] = {-0.75, -0.50, -0.25, 0., 0.25, 0.50, 0.75};
    TH2D* tweakHistograms[nGraphs][nTweaks];
    TH2D* ratioToNominal_histograms[nGraphs][nTweaks];

    for(UInt_t graphIndex = 0; graphIndex < nGraphs; ++graphIndex)
    {
        for(UInt_t tweakIndex = 0; tweakIndex < nTweaks; ++tweakIndex)
        {
            tweakHistograms[graphIndex][tweakIndex] = ptheta->GetTH2DClone(TString::Format("%s_tweak_%1.2f", graphNames[graphIndex].Data(), tweakValues[tweakIndex]));
        }
    }

    //for(Long64_t entry = 0; entry < mcSampleSum->fChain->GetEntries(); ++entry)
    for(Long64_t entry = 0; entry < mcSampleSum->fChain->GetEntries()*0.5; ++entry)
    {
        if(entry % 1000 == 0)
            printf("entry = %d\n", entry);
        mcSampleSum->GetEntry(entry);
        Int_t& SelectedSample = mcSampleSum->SelectedSample;
        if(!(SelectedSample == 89 || SelectedSample == 93))
            continue;
        Double_t& Pmu = mcSampleSum->Pmu[SelectedSample];
        Double_t& CosThetamu = mcSampleSum->CosThetamu[SelectedSample];
        for(UInt_t graphIndex = 0; graphIndex < nGraphs; ++graphIndex)
        {
            //printf("Graph(%d) = \"%s\"\n", graphIndex, graphNames[graphIndex].Data());
            if(!graphs[graphIndex]) continue;
            TGraph* graph = static_cast<TGraph*>(graphs[graphIndex]->At(SelectedSample));
            if(!graph) continue;
            Double_t weight = 1.0;
            for(UInt_t tweakIndex = 0; tweakIndex < nTweaks; ++tweakIndex)
            {
                //printf("tweak = %1.2f\n", tweakValues[tweakIndex]);
                if(graph->GetN() != 1)
                    weight = graph->Eval(tweakValues[tweakIndex], 0, "S");
                else
                    weight = 1;
                tweakHistograms[graphIndex][tweakIndex]->Fill(Pmu*1e-3, CosThetamu, weight);
            }
        }
    }

    Double_t globalMin = 1.0;
    Double_t globalMax = 1.0;
    for(UInt_t graphIndex = 0; graphIndex < nGraphs; ++graphIndex)
    {
        for(UInt_t tweakIndex = 0; tweakIndex < nTweaks; ++tweakIndex)
        {
            ratioToNominal_histograms[graphIndex][tweakIndex] = static_cast<TH2D*>(tweakHistograms[graphIndex][tweakIndex]->Clone(TString::Format("ratioToNominal_%s_%1.2f", graphNames[graphIndex].Data(), tweakValues[tweakIndex])));
            TH2D* hist = ratioToNominal_histograms[graphIndex][tweakIndex];
            hist->Divide(tweakHistograms[graphIndex][noTweakIndex]);
            P0DBANFFInterface::PrettyUpTH2(hist, "Lepton Momentum [GeV/c]", "Lepton Angle [cos(#theta)]");
            if(hist->GetMaximum() > globalMax)
                globalMax = hist->GetMaximum();
            if(hist->GetMinimum() < globalMin)
                globalMin = hist->GetMinimum();
        }
    }
    for(UInt_t graphIndex = 0; graphIndex < nGraphs; ++graphIndex)
    {
        for(UInt_t tweakIndex = 0; tweakIndex < nTweaks; ++tweakIndex)
        {
            TH2D* hist = ratioToNominal_histograms[graphIndex][tweakIndex];
            hist->SetMaximum(globalMax);
            hist->SetMinimum(globalMin);
        }
    }


    TCanvas* canvas = new TCanvas("canvas", "", 800, 600);
    canvas->cd();
    canvas->SetLogz();
    for(UInt_t graphIndex = 0; graphIndex < nGraphs; ++graphIndex)
    {
        for(UInt_t tweakIndex = 0; tweakIndex < nTweaks; ++tweakIndex)
        {
            ratioToNominal_histograms[graphIndex][tweakIndex]->Draw("COLZ");
            TString filename = TString::Format("plots/unrestricted_splines/%s_%1.2f", graphNames[graphIndex].Data(), tweakValues[tweakIndex]);
            filename.ReplaceAll(".", "_dot_");
            P0DBANFFInterface::SaveCanvasAs(canvas, filename.Data());
        }
        tweakHistograms[graphIndex][noTweakIndex]->Draw("COLZ");
        TString filename = TString::Format("plots/unrestricted_splines/%s_nominal", graphNames[graphIndex].Data());
        filename.ReplaceAll(".", "_dot_");
        P0DBANFFInterface::SaveCanvasAs(canvas, filename.Data());
    }

    delete ptheta;
    delete mcSampleSum;
    delete canvas;
}
