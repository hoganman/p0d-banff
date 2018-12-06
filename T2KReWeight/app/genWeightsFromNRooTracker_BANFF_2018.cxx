#include <stdlib.h>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TObjString.h"
#include <TLorentzVector.h>
#include "TGraph.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"

#include "T2KNeutReWeight.h"
#include "T2KNeutUtils.h"

#include "T2KNIWGReWeight.h"
#include "T2KNIWGUtils.h"

#include "ToyMakerExample.hxx"
#include "SampleId.hxx"
#include <Parameters.hxx>
#include <AnalysisUtils.hxx>
#include "AnalysisManager.hxx"

#include "TROOT.h"

using namespace t2krew;

int fNEvts = -1;
bool fMirror = true;
int fToys = 1;
int fId = 0;
char * fInputFile;
char * fOutputFile;
char * fRunPeriod;
bool data = 0;

const Double_t MAXWEIGHT = 100;
const Bool_t RESTRICTWEIGHT = kTRUE;

void Usage();
void ParseArgs(int argc, char **argv);
int GetPThetaBin(double p, double th);
Bool_t ValidWeightRange(const Double_t &weight);

int main(int argc, char *argv[]){

  // P.S (26.01.17) AxialFF Patch

  //Reads the input file, Run Period, and output file name from the command
  //line.
  ParseArgs(argc, argv);

  //Set up objects to store the cross section variations
  double ma_spacing[7] = {-3, -2, -1, 0, 1, 2, 3};
  double pdd_spacing[10] = {-2.0, -1.0, -0.1, 0.0, 0.1, 1.0, 2.0, 1.0, 0.1, 0.0};
  double pfc_spacing[10] = {0.0, -1.0, -2.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0};
  double pfo_spacing[10] = {0.0, -1.0, -2.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0};

  std::vector< std::vector<float> > dial_values;
  std::vector< t2krew::ET2KSyst > dials;
  std::vector< std::string > dial_names;
  std::vector< TClonesArray* > dial_graphs;

  //MAQE dial values;
  std::vector<float> maqe_vec;
  for(int i=0; i<7; i++) maqe_vec.push_back(ma_spacing[i]*(0.3/1.2));
  dial_values.push_back(maqe_vec);
  dials.push_back(t2krew::kNXSec_MaCCQE);
  dial_names.push_back("MAQE");
  TClonesArray *maqe_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(maqe_graph);

  //Q3CUT dial values;
  std::vector<float> q3cut_vec;
  //for(int i=0; i<5; i++) q3cut_vec.push_back( (((double)i - 2.)/2.) * (0.25/1) );
  for(int i=0; i<2; i++) q3cut_vec.push_back((i - 1)*(0.25/1));
  dial_values.push_back(q3cut_vec);
  dials.push_back(t2krew::kNIWGMEC_q3Cut);
  dial_names.push_back("Q3CUT");
  TClonesArray *q3cut_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(q3cut_graph);

  //MEC_C dial values;
  std::vector<float> mec_c;
  for(int i=0; i<2; i++) mec_c.push_back((i - 1)*(1.0/1.0));
  dial_values.push_back(mec_c);
  dials.push_back(t2krew::kNIWGMEC_Norm_C12);
  dial_names.push_back("MEC_C");
  TClonesArray *mecc_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(mecc_graph);

  //PF_C dial values;
  std::vector<float> pf_c;
  if( fMirror ){
    for(int i=0; i<10; i++) pf_c.push_back(pfc_spacing[i]*(66.7/(6.*217.)) + (475./(2.*217.) - 1.));
  }
  else{
    for(int i=3; i<10; i++) pf_c.push_back(pfc_spacing[i]*(66.7/(6.*217.)) + (475./(2.*217.) - 1.));
  }
  dial_values.push_back(pf_c);
  dials.push_back(t2krew::kNIWG2014a_pF_C12);
  dial_names.push_back("PF_C");
  TClonesArray *pfc_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(pfc_graph);

  //    //EB_C dial values;
  //    std::vector<float> eb_c;
  //    for(int i=0; i<5; i++) eb_c.push_back((i - 2)*(7.5/25.) + (27./25. - 1.));
  //    dial_values.push_back(eb_c);
  //    dials.push_back(t2krew::kNNucl_CCQEBindingEnergy_C12);  //New dial
  ////    dials.push_back(t2krew::kNIWG2014a_Eb_C12);  //Old dial
  //    dial_names.push_back("EB_C");
  //    TClonesArray *ebc_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  //    dial_graphs.push_back(ebc_graph);

  //MEC_O dial values;
  std::vector<float> mec_o;
  for(int i=0; i<2; i++) mec_o.push_back((i - 1)*(1.0/1.0));
  dial_values.push_back(mec_o);
  dials.push_back(t2krew::kNIWGMEC_Norm_O16);
  dial_names.push_back("MEC_O");
  TClonesArray *meco_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(meco_graph);

  //PF_O dial values;
  std::vector<float> pf_o;
  if( fMirror ){
      for(int i=0; i<10; i++) pf_o.push_back(pfo_spacing[i]*(66.7/(6.*225.)) + (475./(2.*225.) - 1.));
  }
  else{
      for(int i=3; i<10; i++) pf_o.push_back(pfo_spacing[i]*(66.7/(6.*225.)) + (475./(2.*225.) - 1.));
  }
  dial_values.push_back(pf_o);
  dials.push_back(t2krew::kNIWG2014a_pF_O16);
  dial_names.push_back("PF_O");
  TClonesArray *pfo_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(pfo_graph);

  //    //EB_O dial values;
  //    std::vector<float> eb_o;
  //    for(int i=0; i<5; i++) eb_o.push_back((i - 2)*(7.5/27.) + (27./27. - 1.));
  //    dial_values.push_back(eb_o);
  //    dials.push_back(t2krew::kNNucl_CCQEBindingEnergy_O16);  //New dial
  ////    dials.push_back(t2krew::kNIWG2014a_Eb_O16);  //Old dial
  //    dial_names.push_back("EB_O");
  //    TClonesArray *ebo_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  //    dial_graphs.push_back(ebo_graph);

  //CA5 dial values;
  std::vector<float> ca5;
  for(int i=0; i<7; i++) ca5.push_back((i - 3)*(0.12/1.01));
  dial_values.push_back(ca5);
  dials.push_back(t2krew::kNXSec_CA5RES);
  dial_names.push_back("CA5");
  TClonesArray *ca5_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(ca5_graph);

  //MARES dial values;
  std::vector<float> mares;
  for(int i=0; i<7; i++) mares.push_back((i - 3)*(0.15/0.95));
  dial_values.push_back(mares);
  dials.push_back(t2krew::kNXSec_MaNFFRES);
  dial_names.push_back("MARES");
  TClonesArray *mares_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(mares_graph);

  //BgSclRes dial values;
  std::vector<float> bgsclres;
  for(int i=0; i<7; i++) bgsclres.push_back((i - 3)*(0.2/1.3));
  dial_values.push_back(bgsclres);
  dials.push_back(t2krew::kNXSec_BgSclRES);
  dial_names.push_back("BgSclRes");
  TClonesArray *bgsclres_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(bgsclres_graph);

  //CCNuE dial values;
  std::vector<float> ccnue;
  for(int i=0; i<7; i++) ccnue.push_back((i - 3)*(0.02/1.0));
  dial_values.push_back(ccnue);
  dials.push_back(t2krew::kNIWG2012a_ccnueE0);
  dial_names.push_back("CCNuE");
  TClonesArray *ccnue_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(ccnue_graph);

  //DISMPiShp dial values;
  std::vector<float> dismpishp;
  for(int i=0; i<7; i++) dismpishp.push_back((i - 3)*(0.4/1.0));
  dial_values.push_back(dismpishp);
  dials.push_back(t2krew::kNIWG2012a_dismpishp);
  dial_names.push_back("DISMPiShp");
  TClonesArray *dismpishp_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(dismpishp_graph);

  //Pion Absorption
  std::vector<float> piabs_vec;
  for(int i=0; i<7; i++) piabs_vec.push_back(0.267*((double)i-3.0));
  dial_values.push_back(piabs_vec);
  dials.push_back(t2krew::kNCasc_FrAbs_pi);
  dial_names.push_back("FSI_PI_ABS");
  TClonesArray *piabs_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(piabs_graph);

  //Charge exchange low energy
  std::vector<float> cexlow_vec;
  for(int i=0; i<7; i++) cexlow_vec.push_back(0.267*((double)i-3.0));
  dial_values.push_back(cexlow_vec);
  dials.push_back(t2krew::kNCasc_FrCExLow_pi);
  dial_names.push_back("FSI_CEX_LO");
  TClonesArray *cexlow_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(cexlow_graph);

  //Inelastic low energy
  std::vector<float> inellow_vec;
  for(int i=0; i<7; i++) inellow_vec.push_back(0.267*((double)i-3.0));
  dial_values.push_back(inellow_vec);
  dials.push_back(t2krew::kNCasc_FrInelLow_pi);
  dial_names.push_back("FSI_INEL_LO");
  TClonesArray *inellow_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(inellow_graph);

  //Pion prod
  std::vector<float> piprod_vec;
  for(int i=0; i<7; i++) piprod_vec.push_back(0.267*((double)i-3.0));
  dial_values.push_back(piprod_vec);
  dials.push_back(t2krew::kNCasc_FrPiProd_pi);
  dial_names.push_back("FSI_PI_PROD");
  TClonesArray *piprod_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(piprod_graph);

  //Charge exchange high energy
  std::vector<float> cexhi_vec;
  for(int i=0; i<7; i++) cexhi_vec.push_back(0.267*((double)i-3.0));
  dial_values.push_back(cexhi_vec);
  dials.push_back(t2krew::kNCasc_FrCExHigh_pi);
  dial_names.push_back("FSI_CEX_HI");
  TClonesArray *cexhi_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(cexhi_graph);

  //Inelastic high energy
  std::vector<float> inelhi_vec;
  for(int i=0; i<7; i++) inelhi_vec.push_back(0.267*((double)i-3.0));
  dial_values.push_back(inelhi_vec);
  dials.push_back(t2krew::kNCasc_FrInelHigh_pi);
  dial_names.push_back("FSI_INEL_HI");
  TClonesArray *inelhi_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(inelhi_graph);

  // May want to add this back in in the future
  //    //CC-Coh BS dial values;
  //    std::vector<float> coh_bs_vec;
  //    for(int i=0; i<2; i++) coh_bs_vec.push_back(i);
  //    dial_values.push_back(coh_bs_vec);
  //    dials.push_back(t2krew::kNIWG2014a_coh_bs);
  //    dial_names.push_back("COH_BS");
  //    TClonesArray *coh_bs_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  //    dial_graphs.push_back(coh_bs_graph);

  //MEC PDD C dial values;
  std::vector<float> pdd_c_vec;
  if( fMirror ){
    for(int i=0; i<10; i++) pdd_c_vec.push_back( pdd_spacing[i]/2. );
  }
  else{
    for(int i=0; i<7; i++) pdd_c_vec.push_back( pdd_spacing[i]/2. );
  }
  dial_values.push_back(pdd_c_vec);
  dials.push_back(t2krew::kNIWGMEC_PDDWeight_C12);
  dial_names.push_back("PDD_C");
  TClonesArray *pdd_c_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(pdd_c_graph);

  //MEC PDD O dial values;
  std::vector<float> pdd_o_vec;
  if( fMirror ){
    for(int i=0; i<10; i++) pdd_o_vec.push_back( pdd_spacing[i]/2. );
  }
  else{
    for(int i=0; i<7; i++) pdd_o_vec.push_back( pdd_spacing[i]/2. );
  }
  dial_values.push_back(pdd_o_vec);
  dials.push_back(t2krew::kNIWGMEC_PDDWeight_O16);
  dial_names.push_back("PDD_O");
  TClonesArray *pdd_o_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(pdd_o_graph);

  // Must have SF and RPA dials at end, due to current setup of code and slightly strange dial ranges for these parameters

  //SF->RFG dial values;
  std::vector<float> sfrfg_vec;
  for(int i=1; i>-1; i--) sfrfg_vec.push_back(i);
  dial_values.push_back(sfrfg_vec);
  dials.push_back(t2krew::kNIWG2014a_SF_RFG);
  dial_names.push_back("SF_RFG");
  TClonesArray *sfrfg_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(sfrfg_graph);

  //RPA dial values;
  std::vector<float> rpa_vec;
  for(int i=0; i<7; i++) rpa_vec.push_back( ((double)i-3.)/3. );
  //for(int i=-1; i<2; i++) rpa_vec.push_back(i);
  dial_values.push_back(rpa_vec);
  dial_names.push_back("RPA");
  TClonesArray *rpa_graph = new TClonesArray("TGraph",SampleId::kNSamples);
  dial_graphs.push_back(rpa_graph);


  //Set up custom summary trees
  Int_t    NeutrinoCode         [SampleId::kNSamples];
  Double_t Enu                  [SampleId::kNSamples];
  Double_t TrueNeutrinoDirX;
  Double_t TrueNeutrinoDirY;
  Double_t TrueNeutrinoDirZ;

  Int_t    TrueLepPDG;
  Double_t TrueLepMom[SampleId::kNSamples];
  Double_t TrueCosThetamu[SampleId::kNSamples];
  Double_t TrueLepDirX;
  Double_t TrueLepDirY;
  Double_t TrueLepDirZ;
  Double_t RecoLepMom[SampleId::kNSamples];
  Double_t RecoLepDirX;
  Double_t RecoLepDirY;
  Double_t RecoLepDirZ[SampleId::kNSamples];

  Int_t    TruePreFSIPiPDG;
  Double_t TruePreFSIPiMom;
  Double_t TruePreFSIPiDirX;
  Double_t TruePreFSIPiDirY;
  Double_t TruePreFSIPiDirZ;
  Int_t    TruePostFSIPiPDG;
  Double_t TruePostFSIPiMom;
  Double_t TruePostFSIPiDirX;
  Double_t TruePostFSIPiDirY;
  Double_t TruePostFSIPiDirZ;
  Double_t RecoPiMom;
  Double_t RecoPiDirX;
  Double_t RecoPiDirY;
  Double_t RecoPiDirZ;

  Int_t    ReactionCode         [SampleId::kNSamples];
  Int_t    NuParent             [SampleId::kNSamples];
  Double_t Q2                   [SampleId::kNSamples];
  Double_t Q2QE                 [SampleId::kNSamples];
  Bool_t   HaveTruth            [SampleId::kNSamples];
  Int_t    TgtMat               [SampleId::kNSamples];
  Double_t FluxWeight           [SampleId::kNSamples];
  Int_t    TruthVtx             [SampleId::kNSamples];
  Double_t CCQETuningWeight     [SampleId::kNSamples];
  Double_t CohTuningWeight      [SampleId::kNSamples];
  Double_t RFGTuningWeight      [SampleId::kNSamples];
  Double_t RelRPATuningWeight   [SampleId::kNSamples];
  Double_t NonRelRPATuningWeight[SampleId::kNSamples];
  Double_t pFTuningWeight       [SampleId::kNSamples];
  Double_t MECCTuningWeight     [SampleId::kNSamples];
  Double_t MECOTuningWeight     [SampleId::kNSamples];
  Double_t MaQETuningWeight     [SampleId::kNSamples];

  Int_t Run;

  // Add NEUT reweighting engine
  t2krew::T2KReWeight rw;
  rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
  rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());

  // Uncertainties
  // CCQE:
  rw.Systematics().Include(t2krew::kNIWG2014a_pF_C12);
  rw.Systematics().Include(t2krew::kNIWG2014a_pF_O16);
  //    rw.Systematics().Include(t2krew::kNIWG2014a_Eb_C12);
  //    rw.Systematics().Include(t2krew::kNIWG2014a_Eb_O16);
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
  //    rw.Systematics().Include(t2krew::kNNucl_CCQEBindingEnergy_C12);
  //    rw.Systematics().Include(t2krew::kNNucl_CCQEBindingEnergy_O16);

  // Absolute tweak dials set the fractional uncertainty, instead of
  // in units of "sigma", defined in the code.
  // Useful so that you define the uncertainty within the code, as what is
  // hardcoded may not be the same as what is used for analysis.

  // CCQE:
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_C12);
  rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_O16);
  //    rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_C12);
  //    rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_O16);
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
  //    rw.Systematics().SetAbsTwk(t2krew::kNNucl_CCQEBindingEnergy_C12);
  //    rw.Systematics().SetAbsTwk(t2krew::kNNucl_CCQEBindingEnergy_O16);

  // Read the parameters files following the package hierarchy
  // first the top level package. Set the parameters as fixed
  ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

  // Make sure no parameters have been accessed yet
  ND::params().SetReadParamOverrideFilePointPassed();

  if(!(bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyWeightSystematics")) {
    std::cerr << "< psycheSteering.Systematics.ApplyWeightSystematics = 0 > you should change that otherwise the nominal detector weight won't get applied" << std::endl;
    throw;
  }
  int nWeightSyst=0;
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableChargeConf"       )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableMichelEleEff"     )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcClusterEff"    )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcTrackEff"      )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFgdTrackEff"      )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFgdHybridTrackEff")){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcFgdMatchEff"   )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnablePileUp"           )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFgdMass"          )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableOOFV"             )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableSIPion"           )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableSIProton"         )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableSandMuons"        )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableECalPID"          )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableTpcECalMatchEff"  )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableECalEmHipPID"     )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableFGD2Shower"       )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableNuETPCPileUp"     )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableNuEP0DPileUp"     )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableNuEECalPileUp"    )){ nWeightSyst++; }
  if((bool)ND::params().GetParameterI("psycheSteering.Weights.EnableNuEOOFV"          )){ nWeightSyst++; }

  if(nWeightSyst == 0) {
    std::cerr << "You have to apply all the systematics you want to have a correction" << std::endl;
    throw;
  }else{
    std::cout << "Applying " << nWeightSyst << " correction weight systematics" << std::endl;
  }

  AnalysisManager _man;

  //Need to initialize the input first.
  if (!_man.input().Initialize(fInputFile,"", false)) return false;

  int nmax_entries = fNEvts;
  int nmax_events = -1;

  // Get the number of entries in the tree
  if ( fNEvts < 0 ) {
    nmax_entries = (int)_man.input().GetEntries();
  }
  else {
    nmax_entries = std::min(nmax_entries, (int)_man.input().GetEntries());
  }

  std::cout << " We will process ";
  if (nmax_events==-1){
    // Get the numer of events
    nmax_events=_man.input().GetNEvents(nmax_entries);
    std::cout << nmax_entries << " entries." << std::endl;
  }
  else
    std::cout << nmax_events << " events." << std::endl;

  Run = 0;
  if     (string(fRunPeriod)=="MC"  ) Run=1;
  else if(string(fRunPeriod)=="sand") Run=-1;
  else if(string(fRunPeriod)=="data"){
    Run=2;
    data = true;
  }else{
    std::cout << "\nInvalid run type set\n" << std::endl;
    Usage();
    throw;
  }

  std::cout << " We are running over " << fRunPeriod << " files" << std::endl;

  std::cout << "Finished Reading tracker tree" << std::endl;

  //Start at the 0th RooTrackerVtx tree entry.
  Int_t rtvi = 0;

  TFile *inputFile = TFile::Open(fInputFile);

  TTree *def = (TTree*)inputFile->Get("flattree");
  TTree *hdr = (TTree*)inputFile->Get("header");
  TTree* RTV = NULL;
  if(!data) RTV = (TTree*)inputFile->Get("NRooTrackerVtx");

  TClonesArray *nRooVtxs = new TClonesArray("ND::NRooTrackerVtx");
  int NRooVtx = 0;
  if(!data){
    RTV->GetBranch("Vtx")->SetAutoDelete(kFALSE);
    RTV->SetBranchAddress("Vtx",&nRooVtxs);
    RTV->SetBranchAddress("NVtx",&NRooVtx);
  }
  TFile fout(fOutputFile,"RECREATE");
  TFile* inputfile = new TFile(fInputFile, "READ");
  TTree* config = (TTree*)((inputfile->Get("config"))->Clone());
  fout.cd();
  config->Write();

  TTree* enabledsampleid = new TTree("EnabledSampleID", "EnabledSampleID");
  int  selectionID = -999;
  int  enabledselection = 0;
  enabledsampleid->Branch("SelectionID", &selectionID, "SelectionID/I");
  enabledsampleid->Branch("IsEnabled",   &enabledselection, "IsEnabled/I");
  std::vector<SelectionBase*>selections = _man.sel().GetSelections();
  for(std::vector<SelectionBase*>::iterator it = selections.begin();
      it != selections.end(); it++){
    selectionID = (*it)->GetSampleId();
    enabledselection = (*it)->IsEnabled();
    enabledsampleid->Fill();
  }
  fout.cd();
  enabledsampleid->Write();

  TTree *def1 = def->CloneTree(0);
  TTree *hdr1 = hdr->CloneTree(0);

  TTree ssum_tree("sample_sum", "Tracker Numu Summary");

  int sNTrueVertices = 0;
  int sEvt = 0;
  // ssum_tree.SetAutoFlush(300000);
  // def1->SetAutoFlush(300000);
  // hdr1->SetAutoFlush(300000);
  def->SetBranchAddress("sEvt", &sEvt);
  def->SetBranchAddress("sNTrueVertices", &sNTrueVertices);

  Int_t NSamples = SampleId::kNSamples;
  Int_t SelectedSample = 0;
  double DetNomWeight;

  ssum_tree.Branch("NSamples",             &NSamples,              "NSamples/I");
  ssum_tree.Branch("SelectedSample",       &SelectedSample,        "SelectedSample/I");
  ssum_tree.Branch("Run",                  &Run,                   "Run/I");
  ssum_tree.Branch("DetNomWeight",         &DetNomWeight,          "DetNomWeight/D");

  ssum_tree.Branch("NeutrinoCode",          NeutrinoCode,          "NeutrinoCode[NSamples]/I");
  ssum_tree.Branch("Enu",                   Enu,                   "Enu[NSamples]/D");
  ssum_tree.Branch("TrueNeutrinoDirX",     &TrueNeutrinoDirX,      "TrueNeutrinoDirX/D");
  ssum_tree.Branch("TrueNeutrinoDirY",     &TrueNeutrinoDirY,      "TrueNeutrinoDirY/D");
  ssum_tree.Branch("TrueNeutrinoDirZ",     &TrueNeutrinoDirZ,      "TrueNeutrinoDirZ/D");

  ssum_tree.Branch("TrueLepPDG",           &TrueLepPDG,            "TrueLepPDG/I");
  ssum_tree.Branch("TruePmu",               TrueLepMom,            "TruePmu[NSamples]/D");
  ssum_tree.Branch("TrueLepDirX",          &TrueLepDirX,           "TrueLepDirX/D");
  ssum_tree.Branch("TrueLepDirY",          &TrueLepDirY,           "TrueLepDirY/D");
  ssum_tree.Branch("TrueLepDirZ",          &TrueLepDirZ,           "TrueLepDirZ/D");
  ssum_tree.Branch("TrueCosThetamu",        TrueCosThetamu,        "TrueCosThetamu[NSamples]/D");

  ssum_tree.Branch("Pmu",                   RecoLepMom,            "Pmu[NSamples]/D");
  ssum_tree.Branch("RecoLepDirX",          &RecoLepDirX,           "RecoLepDirX/D");
  ssum_tree.Branch("RecoLepDirY",          &RecoLepDirY,           "RecoLepDirY/D");
  ssum_tree.Branch("RecoLepDirZ",           RecoLepDirZ,           "RecoLepDirZ[NSamples]/D");
  ssum_tree.Branch("CosThetamu",            RecoLepDirZ,           "CosThetamu[NSamples]/D");

  ssum_tree.Branch("TruePostFSIPiPDG",     &TruePostFSIPiPDG,       "TruePostFSIPiPDG/I");
  ssum_tree.Branch("TruePostFSIPiMom",     &TruePostFSIPiMom,       "TruePostFSIPiMom/D");
  ssum_tree.Branch("TruePostFSIPiDirX",    &TruePostFSIPiDirX,      "TruePostFSIPiDirX/D");
  ssum_tree.Branch("TruePostFSIPiDirY",    &TruePostFSIPiDirY,      "TruePostFSIPiDirY/D");
  ssum_tree.Branch("TruePostFSIPiDirZ",    &TruePostFSIPiDirZ,      "TruePostFSIPiDirZ/D");
  ssum_tree.Branch("TruePreFSIPiPDG",      &TruePreFSIPiPDG,        "TruePreFSIPiPDG/I");
  ssum_tree.Branch("TruePreFSIPiMom",      &TruePreFSIPiMom,        "TruePreFSIPiMom/D");
  ssum_tree.Branch("TruePreFSIPiDirX",     &TruePreFSIPiDirX,       "TruePreFSIPiDirX/D");
  ssum_tree.Branch("TruePreFSIPiDirY",     &TruePreFSIPiDirY,       "TruePreFSIPiDirY/D");
  ssum_tree.Branch("TruePreFSIPiDirZ",     &TruePreFSIPiDirZ,       "TruePreFSIPiDirZ/D");
  ssum_tree.Branch("RecoPiMom",            &RecoPiMom,              "RecoPiMom/D");
  ssum_tree.Branch("RecoPiDirX",           &RecoPiDirX,             "RecoPiDirX/D");
  ssum_tree.Branch("RecoPiDirY",           &RecoPiDirY,             "RecoPiDirY/D");
  ssum_tree.Branch("RecoPiDirZ",           &RecoPiDirZ,             "RecoPiDirZ/D");

  ssum_tree.Branch("ReactionCode",          ReactionCode,          "ReactionCode[NSamples]/I");
  ssum_tree.Branch("NuParent",              NuParent,              "NuParent[NSamples]/I");
  ssum_tree.Branch("Q2",                    Q2,                    "Q2[NSamples]/D");
  ssum_tree.Branch("Q2QE",                  Q2QE,                  "Q2QE[NSamples]/D");
  ssum_tree.Branch("HaveTruth",             HaveTruth,             "HaveTruth[NSamples]/B");
  ssum_tree.Branch("TgtMat",                TgtMat,                "TgtMat[NSamples]/I");
  ssum_tree.Branch("FluxWeight",            FluxWeight,            "FluxWeight[NSamples]/D");
  ssum_tree.Branch("TruthVtx",              TruthVtx,              "TruthVtx[NSamples]/I");
  ssum_tree.Branch("CCQETuningWeight",      CCQETuningWeight,      "CCQETuningWeight[NSamples]/D");
  ssum_tree.Branch("CohTuningWeight",       CohTuningWeight,       "CohTuningWeight[NSamples]/D");
  ssum_tree.Branch("RFGTuningWeight",       RFGTuningWeight,       "RFGTuningWeight[NSamples]/D");
  ssum_tree.Branch("RelRPATuningWeight",    RelRPATuningWeight,    "RelRPATuningWeight[NSamples]/D");
  ssum_tree.Branch("NonRelRPATuningWeight", NonRelRPATuningWeight, "NonRelRPATuningWeight[NSamples]/D");
  ssum_tree.Branch("pFTuningWeight",        pFTuningWeight,        "pFTuningWeight[NSamples]/D");
  ssum_tree.Branch("MECCTuningWeight",      MECCTuningWeight,      "MECCTuningWeight[NSamples]/D");
  ssum_tree.Branch("MECOTuningWeight",      MECOTuningWeight,      "MECOTuningWeight[NSamples]/D");
  ssum_tree.Branch("MaQETuningWeight",      MaQETuningWeight,      "MaQETuningWeight[NSamples]/D");

  for(unsigned int i=0; i<dial_graphs.size(); i++){
    ssum_tree.Branch(Form("%sGraph",dial_names[i].c_str()), "TClonesArray", &(dial_graphs[i]), 256000, 0);
  }

  int evtNumber = -1;
  unsigned int nDials = dial_names.size();

  inputFile->cd();

  int last_event_selected = -1;

  // Set a ToyMaker to configure the toy experiment. Initialize it with a random seed. Apply variation if throwing toys.
  bool zero_var = fToys == 1;
  ToyMaker* toyMaker = new ToyMakerExample((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), zero_var);

  // Create and fill the Toy experiment with the appropriate format (number of systematics and number of parameters for each systematic)
  toyMaker->CreateToyExperiments(fToys, _man.syst().GetSystematics());

  // Create the array of PreviousToyBox
  _man.sel().CreateToyBoxArray(nmax_events);

  // Create the array of SystBox
  _man.syst().Initialize(_man.sel(),nmax_events);

  Long64_t entry=0;
  Int_t evt=0;
  int ProcessEntries = (nmax_entries>nmax_events? nmax_entries:nmax_events);
  while (entry < ProcessEntries){
    if(entry%100 == 0)
      std::cout << "Processed " << 100.*((double)entry) / ((double)ProcessEntries) << "% -> "
                << entry << " entries out of " << (nmax_entries>nmax_events? nmax_entries:nmax_events) << std::endl;

    Weight_h totalWeightSyst;
    Weight_h fluxWeightSyst;

    int q = int(entry);

    def->GetEntry(q);
    hdr->GetEntry(q);
    if(q == 0) hdr1->Fill();


    // Fill the event structure
    if (!_man.LoadEvent(entry)) continue;

    // Get the event to be modified. This is the only one used in the analysis
    AnaEventB* event = static_cast<AnaEventB*>(_man.input().GetSuperEvent().Event);

    // Fill the EventBox
    _man.sel().InitializeEvent(*event);

    // Initialize The SystBox for variation systematics
    _man.syst().InitializeEventSystematics(_man.sel(),*event);

    // Get event number
    evtNumber = event->EventInfo.Event;
    //std::cout << "evtNumber: " << evtNumber << std::endl;

    // Clear all the old graphs
    for(unsigned int i=0; i<dial_graphs.size(); i++){
      (dial_graphs[i])->Clear();
    }

    std::set<int> samples;

    // Set default values
    for(int sample = 0; sample < SampleId::kNSamples; ++sample){

      for(unsigned int sys_iter=0; sys_iter<nDials; sys_iter++){
        TGraph* graph = (TGraph*) dial_graphs[sys_iter]->ConstructedAt(sample);
        graph->SetName(Form("%s_gr",dial_names[sys_iter].c_str()));
        for(int piter=(graph->GetN()-1); piter>=0; piter--) graph->RemovePoint(piter);
        graph->SetPoint(0,0.0,1.0);
      }

      NeutrinoCode         [sample] = -999;
      Enu                  [sample] = -999;

      TrueLepMom           [sample] = -999;
      TrueCosThetamu       [sample] = -999;

      RecoLepDirZ          [sample] = -999;
      RecoLepMom           [sample] = -999;

      ReactionCode         [sample] = -999;
      NuParent             [sample] = -999;
      Q2                   [sample] = -999;
      Q2QE                 [sample] = -999;
      HaveTruth            [sample] = false;
      TgtMat               [sample] = -999;
      FluxWeight           [sample] = -999;
      TruthVtx             [sample] = -999;
      CCQETuningWeight     [sample] = -999;
      CohTuningWeight      [sample] = -999;
      RFGTuningWeight      [sample] = -999;
      RelRPATuningWeight   [sample] = -999;
      NonRelRPATuningWeight[sample] = -999;
      pFTuningWeight       [sample] = -999;
      MECCTuningWeight     [sample] = -999;
      MECOTuningWeight     [sample] = -999;
      MaQETuningWeight     [sample] = -999;

    }

    TrueNeutrinoDirX      = -999;
    TrueNeutrinoDirY      = -999;
    TrueNeutrinoDirZ      = -999;

    TrueLepPDG            = -999;
    TrueLepDirX           = -999;
    TrueLepDirY           = -999;
    TrueLepDirZ           = -999;
    RecoLepDirX           = -999;
    RecoLepDirY           = -999;

    TruePostFSIPiPDG      = -999;
    TruePostFSIPiMom      = -999;
    TruePostFSIPiDirX     = -999;
    TruePostFSIPiDirY     = -999;
    TruePostFSIPiDirZ     = -999;
    TruePreFSIPiPDG       = -999;
    TruePreFSIPiMom       = -999;
    TruePreFSIPiDirX      = -999;
    TruePreFSIPiDirY      = -999;
    TruePreFSIPiDirZ      = -999;
    RecoPiMom             = -999;
    RecoPiDirX            = -999;
    RecoPiDirY            = -999;
    RecoPiDirZ            = -999;

    DetNomWeight = -999;;
    // If data event doesn't pass selection, need to set selected to be false
    bool data_passed = false;

    for(int itoy=0; itoy<fToys; itoy++){
      //  Gets the ToyExperiment with index itoy from the ToyMaker
      ToyExperiment* toy = toyMaker->GetToyExperiment(itoy);

      bool passed = _man.ProcessEvent(*toy,*event,totalWeightSyst,fluxWeightSyst);
      DetNomWeight = (double)totalWeightSyst.Correction;
      AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
      // Generated skimmed spline files -> only save events if something passes one of the psyche selections
      // If not, reset the event containers
      if(!passed){
        evt++;
        continue;
      }
      else if(samples.find(int( summary->EventSample )) != samples.end()) continue;
      else samples.insert(int( summary->EventSample ));

      int sample = int(summary->EventSample);

      //AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
      // Data event made it past the selection, want to selected to be true
      if(data) { data_passed = true; }

      if(!data){

        SelectedSample = summary->EventSample;

        if(summary->TrueVertex[sample] == NULL && summary->EventSample == sample){
          std::cerr << "weird error, the event has been selected but doesn't have true vertex" << std::endl;
        }
        if(summary->TrueVertex[sample] != NULL && summary->EventSample == sample){
          //Save stuff
          AnaParticleMomB* leptonCandidate = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample]);

          RecoLepMom[sample] = leptonCandidate->Momentum;
          RecoLepDirX = leptonCandidate->DirectionStart[0];
          RecoLepDirY = leptonCandidate->DirectionStart[1];
          RecoLepDirZ[sample] = leptonCandidate->DirectionStart[2];

          if(sample == SampleId::kFGD1NuMuCC1Pi                  ||
             sample == SampleId::kFGD1NuMuCCOther                ||
             sample == SampleId::kFGD1NuMuCC1PiFwd               ||
             sample == SampleId::kFGD1NuMuCCOtherFwd             ||
             sample == SampleId::kFGD1NuMuCC1PiBwd               ||
             sample == SampleId::kFGD1NuMuCCOtherBwd             ||
             sample == SampleId::kFGD1NuMuCC1PiHAFwd             ||
             sample == SampleId::kFGD1NuMuCCOtherHAFwd           ||
             sample == SampleId::kFGD1NuMuCC1PiHABwd             ||
             sample == SampleId::kFGD1NuMuCCOtherHABwd           ||

             sample == SampleId::kFGD2NuMuCC1Pi                  ||
             sample == SampleId::kFGD2NuMuCCOther                ||
             sample == SampleId::kFGD2NuMuCC1PiFwd               ||
             sample == SampleId::kFGD2NuMuCCOtherFwd             ||
             sample == SampleId::kFGD2NuMuCC1PiBwd               ||
             sample == SampleId::kFGD2NuMuCCOtherBwd             ||
             sample == SampleId::kFGD2NuMuCC1PiHAFwd             ||
             sample == SampleId::kFGD2NuMuCCOtherHAFwd           ||
             sample == SampleId::kFGD2NuMuCC1PiHABwd             ||
             sample == SampleId::kFGD2NuMuCCOtherHABwd           ||

             sample == SampleId::kFGD1AntiNuMuCC1Pi              ||
             sample == SampleId::kFGD1AntiNuMuCCOther            ||

             sample == SampleId::kFGD2AntiNuMuCC1Pi              ||
             sample == SampleId::kFGD2AntiNuMuCCOther            ||

             sample == SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi   ||
             sample == SampleId::kFGD1NuMuBkgInAntiNuModeCCOther ||

             sample == SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi   ||
             sample == SampleId::kFGD2NuMuBkgInAntiNuModeCCOther){
            // When the event summary will be completed with pion mom, this is where the pion momentum will be filled.
            RecoPiMom  = 0;
            RecoPiDirX = 0;
            RecoPiDirY = 0;
            RecoPiDirZ = 0;
          }


          if(leptonCandidate->GetTrueParticle()){
            TrueLepMom[sample]  = leptonCandidate->GetTrueParticle()->Momentum;
            TrueLepPDG  = leptonCandidate->GetTrueParticle()->PDG;
            // TrueLepDirX = leptonCandidate->GetTrueParticle()->Direction[0];
            // TrueLepDirY = leptonCandidate->GetTrueParticle()->Direction[1];
            // TrueLepDirZ = leptonCandidate->GetTrueParticle()->Direction[2];
            TruthVtx   [sample] = leptonCandidate->GetTrueParticle()->VertexID;
          }

          int id = summary->TrueVertex[sample]->ID;
          if(TruthVtx[sample] != id){
            std::cerr << "WARNING: TruthVtx[" << sample << "]("<< TruthVtx[sample] << ") != event->Summary->TrueVertex[sample]->ID (" << id << ")" << std::endl;
            std::cerr << "MACH3 and BANFF wont agree!!" << std::endl;
            throw;
          }
        }

        ND::NRooTrackerVtx * vtx = NULL;

        CohTuningWeight[sample] = 1.0;

        int flavor = -999;
        FluxWeight  [sample] = 1.0;
        Double_t weight = 1.0;

        if(RecoLepMom[sample] < 0){
          continue;
        }

        //Pull out the correct RooTrackerVtx tree entry.
        int rtv_entries = RTV->GetEntries();
        if(TruthVtx[sample] > -1){
          int rtv_curr = rtvi;
          while(vtx == NULL){
            RTV->GetEntry(rtvi);
            //Use the TruthVertexID to get the RooTrackerVertex corresponding to this
            //event.
            for(int i = 0; i < NRooVtx; ++i){
              vtx = (ND::NRooTrackerVtx*) nRooVtxs->At(i);
              if(vtx->TruthVertexID == TruthVtx[sample] && fabs(vtx->StdHepP4[0][3]*1000 - summary->TrueVertex[sample]->NuEnergy) < 0.1) break;
              vtx = NULL;
            }
            if(vtx == NULL) rtvi++;
            if(rtvi==rtv_entries){
              rtvi = rtv_curr;
              break;
            }
          }
        }

        // Apply nominal tuning
        rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,0); // SF->RFG tuning
        rw.Systematics().SetTwkDial(t2krew::kNXSec_VecFFCCQE, 2); // Note that when we set MAQE, we need to set NXSec_VecFFCCQE to 2 for SF->RFG MC
        rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,0);
        rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);

        // Initialise dials to nominal
        for(unsigned int sys_iter1=0; sys_iter1<nDials; sys_iter1++){
          if(sys_iter1==nDials -1){
            rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,0);
            rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);
          }
          else{
            rw.Systematics().SetTwkDial(dials[sys_iter1], 0.);
          }
        }

        bool vtx_match = true;
        // If there is no RooTrackerVertex, or we are looking at sand events, set it all to nominal
        if(!vtx || Run == -1){
          //std::cout << "Cannot find NRooTrackerVtx object - skipping weight for this vertex!" << std::endl;;
          vtx_match = false;
          for(unsigned int sys_iter=0; sys_iter<nDials; sys_iter++){
            TGraph* graph = (TGraph*) dial_graphs[sys_iter]->ConstructedAt(sample);
            graph->SetName(Form("%s_gr",dial_names[sys_iter].c_str()));
            for(int piter=(graph->GetN()-1); piter>=0; piter--) graph->RemovePoint(piter);
            graph->SetPoint(0,0.0,1.0);
          }
        }
        else{
          //std::cout << "Found NRooTrackerVtx object!" << std::endl;;
          //Save the truth information
          ReactionCode[sample] = atoi( ((vtx->EvtCode)->String()).Data() );
          NeutrinoCode[sample] = 0;
          if     ( vtx->NuParentDecMode < 20) { NeutrinoCode[sample]=  14; flavor=0; }
          else if( vtx->NuParentDecMode < 30) { NeutrinoCode[sample]= -14; flavor=1; }
          else if( vtx->NuParentDecMode < 40) { NeutrinoCode[sample]=  12; flavor=2; }
          else if( vtx->NuParentDecMode < 50) { NeutrinoCode[sample]= -12; flavor=3; }
          Enu[sample] = vtx->StdHepP4[0][3];
          FluxWeight[sample] = fluxWeightSyst.Correction;
          const Int_t MuonIndex = T2KNIWGUtils::IdxFsl(*vtx);
          const TVector3 nu(vtx->StdHepP4[0][0],
                            vtx->StdHepP4[0][1],
                            vtx->StdHepP4[0][2]);
          const TVector3 mu(vtx->StdHepP4[MuonIndex][0],
                            vtx->StdHepP4[MuonIndex][1],
                            vtx->StdHepP4[MuonIndex][2]);
          const Double_t muMag = mu.Mag();
          TrueLepMom[sample]  = muMag;
          TrueLepDirX = mu.X() / muMag;
          TrueLepDirY = mu.Y() / muMag;
          TrueLepDirZ = mu.Z() / muMag;
          const Double_t nuMag = nu.Mag();
          TrueNeutrinoDirX = nu.X() / nuMag;
          TrueNeutrinoDirY = nu.Y() / nuMag;
          TrueNeutrinoDirZ = nu.Z() / nuMag;
          TrueCosThetamu[sample] = (nu * mu)/(nuMag*muMag);
          Q2      [sample] = T2KNIWGUtils::Q2  (vtx);
          Q2QE    [sample] = T2KNIWGUtils::Q2QE(vtx);
          TgtMat  [sample] = T2KNIWGUtils::TgtA(*vtx);
          NuParent[sample] = vtx->NuParentPdg;
          int reac = TMath::Abs(ReactionCode[sample]);

          for(int ipart=0; ipart < vtx->NEnvc; ++ipart){
            if(TMath::Abs(vtx->NEipvc[ipart]) != 211 &&
               TMath::Abs(vtx->NEipvc[ipart]) != 111)
              continue;
            const TVector3 pi(vtx->NEpvc[ipart][0], vtx->NEpvc[ipart][1], vtx->NEpvc[ipart][2]);
            const Double_t piMag = pi.Mag();
            if(vtx->NEicrnvc[ipart] == 1 && pi.Mag() > TruePostFSIPiMom){ // the pion exits the nucleus (takes the most energetic)
              TruePostFSIPiPDG      = vtx->NEipvc[ipart];
              TruePostFSIPiMom      = piMag;
              TruePostFSIPiDirX     = pi.X() / piMag;
              TruePostFSIPiDirY     = pi.Y() / piMag;
              TruePostFSIPiDirZ     = pi.Z() / piMag;
            }
            if(vtx->NEiorgvc[ipart]-1 == 1 && pi.Mag() > TruePreFSIPiMom){ // take the one whose parent is the initial state (primary)
              TruePreFSIPiPDG       = vtx->NEipvc[ipart];
              TruePreFSIPiMom       = pi.Mag();
              TruePreFSIPiDirX      = pi.X() / piMag;
              TruePreFSIPiDirY      = pi.Y() / piMag;
              TruePreFSIPiDirZ      = pi.Z() / piMag;
            }
          }

          // std::cout << "ReactionCode " << ReactionCode[sample] << std::endl;
          // std::cout << "Particle " << ipart << std::endl;
          // std::cout << " - n vert nucleus   " << vtx->NEnvert << std::endl;
          // std::cout << " - PDG              " << vtx->NEipvc[ipart]   << std::endl;
          // std::cout << " - Final Stats flag " << vtx->NEiflgvc[ipart] << std::endl;
          // std::cout << " - Origin           " << vtx->NEiorgvc[ipart]-1  << std::endl;
          // std::cout << " - Parent PDG       " << vtx->NEipvc[vtx->NEiorgvc[ipart]-1]  << std::endl;
          // std::cout << " - Exit nucleus?    " << vtx->NEicrnvc[ipart]  << std::endl;
          // std::cout << " - PX[0]            " << vtx->NEpvc[ipart][0]  << std::endl;
          // std::cout << " - PY[1]            " << vtx->NEpvc[ipart][1]  << std::endl;
          // std::cout << " - PZ[2]            " << vtx->NEpvc[ipart][2]  << std::endl;

          //Add Kendall's pion tuning stuff
          double piEnergy = -1.0;

          if(reac == 16){
            for(int ipart = 0; ipart < event->nTrueParticles; ++ipart){
              if(fabs(event->TrueParticles[ipart]->PDG) != 211) continue;
              if(event->TrueParticles[ipart]->TrueVertex == NULL) continue;
              if(event->TrueParticles[ipart]->VertexID == TruthVtx[sample]){
                piEnergy = sqrt(event->TrueParticles[ipart]->Momentum*event->TrueParticles[ipart]->Momentum + 139.57*139.57)/1000.0;
              }
            }
            if     (piEnergy < 0.00){ CohTuningWeight[sample] = 1;     }
            else if(piEnergy < 0.25){ CohTuningWeight[sample] = 0.135; }
            else if(piEnergy < 0.50){ CohTuningWeight[sample] = 0.4;   }
            else if(piEnergy < 0.75){ CohTuningWeight[sample] = 0.294; }
            else if(piEnergy < 1.00){ CohTuningWeight[sample] = 1.206; }
          }
          //Iterate through the systematics to make splines
          for(unsigned int sys_iter=0; sys_iter<nDials; sys_iter++){
            //Set name of TGraph
            TGraph* graph = (TGraph*) dial_graphs[sys_iter]->ConstructedAt(sample);
            graph->SetName(Form("%s_gr",dial_names[sys_iter].c_str()));
            for(int piter=(graph->GetN()-1); piter>=0; piter--) graph->RemovePoint(piter);

            // Initialise dials to nominal
            for(unsigned int sys_iter1=0; sys_iter1<nDials; sys_iter1++){
              if(sys_iter1==nDials -1){
                rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,0);
                rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);
              }
              else{
                rw.Systematics().SetTwkDial(dials[sys_iter1], 0.);
              }
            }

            //Iterate through dial of interest
            bool flat = true;

            if(sys_iter==nDials -1){
              for( int k = -1; k < 2; ++k){
                rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,fabs(k));
                if(k == 1) rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);
                else rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,k);
                rw.Reconfigure();
                weight = rw.CalcWeight(vtx);
                if( fabs(weight-1.0) > 1.e-5 ) flat = false;
                if(RESTRICTWEIGHT && !ValidWeightRange(weight)) weight = MAXWEIGHT;
                if(k < 0) graph->SetPoint(1, 0, weight);
                else if(k > 0) graph->SetPoint(2, 1, weight);
                else graph->SetPoint(0, -1, weight);
              }
            }
            else{
              for(unsigned int dial_iter=0; dial_iter<dial_values[sys_iter].size(); dial_iter++){
                weight = 1;
                // If event is on Hydrogen and we are looking at MAQE
                if(TgtMat[sample] == 1 && dial_names[sys_iter] == "MAQE"){
                  rw.Systematics().SetTwkDial(dials[sys_iter], (1.03 - 1.2)/1.2);
                }
                else{
                  rw.Systematics().SetTwkDial(dials[sys_iter], dial_values[sys_iter][dial_iter]);
                }
                rw.Reconfigure();
                weight = rw.CalcWeight(vtx);
                if( fabs(weight-1.0) > 1.e-5 ) flat = false;
                if(RESTRICTWEIGHT && !ValidWeightRange(weight)) weight = MAXWEIGHT;
                if(sys_iter==nDials-2){
                  graph->SetPoint(dial_iter, dial_iter, weight);
                }
                else if(fMirror && (dial_names[sys_iter] == "PDD_C" || dial_names[sys_iter] == "PDD_O")){
                  double x_val = dial_values[sys_iter][dial_iter];
                  if(dial_iter > 6) x_val = 2.0 - x_val;
                  graph->SetPoint(dial_iter, x_val, weight);
                }
                else if(fMirror && (dial_names[sys_iter] == "PF_C" || dial_names[sys_iter] == "PF_O")){
                  double x_val = dial_values[sys_iter][dial_iter];
                  if(dial_iter < 3) x_val = 2.0*dial_values[sys_iter][3] - x_val;
                  graph->SetPoint(dial_iter, x_val, weight);
                }
                else{
                  graph->SetPoint(dial_iter, dial_values[sys_iter][dial_iter], weight);
                }
              }
              //If the response is flat, only save a single point
              if(flat && sys_iter<nDials-2){
                for(int piter=(graph->GetN()-1); piter>=0; piter--) graph->RemovePoint(piter);
                graph->SetPoint(0,0.0,1.0);
              }
            }
          }
        }

        // Reset dials to nominal
        for(unsigned int sys_iter1=0; sys_iter1<nDials; sys_iter1++){
          if(sys_iter1==nDials -1){
            rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,0);
            rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);
          }
          else{
            rw.Systematics().SetTwkDial(dials[sys_iter1], 0.);
          }
        }

        if(Run == -1){
          CohTuningWeight[sample] = 1;
          RFGTuningWeight[sample] = 1;
          RelRPATuningWeight[sample] = 1;
          NonRelRPATuningWeight[sample] = 1;
          CCQETuningWeight[sample] = 1;
          pFTuningWeight[sample]   = 1;
          MECCTuningWeight[sample] = 1;
          MECOTuningWeight[sample] = 1;
          MaQETuningWeight[sample] = 1;
        }
        else{
          // Apply CCQE tuning
          rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,1); // SF->RFG tuning

          rw.Reconfigure();
          double RFGWeight = rw.CalcWeight(vtx);

          RFGTuningWeight[sample] = RFGWeight;

          rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,0); // no SF tuning
          rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1);
          rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0);

          rw.Reconfigure();
          RelRPATuningWeight[sample] = rw.CalcWeight(vtx);

          rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1);
          rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,-1);

          rw.Reconfigure();
          NonRelRPATuningWeight[sample] = rw.CalcWeight(vtx);

          rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,1); //SF->RFG
          rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1); //RPA correction
          rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0); // Relativistic
          rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_pF_C12, (217.-217.)/217.);
          rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_Norm_C12, (1.0-1.0)/1.0);
          rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_Norm_O16, (1.0-1.0)/1.0);
          rw.Systematics().SetTwkDial(t2krew::kNXSec_MaCCQE, (1.2-1.2)/1.2);

          rw.Reconfigure();
          CCQETuningWeight[sample] = rw.CalcWeight(vtx);

          // Reset to nominal
          rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,0); //SF->RFG
          rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,0); //RPA correction
          rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0); // Relativistic
        }

        HaveTruth[sample] = true;
        if( !vtx_match ){
          std::cout << "Could not match NRooTrackerVtx" << std::endl;
          HaveTruth[sample] = false;
        }
        // break;
      }
    }

    //Fill summary tree
    bool selected = false;
    if(data){
        if (data_passed) { selected = true; }
    }
    else{
      for(int sample = 0; sample < SampleId::kNSamples; ++sample){
        if(TruthVtx[sample] > -1) selected = true;
      }
    }
    if(sNTrueVertices == 0 && selected && !data){
      int currEventNum = evtNumber;
      int count = 1;
      while (currEventNum == evtNumber){
        currEventNum = sEvt;
        def->GetEntry(q-count);
        if(sNTrueVertices > 0 && currEventNum == evtNumber){
          if((q-count) <= last_event_selected) break;
          def1->Fill();
          ssum_tree.Fill();
          break;
        }
        count++;
      }
      def->GetEntry(q);
    }
    if(selected){
      last_event_selected = q;
      def1->Fill();
      if(!data) ssum_tree.Fill();
    }
    // Delete the SystBox array
    _man.syst().FinalizeEventSystematics(*event);

    // Reset the PreviousToyBox
    _man.sel().FinalizeEvent(*event);

    evt++;
  } // number of events
  std::cout << "Event loop done!!" << std::endl;

  fout.cd();

  if(!data) ssum_tree.Write("sample_sum");
  def1->Write("flattree");
  hdr1->Write("header");
  fout.Close();

  inputFile->Close();

  return 0;
}

