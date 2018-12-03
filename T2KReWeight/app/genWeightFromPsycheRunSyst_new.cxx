#define GENWEIGHTFROMPSYCHERUNSYST_NEW_CXX

#include <stdlib.h>
#include <cstdlib>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TGraph2DErrors.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"

#include "T2KGenieReWeight.h"
#include "T2KNeutReWeight.h"
#include "T2KJNuBeamReWeight.h"
#include "T2KNIWGReWeight.h"
#include "T2KNIWGUtils.h"

#include "SampleId.hxx"

using namespace t2krew;

std::string FileInStr;
std::string FileOutStr;
bool IndividualThrow;
bool Sand;
int nEvents;

// Print the cmd line syntax
void Usage(){
  std::cout << "USAGE: genWeightFromPsycheRunSyst_new.exe [OPTIONS] "               << std::endl;
  std::cout << "OPTIONS: MANADATORY"                                                << std::endl;
  std::cout << "         -i INPUT_FILE_NAME: RunSystBinCorr file name"              << std::endl;
  std::cout << "         -o OUTPUT_FILE_NAME: Output file name"                     << std::endl;
  std::cout << "OPTIONS: OPTIONAL"                                                  << std::endl;
  std::cout << "         -n N_EVENTS: Limit number of processed events to N_EVENTS" << std::endl;
  std::cout << "         -s: Toggles running with Sand MC" << std::endl;
}

double calcRPA(double Q2, double A, double B, double D, double E, double U) {
  // Callum's fitted nominals
  // A = 0.59 +/- 20%
  // B = 1.05 +/- 20%
  // D = 1.13 +/- 15%
  // E = 0.88 +/- 40%
  // U = 1.2

  // Kept for convenience
  double eRPA = 1.;

  // Q2 transition; less than U -> polynominal
  if (Q2 < U) {
    // xprime as prescribed by Callum
    const double xprime = Q2/U;
    const double C      = D + U*E*(D-1)/3.;
    eRPA          = A*(1-xprime)*(1-xprime)*(1-xprime) + 3*B*(1-xprime)*(1-xprime)*xprime + 3*C*(1-xprime)*xprime*xprime + D*xprime*xprime*xprime;
  } else {
    eRPA = 1 + (D-1)*exp(-E*(Q2-U));
  }
  return eRPA;
}

double calcRPA(ND::NRooTrackerVtx *vtx=NULL) {
  if(vtx != NULL){
    if(fabs(atoi(((vtx->EvtCode)->String()).Data() )) != 1) return 1.;
    if(vtx->StdHepN < 3) return 1;
    double Q2=T2KNIWGUtils::Q2(vtx);
    return calcRPA(Q2, 0.59, 1.05, 1.13, 0.88, 1.2);
  }else{
    return 1;
  }
}

