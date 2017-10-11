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
int fId = 0;
char * fInputFile;
char * fOutputFile;
char * fRunPeriod;
bool data = 0;

void Usage();
void ParseArgs(int argc, char **argv);
int GetPThetaBin(double p, double th);



int main(int argc, char *argv[]){

  // P.S (26.01.17) AxialFF Patch

    //Reads the input file, Run Period, and output file name from the command
    //line.
    ParseArgs(argc, argv);

    //Set up objects to store the cross section variations
    double ma_spacing[7] = {-3, -2, -1, 0, 1, 2, 3};
    double pdd_spacing[7] = {-2, -1, -0.1, 0, 0.1, 1, 2};

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
    for(int i=0; i<7; i++) pf_c.push_back((i - 3)*(66.7/(6.*217.)) + (475./(2.*217.) - 1.));
    dial_values.push_back(pf_c);
    for(int i = 0; i < 7; ++i){
        std::cout << "pf_c value " << i << " = " << pf_c[i]*217 << std::endl;
    }
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
    for(int i=0; i<7; i++) pf_o.push_back((i - 3)*(66.7/(6.*225.)) + (475./(2.*225.) - 1.));
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
    for(int i=0; i<7; i++) pdd_c_vec.push_back( pdd_spacing[i]/2. );
    dial_values.push_back(pdd_c_vec);
    dials.push_back(t2krew::kNIWGMEC_PDDWeight_C12);
    dial_names.push_back("PDD_C");
    TClonesArray *pdd_c_graph = new TClonesArray("TGraph",SampleId::kNSamples);
    dial_graphs.push_back(pdd_c_graph);

    //MEC PDD O dial values;
    std::vector<float> pdd_o_vec;
    for(int i=0; i<7; i++) pdd_o_vec.push_back( pdd_spacing[i]/2. );
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
    Double_t Enu                  [SampleId::kNSamples];
    Double_t Pmu                  [SampleId::kNSamples];
    Double_t CosThetamu           [SampleId::kNSamples];
    Double_t FluxWeight           [SampleId::kNSamples];
    Double_t TruePmu              [SampleId::kNSamples];
    Double_t TrueCosThetamu       [SampleId::kNSamples];
    Double_t Q2                   [SampleId::kNSamples];
    Double_t Q2QE                 [SampleId::kNSamples];
    Double_t CCQETuningWeight     [SampleId::kNSamples];
    Double_t MaQETuningWeight     [SampleId::kNSamples];
    Double_t MECOTuningWeight     [SampleId::kNSamples];
    Double_t MECCTuningWeight     [SampleId::kNSamples];
    Double_t pFTuningWeight       [SampleId::kNSamples];
    Double_t RFGTuningWeight      [SampleId::kNSamples];
    Double_t RelRPATuningWeight   [SampleId::kNSamples];
    Double_t NonRelRPATuningWeight[SampleId::kNSamples];
    Double_t CohTuningWeight      [SampleId::kNSamples];
    
    Int_t NeutrinoCode[SampleId::kNSamples];
    Int_t ReactionCode[SampleId::kNSamples];
    Int_t TgtMat      [SampleId::kNSamples];
    Int_t NuParent    [SampleId::kNSamples];
    Int_t TruthVtx    [SampleId::kNSamples];
    
    Bool_t HaveTruth[SampleId::kNSamples];
    
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

    AnalysisManager _man;

    //Need to initialize the input first.
    if (!_man.input().Initialize(fInputFile,"", false)) return false;

    int nmax_entries = fNEvts;
    int nmax_events =-1;

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
    if(string(fRunPeriod)=="MC") Run=1;
    else if(string(fRunPeriod)=="sand" ) Run=-1;
    else if(string(fRunPeriod)=="data" ){
        Run=2;
        data = true;
    }
    else{
        std::cout << "\nInvalid run type set\n" << std::endl;
        Usage();
        exit(1);
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

    TTree *def1 = def->CloneTree(0);
    TTree *hdr1 = hdr->CloneTree(0);

    TTree ssum_tree("sample_sum", "Tracker Numu Summary");

    int sNTrueVertices = 0;
    int sEvt = 0;
    ssum_tree.SetAutoFlush(300000);
    def1->SetAutoFlush(300000);
    hdr1->SetAutoFlush(300000);
    def->SetBranchAddress("sEvt", &sEvt);
    def->SetBranchAddress("sNTrueVertices", &sNTrueVertices);

    Int_t NSamples = SampleId::kNSamples;
    Int_t SelectedSample = 0;

    ssum_tree.Branch("NSamples",             &NSamples,              "NSamples/I");
    ssum_tree.Branch("SelectedSample",       &SelectedSample,        "SelectedSample/I");
    ssum_tree.Branch("Enu",                   Enu,                   "Enu[NSamples]/D");
    ssum_tree.Branch("Pmu",                   Pmu,                   "Pmu[NSamples]/D");
    ssum_tree.Branch("CosThetamu",            CosThetamu,            "CosThetamu[NSamples]/D");
    ssum_tree.Branch("ReactionCode",          ReactionCode,          "ReactionCode[NSamples]/I");
    ssum_tree.Branch("NeutrinoCode",          NeutrinoCode,          "NeutrinoCode[NSamples]/I");
    ssum_tree.Branch("FluxWeight",            FluxWeight,            "FluxWeight[NSamples]/D");
    ssum_tree.Branch("TruePmu",               TruePmu,               "TruePmu[NSamples]/D");
    ssum_tree.Branch("TrueCosThetamu",        TrueCosThetamu,        "TrueCosThetamu[NSamples]/D");
    ssum_tree.Branch("Q2",                    Q2,                    "Q2[NSamples]/D");
    ssum_tree.Branch("Q2QE",                  Q2QE,                  "Q2QE[NSamples]/D");
    ssum_tree.Branch("HaveTruth",             HaveTruth,             "HaveTruth[NSamples]/B");
    ssum_tree.Branch("TgtMat",                TgtMat,                "TgtMat[NSamples]/I");
    ssum_tree.Branch("NuParent",              NuParent,              "NuParent[NSamples]/I");
    ssum_tree.Branch("Run",                  &Run,                   "Run/I");
    ssum_tree.Branch("TruthVtx",              TruthVtx,              "TruthVtx[NSamples]/I");
    ssum_tree.Branch("CCQETuningWeight",      CCQETuningWeight,      "CCQETuningWeight[NSamples]/D");
    ssum_tree.Branch("CohTuningWeight",       CohTuningWeight,       "CohTuningWeight[NSamples]/D");
    ssum_tree.Branch("RFGTuningWeight",       RFGTuningWeight,       "RFGTuningWeight[NSamples]/D");
    ssum_tree.Branch("RelRPATuningWeight",    RelRPATuningWeight,    "RelRPATuningWeight[NSamples]/D");
    ssum_tree.Branch("NonRelRPATuningWeight", NonRelRPATuningWeight, "NonRelRPATuningWeight[NSamples]/D");

    for(unsigned int i=0; i<dial_graphs.size(); i++){
      ssum_tree.Branch(Form("%sGraph",dial_names[i].c_str()), "TClonesArray", &(dial_graphs[i]), 256000, 0);
    }

    int evtNumber = -1; 
    unsigned int nDials = dial_names.size();

    inputFile->cd();

    int last_event_selected = -1;

    int n;
    EventVariationBase** varSyst = _man.syst().GetVariationSystematics(n);
    EventWeightBase** weightSyst = _man.syst().GetWeightSystematics(n);
    UInt_t nw=_man.syst().GetNWeightSystematics();
    UInt_t nv=_man.syst().GetNVariationSystematics();
    for(unsigned int i = 0; i < nw; i++){
        EventWeightBase* tmp = weightSyst[i];
        tmp->SetPDF(SystematicBase::kGaussian);
    }
    for(unsigned int i = 0; i < nv; i++){
        EventVariationBase* tmp = varSyst[i];
        if(tmp->Name()=="kBFieldDist"){
            tmp->SetPDF(SystematicBase::kUniform);
        }
        else{
            tmp->SetPDF(SystematicBase::kGaussian);
        }
    }

        // Set a ToyMaker to configure the toy experiment. Initialize it with a random seed
    ToyMaker* toyMaker = new ToyMakerExample((UInt_t)ND::params().GetParameterI("psycheSteering.Systematics.RandomSeed"), 
                                             (bool)  ND::params().GetParameterI("psycheSteering.Systematics.ZeroVariation"));

    // Create and fill the Toy experiment with the appropriate format (number of systematics and number of parameters for each systematic)
    toyMaker->CreateToyExperiments(1, _man.syst().GetSystematics()); 

    // Create the array of PreviousToyBox
    _man.sel().CreateToyBoxArray(nmax_events);

    // Create the array of SystBox
    _man.syst().Initialize(_man.sel(),nmax_events);

    Long64_t entry=0;
    Int_t evt=0;
    Int_t npassed=0;
    int ProcessEntries = (nmax_entries>nmax_events? nmax_entries:nmax_events);
    while (entry < ProcessEntries){
      if(entry%1000 == 0)
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

        //  Gets the ToyExperiment with index itoy from the ToyMaker
        ToyExperiment* toy = toyMaker->GetToyExperiment(0);

        // Fixed 0 sigma for all systematics
        int syst_counter = 0;
        for (UInt_t isyst = 0; isyst < 1000;isyst++){
          if (toy->GetToyVariations(isyst) != NULL){
            syst_counter++;
            for (UInt_t ipar = 0;ipar < toy->GetNParameters(isyst);ipar++){
              toy->SetToyVariation(isyst,ipar,0.0,1.0); 
            }
            if(syst_counter == toy->GetNToyVariations()) break;
          }
        }

        bool passed = _man.ProcessEvent(*toy,*event,totalWeightSyst,fluxWeightSyst);
	
        // Generated skimmed spline files -> only save events if something passes one of the psyche selections
        // If not, reset the event containers
        if(!passed){

          // Delete the SystBox array
          _man.syst().FinalizeEventSystematics(*event);

          // Reset the PreviousToyBox
          _man.sel().FinalizeEvent(*event);

          evt++;

          continue;
        }

        AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);

        if(!data){
          // Clear all the old graphs
          for(unsigned int i=0; i<dial_graphs.size(); i++){
            (dial_graphs[i])->Clear();
          }

          SelectedSample = summary->EventSample;

          // We save something for each possible selection, so must loop through them
            for(int sample = 0; sample < SampleId::kNSamples; ++sample){
 
                if(summary->TrueVertex[sample] != NULL && summary->EventSample == sample){
                    //Save stuff  
                  AnaParticleMomB* leptonCandidate = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample]);
                  
                  Pmu[sample] = leptonCandidate->Momentum;
                  CosThetamu[sample] = leptonCandidate->DirectionStart[2];
                  TruthVtx[sample] = leptonCandidate->GetTrueParticle()->VertexID; 
                }
                else{
                    //Need event in flat tree for spill information so save empty splines
                    Pmu[sample] = -999;
                    CosThetamu[sample] = -999;
                    TruthVtx[sample] = -999;
                }

                ND::NRooTrackerVtx * vtx = NULL;

                CohTuningWeight[sample] = 1.0;

                int flavor = -999;
                FluxWeight[sample] = 1.0;
                Enu[sample] = -999.;
                ReactionCode[sample] = -999.;
                Double_t weight = 1.0;

                if(Pmu[sample] < 0){
                    for(unsigned int sys_iter=0; sys_iter<nDials; sys_iter++){
                        TGraph* graph = (TGraph*) dial_graphs[sys_iter]->ConstructedAt(sample);
                        graph->SetName(Form("%s_gr",dial_names[sys_iter].c_str()));
                        for(int piter=(graph->GetN()-1); piter>=0; piter--) graph->RemovePoint(piter);
                        graph->SetPoint(0,0.0,1.0);
                    }
                    NeutrinoCode[sample] = -999;
                    TruePmu[sample] = -999;
                    TrueCosThetamu[sample] = -999;
                    Q2[sample] = -999;
                    Q2QE[sample] = -999;
                    TgtMat[sample] = -999;
                    NuParent[sample] = -999;
                    HaveTruth[sample] = false;
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
                if(!vtx || Run==-1){
                    std::cout << "Cannot find NRooTrackerVtx object - skipping weight for this vertex!" << std::endl;;
                    vtx_match = false;
                    for(unsigned int sys_iter=0; sys_iter<nDials; sys_iter++){
                        TGraph* graph = (TGraph*) dial_graphs[sys_iter]->ConstructedAt(sample);
                        graph->SetName(Form("%s_gr",dial_names[sys_iter].c_str()));
                        for(int piter=(graph->GetN()-1); piter>=0; piter--) graph->RemovePoint(piter);
                        graph->SetPoint(0,0.0,1.0);
                    }
                } 
                else{
                    //Save the truth information
                    ReactionCode[sample] = atoi( ((vtx->EvtCode)->String()).Data() );
                    NeutrinoCode[sample] = 0;
                    if     ( vtx->NuParentDecMode < 20) { NeutrinoCode[sample]=  14; flavor=0; }
                    else if( vtx->NuParentDecMode < 30) { NeutrinoCode[sample]= -14; flavor=1; }
                    else if( vtx->NuParentDecMode < 40) { NeutrinoCode[sample]=  12; flavor=2; }
                    else if( vtx->NuParentDecMode < 50) { NeutrinoCode[sample]= -12; flavor=3; }
                    Enu[sample] = vtx->StdHepP4[0][3];
                    FluxWeight[sample] = fluxWeightSyst.Correction;
                    TVector3 nu(vtx->StdHepP4[0][0],vtx->StdHepP4[0][1],vtx->StdHepP4[0][2]);
                    TVector3 mu(vtx->StdHepP4[3][0],vtx->StdHepP4[3][1],vtx->StdHepP4[3][2]);
                    TruePmu[sample] = mu.Mag();
                    TrueCosThetamu[sample] = (nu * mu)/(nu.Mag()*mu.Mag());
                    Q2[sample] = T2KNIWGUtils::Q2(vtx);
                    Q2QE[sample] = T2KNIWGUtils::Q2QE(vtx);
                    TgtMat[sample] = T2KNIWGUtils::TgtA(*vtx);
                    NuParent[sample] = vtx->NuParentPdg;

                    //Add Kendall's pion tuning stuff
                    double piEnergy = -1.0;                    
                    if(fabs(ReactionCode[sample]) == 16){
                        for(int i = 0; i < event->nTrueParticles; ++i){
                            if(fabs(event->TrueParticles[i]->PDG) != 211) continue;
                            if(event->TrueParticles[i]->TrueVertex == NULL) continue;
                            if(event->TrueParticles[i]->VertexID == TruthVtx[sample]){
                                piEnergy = sqrt(event->TrueParticles[i]->Momentum*event->TrueParticles[i]->Momentum + 139.57*139.57)/1000.0;
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
                                if(k < 0) graph->SetPoint(1, 0, weight);
                                else if(k > 0) graph->SetPoint(2, 1, weight);
                                else graph->SetPoint(0, -1, weight);
                            }
                        }
                        else{
                            for(unsigned int dial_iter=0; dial_iter<dial_values[sys_iter].size(); dial_iter++){
                                weight = 1;
                                rw.Systematics().SetTwkDial(dials[sys_iter], dial_values[sys_iter][dial_iter]);
                                rw.Reconfigure();
                                weight = rw.CalcWeight(vtx);
                                if( fabs(weight-1.0) > 1.e-5 ) flat = false;
                                if(sys_iter==nDials-2){
                                    graph->SetPoint(dial_iter, dial_iter, weight);
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
            } // Samples
        }

        //Fill summary tree
        bool selected = false;
        if(data){
            selected = true;
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
    std::cout << "genWeightsFromNRooTracker_BANFF_2016.exe -p psyche_inputfile -o outputfile -r run_type [-n n_events = -1]" << std::endl;
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

