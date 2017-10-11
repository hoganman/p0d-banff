//--------------------------------------------------------------
//
// Copied of Tuning_Validation_ND280_NIWG.cxx
// To be used for plotting ediff plots
//
//--------------------------------------------------------------

#include <stdlib.h>
#include <cstdlib>
#include <sys/types.h>
#include <string.h>

#include <iostream>

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
  fOutfile = "Valid_tunings_ediff";
  ParseArgs(argc, argv);

  PlotTunings(fWeightName, fFileName, fOutfile+"_nue.root",    12);
  PlotTunings(fWeightName, fFileName, fOutfile+"_nueb.root",  -12);
  PlotTunings(fWeightName, fFileName, fOutfile+"_numu.root",   14);
  PlotTunings(fWeightName, fFileName, fOutfile+"_numub.root", -14);
  return 0;
}

void PlotTunings(const std::string& weightfile, const std::string& eventfile, const std::string& outfile, const int desired_neutrino)
{
  // Get the arguments
  //cout <<"["<<__FILE__<<":L"<<__LINE__<<"] " << "Writing to: " << outfile << "\n";

  // Open files and set up trees
  TFile *f_sk = new TFile(eventfile.c_str());
  TTree *t_sk = NULL;   f_sk->GetObject("SelectedEventTruth", t_sk);
  TTree *t_ed = NULL;   f_sk->GetObject("EdiffSelectedEventTruth", t_ed);

  TFile *f_w = new TFile(weightfile.c_str());
  TTree* t_w = NULL;   f_w->GetObject("weightstree", t_w);

  int Mode;     t_sk->SetBranchAddress("Mode",&Mode);
  int Target;   t_sk->SetBranchAddress("Target",&Target);
  int NuPdg;    t_sk->SetBranchAddress("NuPdg",&NuPdg);
  double ediff; t_ed->SetBranchAddress("ediff",&ediff);

  t_sk->AddFriend(t_ed);

  TArrayF* weights = NULL;
  t_w->SetBranchAddress("weights",&weights);

  // Open file to save plots (default: Valid_tunings_ediff.root in current directory)
  TFile *fout;
  int dummy;
  char savename[100];
  fout = new TFile(outfile.c_str(),"RECREATE");

  fout->cd();

  // Set up histograms 
  // 9 histograms: sf only, rpa only, maqe only, mec_norm_c only, pf_c only, ccqe tunings, all tunings, untuned, rpa rel ("ccqe tunings" = maqe + mec_norm_c + pf_c)
  // Code ran separately for each flavor (numu, nue, anumu, anue)
  // 1 x-axes (ediff)
  // 4 targets (C, O, H, Other)
  TH1D *hists[9][1][1][4];

  TH1D *sigmahists[21][7];
  TH1D *truesigmahists[21][7];
  //TH1D *truepisigmahists[7][3][7];

  char const* dialnames[21] = {"MAQE", "MEC_C", "pF_C", "Eb_C", "MEC_O", "pF_O", "Eb_O",
                         "CA5", "MaNFFRes", "BgSclRes", "SCCV", "SCCA", "CCNuEE0",
                         "DISMPiShp", "CCCohE0", "NCCohE0", "NCOtherE0",
                         "New_Eb_C", "New_Eb_O", "MEC_PDD_C", "MEC_PDD_O"};

  //-- char* modenames[7] = {"11", "12", "13", "31", "32", "33", "34"};

  for (int i=0; i<9; i++) {
    for (int f=0; f<1; f++) {
      for (int e=0; e<1; e++) {
        for (int t=0; t<4; t++) {
          if (i==0) dummy = sprintf(savename, "sf_only_f%d_e%d_t%d",f,e,t);
          if (i==1) dummy = sprintf(savename, "rpa_only_f%d_e%d_t%d",f,e,t);
          if (i==2) dummy = sprintf(savename, "maqe_only_f%d_e%d_t%d",f,e,t);
          if (i==3) dummy = sprintf(savename, "mec_norm_c_only_f%d_e%d_t%d",f,e,t);
          if (i==4) dummy = sprintf(savename, "pf_c_only_f%d_e%d_t%d",f,e,t);
          if (i==5) dummy = sprintf(savename, "all_ccqe_f%d_e%d_t%d",f,e,t);
          if (i==6) dummy = sprintf(savename, "all_tunings_f%d_e%d_t%d",f,e,t);
          if (i==7) dummy = sprintf(savename, "untuned_f%d_e%d_t%d",f,e,t);
          if (i==8) dummy = sprintf(savename, "rpa_rel_only_f%d_e%d_t%d",f,e,t);

          if (e==0) { // ediff
            hists[i][f][e][t] = new TH1D(savename, savename, 100, -2, 2);
          }
        }
      }
    }
  }

  //-- TH1D* Ca5_Weights = new TH1D("Ca5_3SigmaVariation_Weight_eq_1","Ca5_3SigmaVariation_Weight_eq_1",100,-2,2);
  //-- TH1D* MaNFFRes_Weights = new TH1D("MaNFFRes_3SigmaVariation_Weight_eq_1","MaNFFRes_3SigmaVariation_Weight_eq_1",100,-2,2);
  //-- TH1D* BkgScl_Weights = new TH1D("BkgScl_3SigmaVariation_Weight_eq_1","BkgScl_3SigmaVariation_Weight_eq_1",100,-2,2);
  //-- TH1D* Ca5_HighWeights = new TH1D("Ca5_3SigmaVariation_Weight_gt_1","Ca5_3SigmaVariation_Weight_gt_1",100,-2,2);
  //-- TH1D* MaNFFRes_HighWeights = new TH1D("MaNFFRes_3SigmaVariation_Weight_gt_1","MaNFFRes_3SigmaVariation_Weight_gt_1",100,-2,2);
  //-- TH1D* BkgScl_HighWeights = new TH1D("BkgScl_3SigmaVariation_Weight_gt_1","BkgScl_3SigmaVariation_Weight_gt_1",100,-2,2);
  //-- TH1D* Ca5_LowWeights = new TH1D("Ca5_3SigmaVariation_Weight_lt_1","Ca5_3SigmaVariation_Weight_lt_1",100,-2,2);
  //-- TH1D* MaNFFRes_LowWeights = new TH1D("MaNFFRes_3SigmaVariation_Weight_lt_1","MaNFFRes_3SigmaVariation_Weight_lt_1",100,-2,2);
  //-- TH1D* BkgScl_LowWeights = new TH1D("BkgScl_3SigmaVariation_Weight_lt_1","BkgScl_3SigmaVariation_Weight_lt_1",100,-2,2);
  //-- TH1D* Ca5_NegWeights = new TH1D("Ca5_3SigmaVariation_Weight_lt_0","Ca5_3SigmaVariation_Weight_lt_0",100,-2,2);
  //-- TH1D* MaNFFRes_NegWeights = new TH1D("MaNFFRes_3SigmaVariation_Weight_lt_0","MaNFFRes_3SigmaVariation_Weight_lt_0",100,-2,2);
  //-- TH1D* BkgScl_NegWeights = new TH1D("BkgScl_3SigmaVariation_Weight_lt_0","BkgScl_3SigmaVariation_Weight_lt_0",100,-2,2);

  for(int i=0; i < 21; ++i){
    for(int j = 0; j < 7; ++j){
      sigmahists[i][j] = new TH1D(Form("%s_sigma_%d",dialnames[i], (j-3)), Form("%s_sigma_%d",dialnames[i], (j-3)), 100, -2, 2);
      truesigmahists[i][j] = new TH1D(Form("%s_trueSample_sigma_%d",dialnames[i], (j-3)), Form("%s_trueSample_sigma_%d",dialnames[i], (j-3)), 100, -2, 2);
      //-- for(int k = 0; k < 7; ++k){
      //--   if(i >=7 && i < 10) truepisigmahists[k][i-7][j] = new TH1D(Form("%s_%s_trueSample_sigma_%d",dialnames[i], modenames[k], (j-3)), Form("%s_%s_trueSample_sigma_%d",dialnames[i], modenames[k], (j-3)), 100, -2, 2);
      //-- }
    }
  }

  TH1D* protonFSIhist = new TH1D("Proton_FSI_hist","Proton_FSI_hist", 100, -3, 3);

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
    //-- int selec = -1;
    //-- if(NuPdg == 14) selec = 0;
    //-- else if(NuPdg == 12) selec = 1;
    //-- else if(NuPdg == -14) selec = 2;
    //-- else if(NuPdg == -12) selec = 3;
    //-- else std::cout << "["<<__FILE__<<":L"<<__LINE__<<"] " <<"Neutrino PDG error" << std::endl;

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
        sigmahists[i][j]->Fill(ediff, weight);

        //-- if(i == 7 && (abs(Mode)==11)) truepisigmahists[0][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 7 && (abs(Mode)==12)) truepisigmahists[1][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 7 && (abs(Mode)==13)) truepisigmahists[2][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 7 && (abs(Mode)==31)) truepisigmahists[3][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 7 && (abs(Mode)==32)) truepisigmahists[4][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 7 && (abs(Mode)==33)) truepisigmahists[5][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 7 && (abs(Mode)==34)) truepisigmahists[6][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 7 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) > 1) Ca5_HighWeights->Fill(ediff);
        //-- if(i == 7 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) == 1) Ca5_Weights->Fill(ediff);
        //-- if(i == 7 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) < 1 && weights->At(dial)>=0) Ca5_LowWeights->Fill(ediff);
        //-- if(i == 7 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) < 0) Ca5_LowWeights->Fill(ediff);

        //-- if(i == 8 && (abs(Mode)==11)) truepisigmahists[0][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 8 && (abs(Mode)==12)) truepisigmahists[1][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 8 && (abs(Mode)==13)) truepisigmahists[2][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 8 && (abs(Mode)==31)) truepisigmahists[3][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 8 && (abs(Mode)==32)) truepisigmahists[4][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 8 && (abs(Mode)==33)) truepisigmahists[5][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 8 && (abs(Mode)==34)) truepisigmahists[6][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 8 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) > 1) MaNFFRes_HighWeights->Fill(ediff);
        //-- if(i == 8 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) == 1) MaNFFRes_Weights->Fill(ediff);
        //-- if(i == 8 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) < 1 && weights->At(dial)>=0) MaNFFRes_LowWeights->Fill(ediff);
        //-- if(i == 8 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) < 0) MaNFFRes_LowWeights->Fill(ediff);

        //-- if(i == 9 && (abs(Mode)==11)) truepisigmahists[0][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 9 && (abs(Mode)==12)) truepisigmahists[1][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 9 && (abs(Mode)==13)) truepisigmahists[2][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 9 && (abs(Mode)==31)) truepisigmahists[3][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 9 && (abs(Mode)==32)) truepisigmahists[4][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 9 && (abs(Mode)==33)) truepisigmahists[5][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 9 && (abs(Mode)==34)) truepisigmahists[6][i-7][j]->Fill(ediff, weights->At(dial)); //Pi
        //-- if(i == 9 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) > 1) BkgScl_HighWeights->Fill(ediff);
        //-- if(i == 9 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) == 1) BkgScl_Weights->Fill(ediff);
        //-- if(i == 9 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) < 1 && weights->At(dial)>=0) BkgScl_LowWeights->Fill(ediff);
        //-- if(i == 9 && (intmode == 1 || intmode == 5 || intmode == 6) && j == 0 && weights->At(dial) < 0) BkgScl_NegWeights->Fill(ediff);

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

        if (good_event) { truesigmahists[i][j]->Fill(ediff, weight); }
      }
    }


    protonFSIhist->Fill(ediff, weights->At(dial));

    // Assume weights file was run from dial=126 up (i.e. only 8 different tunings in array)
    if ((intmode==0 || intmode==8)) // accept CCQE and MEC events only
    {
      hists[0][0][0][target]->Fill(ediff, weights->At(0));

      // rpa only
      hists[1][0][0][target]->Fill(ediff, weights->At(1));

      // maqe only
      hists[2][0][0][target]->Fill(ediff, weights->At(2));

      // mec_norm_c only
      hists[3][0][0][target]->Fill(ediff, weights->At(3));

      // pf_c only
      hists[4][0][0][target]->Fill(ediff, weights->At(4));

      // all ccqe tunings
      hists[5][0][0][target]->Fill(ediff, weights->At(5));

      // all tunings
      hists[6][0][0][target]->Fill(ediff, weights->At(6));

      // all tunings
      hists[8][0][0][target]->Fill(ediff, weights->At(8));

      // untuned
      hists[7][0][0][target]->Fill(ediff);

      if(target != 3){
        // sf only
        hists[0][0][0][3]->Fill(ediff, weights->At(0));

        // rpa only
        hists[1][0][0][3]->Fill(ediff, weights->At(1));

        // maqe only
        hists[2][0][0][3]->Fill(ediff, weights->At(2));

        // mec_norm_c only
        hists[3][0][0][3]->Fill(ediff, weights->At(3));

        // pf_c only
        hists[4][0][0][3]->Fill(ediff, weights->At(4));

        // all ccqe tunings
        hists[5][0][0][3]->Fill(ediff, weights->At(5));

        // all tunings
        hists[6][0][0][3]->Fill(ediff, weights->At(6));

        // all tunings
        hists[8][0][0][3]->Fill(ediff, weights->At(8));

        // untuned
        hists[7][0][0][3]->Fill(ediff);
      }
    }
  } // end loop over entries

  //    f_sk->Close();
  //    f_w->Close();

  // Now make plots
  int colors[9] = {8,4,2,1,2,4,8}; // green, blue, red, black, red, blue, green
  int sigmacolors[9] = {8,4,2,1,2,4,8}; // green, blue, red, black, red, blue, green
  std::string outname = outfile;
  outname.replace(outname.end()-4,outname.end(),"pdf");
  cout << "["<<__FILE__<<":L"<<__LINE__<<"] " <<"Writing to: " << outname << " == " << outname.c_str() << "\n";

  fout->cd();
  // Make ratio histograms
  TH1D *ratios[9][1][1][4];
  TH1D *sigmaratios[21][7];
  TH1D *truesigmaratios[21][7];
  //-- TH1D *truepisigmaratios[7][3][7];

  TH1D *protonFSIratio;

  for (int i=0; i<9; i++) {
    for (int s=0; s<1; s++) {
      for (int e=0; e<1; e++) {
        for (int f=0; f<4; ++f) {
          ratios[i][s][e][f] = (TH1D*)hists[i][s][e][f]->Clone();
          ratios[i][s][e][f]->Divide(hists[7][s][e][f]);

          // Also set line colours and styles
          hists[i][s][e][f]->SetLineColor(colors[i]);
          hists[i][s][e][f]->GetYaxis()->SetRangeUser(0,hists[i][s][e][f]->GetMaximum()*1.5);
          ratios[i][s][e][f]->SetLineColor(colors[i]);
          ratios[i][s][e][f]->GetYaxis()->SetRangeUser(0,2);
        }
      }
    }
  }

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

      //-- if(i >=7 && i < 10){
      //--   for(int k = 0; k < 7; ++k){
      //--     truepisigmaratios[k][i-7][j] = (TH1D*)truepisigmahists[k][i-7][j]->Clone();
      //--     truepisigmaratios[k][i-7][j]->Divide(truepisigmahists[k][i-7][3]);
      //--     if(j <= 2) truepisigmaratios[k][i-7][j]->SetLineStyle(2);
      //--     if(truepisigmaratios[k][i-7][j]->GetMaximum()*1.05 > 2) truepisigmaratios[k][i-7][j]->GetYaxis()->SetRangeUser(0,truepisigmaratios[k][i-7][j]->GetMaximum()*1.05);
      //--     else truepisigmaratios[k][i-7][j]->GetYaxis()->SetRangeUser(0,2);
      //--     truepisigmaratios[k][i-7][j]->SetLineColor(sigmacolors[j]);
      //--     truepisigmahists[k][i-7][j]->SetLineColor(sigmacolors[j]);
      //--     if(j <= 2) truepisigmahists[k][i-7][j]->SetLineStyle(2);
      //--   }
      //-- }
    }
  }

  protonFSIratio = (TH1D*)protonFSIhist->Clone();
  protonFSIratio->Divide(sigmahists[15][3]);
  protonFSIratio->SetLineColor(colors[2]);
  protonFSIratio->GetYaxis()->SetRangeUser(0,2);


  cout << "["<<__FILE__<<":L"<<__LINE__<<"] " <<"Writing to: " << outname << " == " << outname.c_str() << "\n";
  TCanvas *c = new TCanvas("","",800,600);
  c->Print(Form("%s[",outname.c_str()));

  //-- Ca5_HighWeights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",Ca5_HighWeights->GetTitle()));

  //-- MaNFFRes_HighWeights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",MaNFFRes_HighWeights->GetTitle()));

  //-- BkgScl_HighWeights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",BkgScl_HighWeights->GetTitle()));

  //-- Ca5_Weights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",Ca5_Weights->GetTitle()));

  //-- MaNFFRes_Weights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",MaNFFRes_Weights->GetTitle()));

  //-- BkgScl_Weights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",BkgScl_Weights->GetTitle()));


  //-- Ca5_LowWeights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",Ca5_LowWeights->GetTitle()));

  //-- MaNFFRes_LowWeights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",MaNFFRes_LowWeights->GetTitle()));

  //-- BkgScl_LowWeights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",BkgScl_LowWeights->GetTitle()));

  //-- Ca5_NegWeights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",Ca5_NegWeights->GetTitle()));

  //-- MaNFFRes_NegWeights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",MaNFFRes_NegWeights->GetTitle()));

  //-- BkgScl_NegWeights->Draw();
  //-- c->Update();
  //-- c->Print(outname.c_str(), Form("Title:%s",BkgScl_NegWeights->GetTitle()));


  //    TLegend *leg0 = new TLegend(0.525, 0.625, 0.89, 0.89);
  //    leg0->SetFillColor(kWhite);
  //    std::string name0 = "Proton_FSI_histograms";
  //    leg0->AddEntry(sigmahists[15][3],"Nominal","l");
  //    leg0->AddEntry(protonFSIhist,"Proton FSI","l");
  //    sigmahists[15][3]->Draw();
  //    protonFSIhist->Draw("same");
  //    leg0->Draw("same");
  //    c->Update();
  //    c->Print(Form("%s",outname.c_str()), Form("Title:%s",protonFSIhist->GetTitle()));
  //
  //    c->Write(name0.c_str());
  //
  //    TLegend *legp = new TLegend(0.525, 0.625, 0.89, 0.89);
  //    legp->SetFillColor(kWhite);
  //    std::string namep = "Proton_FSI_ratio";
  //    legp->AddEntry(sigmaratios[15][3],"Nominal","l");
  //    legp->AddEntry(protonFSIratio,"Proton FSI","l");
  //    sigmaratios[15][3]->Draw();
  //    protonFSIratio->Draw("same");
  //    legp->Draw("same");
  //    c->Update();
  //
  //    c->Write(namep.c_str());

  for(int i = 0; i < 21; ++i){
    //-- TLegend *leg = new TLegend(0.525, 0.625, 0.89, 0.89);
    //-- leg->SetFillColor(kWhite);
    //-- std::string name = "3 sigma histograms ";
    //--   name.append(dialnames[i]);
    //--   for(int j = 6; j > -1; --j){
    //--     if(j == 5 || j == 4 || j == 2 || j == 1) continue;
    //--     leg->AddEntry(sigmahists[i][j],Form("%s_sigma_%d", dialnames[i],(j-3)),"l");
    //--     if(j == 6){
    //--       sigmahists[i][j]->SetTitle(name.c_str());
    //--       sigmahists[i][j]->SetXTitle("E_{reco}-E_{true} (GeV)");
    //--       sigmahists[i][j]->Draw();
    //--     }
    //--     else sigmahists[i][j]->Draw("same");
    //--   }
    //-- leg->Draw("same");
    //-- c->Update();
    //-- c->Print(outname.c_str(),Form("Title:%s",name.c_str()));
    //-- c->Write(name.c_str());

    //-- name.replace(8,10,"ratios");

    //-- TLegend *leg1 = new TLegend(0.525, 0.625, 0.89, 0.89);
    //-- leg1->SetFillColor(kWhite);
    //-- std::string name1 = "3_sigma_ratios_";
    //--   name1.append(dialnames[i]);
    //--   for(int j = 6; j > -1; --j){
    //--     if(j == 5 || j == 4 || j == 2 || j == 1) continue;
    //--     leg1->AddEntry(sigmaratios[i][j],Form("%s_sigma_%d", dialnames[i],(j-3)),"l");
    //--     if(j == 6){
    //--       sigmaratios[i][j]->SetTitle(name.c_str());
    //--       sigmaratios[i][j]->SetXTitle("E_{reco}-E_{true} (GeV)");
    //--       sigmaratios[i][j]->Draw();
    //--     }
    //--     else sigmaratios[i][j]->Draw("same");
    //--   }
    //-- leg1->Draw("same");
    //-- c->Update();
    //-- c->Print(outname.c_str(),Form("Title:%s",name.c_str()));
    //-- c->Write(name1.c_str());

    TLegend *tleg = new TLegend(0.525, 0.625, 0.89, 0.89);
    tleg->SetFillColor(kWhite);
    std::string tname = "3 sigma histograms for true interactions ";
      tname.append(dialnames[i]);
      for(int j = 6; j > -1; --j){
        if(j == 5 || j == 4 || j == 2 || j == 1) continue;
        tleg->AddEntry(truesigmahists[i][j],Form("%s_truesigma_%d", dialnames[i],(j-3)),"l");
        if(j == 6){
          truesigmahists[i][j]->SetTitle(tname.c_str());
          truesigmahists[i][j]->SetXTitle("E_{reco}-E_{true} (GeV)");
          truesigmahists[i][j]->Draw();
        }
        else truesigmahists[i][j]->Draw("same");
      }
    c->cd();
    fix_maximum();
    tleg->Draw("same");
    c->Update();
    c->Print(outname.c_str(),Form("Title:%s",tname.c_str()));
    c->Write(tname.c_str());

    tname.replace(8,10,"ratios");

    TLegend *tleg1 = new TLegend(0.525, 0.625, 0.89, 0.89);
    tleg1->SetFillColor(kWhite);
    std::string tname1 = "3_sigma_ratios_true_interactions_";
      tname1.append(dialnames[i]);
      for(int j = 6; j > -1; --j){
        if(j == 5 || j == 4 || j == 2 || j == 1) continue;
        tleg1->AddEntry(truesigmaratios[i][j],Form("%s_truesigma_%d", dialnames[i],(j-3)),"l");
        if(j == 6){
          truesigmaratios[i][j]->SetTitle(tname1.c_str());
          truesigmaratios[i][j]->SetXTitle("E_{reco}-E_{true} (GeV)");
          truesigmaratios[i][j]->Draw();
        }
        else truesigmaratios[i][j]->Draw("same");
      }
    c->cd();
    fix_maximum();
    tleg1->Draw("same");
    c->Update();
    c->Print(outname.c_str(),Form("Title:%s",tname1.c_str()));
    c->Write(tname1.c_str());

    //-- if(i >=7 && i < 10){
    //--   for(int k = 0; k < 7; ++k){

    //--     TLegend *ptleg = new TLegend(0.525, 0.625, 0.89, 0.89);
    //--     ptleg->SetFillColor(kWhite);
    //--     std::string ptname = "3 sigma histograms for true interactions ";
    //--     ptname.append(dialnames[i]);
    //--     ptname.append(Form(" %s",modenames[k]));
    //--     for(int j = 6; j > -1; --j){
    //--       if(j == 5 || j == 4 || j == 2 || j == 1) continue;
    //--       ptleg->AddEntry(truepisigmahists[k][i-7][j],Form("%s_%s_truepisigma_%d", dialnames[i], modenames[k], (j-3)),"l");
    //--       if(j == 6){
    //--         truepisigmahists[k][i-7][j]->SetTitle(ptname.c_str());
    //--         truepisigmahists[k][i-7][j]->SetXTitle("E_{reco}-E_{true} (GeV)");
    //--         truepisigmahists[k][i-7][j]->Draw();
    //--       }
    //--       else truepisigmahists[k][i-7][j]->Draw("same");
    //--     }
    //--     ptleg->Draw("same");
    //--     c->Update();
    //--     c->Print(outname.c_str(),Form("Title:%s",ptname.c_str()));
    //--     c->Write(ptname.c_str());

    //--     ptname.replace(8,10,"ratios");

    //--     TLegend *ptleg1 = new TLegend(0.525, 0.625, 0.89, 0.89);
    //--     ptleg1->SetFillColor(kWhite);
    //--     std::string ptname1 = "3_sigma_ratios_true_interactions_";
    //--     ptname1.append(dialnames[i]);
    //--     ptname1.append(Form("_%s",modenames[k]));
    //--     for(int j = 6; j > -1; --j){
    //--       if(j == 5 || j == 4 || j == 2 || j == 1) continue;
    //--       ptleg1->AddEntry(truepisigmaratios[k][i-7][j],Form("%s_%s_truepisigma_%d", dialnames[i], modenames[k], (j-3)),"l");
    //--       if(j == 6){
    //--         truepisigmaratios[k][i-7][j]->SetTitle(ptname1.c_str());
    //--         truepisigmaratios[k][i-7][j]->SetXTitle("E_{reco}-E_{true} (GeV)");
    //--         truepisigmaratios[k][i-7][j]->Draw();
    //--       }
    //--       else truepisigmaratios[k][i-7][j]->Draw("same");
    //--     }
    //--     ptleg1->Draw("same");
    //--     c->Update();
    //--     c->Print(outname.c_str(),Form("Title:%s",ptname1.c_str()));
    //--     c->Write(ptname1.c_str());
    //--   }
    //-- }           
  }

  // Plot 1: All ccqe tunings overlaid, with 'all ccqe tunings' too, and untuned
  // Make legend
  TLegend *leg1 = new TLegend(0.525, 0.625, 0.89, 0.89);
  leg1->SetFillColor(kWhite);
  leg1->AddEntry(hists[2][0][0][0],"MaQE tuning only","l");
  leg1->AddEntry(hists[3][0][0][0],"Mec_norm_C tuning only","l");
  leg1->AddEntry(hists[4][0][0][0],"pF_C tuning only","l");
  leg1->AddEntry(hists[5][0][0][0],"All CCQE tunings","l");
  leg1->AddEntry(hists[7][0][0][0],"Untuned","l");

  for (int s=0; s<1; s++) {
    for (int e=0; e<1; e++) {
      for (int f=0; f<4; ++f) {
        std::string name = "CCQE tuning for";
        if(s == 0) name.append(" #nu");
        if(e == 0) name.append(" vs E_{reco}-E_{true}");
        if(f == 0) name.append(" on carbon");
        if(f == 1) name.append(" on oxygen");
        if(f == 2) name.append(" on hydrogen");
        if(f == 3) name.append(" on all nuclei");

        hists[7][s][e][f]->SetTitle(name.c_str());
        if(e == 0){
          hists[7][s][e][f]->SetXTitle("E_{reco}-E_{true} (GeV)");
          ratios[7][s][e][f]->SetXTitle("E_{reco}-E_{true} (GeV)");
        }
        name.insert(11, " ratio");
        ratios[7][s][e][f]->SetTitle(name.c_str());

        std::string epsname = "CC0Pi_tunings";
        if(s == 0) epsname.append("_nu");
        if(e == 0) epsname.append("_ediff");
        if(f == 0) epsname.append("_carbon");
        if(f == 1) epsname.append("_oxygen");
        if(f == 2) epsname.append("_hydrogen");
        if(f == 3) epsname.append("_all");
        std::string nam = epsname;
        //                epsname.append(".eps");

        hists[7][s][e][f]->Draw();
        hists[2][s][e][f]->Draw("same");
        hists[3][s][e][f]->Draw("same");
        hists[4][s][e][f]->Draw("same");
        hists[5][s][e][f]->Draw("same");
        leg1->Draw("same");
        c->Update();
        //                c->Print(epsname.c_str());
        c->Print(outname.c_str(),Form("Title:%s",epsname.c_str()));
        c->Write(nam.c_str());

        epsname.insert(0,"Ratios_");
        nam.insert(0,"Ratios_");

        ratios[7][s][e][f]->Draw();
        ratios[2][s][e][f]->Draw("same");
        ratios[3][s][e][f]->Draw("same");
        ratios[4][s][e][f]->Draw("same");
        ratios[5][s][e][f]->Draw("same");
        leg1->Draw("same");
        c->Update();
        //                c->Print(epsname.c_str());
        c->Print(outname.c_str(),Form("Title:%s",epsname.c_str()));
        c->Write(nam.c_str());
      }
    }
  }

  // Plot 2: Overlaid: SF only, RES only, All CCQE tunings, Untuned

  // Make legend
  TLegend *leg2 = new TLegend(0.525, 0.625, 0.89, 0.89);
  leg2->SetFillColor(kWhite);
  leg2->AddEntry(hists[0][0][0][0],"SF tuning only","l");
  leg2->AddEntry(hists[1][0][0][0],"Non-rel RPA tuning only","l");
  leg2->AddEntry(hists[8][0][0][0],"Rel RPA tuning only","l");
  leg2->AddEntry(hists[5][0][0][0],"CCQE tunings only","l");
  leg2->AddEntry(hists[7][0][0][0],"Untuned","l");

  for (int s=0; s<1; s++) {
    for (int e=0; e<1; e++) {
      for (int f=0; f<4; ++f) {
        std::string name = "Individual tuning for";
        if(s == 0) name.append(" #nu");
        if(e == 0) name.append(" vs E_{reco}-E_{true}");
        if(f == 0) name.append(" on carbon");
        if(f == 1) name.append(" on oxygen");
        if(f == 2) name.append(" on hydrogen");
        if(f == 3) name.append(" on all nuclei");

        hists[7][s][e][f]->SetTitle(name.c_str());
        name.insert(17, " ratio");
        ratios[7][s][e][f]->SetTitle(name.c_str());

        std::string epsname = "Individual_tunings";
        if(s == 0) epsname.append("_nu");
        if(e == 0) epsname.append("_ediff");
        if(f == 0) epsname.append("_carbon");
        if(f == 1) epsname.append("_oxygen");
        if(f == 2) epsname.append("_hydrogen");
        if(f == 3) epsname.append("_all");
        std::string nam = epsname;
        //                epsname.append(".eps");

        hists[7][s][e][f]->Draw();
        hists[0][s][e][f]->Draw("same");
        hists[1][s][e][f]->Draw("same");
        hists[8][s][e][f]->Draw("same");
        hists[5][s][e][f]->Draw("same");

        leg2->Draw("same");
        c->Update();
        //                c->Print(epsname.c_str());
        c->Print(outname.c_str(),Form("Title:%s",epsname.c_str()));
        c->Write(nam.c_str());

        epsname.insert(0,"Ratios_");
        nam.insert(0,"Ratios_");

        ratios[7][s][e][f]->Draw();
        ratios[0][s][e][f]->Draw("same");
        ratios[1][s][e][f]->Draw("same");
        ratios[8][s][e][f]->Draw("same");
        ratios[5][s][e][f]->Draw("same");

        leg2->Draw("same");
        c->Update();
        //                c->Print(epsname.c_str());
        c->Print(outname.c_str(),Form("Title:%s",epsname.c_str()));
        c->Write(nam.c_str());
      }
    }
  }

  // Plot 3: Overlaid: All tunings, untuned

  // Make legend
  TLegend *leg3 = new TLegend(0.525, 0.625, 0.89, 0.89);
  leg3->SetFillColor(kWhite);
  leg3->AddEntry(hists[7][0][0][0],"Untuned","l");
  leg3->AddEntry(hists[6][0][0][0],"With full NIWG 2015 tuning","l");

  for (int s=0;s<1;s++) {
    for (int e=0; e<1; e++) {
      for (int f=0; f<4; ++f) {
        std::string name = "Full NIWG tuning for";
        if(s == 0) name.append(" #nu");
        if(e == 0) name.append(" vs E_{reco}-E_{true}");
        if(f == 0) name.append(" on carbon");
        if(f == 1) name.append(" on oxygen");
        if(f == 2) name.append(" on hydrogen");
        if(f == 3) name.append(" on all nuclei");

        hists[7][s][e][f]->SetTitle(name.c_str());
        name.insert(16, " ratio");
        ratios[7][s][e][f]->SetTitle(name.c_str());


        std::string epsname = "All_tunings";
        if(s == 0) epsname.append("_nu");
        if(e == 0) epsname.append("_ediff");
        if(f == 0) epsname.append("_carbon");
        if(f == 1) epsname.append("_oxygen");
        if(f == 2) epsname.append("_hydrogen");
        if(f == 3) epsname.append("_all");
        std::string nam = epsname;
        //                epsname.append(".eps");

        hists[7][s][e][f]->Draw();
        hists[6][s][e][f]->Draw("same");

        leg3->Draw("same");
        c->Update();
        //                c->Print(epsname.c_str());
        c->Print(outname.c_str(),Form("Title:%s",epsname.c_str()));
        c->Write(nam.c_str());

        epsname.insert(0,"Ratios_");
        nam.insert(0,"Ratios_");

        ratios[7][s][e][f]->Draw();
        ratios[6][s][e][f]->Draw("same");

        leg3->Draw("same");
        c->Update();
        //                c->Print(epsname.c_str());
        c->Print(outname.c_str(),Form("Title:%s",epsname.c_str()));
        c->Write(nam.c_str());
      }
    }
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
    cout << "./ediff_Tuning_Validation_ND280_NIWG.exe -w weightfile -s skfile  [-o outputfile]" << endl;
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