void ReweightTree(){

  std::vector< t2krew::ET2KSyst > dials;
  t2krew::T2KReWeight rw;
  rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
  rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());

  // Uncertainties
  // CCQE:
  std::cout << "Setting the NIWG dials" << std::endl;
  rw.Systematics().Include(t2krew::kNIWG2014a_pF_C12);
  rw.Systematics().Include(t2krew::kNIWG2014a_pF_O16);
  rw.Systematics().Include(t2krew::kNIWG2014a_Eb_C12);
  rw.Systematics().Include(t2krew::kNIWG2014a_Eb_O16);
  rw.Systematics().Include(t2krew::kNIWGMEC_Norm_C12);
  rw.Systematics().Include(t2krew::kNIWGMEC_Norm_O16);
  rw.Systematics().Include(t2krew::kNIWGMEC_q3Cut);
  rw.Systematics().Include(t2krew::kNXSec_MaCCQE);
  rw.Systematics().Include(t2krew::kNXSec_VecFFCCQE); // used to set MAQE to act according to for RFG MC (2) or SF MC (402)

  // CC and NC single pion resonance:
  rw.Systematics().Include(t2krew::kNXSec_CA5RES);
  rw.Systematics().Include(t2krew::kNXSec_MaNFFRES);
  rw.Systematics().Include(t2krew::kNXSec_BgSclRES);

  // nue/numu uncertainties
  rw.Systematics().Include(t2krew::kNXSec_SCCVecQE);
  rw.Systematics().Include(t2krew::kNXSec_SCCAxlQE);
  rw.Systematics().Include(t2krew::kNIWG2012a_ccnueE0);

  // All other CC and NC
  rw.Systematics().Include(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().Include(t2krew::kNIWG2012a_cccohE0);
  rw.Systematics().Include(t2krew::kNIWG2012a_nccohE0);
  rw.Systematics().Include(t2krew::kNIWG2012a_ncotherE0);

  rw.Systematics().Include(t2krew::kNIWG2014a_SF_RFG);
  rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_shape);

  // Add FSI dials
  rw.Systematics().Include(t2krew::kNCasc_FrAbs_pi);
  rw.Systematics().Include(t2krew::kNCasc_FrCExLow_pi);
  rw.Systematics().Include(t2krew::kNCasc_FrInelLow_pi);
  rw.Systematics().Include(t2krew::kNCasc_FrPiProd_pi);
  rw.Systematics().Include(t2krew::kNCasc_FrCExHigh_pi);
  rw.Systematics().Include(t2krew::kNCasc_FrInelHigh_pi);

  //-- PDD Weights, New Eb dial
  rw.Systematics().Include(t2krew::kNIWGMEC_PDDWeight_C12);
  rw.Systematics().Include(t2krew::kNIWGMEC_PDDWeight_O16);
  rw.Systematics().Include(t2krew::kNNucl_CCQEBindingEnergy_C12);
  rw.Systematics().Include(t2krew::kNNucl_CCQEBindingEnergy_O16);

  dials.push_back(t2krew::kNIWG2014a_pF_C12);
  dials.push_back(t2krew::kNIWG2014a_pF_O16);
  dials.push_back(t2krew::kNIWG2014a_Eb_C12);
  dials.push_back(t2krew::kNIWG2014a_Eb_O16);
  dials.push_back(t2krew::kNIWGMEC_Norm_C12);
  dials.push_back(t2krew::kNIWGMEC_Norm_O16);
  dials.push_back(t2krew::kNIWGMEC_q3Cut);
  dials.push_back(t2krew::kNXSec_MaCCQE);
  dials.push_back(t2krew::kNXSec_VecFFCCQE); // used to set MAQE to act according to for RFG MC (2) or SF MC (402)
  dials.push_back(t2krew::kNXSec_CA5RES);
  dials.push_back(t2krew::kNXSec_MaNFFRES);
  dials.push_back(t2krew::kNXSec_BgSclRES);
  dials.push_back(t2krew::kNXSec_SCCVecQE);
  dials.push_back(t2krew::kNXSec_SCCAxlQE);
  dials.push_back(t2krew::kNIWG2012a_ccnueE0);
  dials.push_back(t2krew::kNIWG2012a_dismpishp);
  dials.push_back(t2krew::kNIWG2012a_cccohE0);
  dials.push_back(t2krew::kNIWG2012a_nccohE0);
  dials.push_back(t2krew::kNIWG2012a_ncotherE0);
  dials.push_back(t2krew::kNIWG2014a_SF_RFG);
  dials.push_back(t2krew::kNIWG_rpaCCQE_norm);
  dials.push_back(t2krew::kNIWG_rpaCCQE_shape);
  dials.push_back(t2krew::kNCasc_FrAbs_pi);
  dials.push_back(t2krew::kNCasc_FrCExLow_pi);
  dials.push_back(t2krew::kNCasc_FrInelLow_pi);
  dials.push_back(t2krew::kNCasc_FrPiProd_pi);
  dials.push_back(t2krew::kNCasc_FrCExHigh_pi);
  dials.push_back(t2krew::kNCasc_FrInelHigh_pi);
  dials.push_back(t2krew::kNIWGMEC_PDDWeight_C12);
  dials.push_back(t2krew::kNIWGMEC_PDDWeight_O16);
  dials.push_back(t2krew::kNNucl_CCQEBindingEnergy_C12);
  dials.push_back(t2krew::kNNucl_CCQEBindingEnergy_O16);

  // CCQE:
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_Norm_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_Norm_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_q3Cut);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaCCQE);

  // CC and NC single pion resonance:
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_CA5RES);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaNFFRES);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_BgSclRES);

  // nue/numu uncertainties
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_SCCVecQE);
  rw.Systematics().SetAbsTwk(t2krew::kNXSec_SCCAxlQE);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_ccnueE0);

  // All other CC and NC
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_dismpishp);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_cccohE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_nccohE0);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_ncotherE0);

  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_norm);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_shape);

  // FSI dials
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrAbs_pi);
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrCExLow_pi);
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrInelLow_pi);
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrPiProd_pi);
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrCExHigh_pi);
  rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrInelHigh_pi);

  //-- PDD Weights, New Eb dial
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_PDDWeight_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_PDDWeight_O16);
  rw.Systematics().SetAbsTwk(t2krew::kNNucl_CCQEBindingEnergy_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNNucl_CCQEBindingEnergy_O16);


  // Initialise dials to nominal
  for(unsigned int sys_iter1=0; sys_iter1<dials.size(); sys_iter1++){
    rw.Systematics().SetTwkDial(dials[sys_iter1], 0.);
  }
  // Apply nominal tuning
  rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,0); // SF->RFG tuning
  rw.Systematics().SetTwkDial(t2krew::kNXSec_VecFFCCQE, 2); // Note that when we set MAQE, we need to set NXSec_VecFFCCQE to 2 for SF->RFG MC
  rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm, 1);
  rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);

  std::cout << "Setting the NIWG dials -done-" << std::endl;
  rw.Reconfigure();
  std::cout << "Reconfigured NIWG dials -done-" << std::endl;


  TFile *FileIn = new TFile(FileInStr.c_str(), "READ");
  TTree* RTV = NULL;
  TClonesArray *nRooVtxs = new TClonesArray("ND::NRooTrackerVtx",100);
  int NRooVtx = 0;
  if(!Sand){
    RTV = (TTree*)FileIn->Get("NRooTrackerVtx");
    RTV->SetBranchAddress("Vtx",  &nRooVtxs);
    RTV->SetBranchAddress("NVtx", &NRooVtx);
  }
  TTree* all_syst = (TTree*)FileIn->Get("all");
  int    Run            ;
  int    SubRun         ;
  int    EventNumber    ;
  int    SelectionNom   ;
  double TrueEnuNom     ;
  int    TrueNuPDGNom   ;
  int    TrueVertexIDNom;
  double LeptonMomNom   ;
  double LeptonCosNom   ;
  double WeightNom      ;
  double FluxWeightNom  ;
  int    TrueVertexIDToy[2000];
  int    SelectionToy   [2000];
  double TrueEnuToy     [2000];
  int    TrueNuPDGToy   [2000];
  double LeptonMomToy   [2000];
  double LeptonCosToy   [2000];
  double WeightToy      [2000];
  double FluxWeightToy  [2000];

  all_syst->SetBranchAddress("Run"            , &Run            );
  all_syst->SetBranchAddress("SubRun"         , &SubRun         );
  all_syst->SetBranchAddress("EventNumber"    , &EventNumber    );
  all_syst->SetBranchAddress("SelectionNom"   , &SelectionNom   );
  all_syst->SetBranchAddress("TrueEnuNom"     , &TrueEnuNom     );
  all_syst->SetBranchAddress("TrueNuPDGNom"   , &TrueNuPDGNom   );
  all_syst->SetBranchAddress("TrueVertexIDNom", &TrueVertexIDNom);
  all_syst->SetBranchAddress("LeptonMomNom"   , &LeptonMomNom   );
  all_syst->SetBranchAddress("LeptonCosNom"   , &LeptonCosNom   );
  all_syst->SetBranchAddress("WeightNom"      , &WeightNom      );
  all_syst->SetBranchAddress("FluxWeightNom"  , &FluxWeightNom  );
  all_syst->SetBranchAddress("TrueVertexIDToy",  TrueVertexIDToy);
  all_syst->SetBranchAddress("SelectionToy"   ,  SelectionToy   );
  all_syst->SetBranchAddress("TrueEnuToy"     ,  TrueEnuToy     );
  all_syst->SetBranchAddress("TrueNuPDGToy"   ,  TrueNuPDGToy   );
  all_syst->SetBranchAddress("LeptonMomToy"   ,  LeptonMomToy   );
  all_syst->SetBranchAddress("LeptonCosToy"   ,  LeptonCosToy   );
  all_syst->SetBranchAddress("WeightToy"      ,  WeightToy      );
  all_syst->SetBranchAddress("FluxWeightToy"  ,  FluxWeightToy  );

  int prevRTV, rtvi=0, prevTruthID;
  double prevNIWGWeight;
  TFile* FileOut = new TFile(FileOutStr.c_str(), "RECREATE");
  TTree* NIWGWeightTree = new TTree("NIWGWeightTree", "NIWGWeightTree");
  double NIWGWeightNom = 1;
  double EnuNom        = 1;
  double LeptonMom1p1hFDS = 1;
  double NIWGWeightToy[2000];
  double EnuToy[2000];
  int nToy=2000;
  int Toy[2000];

  const char* T2KREWEIGHT = getenv("T2KREWEIGHT");
  TFile* FileFDS_1p1h_carbon = TFile::Open(TString::Format("%s/app/numu_C12_MA1030_MDLQE0002_theta.root", T2KREWEIGHT));
  TFile* FileFDS_1p1h_oxygen = TFile::Open(TString::Format("%s/app/numu_O16_MA1030_MDLQE0002_theta.root", T2KREWEIGHT));
  TGraph2DErrors* FDS_1p1h_carbon = (TGraph2DErrors*)FileFDS_1p1h_carbon->Get("nieves_minus_neut_graph");
  TGraph2DErrors* FDS_1p1h_oxygen = (TGraph2DErrors*)FileFDS_1p1h_oxygen->Get("nieves_minus_neut_graph");

  NIWGWeightTree->Branch("T2KRW_NIWGWeightNom"   , &NIWGWeightNom   , "T2KRW_NIWGWeightNom/D");
  NIWGWeightTree->Branch("T2KRW_EnuNom"          , &EnuNom          , "T2KRW_EnuNom/D");
  NIWGWeightTree->Branch("T2KRW_LeptonMom1p1hFDS", &LeptonMom1p1hFDS, "T2KRW_LeptonMom1p1hFDS/D");
  NIWGWeightTree->Branch("T2KRW_nToys"           , &nToy            , "T2KRW_nToys/I");
  NIWGWeightTree->Branch("T2KRW_Toy"             ,  Toy             , "T2KRW_Toy[T2KRW_nToys]/I");
  NIWGWeightTree->Branch("T2KRW_NIWGWeightToy"   ,  NIWGWeightToy   , "T2KRW_NIWGWeightToy[T2KRW_nToys]/D");
  NIWGWeightTree->Branch("T2KRW_EnuToy"          ,  EnuToy          , "T2KRW_EnuToy[T2KRW_nToys]/D");

  const Long64_t nEntries = all_syst->GetEntries();
  for(Long64_t iEntry = 0; iEntry < nEntries; ++iEntry){
    NIWGWeightNom = 1;
    EnuNom = -999;
    if(iEntry%100==0) std::cout << "Progress " << (double)iEntry * 100./ nEntries << "%" << std::endl;
    for(int iToy = 0; iToy < 2000; ++iToy){
      Toy[iToy]           = -999;
      NIWGWeightToy[iToy] = 1;
      EnuToy       [iToy] = -999;
    }

    all_syst->GetEntry(iEntry);

    if(Sand){
      NIWGWeightTree->Fill();
      continue;
    }


    if(SelectionNom > SampleId::kUnassigned){
      bool foundvtx = false;
      ND::NRooTrackerVtx * vtx = NULL;
      if(TrueVertexIDNom != -999){
        prevTruthID = TrueVertexIDNom;
        prevRTV = rtvi;
        if(!foundvtx){
          while(vtx == NULL){
            //Pull out the correct RooTrackerVtx tree entry.
            RTV->GetEntry(rtvi);
            for(int i = 0; i < NRooVtx; ++i){
              vtx = (ND::NRooTrackerVtx*)(*nRooVtxs)[i];
              if(vtx->TruthVertexID == TrueVertexIDNom &&
                 fabs(vtx->StdHepP4[0][3]*1000 - TrueEnuNom) < 0.1){
                foundvtx = true;
                break;
              }
              vtx = NULL;
            }
            if(vtx==NULL) rtvi++;
            if(rtvi == RTV->GetEntries()){
              std::cout << "Looping to find correct vertex Nominal. VtxID: " << TrueVertexIDNom << std::endl;
              rtvi = 0;
              //break;
            }
            if(rtvi == prevRTV - 1){
              std::cout << "Event failed to find ANY vertex Nominal. VtxID: " << TrueVertexIDNom << std::endl;
              break;
            }
          }
        }else{
          //may not work, depending on how RTV->GetEntry(rvti) works
          for(int iPart = 0; iPart > NRooVtx; ++iPart){
            vtx = (ND::NRooTrackerVtx*)(*nRooVtxs)[iPart];
            if(vtx->TruthVertexID == TrueVertexIDNom &&
               fabs(vtx->StdHepP4[0][3]*1000 - TrueEnuNom) < 0.1){
              break;
            }
          }
        }
        if(vtx != NULL){
          NIWGWeightNom = rw.CalcWeight(vtx);
          if(NIWGWeightNom>100 || NIWGWeightNom<0){
             std::cout << "NIWGWeightNom = " << NIWGWeightNom << " setting it to 1." << std::endl;
            NIWGWeightNom=1;
          }
          EnuNom        = vtx->StdHepP4[0][3]*1000;
          double MomDiff = 0;
          if(vtx->StdHepN > 2){
            double mom = TMath::Sqrt(vtx->StdHepP4[3][0]*vtx->StdHepP4[3][0]+
                                     vtx->StdHepP4[3][1]*vtx->StdHepP4[3][1]+
                                     vtx->StdHepP4[3][2]*vtx->StdHepP4[3][2]);
            double angle = (TMath::ACos(vtx->StdHepP4[3][2] / mom) * 180.0 / 3.14159265);
            if      (vtx->StdHepPdg[1] == 1000060120 && TMath::Abs(vtx->StdHepPdg[3]) == 13){
              MomDiff = FDS_1p1h_carbon->Interpolate(EnuNom,angle);
            }else if(vtx->StdHepPdg[1] == 1000080160 && TMath::Abs(vtx->StdHepPdg[3]) == 13){
              MomDiff = FDS_1p1h_oxygen->Interpolate(EnuNom,angle);
            }
          }
          LeptonMom1p1hFDS = LeptonMomNom + MomDiff;
          //Add Kendall's pion tuning stuff
          double piEnergy = -1.0;
          if(fabs(atoi(((vtx->EvtCode)->String()).Data() )) == 16){
            for(int iPart = 0; iPart < vtx->StdHepN; ++iPart){
              if(fabs(vtx->StdHepPdg[iPart]) != 211) continue;
              piEnergy = vtx->StdHepP4[iPart][3];
            }
            if     (piEnergy < 0   ) { NIWGWeightNom = 1     * NIWGWeightNom; }
            else if(piEnergy < 0.25) { NIWGWeightNom = 0.135 * NIWGWeightNom; }
            else if(piEnergy < 0.5 ) { NIWGWeightNom = 0.4   * NIWGWeightNom; }
            else if(piEnergy < 0.75) { NIWGWeightNom = 0.294 * NIWGWeightNom; }
            else if(piEnergy < 1.0 ) { NIWGWeightNom = 1.206 * NIWGWeightNom; }
          }
          NIWGWeightNom = NIWGWeightNom * calcRPA(vtx);
        }else{
          std::cout << "Didnt find vertex for this event nom" << std::endl;
        }
        prevNIWGWeight = NIWGWeightNom;
      }else{
        std::cout << "No vertex id saved for this event nom" << std::endl;
        NIWGWeightNom   =1;
        LeptonMom1p1hFDS=LeptonMomNom;
      }

      if(!IndividualThrow){
        NIWGWeightTree->Fill();
        continue;
      }
    }
    for(int iToy=0; iToy < 2000; ++iToy){
      Toy[iToy] = iToy;
      EnuToy[iToy] = -999;
      NIWGWeightToy[iToy] = 1;

      if(SelectionToy[iToy] > SampleId::kUnassigned){
        bool foundvtx = false;
        ND::NRooTrackerVtx * vtx = NULL;
        if(TrueVertexIDToy[iToy] != -999){
          prevTruthID = TrueVertexIDToy[iToy];
          prevRTV = rtvi;
          if(!foundvtx){
            while(vtx == NULL){
              //Pull out the correct RooTrackerVtx tree entry.
              RTV->GetEntry(rtvi);
              for(int i = 0; i < NRooVtx; ++i){
                vtx = (ND::NRooTrackerVtx*)(*nRooVtxs)[i];
                if(vtx->TruthVertexID == TrueVertexIDToy[iToy] &&
                   fabs(vtx->StdHepP4[0][3]*1000 - TrueEnuToy[iToy]) < 0.1){
                  foundvtx = true;
                  break;
                }
                vtx = NULL;
              }
              if(vtx==NULL) rtvi++;
              if(rtvi == RTV->GetEntries()){
                std::cout << "Looping to find correct vertex Toy" << iToy << ". VtxID: " << TrueVertexIDToy[iToy] << std::endl;
                rtvi = 0;
                //break;
              }
              if(rtvi == prevRTV - 1){
                std::cout << "Event failed to find ANY vertex Toy" << iToy << ". VtxID: " << TrueVertexIDToy[iToy] << std::endl;
                break;
              }
            }
          }else{
            //may not work, depending on how RTV->GetEntry(rvti) works
            for(int i = 0; i < NRooVtx; ++i){
              vtx = (ND::NRooTrackerVtx*)(*nRooVtxs)[i];
              if(vtx->TruthVertexID == TrueVertexIDToy[iToy] &&
                 fabs(vtx->StdHepP4[0][3]*1000 - TrueEnuToy[iToy]) < 0.1){
                break;
              }
            }
          }
          if(vtx != NULL){
            NIWGWeightToy[iToy] = rw.CalcWeight(vtx);
            if(NIWGWeightToy[iToy]>100 || NIWGWeightToy[iToy] < 0){
              std::cout << "NIWGWeightToy[" << iToy << "] = " << NIWGWeightToy[iToy] << " setting it to 1." << std::endl;
              NIWGWeightToy[iToy]=1;
            }
            EnuToy       [iToy] = vtx->StdHepP4[0][3]*1000;

            //Add Kendall's pion tuning stuff
            double piEnergy = -1.0;
            if(fabs(atoi(((vtx->EvtCode)->String()).Data() )) == 16){
              for(int iPart = 0; iPart < vtx->StdHepN; ++iPart){
                if(fabs(vtx->StdHepPdg[iPart]) != 211) continue;
                piEnergy = vtx->StdHepP4[iPart][3];
              }
            }
            if     (piEnergy < 0   ) { NIWGWeightToy[iToy] = 1     * NIWGWeightToy[iToy]; }
            else if(piEnergy < 0.25) { NIWGWeightToy[iToy] = 0.135 * NIWGWeightToy[iToy]; }
            else if(piEnergy < 0.5 ) { NIWGWeightToy[iToy] = 0.4   * NIWGWeightToy[iToy]; }
            else if(piEnergy < 0.75) { NIWGWeightToy[iToy] = 0.294 * NIWGWeightToy[iToy]; }
            else if(piEnergy < 1.0 ) { NIWGWeightToy[iToy] = 1.206 * NIWGWeightToy[iToy]; }
            NIWGWeightToy[iToy] = NIWGWeightToy[iToy] * calcRPA(vtx);
          }else{
            std::cout << "Didnt find vertex for this toy event" << std::endl;
          }
          prevNIWGWeight = NIWGWeightToy[iToy];
        }else{
          std::cout << "No vertex id saved for this event toy" << iToy << std::endl;
          NIWGWeightToy[iToy]   =1;
        }
      }
    }
    NIWGWeightTree->Fill();
  }
  FileOut->cd();
  NIWGWeightTree->Write();
  FileOut->Close();

}


int ParseArgs(int argc, char **argv){

  IndividualThrow = true;
  Sand = false;
  for (;;) {
    int c = getopt(argc, argv, "sn:i:o:");
    if (c < 0)
      break;
    switch (c){
    case 'n':{
      std::istringstream tmp(optarg);
      tmp >> nEvents;
      std::cout << "Looping over " << nEvents << " events." << std::endl;
      break;
    }
    case 'i':{
      FileInStr = optarg;
      break;
    }
    case 'o':{
      FileOutStr = optarg;
      break;
    }
    case 's':{
      Sand=true;
      break;
    }
    default:{
      std::cerr << "Option " << c << " is not recognised." << std::endl;
      Usage();
      throw;
      break;
    }
    }
  }
  return 0;
}


int main(int argc, char *argv[]){
  ParseArgs(argc,argv);
  ReweightTree();
  return 1;
}

