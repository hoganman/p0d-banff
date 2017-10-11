//
// Example to show how to reweight NEUT events from the NRooTrackerVtx
// objects stored in an oaAnalysis tree. Run with: 
//
// ./generateWeightsFromNRooTracker_NEUTexample.exe -n nd_inputfile
//   
// Jim Dobson - 28/03/2011
// Kendall Mahn - 29/08/2011
// 
#include <stdlib.h>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include <TLorentzVector.h>

#include "T2KReWeight.h"
#include "T2KSyst.h"

//#include "T2KGenieReWeight.h" 
//#include "T2KGenieUtils.h"

#include "T2KNeutReWeight.h"
#include "T2KNeutUtils.h"
#include "T2KNIWGUtils.h"

#include "T2KJNuBeamReWeight.h"

#include "T2KNIWGReWeight.h"

#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__NRooTrackerVtx.h"
#endif

// For weight storer class
#include "T2KWeightsStorer.h"

#include "SK__h1.h"

using std::cout;
using std::cerr;
using std::endl;

using namespace t2krew;

int fNEvts = -1;
int sNEvts = -1; // summary tree
char * fNDFileList;
char * fSKFileName;

Int_t           selRunID;
Int_t           selEventID;
Int_t           selLevel;
Int_t           selNeutGenie;
Int_t           selNeutTrajId;
Int_t           selMuTrueFV;
Int_t           selCCQE;
Int_t           selMuNufam;
Int_t           selMuReac;
Int_t           selMuBunchNum;
Double_t        selMuMom;
Double_t        selMuMomDirX;
Double_t        selMuMomDirY;
Double_t        selMuMomDirZ;
Double_t        selMuPosX;
Double_t        selMuPosY;
Double_t        selMuPosZ;
Double_t        selMuPosT;
Double_t        selMuFluxWeight;
Int_t           trueID;
Int_t           truePDG;
Int_t           trueParentPDG;
Int_t           trueGrandParentPDG;
Double_t        trueMom;
Double_t        trueMomDirX;
Double_t        trueMomDirY;
Double_t        trueMomDirZ;
Double_t        truePosX;
Double_t        truePosY;
Double_t        truePosZ;
Int_t           truePrimaryID;
Double_t        truePrimaryMom;
Double_t        truePrimaryMomDirX;
Double_t        truePrimaryMomDirY;
Double_t        truePrimaryMomDirZ;
Double_t        truePrimaryPosX;
Double_t        truePrimaryPosY;
Double_t        truePrimaryPosZ;
Double_t        selMuEnu;
Int_t           selMuNuParPDG;
Int_t           selMuNuID;
Int_t           NeutID;
Int_t           NeutIDctr;
Int_t           PrimaryIDctr;
Int_t           VtxIDctr;
Int_t           ReactionCode;

void Usage();
void ParseArgs(int argc, char **argv);
int GetPThetaBin(double p, double th);

