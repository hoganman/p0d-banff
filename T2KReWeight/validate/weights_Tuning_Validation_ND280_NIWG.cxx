//---------------------------------------------------------------
//
// Copy of TuningValidation_ND280_NIWG.cxx
// To be used for plotting weights
//
//---------------------------------------------------------------

#include <stdlib.h>
#include <cstdlib>
#include <sys/types.h>
#include <string.h>

#include <iostream>

#include "TStyle.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TArrayF.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TString.h"

#include "move_stats.h"

//-- WARNING 
//--    The old Eb dials (kNIWG2014a_Eb_C12/O16) are now
//--    obsolete and will not be used in future analyses
//--    and will be removed from the code at a future time.
//--    In a similar manner, the SCCA and SCCV dials need more
//--    validation and will not be used in future analyses
//--    until more validation can be done. They will be 
//--    removed from the code in the future, if needed.
//-- WARNING

namespace {
  using std::cout;
  using std::cerr;
  using std::endl;
  using std::string;

  void Usage();
  int ParseArgs(int argc, char **argv);

  string fWeightName;
  string fFileName;
  string fOutfile;
}

void PlotTunings(const std::string& weightfile, const std::string& eventfile, const std::string& outfile, const int desired_neutrino);

int main(int argc, char *argv[])
{
  fWeightName = "";
  fFileName = "";
  fOutfile = "Weights_Valid_Tunings";
  ParseArgs(argc, argv);

  PlotTunings(fWeightName, fFileName, fOutfile+"_nue.root",    12);
  PlotTunings(fWeightName, fFileName, fOutfile+"_nueb.root",  -12);
  PlotTunings(fWeightName, fFileName, fOutfile+"_numu.root",   14);
  PlotTunings(fWeightName, fFileName, fOutfile+"_numub.root", -14);
  return 0;
}

