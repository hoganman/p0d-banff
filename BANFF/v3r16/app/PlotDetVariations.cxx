#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH1.h"

#include "ND280AnalysisUtils.hxx"
#include "SystId.hxx"

#include "BinningDefinition.hxx"

std::string OutFileName;
std::string InFileName;    


void PlotVariation(){
  int Run         = -999;
  int SubRun      = -999;
  int EventNumber = -999;
  int SystematicInt= -999;

  int nVar = 5;
  double *Sigma      = new double[5];
  int    *Selection  = new int   [5];
  double *TrueEnu    = new double[5];
  int    *TrueNuPDG  = new int   [5];
  double *LeptonMom  = new double[5];
  double *LeptonCos  = new double[5];
  double *DetWeight  = new double[5];
  double *FluxWeight = new double[5];

  ifstream FileList;
  FileList.open(InFileName.c_str());
  TChain *InTree = new TChain("Validation");

  if(FileList.is_open()){
    std::string line;
    while(getline(FileList, line)){
      std::cout << "Adding the file " << line << " to the TChain" << std::endl;
      InTree->Add(line.c_str());
    }
    FileList.close();
  }else{
    std::cerr << "List of files not open!!" << std::endl;
    throw;
  }
    
    
  InTree->SetBranchAddress("Run",         &Run          );
  InTree->SetBranchAddress("SubRun",      &SubRun       );
  InTree->SetBranchAddress("EventNumber", &EventNumber  );
  InTree->SetBranchAddress("Systematic",  &SystematicInt);
  InTree->SetBranchAddress("nVar",        &nVar         );
  InTree->SetBranchAddress("Sigma",        Sigma        );
  InTree->SetBranchAddress("Selection",    Selection    );
  InTree->SetBranchAddress("TrueEnu",      TrueEnu      );
  InTree->SetBranchAddress("TrueNuPDG",    TrueNuPDG    );
  InTree->SetBranchAddress("LeptonMom",    LeptonMom    );
  InTree->SetBranchAddress("LeptonCos",    LeptonCos    );
  InTree->SetBranchAddress("DetWeight",    DetWeight    );
  InTree->SetBranchAddress("FluxWeight",   FluxWeight   );
  double POT_Weight = 1;

  std::map<int,TH1D*> SigmaVarMinus2;
  std::map<int,TH1D*> SigmaVarMinus1;
  std::map<int,TH1D*> SigmaVarZero  ;
  std::map<int,TH1D*> SigmaVarPlus1 ;
  std::map<int,TH1D*> SigmaVarPlus2 ;

  ND::params().SetReadParamOverrideFilePointPassed();
  BANFF::BinningDefinition& bd = BANFF::BinningDefinition::Get();
  int nBinsRedu = bd.GetNbins_Det();
    
  for(int i = 0; i < (int)InTree->GetEntries(); ++i){
    InTree->GetEntry(i);
    if(i%10000 == 0){
      std::cout << "Progress " << 100. * ((double) i) / InTree->GetEntries() << "%" << std::endl;
    }
    SystId::SystEnum Systematic = (SystId::SystEnum)SystematicInt;
    //if(Systematic != SystId::kBFieldDist) continue;
    if(!SigmaVarMinus2[Systematic]) SigmaVarMinus2[Systematic] = new TH1D(Form("SigmaVarMinus2Systematic_%s",ConvertSystematic(Systematic).c_str()),
									  Form("SigmaVarMinus2Systematic_%s",ConvertSystematic(Systematic).c_str()), nBinsRedu, 0, nBinsRedu);
    if(!SigmaVarMinus1[Systematic]) SigmaVarMinus1[Systematic] = new TH1D(Form("SigmaVarMinus1Systematic_%s",ConvertSystematic(Systematic).c_str()),
									  Form("SigmaVarMinus1Systematic_%s",ConvertSystematic(Systematic).c_str()), nBinsRedu, 0, nBinsRedu);
    if(!SigmaVarZero  [Systematic]) SigmaVarZero  [Systematic] = new TH1D(Form("SigmaVarZero__Systematic_%s",ConvertSystematic(Systematic).c_str()),
									  Form("SigmaVarZero__Systematic_%s",ConvertSystematic(Systematic).c_str()), nBinsRedu, 0, nBinsRedu);
    if(!SigmaVarPlus1 [Systematic]) SigmaVarPlus1 [Systematic] = new TH1D(Form("SigmaVarPlus1_Systematic_%s",ConvertSystematic(Systematic).c_str()),
									  Form("SigmaVarPlus1_Systematic_%s",ConvertSystematic(Systematic).c_str()), nBinsRedu, 0, nBinsRedu);
    if(!SigmaVarPlus2 [Systematic]) SigmaVarPlus2 [Systematic] = new TH1D(Form("SigmaVarPlus2_Systematic_%s",ConvertSystematic(Systematic).c_str()),
									  Form("SigmaVarPlus2_Systematic_%s",ConvertSystematic(Systematic).c_str()), nBinsRedu, 0, nBinsRedu);
      
    bool isSand = 0;
    Int_t runp = anaUtils::GetRunPeriod(Run, SubRun);

    if(anaUtils::GetSandMode(Run) >= 0) isSand = true;
        
    switch (runp){    case 0://run1 water
        POT_Weight = 0.018057359;
        break;
    case 1://run2 water
      POT_Weight = 0.038891937;
      break;
    case 2://run2 air
      POT_Weight = 0.036044656;
      break;       
    case 3://run3b air
      POT_Weight = 0.048789205;
      break;
    case 4://run3c air
      POT_Weight = 0.051857034;
      if(isSand) POT_Weight = 0.952100314;
      break;
    case 5://run4 water
      POT_Weight = 0.086816154;
      break;
    case 6://run4 air
      POT_Weight = 0.050940393;
      break;
    case 8://run5 antinu-water
      POT_Weight = 0.018929830;
      if(isSand) POT_Weight = 0.485992368;
      break;
    case 9://run6b antinu-air
      POT_Weight = 0.128547915;
      break;
    case 10://run6c antinu-air
    case 11://run6d antinu-air
    case 12://run6e antinu-air
      POT_Weight = 0;
      break;
    case 15://run7 antinu-water
      POT_Weight = 0.072296838;
      break;
    case 17://run8 antinu-water
      POT_Weight = 0.344254256;
      break;
    case 18://run8 antinu-air
      POT_Weight = 0.051327196;
      break;
    default:
      std::cout << "NOT FOUND?!?!" << std::endl;
      throw;
    } 
    int BinRedu=0;
    BinRedu = bd.GetGlobalBinMatrixMomCos_Det((SampleId::SampleEnum)Selection[0], LeptonMom[0], LeptonCos[0]);
    if(DetWeight[0] > 0. && DetWeight[0] < 100. && FluxWeight[0] > 0. && FluxWeight[0] < 100.)
      SigmaVarMinus2[Systematic]->SetBinContent(BinRedu, SigmaVarMinus2[Systematic]->GetBinContent(BinRedu) + DetWeight[0] * FluxWeight[0] * POT_Weight);

    BinRedu = bd.GetGlobalBinMatrixMomCos_Det((SampleId::SampleEnum)Selection[1], LeptonMom[1], LeptonCos[1]);
    if(DetWeight[1] > 0. && DetWeight[1] < 100. && FluxWeight[1] > 0. && FluxWeight[1] < 100.)
      SigmaVarMinus1[Systematic]->SetBinContent(BinRedu, SigmaVarMinus1[Systematic]->GetBinContent(BinRedu) + DetWeight[1] * FluxWeight[1] * POT_Weight);

    BinRedu = bd.GetGlobalBinMatrixMomCos_Det((SampleId::SampleEnum)Selection[2], LeptonMom[2], LeptonCos[2]);
    if(DetWeight[2] > 0. && DetWeight[2] < 100. && FluxWeight[2] > 0. && FluxWeight[2] < 100.)
      SigmaVarZero  [Systematic]->SetBinContent(BinRedu, SigmaVarZero  [Systematic]->GetBinContent(BinRedu) + DetWeight[2] * FluxWeight[2] * POT_Weight);

    BinRedu = bd.GetGlobalBinMatrixMomCos_Det((SampleId::SampleEnum)Selection[3], LeptonMom[3], LeptonCos[3]);
    if(DetWeight[3] > 0. && DetWeight[3] < 100. && FluxWeight[3] > 0. && FluxWeight[3] < 100.)
      SigmaVarPlus1 [Systematic]->SetBinContent(BinRedu, SigmaVarPlus1 [Systematic]->GetBinContent(BinRedu) + DetWeight[3] * FluxWeight[3] * POT_Weight);

    BinRedu = bd.GetGlobalBinMatrixMomCos_Det((SampleId::SampleEnum)Selection[4], LeptonMom[4], LeptonCos[4]);
    if(DetWeight[4] > 0. && DetWeight[4] < 100. && FluxWeight[4] > 0. && FluxWeight[4] < 100.)
      SigmaVarPlus2 [Systematic]->SetBinContent(BinRedu, SigmaVarPlus2 [Systematic]->GetBinContent(BinRedu) + DetWeight[4] * FluxWeight[4] * POT_Weight);
    
  }
  
  TFile *OutFile = new TFile(OutFileName.c_str(), "RECREATE");
  OutFile->cd();
  for(std::map<int,TH1D*>::iterator it = SigmaVarMinus2.begin(); it != SigmaVarMinus2.end(); ++it) if((*it).second)(*it).second->Write();
  for(std::map<int,TH1D*>::iterator it = SigmaVarMinus1.begin(); it != SigmaVarMinus1.end(); ++it) if((*it).second)(*it).second->Write();
  for(std::map<int,TH1D*>::iterator it = SigmaVarZero  .begin(); it != SigmaVarZero  .end(); ++it) if((*it).second)(*it).second->Write();
  for(std::map<int,TH1D*>::iterator it = SigmaVarPlus1 .begin(); it != SigmaVarPlus1 .end(); ++it) if((*it).second)(*it).second->Write();
  for(std::map<int,TH1D*>::iterator it = SigmaVarPlus2 .begin(); it != SigmaVarPlus2 .end(); ++it) if((*it).second)(*it).second->Write();
   
  for(std::map<int,TH1D*>::iterator it = SigmaVarPlus2 .begin(); it != SigmaVarPlus2 .end(); ++it){
    if((*it).second){
      SystId::SystEnum Syst = (SystId::SystEnum)(*it).first;
      SigmaVarMinus2[Syst]->Divide(SigmaVarZero[Syst]);
      SigmaVarMinus1[Syst]->Divide(SigmaVarZero[Syst]);
      SigmaVarPlus1 [Syst]->Divide(SigmaVarZero[Syst]);
      SigmaVarPlus2 [Syst]->Divide(SigmaVarZero[Syst]);
      SigmaVarZero  [Syst]->Divide(SigmaVarZero[Syst]);
      SigmaVarZero  [Syst]->SetLineColor(kBlack);
      SigmaVarMinus2[Syst]->SetLineColor(kRed  );
      SigmaVarMinus1[Syst]->SetLineColor(kRed  );
      SigmaVarPlus1 [Syst]->SetLineColor(kBlue );
      SigmaVarPlus2 [Syst]->SetLineColor(kBlue );
      SigmaVarMinus2[Syst]->SetLineStyle(2);
      SigmaVarPlus2 [Syst]->SetLineStyle(2);
      SigmaVarZero  [Syst]->SetStats(0);
      SigmaVarMinus2[Syst]->SetStats(0);
      SigmaVarMinus1[Syst]->SetStats(0);
      SigmaVarPlus1 [Syst]->SetStats(0);
      SigmaVarPlus2 [Syst]->SetStats(0);
      // max[0] = SigmaVarZero  [Syst]->GetMaximum();
      // max[1] = SigmaVarMinus2[Syst]->GetMaximum();
      // max[2] = SigmaVarMinus1[Syst]->GetMaximum();
      // max[3] = SigmaVarPlus1 [Syst]->GetMaximum();
      // max[4] = SigmaVarPlus2 [Syst]->GetMaximum();
      // min[0] = SigmaVarZero  [Syst]->GetMinimum();
      // min[1] = SigmaVarMinus2[Syst]->GetMinimum();
      // min[2] = SigmaVarMinus1[Syst]->GetMinimum();
      // min[3] = SigmaVarPlus1 [Syst]->GetMinimum();
      // min[4] = SigmaVarPlus2 [Syst]->GetMinimum();
      double maxAll = -1;
      double minAll = 50000;
      for(int iM = 0; iM < 5; ++iM){
        for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
          if(SigmaVarZero  [Syst]->GetBinContent(iBin) > maxAll) maxAll = SigmaVarZero  [Syst]->GetBinContent(iBin);
          if(SigmaVarMinus2[Syst]->GetBinContent(iBin) > maxAll) maxAll = SigmaVarMinus2[Syst]->GetBinContent(iBin);
          if(SigmaVarMinus1[Syst]->GetBinContent(iBin) > maxAll) maxAll = SigmaVarMinus1[Syst]->GetBinContent(iBin);
          if(SigmaVarPlus1 [Syst]->GetBinContent(iBin) > maxAll) maxAll = SigmaVarPlus1 [Syst]->GetBinContent(iBin);
          if(SigmaVarPlus2 [Syst]->GetBinContent(iBin) > maxAll) maxAll = SigmaVarPlus2 [Syst]->GetBinContent(iBin);
          if(SigmaVarZero  [Syst]->GetBinContent(iBin) < minAll) minAll = SigmaVarZero  [Syst]->GetBinContent(iBin);
          if(SigmaVarMinus2[Syst]->GetBinContent(iBin) < minAll) minAll = SigmaVarMinus2[Syst]->GetBinContent(iBin);
          if(SigmaVarMinus1[Syst]->GetBinContent(iBin) < minAll) minAll = SigmaVarMinus1[Syst]->GetBinContent(iBin);
          if(SigmaVarPlus1 [Syst]->GetBinContent(iBin) < minAll) minAll = SigmaVarPlus1 [Syst]->GetBinContent(iBin);
          if(SigmaVarPlus2 [Syst]->GetBinContent(iBin) < minAll) minAll = SigmaVarPlus2 [Syst]->GetBinContent(iBin);
          if(SigmaVarZero  [Syst]->GetBinContent(iBin) < 0.8) std::cout << "Bin n " << iBin << " is super low?!?!?" << std::endl;
          if(SigmaVarMinus2[Syst]->GetBinContent(iBin) < 0.8) std::cout << "Bin n " << iBin << " is super low?!?!?" << std::endl;
          if(SigmaVarMinus1[Syst]->GetBinContent(iBin) < 0.8) std::cout << "Bin n " << iBin << " is super low?!?!?" << std::endl;
          if(SigmaVarPlus1 [Syst]->GetBinContent(iBin) < 0.8) std::cout << "Bin n " << iBin << " is super low?!?!?" << std::endl;
          if(SigmaVarPlus2 [Syst]->GetBinContent(iBin) < 0.8) std::cout << "Bin n " << iBin << " is super low?!?!?" << std::endl;
        }
      }
      SigmaVarZero  [Syst]->SetMaximum(maxAll*1.2);
      SigmaVarZero  [Syst]->SetMinimum(minAll*0.8);
      // SigmaVarZero  [Syst]->SetMaximum(1.2);
      // SigmaVarZero  [Syst]->SetMinimum(0.8);
      TCanvas *c = new TCanvas(ConvertSystematic(Syst).c_str());
      gPad->SetBottomMargin(gPad->GetBottomMargin()*4.);
      SigmaVarZero  [Syst]->SetTitle(ConvertSystematic(Syst).c_str());
      SigmaVarZero  [Syst]->Draw();
      SigmaVarMinus2[Syst]->Draw("SAME");
      SigmaVarMinus1[Syst]->Draw("SAME");
      SigmaVarPlus1 [Syst]->Draw("SAME");
      SigmaVarPlus2 [Syst]->Draw("SAME");
      SigmaVarZero  [Syst]->Draw("SAME");
      std::string CurSampleStr = "";
      for(int iBin = 1; iBin < nBinsRedu+1; ++iBin){
        std::string SampleStr = ConvertSample(bd.GetSampleFromIndex_Det(iBin-1));
        if(SampleStr != CurSampleStr){
          SigmaVarZero  [Syst]->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
          SigmaVarMinus2[Syst]->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
          SigmaVarMinus1[Syst]->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
          SigmaVarPlus1 [Syst]->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
          SigmaVarPlus2 [Syst]->GetXaxis()->SetBinLabel(iBin,SampleStr.c_str());
          CurSampleStr = SampleStr;
              
        }
      }
      TLegend* leg = new TLegend(0.8,0.7,0.9,0.9);
      leg->SetFillColor(kWhite);
      leg->AddEntry(SigmaVarMinus2[Syst],"-2 #sigma", "L");
      leg->AddEntry(SigmaVarMinus1[Syst],"-1 #sigma", "L");
      leg->AddEntry(SigmaVarZero  [Syst],"0 #sigma",  "L");
      leg->AddEntry(SigmaVarPlus1 [Syst],"+1 #sigma", "L");
      leg->AddEntry(SigmaVarPlus2 [Syst],"+2 #sigma", "L");
      gPad->RedrawAxis();
      leg->Draw();
      c->Write();
      c->SaveAs((ConvertSystematic(Syst)+".pdf").c_str());
    }
  }
  OutFile->Close();
  return;
};




void Usage(){
  std::cout << "USAGE: PlotVariationDet.exe -i listoffile -o output.root" << std::endl;
};

void ParseArg(int argc, char *argv[]){

  for (;;) {
    int c = getopt(argc, argv, "o:i:");
    if (c < 0)
      break;
    switch (c) {
    case 'o': {
      OutFileName = optarg;
      break;
    }
    case 'i': {
      InFileName = optarg;
      break;
    }
    default: {
      Usage();
      throw;
    }
    }
  }
};


int main(int argc, char *argv[]){
  ParseArg(argc, argv);
  PlotVariation();
  return 1;
};
 