// Print the cmd line syntax
void Usage(){
  std::cout << "Cmd line syntax should be:" << std::endl;
  std::cout << "genWeightsFromNRooTracker_BANFF_2016.exe -p psyche_inputfile -o outputfile -r run_type [-n n_events = -1 -m mirrored splines = 1 -t n_toys = 1]" << std::endl;
  std::cout << "psyche_inputfile is a flat tree suitable for input into psyche." << std::endl;
  std::cout << "run_type can be 'MC', 'sand' or 'data'." << std::endl;
  std::cout << "outputfile is the desired name of the output file." << std::endl;
  std::cout << "n_events is the number of events to process - if this is not set the whole input file will be processed." << std::endl;

}

// Messy way to process cmd line arguments.
void ParseArgs(int argc, char **argv){
  int nargs = 1;

  char* null = "";
  fInputFile  = null;
  fOutputFile = null;
  fRunPeriod  = null;

  if(argc<(nargs*2+1)){ Usage(); exit(1); }
  for(int i = 1; i < argc; i+=2){
    if     (string(argv[i]) == "-p") fInputFile  = argv[i+1];
    else if(string(argv[i]) == "-o") fOutputFile = argv[i+1];
    else if(string(argv[i]) == "-r") fRunPeriod  = argv[i+1];
    else if(string(argv[i]) == "-n") fNEvts = atoi(argv[i+1]);
    else if(string(argv[i]) == "-m") fMirror = (bool)atoi(argv[i+1]);
    else if(string(argv[i]) == "-t") fToys = atoi(argv[i+1]);
    else {
      std::cout << "Invalid argument:" << argv[i] << " "<< argv[i+1] << std::endl;
      Usage();
      exit(1);
    }
  }
  // Final idiot proof check
  if(std::string(fInputFile) == "" || std::string(fOutputFile) == "" || std::string(fRunPeriod) == ""){
    Usage();
    exit(1);
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

Bool_t ValidWeightRange(const Double_t &weight)
{
    if(!TMath::Finite(weight) || TMath::IsNaN(weight))
        return kFALSE;
    else if(weight > MAXWEIGHT)
        return kFALSE;
    else
        return kTRUE;
}