void PlotTunings(const std::string& weightfile, const std::string& eventfile, const std::string& outfile, const int desired_neutrino)
{
  // Open files and set up trees
  TFile *f_sk = new TFile(eventfile.c_str());
  TTree *t_sk = NULL;   f_sk->GetObject("SelectedEventTruth", t_sk);

  TFile *f_w = new TFile(weightfile.c_str());
  TTree* t_w = NULL;    f_w->GetObject("weightstree", t_w);

  double Plep;  t_sk->SetBranchAddress("Plep",&Plep);
  double Q2;    t_sk->SetBranchAddress("Q2",&Q2);
  int Mode;     t_sk->SetBranchAddress("Mode",&Mode);
  int Target;   t_sk->SetBranchAddress("Target",&Target);
  int NuPdg;    t_sk->SetBranchAddress("NuPdg",&NuPdg);

  TArrayF* weights = NULL;
  t_w->SetBranchAddress("weights",&weights);

  // Open file to save plots (default: Valid_tunings_Plep.root in current directory)
  TFile *fout;
  fout = new TFile(outfile.c_str(),"RECREATE");
  fout->cd();

  // Set up histograms 
  TH1D *sigmahists[21][7];
  TH1D *truesigmahists[21][7];
  char const* dialnames[21] = {"MAQE", "MEC_C", "pF_C", "Eb_C", "MEC_O", "pF_O", "Eb_O",
                               "CA5", "MaNFFRes", "BgSclRes", "SCCV", "SCCA", "CCNuEE0",
                               "DISMPiShp", "CCCohE0", "NCCohE0", "NCOtherE0",
                               "New_Eb_C", "New_Eb_O", "MEC_PDD_C", "MEC_PDD_O"};

  for(int i=0; i < 21; ++i){
    for(int j = 0; j < 7; ++j){
      sigmahists[i][j] = new TH1D(Form("%s_sigma_%d",dialnames[i], (j-3)), Form("%s_sigma_%d",dialnames[i], (j-3)), 200, -5, 25);
      truesigmahists[i][j] = new TH1D(Form("%s_trueSample_sigma_%d",dialnames[i], (j-3)), Form("%s_trueSample_sigma_%d",dialnames[i], (j-3)), 200, -5, 25);
    }
  }

  // Loop over entries and fill histograms
  for (int ientry=0; ientry<t_w->GetEntries();ientry++)
  {
    t_w->GetEntry(ientry);
    t_sk->GetEntry(ientry);

    // Check interaction mode
    int intmode=-1;
    if (NuPdg != desired_neutrino) {
        continue;
    }

    if (abs(Mode)==1) 
      intmode = 0; // CCQE
    else if (abs(Mode)==11 || abs(Mode)==12 || abs(Mode)==13)
      intmode = 1; // CC1pi
    else if (abs(Mode)==16)
      intmode = 2; // CC coherent
    else if (abs(Mode)==17 || abs(Mode)==21 || abs(Mode)==22 || abs(Mode)==23 || abs(Mode)==26)
      intmode = 3; // CC other
    else if (abs(Mode)==36)
      intmode = 4; // NC coherent
    else if (abs(Mode)==31 || abs(Mode)==32)
      intmode = 5; // NC 1pi0
    else if (abs(Mode)==33 || abs(Mode)==34)
      intmode = 6; // NC 1pi+/-
    else if (abs(Mode)==38 || abs(Mode)==39 || abs(Mode)==41 || abs(Mode)==42 || abs(Mode)==43 || abs(Mode)==44 || abs(Mode)==45 || abs(Mode)==46 || abs(Mode)==51 || abs(Mode)==52)
      intmode = 7; // NC other
    else if (abs(Mode)==2)
      intmode = 8; // MEC

    if (intmode == -1)
    {
      continue;
    }

    // Check event selection
    //int selec = -1;
    //if(NuPdg == 14) selec = 0;
    //else if(NuPdg == 12) selec = 1;
    //else if(NuPdg == -14) selec = 2;
    //else if(NuPdg == -12) selec = 3;
    //else std::cout << "["<<__FILE__<<":L"<<__LINE__<<"] " <<"Neutrino PDG error" << std::endl;

    int target = -1;
    if ((Target%1000000000)/10000 == 6) target = 0;
    else if ((Target%1000000000)/10000 == 8) target = 1;
    else if ((Target%1000000000)/10000 == 1) target = 2;
    else target = 3;


    // Fill histograms
    int dial = 9;
    for(int i = 0; i < 21; ++i){
      for(int j = 0; j < 7; ++j){
        dial = 9 + i*7 + j;
        double weight = weights->At(dial);
        if (weight > 25) { weight = 26; }
        sigmahists[i][j]->Fill(weight);
        //sigmahists[i][j]->Fill(Plep, weight);

        bool good_event = false;
        switch (dial) {
        // MAQE dial -3 to +3 sigma variations
        case   9:
        case  10:
        case  11:
        case  12: //< 'nominal'
        case  13:
        case  14:
        case  15:
            //maqedial = 0.0 + (dial - 12)*(0.08/1.2); // 0.08 uncertainty, but do dial for 10%
            if (intmode == 0) { good_event = true; }
            break;

        // Carbon CCQE dials (MEC)
        case  16:
        case  17:
        case  18:
        case  19: //< 'nominal'
        case  20:
        case  21:
        case  22:
            // meccdial = 0.0 + (dial - 19)*(0.25/1.0);
            if ((intmode == 8) && (target == 0)) { good_event = true; }
            break;

        // Carbon CCQE dials (pF)
        case  23:
        case  24:
        case  25:
        case  26: //< 'nominal'
        case  27:
        case  28:
        case  29:
            //pfcdial = 0.0 + (dial - 26)*(16./239.);
            if ((intmode == 0) && (target == 0)) { good_event = true; }
            break;

        // Carbon CCQE dials (Eb)
        case  30:
        case  31:
        case  32:
        case  33: //< 'nominal'
        case  34:
        case  35:
        case  36:
            //ebcdial = 0.0 + (dial - 33)*(9./25.);
            if ((intmode == 0) && (target == 0)) { good_event = true; }
            break;

        // Oxygen CCQE dials (MEC)
        case  37:
        case  38:
        case  39:
        case  40: //< 'nominal'
        case  41:
        case  42:
        case  43:
            //mecodial = 0.0 + (dial - 40)*(0.25/1.0);
            if ((intmode == 8) && (target == 1)) { good_event = true; }
            break;

        // Oxygen CCQE dials (pF)
        case  44:
        case  45:
        case  46:
        case  47: //< 'nominal'
        case  48:
        case  49:
        case  50:
            //pfodial = 0.0 + (dial - 47)*(30./225.);
            if ((intmode == 0) && (target == 1)) { good_event = true; }
            break;

        // Oxygen CCQE dials (Eb)
        case  51:
        case  52:
        case  53:
        case  54: //< 'nominal'
        case  55:
        case  56:
        case  57:
            //ebodial = 0.0 + (dial - 54)*(9./27.);
            if ((intmode == 0) && (target == 1)) { good_event = true; }
            break;

        // CC1Pi dials (CA5)
        case  58:
        case  59:
        case  60:
        case  61: //< 'nominal'
        case  62:
        case  63:
        case  64:
            //cadial = 0.0 + (dial - 61)*(0.12/1.01);
            if ((intmode == 1) || (intmode == 6) || (intmode == 5)) { good_event = true; } 
            break;

        // CC1Pi dials (manres)
        case  65:
        case  66:
        case  67:
        case  68: //< 'nominal'
        case  69:
        case  70:
        case  71:
            //manresdial = 0.0 + (dial - 68)*(0.15/0.95);
            if ((intmode == 1) || (intmode == 6) || (intmode == 5)) { good_event = true; } 
            break;

        // CC1Pi dials (bkg)
        case  72:
        case  73:
        case  74:
        case  75: //< 'nominal'
        case  76:
        case  77:
        case  78:
            //bkgdial = 0.0 + (dial - 75)*(0.2/1.3);
            if ((intmode == 1) || (intmode == 6) || (intmode == 5)) { good_event = true; } 
            break;

        // SSC dials (vector)
        case  79:
        case  80:
        case  81:
        case  82: //< 'nominal'
        case  83:
        case  84:
        case  85:
            //sccvdial = 0.0 + (dial - 82)*(4.4);
            if ((intmode == 0) || (intmode == 7)) { good_event = true; }
            break;

        // SSC dials (axial)
        case  86:
        case  87:
        case  88:
        case  89: //< 'nominal'
        case  90:
        case  91:
        case  92:
            //sccadial = 0.0 + (dial - 89)*(0.15);
            if ((intmode == 0) || (intmode == 7))  { good_event = true; }
            break;

        // numu/nue dials
        case  93:
        case  94:
        case  95:
        case  96: //< 'nominal'
        case  97:
        case  98:
        case  99:
            //ccnuedial = 0.0 + (dial - 96)*(0.02/1.);
            if (fabs(NuPdg) == 12) { good_event = true; }
            break;

        // DIS dial
        case 100:
        case 101:
        case 102:
        case 103: //< 'nominal'
        case 104:
        case 105:
        case 106:
            //dismpidial = 0.0 + (dial - 103)*(0.4/1.);
            if (intmode == 3) { good_event = true; }
            break;

        // CC Coherent dial
        case 107:
        case 108:
        case 109:
        case 110: //< 'nominal'
        case 111:
        case 112:
        case 113:
            //cccohdial = 0.0 + (dial - 110)*(1/1);
            if (intmode == 2) { good_event = true; }
            break;

        // NC Coherent dial
        case 114:
        case 115:
        case 116:
        case 117: //< 'nominal'
        case 118:
        case 119:
        case 120:
            //nccohdial = 0.0 + (dial - 117)*(0.3/1.);
            if (intmode == 4) { good_event = true; }
            break;

        // NC Other dial
        case 121:
        case 122:
        case 123:
        case 124: //< 'nominal'
        case 125:
        case 126:
        case 127:
            //ncothdial = 0.0 + (dial - 124)*(0.3/1.);
            if (intmode == 7) { good_event = true; }
            break;

        // New Eb dial on Carbon
        case 128:
        case 129:
        case 130:
        case 131: //< 'nominal'
        case 132:
        case 133:
        case 134:
            //newebcdial = 0.0 + (dial - 131)*(9./25.);
            if ((intmode == 0) && (target == 0)) { good_event = true; }
            //if ((intmode == 0) && (target == 2)) { good_event = true; } //Check if hydrogen
            break;

        // New Eb dial on Oxygen
        case 135:
        case 136:
        case 137:
        case 138: //< 'nominal'
        case 139:
        case 140:
        case 141:
            //newebodial = 0.0 + (dial - 138)*(9./27.);
            if ((intmode == 0) && (target == 1)) { good_event = true; }
            break;

        // New MEC PPD dial on Carbon
        case 142:
        case 143:
        case 144:
        case 145: //< 'nominal'
        case 146:
        case 147:
        case 148:
            //mecpddcdial = 0.0 + (dial - 145)/3.; //-1, ..., 0, ..., 1
            if ((intmode == 8) && (target == 0)) { good_event = true; }
            break;

        // New MEC PPD dial on Oxygen
        case 149:
        case 150:
        case 151:
        case 152: //< 'nominal'
        case 153:
        case 154:
        case 155:
            //mecpddodial = 0.0 + (dial - 152)/3.; //-1,0,1
            if ((intmode == 8) && (target == 1)) { good_event = true; }
            break;

        default:
            break;

        }

        if (good_event) { truesigmahists[i][j]->Fill(weight); }
        //if (good_event) { truesigmahists[i][j]->Fill(Plep, weight); }
      }
    }
  } // end loop over entries

  //    f_sk->Close();
  //    f_w->Close();

  // Now make plots
  //int colors[9] = {797,418,432,600,619,616,632,1,600}; // orange, dark green, cyan, blue, purple, pink, red, black
  int sigmacolors[9] = {8,4,2,1,2,4,8}; // green, blue, red, black, red, blue, green
  //int sigmacolors[9] = {619,632,797,1,797, 632, 619}; // orange, dark green, cyan, blue, purple, pink, red, black
  std::string outname = outfile;
  outname.replace(outname.end()-4,outname.end(),"pdf");
  cout << "["<<__FILE__<<":L"<<__LINE__<<"] " <<"Writing to: " << outname << " == " << outname.c_str() << "\n";

  fout->cd();
  // Make ratio histograms
  TH1D *sigmaratios[21][7];
  TH1D *truesigmaratios[21][7];


  for(int i = 0; i < 21; ++i){
    double min=0; double max=2;
    for (int j = 0; j < 7; ++j){
      sigmaratios[i][j] = (TH1D*)sigmahists[i][j]->Clone();
      sigmaratios[i][j]->Divide(sigmahists[i][3]);
      sigmaratios[i][j]->GetYaxis()->SetRangeUser(min,max);
      sigmaratios[i][j]->SetLineColor(sigmacolors[j]);
      if(j <= 2) sigmaratios[i][j]->SetLineStyle(2);
      sigmahists[i][j]->SetLineColor(sigmacolors[j]);
      if(j <= 2) sigmahists[i][j]->SetLineStyle(2);

      truesigmaratios[i][j] = (TH1D*)truesigmahists[i][j]->Clone();
      truesigmaratios[i][j]->Divide(truesigmahists[i][3]);
      if(j <= 2) truesigmaratios[i][j]->SetLineStyle(2);
      if(truesigmaratios[i][j]->GetMaximum()*1.05 > 2) truesigmaratios[i][j]->GetYaxis()->SetRangeUser(0,truesigmaratios[i][j]->GetMaximum()*1.05);
      else truesigmaratios[i][j]->GetYaxis()->SetRangeUser(min,max);
      truesigmaratios[i][j]->SetLineColor(sigmacolors[j]);
      truesigmahists[i][j]->SetLineColor(sigmacolors[j]);
      if(j <= 2) truesigmahists[i][j]->SetLineStyle(2);

    }
  }


  cout << "["<<__FILE__<<":L"<<__LINE__<<"] " <<"Writing to: " << outname << " == " << outname.c_str() << "\n";
  TCanvas *c = new TCanvas("","",800,600);
  c->Print(Form("%s[",outname.c_str()));

  gStyle->SetOptStat("nMiou");

  for(int i = 0; i < 21; ++i){
    TLegend *tleg = new TLegend(0.525, 0.425, 0.89, 0.69);
    tleg->SetFillColor(kWhite);
    std::string tname = "3 sigma histograms for true interactions ";
      tname.append(dialnames[i]);
      //for(int j = 6; j > -1; --j){
      //  if(j == 5 || j == 1) continue;
      for(int j = 5; j > 0; --j){
        truesigmahists[i][j]->SetMinimum(0.01);
        tleg->AddEntry(truesigmahists[i][j],Form("%s_truesigma_%d", dialnames[i],(j-3)),"l");
      //if(j == 6){
        if(j == 5){
          truesigmahists[i][j]->SetTitle(tname.c_str());
          truesigmahists[i][j]->SetXTitle("Weight value");
          truesigmahists[i][j]->Draw();
        }
        else truesigmahists[i][j]->Draw("sames");
      }
    c->cd();
    c->SetLogy();
    fix_maximum();
    move_stats("h");
    tleg->Draw("same");
    c->Update();
    c->Print(outname.c_str(),Form("Title:%s",tname.c_str()));
    c->Write(tname.c_str());

    tname.replace(8,10,"ratios");

    TLegend *tleg1 = new TLegend(0.525, 0.425, 0.89, 0.69);
    tleg1->SetFillColor(kWhite);
    std::string tname1 = "3_sigma_ratios_true_interactions_";
      tname1.append(dialnames[i]);
      //for(int j = 6; j > -1; --j){
        //if(j == 5 || j == 1) continue;
      for(int j = 5; j > 0; --j){
        truesigmaratios[i][j]->SetMinimum(0.01);
        tleg1->AddEntry(truesigmaratios[i][j],Form("%s_truesigma_%d", dialnames[i],(j-3)),"l");
        //if(j == 6){
        if(j == 5){
          truesigmaratios[i][j]->SetTitle(tname1.c_str());
          truesigmaratios[i][j]->SetXTitle("Weight value");
          truesigmaratios[i][j]->Draw();
        }
        else truesigmaratios[i][j]->Draw("sames");
      }
    c->cd();
    c->SetLogy();
    fix_maximum();
    move_stats("h");
    tleg1->Draw("same");
    c->Update();
    c->Print(outname.c_str(),Form("Title:%s",tname1.c_str()));
    c->Write(tname1.c_str());
  }

  c->Print(Form("%s]",outname.c_str()));

  // Close output file
  cout << "["<<__FILE__<<":L"<<__LINE__<<"] " <<"Closing files" << endl;
  fout ->Close();
}







// --------------------------------------------------------------------- //

namespace {
  // Print the cmd line syntax
  void Usage(){
    cout << "Cmd line syntax should be:" << endl;
    cout << "./plep_Tuning_Validation_ND280_NIWG.exe -w weightfile -s skfile  [-o outputfile]" << endl;
    exit(2);
  }


  int ParseArgs(int argc, char **argv){

    for (;;) {
      int c = getopt(argc, argv, "w:s:o:");
      if (c < 0)
        break;
      switch (c) {
        case 'w': {
                    fWeightName = optarg;
                    break;
                  }
        case 's': {
                    fFileName = optarg;
                    break;
                  }
        case 'o': {
                    fOutfile = optarg;
                    break;
                  }
        default:  {
                    std::cerr << "Option " << c << " is not recognised." << std::endl;
                    Usage();
                  }
      }
    }
    return 0;
  }

};