int main(int argc, char *argv[])
{
  //cout<<"blah"<<endl;
// This example only works when compiled against oaAnalysis
#ifdef __T2KRW_OAANALYSIS_ENABLED__
 ParseArgs(argc, argv);


 T2KWeightsStorer *storer_qe = new T2KWeightsStorer("ccqe_weights.root"); //forweightstorer
 T2KWeightsStorer *storer_nqe = new T2KWeightsStorer("nonccqe_weights.root"); //forweightstorer

 //Open the flux tuning files
 TFile fflux_run1("tuned11bv3.2/run1/nd5_tuned11bv3.2_11anom_run1.root");
 TFile fflux_run2("tuned11bv3.2/run2/nd5_tuned11bv3.2_11anom_run2.root");
 TFile fflux_run3b("tuned11bv3.2/run3b/nd5_tuned11bv3.2_11anom_run3b.root");
 TFile fflux_run3c("tuned11bv3.2/run3c/nd5_tuned11bv3.2_11anom_run3c.root");
 //Tuning histograms for two runs and four flavors 
 TH1D *tune_hists[4][4];
 tune_hists[0][0] = (TH1D*)(fflux_run1.Get("enu_nd5_tuned11b_numu_ratio"));
 tune_hists[0][1] = (TH1D*)(fflux_run1.Get("enu_nd5_tuned11b_numub_ratio"));
 tune_hists[0][2] = (TH1D*)(fflux_run1.Get("enu_nd5_tuned11b_nue_ratio"));
 tune_hists[0][3] = (TH1D*)(fflux_run1.Get("enu_nd5_tuned11b_nueb_ratio"));
 tune_hists[1][0] = (TH1D*)(fflux_run2.Get("enu_nd5_tuned11b_numu_ratio"));
 tune_hists[1][1] = (TH1D*)(fflux_run2.Get("enu_nd5_tuned11b_numub_ratio"));
 tune_hists[1][2] = (TH1D*)(fflux_run2.Get("enu_nd5_tuned11b_nue_ratio"));
 tune_hists[1][3] = (TH1D*)(fflux_run2.Get("enu_nd5_tuned11b_nueb_ratio"));
 tune_hists[2][0] = (TH1D*)(fflux_run3b.Get("enu_nd5_tuned11b_numu_ratio"));
 tune_hists[2][1] = (TH1D*)(fflux_run3b.Get("enu_nd5_tuned11b_numub_ratio"));
 tune_hists[2][2] = (TH1D*)(fflux_run3b.Get("enu_nd5_tuned11b_nue_ratio"));
 tune_hists[2][3] = (TH1D*)(fflux_run3b.Get("enu_nd5_tuned11b_nueb_ratio"));
 tune_hists[3][0] = (TH1D*)(fflux_run3c.Get("enu_nd5_tuned11b_numu_ratio"));
 tune_hists[3][1] = (TH1D*)(fflux_run3c.Get("enu_nd5_tuned11b_numub_ratio"));
 tune_hists[3][2] = (TH1D*)(fflux_run3c.Get("enu_nd5_tuned11b_nue_ratio"));
 tune_hists[3][3] = (TH1D*)(fflux_run3c.Get("enu_nd5_tuned11b_nueb_ratio"));

 //Open the files with POT and migration weights
 TFile fanalysis("./AnalysisOutputs.root");
 TVectorF *potmc_run1 = (TVectorF*)(fanalysis.Get("PoT/potmc_run1"));
 TVectorF *potdata_run1 = (TVectorF*)(fanalysis.Get("PoT/potdata_run1"));
 TVectorF *potmc_run2 = (TVectorF*)(fanalysis.Get("PoT/potmc_run2"));
 TVectorF *potdata_run2 = (TVectorF*)(fanalysis.Get("PoT/potdata_run2"));
 TVectorF *potmc_run3b = (TVectorF*)(fanalysis.Get("PoT/potmc_run3b"));
 TVectorF *potdata_run3b = (TVectorF*)(fanalysis.Get("PoT/potdata_run3b"));
 TVectorF *potmc_run3c = (TVectorF*)(fanalysis.Get("PoT/potmc_run3c"));
 TVectorF *potdata_run3c = (TVectorF*)(fanalysis.Get("PoT/potdata_run3c"));
 TVectorF *potmc_sand = (TVectorF*)(fanalysis.Get("PoT/sandmuon_mc"));
 float pot_weight[5];
 pot_weight[0] = (*potdata_run1)(0)/(*potmc_run1)(0);
 pot_weight[1] = (*potdata_run2)(0)/(*potmc_run2)(0);
 pot_weight[2] = (*potdata_run3b)(0)/(*potmc_run3b)(0);
 pot_weight[3] = (*potdata_run3c)(0)/(*potmc_run3c)(0);
 pot_weight[4] = ((*potdata_run1)(0)+(*potdata_run2)(0)+(*potdata_run3b)(0)+(*potdata_run3c)(0))/(*potmc_sand)(0);
 TMatrixD *mig_weight = (TMatrixD*)(fanalysis.Get("Migration/migration"));
 TVectorF *pile_up[4];
 pile_up[0] = (TVectorF*)(fanalysis.Get("PoT/pileup_run1"));
 pile_up[1] = (TVectorF*)(fanalysis.Get("PoT/pileup_run2"));
 pile_up[2] = (TVectorF*)(fanalysis.Get("PoT/pileup_run3b"));
 pile_up[3] = (TVectorF*)(fanalysis.Get("PoT/pileup_run3c"));

 //Set up custom summary trees
 Double_t Enu,Pmu,Xnu,Ynu,CosThetamu,FluxWeight,DetWeight,POTWeight,TruePmu,TrueCosThetamu,Q2,Q2QE,PileUpWeight; 
 Int_t NeutrinoCode,ReactionCode1,TgtMat,NuParent,Run;
 Bool_t HaveTruth;
 TFile fqe("ccqe_summary.root","RECREATE");
 TTree* ssum_qe_tree = new TTree("sample_sum","CCQE Summary" );
 ssum_qe_tree->Branch("Enu",           &Enu,            "Enu/D");
 ssum_qe_tree->Branch("Pmu",           &Pmu,            "Pmu/D");
 ssum_qe_tree->Branch("Xnu",           &Xnu,            "Xnu/D");
 ssum_qe_tree->Branch("Ynu",           &Ynu,            "Ynu/D");
 ssum_qe_tree->Branch("CosThetamu",    &CosThetamu,     "CosThetamu/D");
 ssum_qe_tree->Branch("ReactionCode",  &ReactionCode1,  "ReactionCode/I");
 ssum_qe_tree->Branch("NeutrinoCode",  &NeutrinoCode,   "NeutrinoCode/I");
 ssum_qe_tree->Branch("FluxWeight",    &FluxWeight,     "FluxWeight/D"); 
 ssum_qe_tree->Branch("DetWeight",     &DetWeight,      "DetWeight/D"); 
 ssum_qe_tree->Branch("POTWeight",     &POTWeight,      "POTWeight/D"); 
 ssum_qe_tree->Branch("PileUpWeight",  &PileUpWeight,   "PileUpWeight/D"); 
 ssum_qe_tree->Branch("TruePmu",       &TruePmu,        "TruePmu/D");
 ssum_qe_tree->Branch("TrueCosThetamu",&TrueCosThetamu, "TrueCosThetamu/D");
 ssum_qe_tree->Branch("Q2",            &Q2,             "Q2/D");
 ssum_qe_tree->Branch("Q2QE",          &Q2QE,           "Q2QE/D");
 ssum_qe_tree->Branch("HaveTruth",     &HaveTruth,      "HaveTruth/B");
 ssum_qe_tree->Branch("TgtMat",        &TgtMat,         "TgtMat/I");
 ssum_qe_tree->Branch("NuParent",      &NuParent,       "NuParent/I");
 ssum_qe_tree->Branch("Run",           &Run,            "Run/I");


 TFile fnqe("nonccqe_summary.root","RECREATE");
 TTree* ssum_nqe_tree = new TTree("sample_sum","CCnQE Summary" );
 ssum_nqe_tree->Branch("Enu",           &Enu,            "Enu/D");
 ssum_nqe_tree->Branch("Pmu",           &Pmu,            "Pmu/D");
 ssum_nqe_tree->Branch("Xnu",           &Xnu,            "Xnu/D");
 ssum_nqe_tree->Branch("Ynu",           &Ynu,            "Ynu/D");
 ssum_nqe_tree->Branch("CosThetamu",    &CosThetamu,     "CosThetamu/D");
 ssum_nqe_tree->Branch("ReactionCode",  &ReactionCode1,  "ReactionCode/I");
 ssum_nqe_tree->Branch("NeutrinoCode",  &NeutrinoCode,   "NeutrinoCode/I");
 ssum_nqe_tree->Branch("FluxWeight",    &FluxWeight,     "FluxWeight/D"); 
 ssum_nqe_tree->Branch("DetWeight",     &DetWeight,      "DetWeight/D"); 
 ssum_nqe_tree->Branch("POTWeight",     &POTWeight,      "POTWeight/D"); 
 ssum_nqe_tree->Branch("PileUpWeight",  &PileUpWeight,   "PileUpWeight/D"); 
 ssum_nqe_tree->Branch("TruePmu",       &TruePmu,        "TruePmu/D");
 ssum_nqe_tree->Branch("TrueCosThetamu",&TrueCosThetamu, "TrueCosThetamu/D");
 ssum_nqe_tree->Branch("Q2",            &Q2,             "Q2/D");
 ssum_nqe_tree->Branch("Q2QE",          &Q2QE,           "Q2QE/D");
 ssum_nqe_tree->Branch("HaveTruth",     &HaveTruth,      "HaveTruth/B");
 ssum_nqe_tree->Branch("TgtMat",        &TgtMat,         "TgtMat/I");
 ssum_nqe_tree->Branch("NuParent",      &NuParent,       "NuParent/I");
 ssum_nqe_tree->Branch("Run",           &Run,            "Run/I");

 // Load the oaAnalysis TNRooTrackerVtx tree containing
 // TClonesArray of objects inheriting from TRooTrackerBase. 
 std::vector<std::string> file_list;
 std::vector<int> run_period;
 std::vector<bool> sand_muon;
 ifstream ndfiles (fNDFileList);
 if (ndfiles.is_open()) {
   while ( ndfiles.good() ){
     std::string line;
     getline (ndfiles,line);
     if((int)line.size()<5) continue;
     file_list.push_back(line);
     if(line.find("run1") != string::npos) run_period.push_back(1);
     else if(line.find("run2") != string::npos) run_period.push_back(2);
     else if(line.find("run3b") != string::npos) run_period.push_back(3);
     else run_period.push_back(4);
     if(line.find("sandmuon") != string::npos) sand_muon.push_back(true);
     else sand_muon.push_back(false);
     std::cout << "Using file: " << file_list.back() << " Run: " << run_period.back() << " Sand muon?: " << sand_muon.back() << std::endl;
   }
   ndfiles.close();
 } 

 //Nrootracker objects
 int NVtx; 
 TClonesArray * nRooVtxs = new TClonesArray("ND::NRooTrackerVtx", 100);

 // Add NEUT reweighting engine
 t2krew::T2KReWeight rw; 
 rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
 rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());
 // Include some NEUT specific T2K systematics.


 //rw.Systematics().Include(t2krew::kNXSec_MaCCQE);
 rw.Systematics().Include(t2krew::kNXSec_1overMaCCQE2);
 rw.Systematics().Include(t2krew::kNXSec_MaRES);
 rw.Systematics().Include(t2krew::kNNucl_CCQEPauliSupViaKF);
 rw.Systematics().Include(t2krew::kNXSec_BYOnOffDIS);
 rw.Systematics().Include(t2krew::kNIWG2012a_dismpishp); 
 rw.Systematics().Include(t2krew::kNIWG2012a_sf);
 rw.Systematics().Include(t2krew::kNIWG2012a_eb);
 rw.Systematics().Include(t2krew::kNIWG2012a_pf);
 rw.Systematics().Include(t2krew::kNIWGDeltaMass_mean);
 rw.Systematics().Include(t2krew::kNIWGDeltaMass_width);
 rw.Systematics().Include(t2krew::kNNucl_PilessDcyRES);
 rw.Systematics().Include(t2krew::kNIWG2012a_mbcc1pi_enushp);

 #ifdef __T2KRW_NEUT_ENABLED__

 // Settings for NEUT
 T2KNeutReWeight * neut_rw_ptr = (T2KNeutReWeight*) rw.WghtEngine("neut_rw");

 //((NReWeightNuXSecCCQE*) neut_rw_ptr->GetWghtCalc("xsec_ccqe"))->SetMode(NReWeightNuXSecCCQE::kModeMa);
 ((NReWeightNuXSecCCQE*) neut_rw_ptr->GetWghtCalc("xsec_ccqe"))->SetMode(NReWeightNuXSecCCQE::kMode1overMa2);

 for(int dial=0; dial<=59; dial++){
 //for(int dial=0; dial<=0; dial++){
   // Set absolute tweak dial
   //rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaCCQE); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNXSec_1overMaCCQE2); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaRES); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNNucl_CCQEPauliSupViaKF); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNXSec_BYOnOffDIS); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_dismpishp); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_sf); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_eb); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_pf); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNIWGDeltaMass_mean); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNIWGDeltaMass_width); // abs
   rw.Systematics().SetAbsTwk(t2krew::kNNucl_PilessDcyRES); //abs
   rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_mbcc1pi_enushp);

   //MAQE dial values
   /*if(dial>=0&&dial<=6){
     Double_t dialvalue = 0.20*(dial-3); // value is a fractional shift, here it starts at -3*16% = -45% shift
     rw.Systematics().SetTwkDial(t2krew::kNXSec_MaCCQE, dialvalue); 
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNXSec_MaCCQE, 0.0); 
   }*/
   if(dial>=0 && dial<=6){
     Double_t dialvalue = 0.30*(dial-3); // value is a fractional shift, here it starts at
     rw.Systematics().SetTwkDial(t2krew::kNXSec_1overMaCCQE2, dialvalue); 
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNXSec_1overMaCCQE2, 0.0); 
   }

   //MARES dial values
   if(dial>=7&&dial<=13){
     Double_t dialvalue = 0.15*(dial-7-3); // value is a fractional shift, here it starts at -3*16% = -45% shift 
     rw.Systematics().SetTwkDial(t2krew::kNXSec_MaRES, dialvalue); 
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNXSec_MaRES, 0.0); 
   }

   //Kappa dial values
   if(dial>=14&&dial<=17){
     Double_t dialvalue = (dial-14)*0.007; // 1s is set to 0.01 not 0.007
     rw.Systematics().SetTwkDial(t2krew::kNNucl_CCQEPauliSupViaKF, dialvalue); 
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNNucl_CCQEPauliSupViaKF, 0.0); 
   }

   //B-Y On/Off dial values
   if(dial>=18 && dial<=19){
     Double_t dialvalue = (dial-18);
     rw.Systematics().SetTwkDial(t2krew::kNXSec_BYOnOffDIS, dialvalue);
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNXSec_BYOnOffDIS, 1.0); 
   }

   //DIS/Multipi shape dial values
   if(dial>=20 && dial<=26){
     Double_t dialvalue = (dial-23.0)*0.4;
     rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_dismpishp, dialvalue);
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_dismpishp, 0.0); 
   }

   //SF Off/On dial values
   if(dial>=27 && dial<=28){
     Double_t dialvalue = (dial-27);
     rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_sf, dialvalue);
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_sf, 0.0); 
   }

   //Binding Energy
   if(dial>=29 && dial<=31){
     Double_t dialvalue = (dial-30)*0.36;
     rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_eb, dialvalue);
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_eb, 0.0); 
   }

   //Fermi Surface momentum
   if(dial>=32 && dial<=38){
     Double_t dialvalue = (dial-35)*0.14;
     rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_pf, dialvalue);
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_pf, 0.0); 
   }

   //Delta mass 
   if(dial>=39 && dial<=45){
     Double_t dialvalue = (dial-42)*0.03;
     rw.Systematics().SetTwkDial(t2krew::kNIWGDeltaMass_mean, dialvalue);
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNIWGDeltaMass_mean, 0.0); 
   }

   //Delta width
   if(dial>=46 && dial<=52){
     Double_t dialvalue = (dial-49)*0.3;
     rw.Systematics().SetTwkDial(t2krew::kNIWGDeltaMass_width, dialvalue);
   }else{
     rw.Systematics().SetTwkDial(t2krew::kNIWGDeltaMass_width, 0.0); 
   }

   //Pionless delta decay
   if(dial>=53 &&dial<=57){
     Double_t dialvalue = (double)(dial-55)*0.2;
     rw.Systematics().SetTwkDial(t2krew::kNNucl_PilessDcyRES,dialvalue); 
   } else {
     rw.Systematics().SetTwkDial(t2krew::kNNucl_PilessDcyRES,0.0); 
   }

   //MB 1pi enu shape
   if(dial>=58 &&dial<=59){
     Double_t dialvalue = (double)(dial-58)*0.5;
     rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_mbcc1pi_enushp,dialvalue);
   } else {
     rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_mbcc1pi_enushp,0);
   }


   rw.Reconfigure();

   storer_qe->NewSystSet(rw.Systematics()); // save the current twk dial values to the weight storer class
   storer_nqe->NewSystSet(rw.Systematics()); // save the current twk dial values to the weight storer class

   for(int fiter=0; fiter<(int)file_list.size(); fiter++){
  
     TFile ND_infile(file_list[fiter].c_str(), "READ");
     ND_infile.Print();

     TTree * ND_tree = (TTree*) ND_infile.Get("TruthDir/NRooTrackerVtx");

     if(!sand_muon[fiter]){
       ND_tree->SetBranchAddress("Vtx", &nRooVtxs);
       ND_tree->SetBranchAddress("NVtx", &NVtx);
       HaveTruth = true;
     } else {
       HaveTruth = false;
     }
     
     TTree * sum_tree = (TTree*) ND_infile.Get("SummaryTree/SummaryTree");

     sum_tree->SetBranchAddress("selRunID",&selRunID);
     sum_tree->SetBranchAddress("selEventID",&selEventID);
     sum_tree->SetBranchAddress("selLevel",&selLevel);
     sum_tree->SetBranchAddress("selNeutGenie",&selNeutGenie);
     sum_tree->SetBranchAddress("selNeutTrajId",&selNeutTrajId);
     sum_tree->SetBranchAddress("selMuTrueFV",&selMuTrueFV);
     sum_tree->SetBranchAddress("selCCQE",&selCCQE);
     sum_tree->SetBranchAddress("selMuNufam",&selMuNufam);
     sum_tree->SetBranchAddress("selMuReac",&selMuReac);
     sum_tree->SetBranchAddress("selMuBunchNum",&selMuBunchNum);
     sum_tree->SetBranchAddress("selMuMom",&selMuMom);
     sum_tree->SetBranchAddress("selMuMomDirX",&selMuMomDirX);
     sum_tree->SetBranchAddress("selMuMomDirY",&selMuMomDirY);
     sum_tree->SetBranchAddress("selMuMomDirZ",&selMuMomDirZ);
     sum_tree->SetBranchAddress("selMuPosX",&selMuPosX);
     sum_tree->SetBranchAddress("selMuPosY",&selMuPosY);
     sum_tree->SetBranchAddress("selMuPosZ",&selMuPosZ);
     sum_tree->SetBranchAddress("selMuPosT",&selMuPosT);
     sum_tree->SetBranchAddress("selMuFluxWeight",&selMuFluxWeight);
     sum_tree->SetBranchAddress("trueID",&trueID);
     sum_tree->SetBranchAddress("truePDG",&truePDG);
     sum_tree->SetBranchAddress("trueParentPDG",&trueParentPDG);
     sum_tree->SetBranchAddress("trueGrandParentPDG",&trueGrandParentPDG);
     sum_tree->SetBranchAddress("trueMom",&trueMom);
     sum_tree->SetBranchAddress("trueMomDirX",&trueMomDirX);
     sum_tree->SetBranchAddress("trueMomDirY",&trueMomDirY);
     sum_tree->SetBranchAddress("trueMomDirZ",&trueMomDirZ);
     sum_tree->SetBranchAddress("truePosX",&truePosX);
     sum_tree->SetBranchAddress("truePosY",&truePosY);
     sum_tree->SetBranchAddress("truePosZ",&truePosZ);
     sum_tree->SetBranchAddress("truePrimaryID",&truePrimaryID);
     sum_tree->SetBranchAddress("truePrimaryMom",&truePrimaryMom);
     sum_tree->SetBranchAddress("truePrimaryMomDirX",&truePrimaryMomDirX);
     sum_tree->SetBranchAddress("truePrimaryMomDirY",&truePrimaryMomDirY);
     sum_tree->SetBranchAddress("truePrimaryMomDirZ",&truePrimaryMomDirZ);
     sum_tree->SetBranchAddress("truePrimaryPosX",&truePrimaryPosX);
     sum_tree->SetBranchAddress("truePrimaryPosY",&truePrimaryPosY);
     sum_tree->SetBranchAddress("truePrimaryPosZ",&truePrimaryPosZ);
     sum_tree->SetBranchAddress("selMuEnu",&selMuEnu);
     sum_tree->SetBranchAddress("selMuNuParPDG",&selMuNuParPDG);
     sum_tree->SetBranchAddress("selMuNuID",&selMuNuID);
     sum_tree->SetBranchAddress("NeutID",&NeutID);
     sum_tree->SetBranchAddress("NeutIDctr",&NeutIDctr);
     sum_tree->SetBranchAddress("PrimaryIDctr",&PrimaryIDctr);
     sum_tree->SetBranchAddress("VtxIDctr",&VtxIDctr);
     sum_tree->SetBranchAddress("ReactionCode",&ReactionCode);

     if(HaveTruth)
       fNEvts = ND_tree->GetEntries();
     else 
       fNEvts = 0;
       cout << "Will reweight ND nevents: " << fNEvts << endl;
     sNEvts = sum_tree->GetEntries(); //summary tree
       cout << "Will reweight summary tree nevents: " << sNEvts << endl;//summary tree
     if(sNEvts!=fNEvts) cout << "warning, not same number of events in summary tree and ND tree" << endl; 

     for(int i = 0; i < sNEvts; i++){
       if(!sand_muon[fiter]){
         ND_tree->LoadTree(i);
         ND_tree->GetEntry(i);
         HaveTruth = true;
       }
       sum_tree->LoadTree(i);
       sum_tree->GetEntry(i);
       if(selMuPosZ>600.) continue;
       Double_t weight = 1.0;
       Pmu = selMuMom;
       Xnu = truePrimaryPosX;
       Ynu = truePrimaryPosY;
       CosThetamu = selMuMomDirZ;
       ReactionCode1 = ReactionCode;
       NeutrinoCode = selMuNuID;
       Enu = selMuEnu;
       int flavor = 0;
       if(NeutrinoCode==-14) flavor = 1;
       else if(NeutrinoCode==12) flavor = 2;
       else if(NeutrinoCode==-12) flavor = 3;
       TgtMat = 0;

       int flux_bin = tune_hists[run_period[fiter]-1][flavor]->FindBin(Enu);
       if(HaveTruth)
         FluxWeight = tune_hists[run_period[fiter]-1][flavor]->GetBinContent(flux_bin);
       else FluxWeight = 1.0;
       POTWeight = pot_weight[(sand_muon[fiter] ? 4 : run_period[fiter]-1)];
       Run = (sand_muon[fiter] ? 0 : run_period[fiter]);
       PileUpWeight = 1.0;
       if( !(sand_muon[fiter]) ) PileUpWeight = 1.0-(*(pile_up[run_period[fiter]-1]))(0);
       int ptbin = GetPThetaBin(Pmu, CosThetamu)+(selCCQE==1?0:20);
       DetWeight = (*mig_weight)(ptbin,ptbin);

       // The passthrough information saves all neutrino vertexes in an event
       // One has to separately associate these to the reconstructed track which is not done here.
       // This loop calculates the weight for all of these vertices
       bool match = false;
       //std::cout << "Do vertex loop" << std::endl;
       for(int j = 0; j<NVtx; j++){
         if(j==selNeutGenie){
           ND::NRooTrackerVtx * vtx = (ND::NRooTrackerVtx*) nRooVtxs->At(j);
           if(!vtx){
             cerr << "Cannot find NRooTrackerVtx object - skipping weight for this vertex!";
             continue;
           }
           TVector3 nu(vtx->StdHepP4[0][0],vtx->StdHepP4[0][1],vtx->StdHepP4[0][2]);
           TVector3 mu(vtx->StdHepP4[3][0],vtx->StdHepP4[3][1],vtx->StdHepP4[3][2]);
           TruePmu = mu.Mag();
           TrueCosThetamu = (nu * mu)/(nu.Mag()*mu.Mag());
           Q2 = T2KNIWGUtils::Q2(vtx);
           Q2QE = T2KNIWGUtils::Q2QE(vtx);
           TgtMat = T2KNIWGUtils::TgtA(*vtx);
           NuParent = vtx->NuParentPdg;
           match = true;
	   weight = rw.CalcWeight(vtx);
           if(weight>10.0) std::cout << "weight: " << weight << " Mode " << ReactionCode << std::endl;
         } // if correct vertex from summary tree
       } // vertex container
       if(!match && !(sand_muon[fiter]) ){
           std::cout << "Could not match NRooTrackerVtx" << std::endl;
           HaveTruth = false;
       } 

      // std::cout << "Fill weight storer" << std::endl;
       //Store weights sample
       if(selCCQE==1) storer_qe->AddWeight(weight); // add weight for each    
       else storer_nqe->AddWeight(weight); // add weight for each    

       //Fill summary tree
       if(selCCQE==1 && dial<=0) ssum_qe_tree->Fill();
       else if(dial<=0) ssum_nqe_tree->Fill();

     } // numner of events
     ND_infile.Close("R"); 
     ND_tree=NULL;
     sum_tree=NULL;
   } // file loop
 } //dial Loop
 

