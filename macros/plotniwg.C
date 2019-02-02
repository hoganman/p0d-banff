#include"TString.h"
#include"TCanvas.h"
#include"TChain.h"
#include"TLegend.h"
#include"TFile.h"
void plotniwg()
{

    const std::string outputName = "comparisons.pdf";
    //TFile* outfile = new TFile(outputName)
    TChain* all_pod = new TChain("all");
    TChain* niwg_pod = new TChain("NIWGWeightTree");
    all_pod->AddFriend(niwg_pod);

    TChain* all_tracker = new TChain("all");
    TChain* niwg_tracker = new TChain("NIWGWeightTree");
    all_tracker->AddFriend(niwg_tracker);
    for(int i = 1; i < 99; i++) niwg_pod->Add(Form("/physics/home/mhogan/niwg_weights/mcp6_Spin_B/neut/run2-air/Run2_Air_niwg_weights_%d.root",i));
    for(int i = 1; i < 99; i++) niwg_tracker->Add(Form("/physics/home/mhogan/niwg_weights/tracker_only/mcp6_Spin_B/neut/run2-air/Run2_Air_niwg_weights_%d.root",i));
    for(int i = 1; i < 99; i++) all_pod->Add(Form("/physics/home/mhogan/bincorr/mcp6_Spin_B/neut/run2-air/Run2_Air_%d.root",i));
    for(int i = 1; i < 99; i++) all_tracker->Add(Form("/physics/home/mhogan/bincorr/tracker_only/mcp6_Spin_B/neut/run2-air/Run2_Air_%d.root",i));

    TCanvas* canvas = new TCanvas("canvas", "", 800, 600);
    canvas->Divide(1,2);

    const UInt_t nvars = 3;
    //const std::string variables[] = {"T2KRW_NIWGWeightNom", "T2KRW_EnuNom", "T2KRW_LeptonMom1p1hFDS", "T2KRW_nToys", "T2KRW_Toy[Iteration$]", "T2KRW_NIWGWeightToy[Iteration$]", "T2KRW_EnuToy[Iteration$]"};
    const std::string variables[] = {"TrueEnuNom", "LeptonMomNom", "LeptonCosNom"};
    TLegend* lpod = new TLegend(0.7,0.7,0.9,0.9,"P0D Samples");
    TLegend* ltracker = new TLegend(0.7,0.7,0.9,0.9,"Tracker Samples");

    for(UInt_t iVar = 0; iVar < nvars; ++iVar)
    {
        canvas->cd(1);
        all_tracker->Draw(Form("%s:SelectionNom>>htracker", variables[iVar].c_str(),"","goff"));
        htracker->GetXaxis()->SetTitle("Selected Sample");
        htracker->GetYaxis()->SetTitle(variables[iVar].c_str());
        htracker->Draw("COLZ");
        ltracker->Draw();

        canvas->cd(2);
        all_pod->Draw(Form("%s:SelectionNom>>hpod", variables[iVar].c_str(),"","goff"));
        hpod->GetXaxis()->SetTitle("Selected Sample");
        hpod->GetYaxis()->SetTitle(variables[iVar].c_str());
        hpod->Draw("COLZ");
        lpod->Draw();

        canvas->Print(Form("%s(", outputName.c_str()), "pdf");
    }
    canvas->Print(Form("%s)", outputName.c_str()), "pdf");



}