#endif
 fqe.cd();
 ssum_qe_tree->Write("sample_sum");
 fqe.Close();
 fnqe.cd();
 ssum_nqe_tree->Write("sample_sum");
 fnqe.Close();

 storer_qe->SaveToFile(); // save the weights to a file
 storer_nqe->SaveToFile(); // save the weights to a file
 delete storer_qe; 
 delete storer_nqe; 

#endif // __T2KRW_OAANALYSIS_ENABLED__
 return 0;
}

// Print the cmd line syntax
void Usage(){
  cout << "Cmd line syntax should be:" << endl;
  cout << "genWeightsFromJReWeight.exe -n nd_inputfile -s sk_inputfile [-e nevents]" << endl;
}

// Messy way to process cmd line arguments.
void ParseArgs(int argc, char **argv){
  int nargs = 1; 
  if(argc<(nargs*2+1)){ Usage(); exit(1); }
  for(int i = 1; i < argc; i+=2){
    if(string(argv[i]) == "-n") fNDFileList = argv[i+1];
    else if(string(argv[i]) == "-s") fSKFileName = argv[i+1];
    else if(string(argv[i]) == "-e") fNEvts = std::atoi(argv[i+1]);
    else {  
        cout << "Invalid argument:" << argv[i] << " "<< argv[i+1] << endl;
        Usage();
        exit(1);
    }
  } 
}

int GetPThetaBin(double p, double th){
  int pbin, thbin;

  if(p<400.) pbin=0;
  else if(p<500.) pbin=1;
  else if(p<700.) pbin=2;
  else if(p<900.) pbin=3;
  else pbin=4;

  if(th<0.84) thbin=0;
  else if(th<0.9) thbin=1;
  else if(th<0.94) thbin=2;
  else thbin=3;

  return pbin*4+thbin;

}
