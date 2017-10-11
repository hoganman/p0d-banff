//
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
#include "TObjString.h"
#include <TLorentzVector.h>

#include "T2KReWeight.h"
#include "T2KSyst.h"
#include "TGraph.h"
#include "TSpline.h"

#include "T2KNeutReWeight.h"
#include "T2KNeutUtils.h"
#include "T2KNIWGUtils.h"

#include "T2KJNuBeamReWeight.h"

#include "T2KNIWGReWeight.h"
#include "EventNumuTracker.h"
#include "PropagateTrackerSystematic.h" 
#include "ReadTrackerTree.h" 
#include "Experiments.h"
#include "Latency.hxx"


#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__NRooTrackerVtx.h"
#endif

// For weight storer class
#include "T2KWeightsStorer.h"


using std::cout;
using std::cerr;
using std::endl;

using namespace t2krew;

int fNEvts = -1;
int fId = 0;
char * fNDFileList;
char * fRunPeriod;

void Usage();
void ParseArgs(int argc, char **argv);
int GetPThetaBin(double p, double th);

int main(int argc, char *argv[])
{
  //cout<<"blah"<<endl;
// This example only works when compiled against oaAnalysis
#ifdef __T2KRW_OAANALYSIS_ENABLED__
 ParseArgs(argc, argv);

  
 //Read in the flux tuning need to install the files here
 char flavor_name[4][20] = {"numu","numub","nue","nueb"};
 TFile *flux_file[5];
 flux_file[0] = new TFile("tuned11bv3.2/run1/nd5_tuned11bv3.2_11anom_run1_fine.root");
 flux_file[1] = new TFile("tuned11bv3.2/run2/nd5_tuned11bv3.2_11anom_run2_fine.root");
 flux_file[2] = new TFile("tuned11bv3.2/run3b/nd5_tuned11bv3.2_11anom_run3b_fine.root");
 flux_file[3] = new TFile("tuned11bv3.2/run3c/nd5_tuned11bv3.2_11anom_run3c_fine.root");
 flux_file[4] = new TFile("run4_to0412/nd5_tuned11bv3.2_11anom_run4_fine.root");
 TH1D *flux_hist[5][4];
 for(int i=0; i<5; i++)
   for(int j=0; j<4; j++)
      flux_hist[i][j] = (TH1D*)flux_file[i]->Get(Form("enu_nd5_tuned11b_%s_ratio",flavor_name[j]));
 

 //Set up objects to store the cross section variations
 double ma_spacing[9] = { -1.0, -0.6, -0.3, -0.15, 0., 0.15, 0.3, 0.6, 1.0 };

 std::vector< std::vector<float> > dial_values;
 std::vector< t2krew::ET2KSyst > dials;
 std::vector< std::string > dial_names;
 std::vector< TGraph* > dial_graphs;

 //MAQE dial values;
 std::vector<float> maqe_vec;
 for(int i=0; i<9; i++) maqe_vec.push_back(ma_spacing[i]);
 dial_values.push_back(maqe_vec);
 dials.push_back(t2krew::kNXSec_MaCCQE);
 dial_names.push_back("MAQE");
 TGraph *maqe_graph = new TGraph();
 dial_graphs.push_back(maqe_graph);

 //MARES dial values;
 std::vector<float> mares_vec;
 //for(int i=0; i<7; i++) mares_vec.push_back(0.20*( (double)i-3.0));
 for(int i=0; i<7; i++) mares_vec.push_back(0.20*( (double)i-3.0));
 dial_values.push_back(mares_vec);
 dials.push_back(t2krew::kNXSec_MaRES);
 dial_names.push_back("MARES");
 TGraph *mares_graph = new TGraph();
 dial_graphs.push_back(mares_graph);

 //DIS/Multipi shape dial
 std::vector<float> multipi_vec;
 for(int i=0; i<7; i++) multipi_vec.push_back(0.40*( (double)i-3.0));
 dial_values.push_back(multipi_vec);
 dials.push_back(t2krew::kNIWG2012a_dismpishp);
 dial_names.push_back("DISMPISHP");
 TGraph *multipi_graph = new TGraph();
 dial_graphs.push_back(multipi_graph);

 //Spectral function
 std::vector<float> sf_vec;
 for(int i=0; i<2; i++) sf_vec.push_back((double)i);
 dial_values.push_back(sf_vec);
 dials.push_back(t2krew::kNIWG2012a_sf);
 dial_names.push_back("SF");
 TGraph *sf_graph = new TGraph();
 dial_graphs.push_back(sf_graph);

 //Binding Energy
 std::vector<float> eb_vec;
 for(int i=0; i<3; i++) eb_vec.push_back(0.36*((double)i-1.0));
 dial_values.push_back(eb_vec);
 dials.push_back(t2krew::kNIWG2012a_eb);
 dial_names.push_back("EB");
 TGraph *eb_graph = new TGraph();
 dial_graphs.push_back(eb_graph);

 //Fermi Momentum
 std::vector<float> pf_vec;
 for(int i=0; i<7; i++) pf_vec.push_back(0.14*((double)i-3.0));
 dial_values.push_back(pf_vec);
 dials.push_back(t2krew::kNIWG2012a_pf);
 dial_names.push_back("PF");
 TGraph *pf_graph = new TGraph();
 dial_graphs.push_back(pf_graph);

 //Pionless delta decay
 std::vector<float> pdd_vec;
 for(int i=0; i<5; i++) pdd_vec.push_back(0.2*((double)i-2.0));
 dial_values.push_back(pdd_vec);
 dials.push_back(t2krew::kNNucl_PilessDcyRES);
 dial_names.push_back("PDD");
 TGraph *pdd_graph = new TGraph();
 dial_graphs.push_back(pdd_graph);

 //W shape
 std::vector<float> wshape_vec;
 for(int i=0; i<7; i++) wshape_vec.push_back(0.3*((double)i-3.0));
 dial_values.push_back(wshape_vec);
 dials.push_back(t2krew::kNIWGDeltaMass_width);
 dial_names.push_back("WSHAPE");
 TGraph *wshape_graph = new TGraph();
 dial_graphs.push_back(wshape_graph);

 //Pion Absorption
 std::vector<float> piabs_vec;
 for(int i=0; i<6; i++) piabs_vec.push_back(0.3*((double)i-2.0));
 dial_values.push_back(piabs_vec);
 dials.push_back(t2krew::kNCasc_FrAbs_pi);
 dial_names.push_back("FSI_PI_ABS");
 TGraph *piabs_graph = new TGraph();
 dial_graphs.push_back(piabs_graph);

 //Charge exchange low energy
 std::vector<float> cexlow_vec;
 for(int i=0; i<6; i++) cexlow_vec.push_back(0.4*((double)i-2.0));
 dial_values.push_back(cexlow_vec);
 dials.push_back(t2krew::kNCasc_FrCExLow_pi);
 dial_names.push_back("FSI_CEX_LO");
 TGraph *cexlow_graph = new TGraph();
 dial_graphs.push_back(cexlow_graph);

 //Inelastic low energy
 std::vector<float> inellow_vec;
 for(int i=0; i<6; i++) inellow_vec.push_back(0.4*((double)i-2.0));
 dial_values.push_back(inellow_vec);
 dials.push_back(t2krew::kNCasc_FrInelLow_pi);
 dial_names.push_back("FSI_INEL_LO");
 TGraph *inellow_graph = new TGraph();
 dial_graphs.push_back(inellow_graph);

 //Pion prod
 std::vector<float> piprod_vec;
 for(int i=0; i<6; i++) piprod_vec.push_back(0.4*((double)i-2.0));
 dial_values.push_back(piprod_vec);
 dials.push_back(t2krew::kNCasc_FrPiProd_pi);
 dial_names.push_back("FSI_PI_PROD");
 TGraph *piprod_graph = new TGraph();
 dial_graphs.push_back(piprod_graph);

  
 //Charge exchange high energy
 std::vector<float> cexhi_vec;
 for(int i=0; i<6; i++) cexhi_vec.push_back(0.4*((double)i-2.0));
 dial_values.push_back(cexhi_vec);
 dials.push_back(t2krew::kNCasc_FrCExHigh_pi);
 dial_names.push_back("FSI_CEX_HI");
 TGraph *cexhi_graph = new TGraph();
 dial_graphs.push_back(cexhi_graph);


  //Inelastic high energy
 std::vector<float> inelhi_vec;
 for(int i=0; i<6; i++) inelhi_vec.push_back(0.4*((double)i-2.0));
 dial_values.push_back(inelhi_vec);
 dials.push_back(t2krew::kNCasc_FrInelHigh_pi);
 dial_names.push_back("FSI_INEL_HI");
 TGraph *inelhi_graph = new TGraph();
 dial_graphs.push_back(inelhi_graph);

  //BY dial values;
  std::vector<float> by_corr_vec;
  for(int i=0; i<2; i++) by_corr_vec.push_back((float)i);
  dial_values.push_back(by_corr_vec);
  dials.push_back(t2krew::kNXSec_BYOnOffDIS);
  dial_names.push_back("BY_CORR");
  TGraph *by_corr_graph = new TGraph();
  dial_graphs.push_back(by_corr_graph);
        

 std::vector<float> pdd_mec_vec;
 for(int i=0; i<5; i++) pdd_mec_vec.push_back(0.2*((double)i-2.0));
 dial_values.push_back(pdd_mec_vec);
 dials.push_back(t2krew::kNNucl_PilessDcyOnlyRES);
 dial_names.push_back("PDD_MEC");
 TGraph *pdd_mec_graph = new TGraph();
 dial_graphs.push_back(pdd_mec_graph);

 TGraph *mec_graph = new TGraph();

 //Set up custom summary trees
 Double_t Enu,Pmu,CosThetamu,FluxWeight,DetWeight,POTWeight,TruePmu,TrueCosThetamu,Q2,Q2QE,PileUpWeight; 
 Int_t NeutrinoCode,ReactionCode,TgtMat,NuParent,Run,EventType,TruthVtx;
 Bool_t HaveTruth;

 /*TFile fout(outfile.c_str(),"RECREATE");
 TTree* ssum_tree = new TTree("sample_sum","Tracker Numu Summary" );
 ssum_tree->Branch("Enu",           &Enu,            "Enu/D");
 ssum_tree->Branch("Pmu",           &Pmu,            "Pmu/D");
 ssum_tree->Branch("CosThetamu",    &CosThetamu,     "CosThetamu/D");
 ssum_tree->Branch("ReactionCode",  &ReactionCode,   "ReactionCode/I");
 ssum_tree->Branch("NeutrinoCode",  &NeutrinoCode,   "NeutrinoCode/I");
 ssum_tree->Branch("FluxWeight",    &FluxWeight,     "FluxWeight/D"); 
 ssum_tree->Branch("DetWeight",     &DetWeight,      "DetWeight/D"); 
 ssum_tree->Branch("POTWeight",     &POTWeight,      "POTWeight/D"); 
 ssum_tree->Branch("PileUpWeight",  &PileUpWeight,   "PileUpWeight/D"); 
 ssum_tree->Branch("TruePmu",       &TruePmu,        "TruePmu/D");
 ssum_tree->Branch("TrueCosThetamu",&TrueCosThetamu, "TrueCosThetamu/D");
 ssum_tree->Branch("Q2",            &Q2,             "Q2/D");
 ssum_tree->Branch("Q2QE",          &Q2QE,           "Q2QE/D");
 ssum_tree->Branch("HaveTruth",     &HaveTruth,      "HaveTruth/B");
 ssum_tree->Branch("TgtMat",        &TgtMat,         "TgtMat/I");
 ssum_tree->Branch("NuParent",      &NuParent,       "NuParent/I");
 ssum_tree->Branch("Run",           &Run,            "Run/I");
 ssum_tree->Branch("EventType",     &EventType,      "EventType/I");
 ssum_tree->Branch("TruthVtx",      &TruthVtx,      "TruthVtx/I");
 for(unsigned int i=0; i<dial_graphs.size(); i++){
   ssum_tree->Branch(Form("%sGraph",dial_names[i].c_str()), "TGraph",    &(dial_graphs[i]),  256000,   0);
 }*/



 // Load the oaAnalysis TNRooTrackerVtx tree containing
 /*std::vector<std::string> file_list;
 ifstream ndfiles (fNDFileList);
 if (ndfiles.is_open()) {
   while ( ndfiles.good() ){
     std::string line;
     getline (ndfiles,line);
     if((int)line.size()<5) continue;
     file_list.push_back(line);
     std::cout << "Using file: " << file_list.back() << std::endl;
   }
   ndfiles.close();
 } */
 std::vector<std::string> file_list;
 std::string file(fNDFileList);
 file_list.push_back(file);
 

 // Add NEUT reweighting engine
 t2krew::T2KReWeight rw; 
 rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
 rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());

 // Include cross section parameters
 rw.Systematics().Include(t2krew::kNXSec_MaCCQE);
 rw.Systematics().Include(t2krew::kNXSec_MaRES);
 rw.Systematics().Include(t2krew::kNIWG2012a_dismpishp); 
 rw.Systematics().Include(t2krew::kNIWG2012a_sf);
 rw.Systematics().Include(t2krew::kNIWG2012a_eb);
 rw.Systematics().Include(t2krew::kNIWG2012a_pf);
 rw.Systematics().Include(t2krew::kNNucl_PilessDcyRES);
 rw.Systematics().Include(t2krew::kNIWGDeltaMass_width);
 rw.Systematics().Include(t2krew::kNCasc_FrAbs_pi);
 rw.Systematics().Include(t2krew::kNCasc_FrCExLow_pi);
 rw.Systematics().Include(t2krew::kNCasc_FrInelLow_pi);
 rw.Systematics().Include(t2krew::kNCasc_FrPiProd_pi);
 rw.Systematics().Include(t2krew::kNCasc_FrCExHigh_pi);
 rw.Systematics().Include(t2krew::kNCasc_FrInelHigh_pi);
 rw.Systematics().Include(t2krew::kNNucl_PilessDcyOnlyRES);
 rw.Systematics().Include(t2krew::kNXSec_BYOnOffDIS);
 rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaCCQE); // abs
 rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaRES); // abs
 rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_dismpishp); // abs
 rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_sf); // abs
 rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_eb); // abs
 rw.Systematics().SetAbsTwk(t2krew::kNIWG2012a_pf); // abs
 rw.Systematics().SetAbsTwk(t2krew::kNNucl_PilessDcyRES); //abs
 rw.Systematics().SetAbsTwk(t2krew::kNIWGDeltaMass_width); //abs
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrAbs_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrCExLow_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrInelLow_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrPiProd_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrCExHigh_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNCasc_FrInelHigh_pi);
 rw.Systematics().SetAbsTwk(t2krew::kNNucl_PilessDcyOnlyRES); //abs
 rw.Systematics().SetAbsTwk(t2krew::kNXSec_BYOnOffDIS);

 #ifdef __T2KRW_NEUT_ENABLED__

 // Settings for NEUT
 T2KNeutReWeight * neut_rw_ptr = (T2KNeutReWeight*) rw.WghtEngine("neut_rw");
 ((NReWeightNuXSecCCQE*) neut_rw_ptr->GetWghtCalc("xsec_ccqe"))->SetMode(NReWeightNuXSecCCQE::kModeMa);
 ((neut::rew::NReWeightNuclPiless*) neut_rw_ptr->GetWghtCalc("nucl_piless"))->SetOnlyModeEnu(1.5);

 //Iterate through the file list
 for(int fiter=0; fiter<(int)file_list.size(); fiter++){

    //Create a temporary file with the name of file in it (this is what ReadTrackerTree wants)
    ofstream myfile (Form("tmp_file_%d.txt",fId));
    if (myfile.is_open()){
      myfile << file_list[fiter] << std::endl;
      myfile.close();
    }  
   
   //Create the output file
   std::string outfile(file_list[fiter]);
   outfile.replace(outfile.find(".root"),5,"_spline.root");

   TFile fout(outfile.c_str(),"RECREATE");
   TTree ssum_tree("sample_sum","Tracker Numu Summary" );
   ssum_tree.Branch("Enu",           &Enu,            "Enu/D");
   ssum_tree.Branch("Pmu",           &Pmu,            "Pmu/D");
   ssum_tree.Branch("CosThetamu",    &CosThetamu,     "CosThetamu/D");
   ssum_tree.Branch("ReactionCode",  &ReactionCode,   "ReactionCode/I");
   ssum_tree.Branch("NeutrinoCode",  &NeutrinoCode,   "NeutrinoCode/I");
   ssum_tree.Branch("FluxWeight",    &FluxWeight,     "FluxWeight/D"); 
   ssum_tree.Branch("DetWeight",     &DetWeight,      "DetWeight/D"); 
   ssum_tree.Branch("POTWeight",     &POTWeight,      "POTWeight/D"); 
   ssum_tree.Branch("PileUpWeight",  &PileUpWeight,   "PileUpWeight/D"); 
   ssum_tree.Branch("TruePmu",       &TruePmu,        "TruePmu/D");
   ssum_tree.Branch("TrueCosThetamu",&TrueCosThetamu, "TrueCosThetamu/D");
   ssum_tree.Branch("Q2",            &Q2,             "Q2/D");
   ssum_tree.Branch("Q2QE",          &Q2QE,           "Q2QE/D");
   ssum_tree.Branch("HaveTruth",     &HaveTruth,      "HaveTruth/B");
   ssum_tree.Branch("TgtMat",        &TgtMat,         "TgtMat/I");
   ssum_tree.Branch("NuParent",      &NuParent,       "NuParent/I");
   ssum_tree.Branch("Run",           &Run,            "Run/I");
   ssum_tree.Branch("EventType",     &EventType,      "EventType/I");
   ssum_tree.Branch("TruthVtx",      &TruthVtx,      "TruthVtx/I");
   for(unsigned int i=0; i<dial_graphs.size(); i++){
     ssum_tree.Branch(Form("%sGraph",dial_names[i].c_str()), "TGraph",    &(dial_graphs[i]),  256000,   0);
   }
   ssum_tree.Branch("MECGraph", "TGraph",  &(mec_graph),  256000,   0);

   TFile fmec("martini_nieves_ratio.root");
   TGraph *mec_ratio = (TGraph*)fmec.Get("mec_ratio");

   //Read in the tracker tree
   std::vector< EventNumuTracker > events; 
   std::cout << "Reading tracker tree" << std::endl;
   ReadTrackerTree run2(Form("tmp_file_%d.txt",fId),events,fRunPeriod,false,true,6e20,4); 
   Run = 0;
   if(string(fRunPeriod)=="2a" || string(fRunPeriod)=="2w") Run=1;
   else if(string(fRunPeriod)=="3b" ) Run=2;
   else if(string(fRunPeriod)=="3c" ) Run=3;
   else if(string(fRunPeriod)=="4a" ) Run=4;
   else if(string(fRunPeriod)=="4w" ) Run=4;
   else if(string(fRunPeriod)=="sand") Run=-1;
   else if(string(fRunPeriod)=="mec") Run=-2;

   std::cout << "For Run Period " << fRunPeriod << " use flux histograms " << Run << std::endl;
   
   std::cout << "Finished Reading tracker tree" << std::endl;
  
   //Iterate through the events
   for( unsigned int i = 0; i < events.size(); i++ ) {
   //for( unsigned int i = 0; i < 100; i++ ) {

     if(i%100==0) 
     std::cout << "Event number " << i << std::endl;

     //Save stuff   
     Double_t weight = 1.0;
     Pmu = events[i].Orig.muon_mom;
     CosThetamu = events[i].Orig.muon_costheta;
     EventType = 0;
     if( events[i].Orig.cutCCinc==1 && events[i].Orig.cutCC0pi==1 ) EventType = 1;
     else if( events[i].Orig.cutCCinc==1 && events[i].Orig.cutCC1pi==1 ) EventType = 2;
     else if( events[i].Orig.cutCCinc==1 && events[i].Orig.cutCCoth==1 ) EventType = 3;
     int flavor = 0;
     FluxWeight = 1.0;
     Enu = 0.;
     ReactionCode = -999.;
     TruthVtx = events[i].Stat.TruthVertexID;    

     //Get the nRooTrackerVtx
     ND::NRooTrackerVtx * vtx = (ND::NRooTrackerVtx*)(events[i].NRooTrackerVtx);
     //if(vtx)
     //  std::cout << "VTX test " << TruthVtx << " " <<  vtx->NuParentDecMode << std::endl;
     bool vtx_match = true;
     if(!vtx || Run==-1){
        cerr << "Cannot find NRooTrackerVtx object - skipping weight for this vertex!";
        vtx_match = false;
        for(unsigned int sys_iter=0; sys_iter<dial_names.size(); sys_iter++){
           dial_graphs[sys_iter]->SetName(Form("%s_gr",dial_names[sys_iter].c_str()));
           for(int piter=(dial_graphs[sys_iter]->GetN()-1); piter>=0; piter--) dial_graphs[sys_iter]->RemovePoint(piter);
           dial_graphs[sys_iter]->SetPoint(0,0.0,1.0);
        }
        mec_graph->SetName("mec_gr");
        for(int piter=(mec_graph->GetN()-1); piter>=0; piter--) mec_graph->RemovePoint(piter);
        mec_graph->SetPoint(0,0.0,1.0);
     } else {

       //Save the truth information
       ReactionCode = atoi( ((vtx->EvtCode)->String()).Data() );
       //std::cout << ReactionCode << std::endl;
       NeutrinoCode = 0;
       if( vtx->NuParentDecMode < 20) { NeutrinoCode=14; flavor=0; }
       else if( vtx->NuParentDecMode < 30) { NeutrinoCode=-14; flavor=1; }
       else if( vtx->NuParentDecMode < 40) { NeutrinoCode=12; flavor=2; }
       else if( vtx->NuParentDecMode < 50) { NeutrinoCode=-12; flavor=3; }
       Enu = vtx->StdHepP4[0][3];
       int Run1 = Run;
       if(Run1==-2) Run1=4;
       FluxWeight = flux_hist[Run1][flavor]->GetBinContent( flux_hist[Run1][flavor]->FindBin(Enu));
       TVector3 nu(vtx->StdHepP4[0][0],vtx->StdHepP4[0][1],vtx->StdHepP4[0][2]);
       TVector3 mu(vtx->StdHepP4[3][0],vtx->StdHepP4[3][1],vtx->StdHepP4[3][2]);
       TruePmu = mu.Mag();
       TrueCosThetamu = (nu * mu)/(nu.Mag()*mu.Mag());
       Q2 = T2KNIWGUtils::Q2(vtx);
       Q2QE = T2KNIWGUtils::Q2QE(vtx);
       TgtMat = T2KNIWGUtils::TgtA(*vtx);
       NuParent = vtx->NuParentPdg;
     
       if(Run>=0){ 
       //Iterate through the systematics to make splines 
       for(unsigned int sys_iter=0; sys_iter<dial_names.size(); sys_iter++){

          //Initialize parameters to nominal
          for(unsigned int sys_iter1=0; sys_iter1<dial_names.size(); sys_iter1++) 
            if(dials[sys_iter1]==t2krew::kNXSec_BYOnOffDIS)
              rw.Systematics().SetTwkDial(dials[sys_iter1], 1.);
            else
              rw.Systematics().SetTwkDial(dials[sys_iter1], 0.);


          //Set name of TGraph
          dial_graphs[sys_iter]->SetName(Form("%s_gr",dial_names[sys_iter].c_str()));

          //Iterate through dial of interest
          for(int piter=(dial_graphs[sys_iter]->GetN()-1); piter>=0; piter--) dial_graphs[sys_iter]->RemovePoint(piter);
          bool flat = true;
          for(unsigned int dial_iter=0; dial_iter<dial_values[sys_iter].size(); dial_iter++){
            rw.Systematics().SetTwkDial(dials[sys_iter], dial_values[sys_iter][dial_iter]);
            rw.Reconfigure();
            weight = rw.CalcWeight(vtx);
            if( fabs(weight-1.0) > 1.e-5 ) flat = false;
            dial_graphs[sys_iter]->SetPoint(dial_iter, dial_values[sys_iter][dial_iter], weight);
          } 
          //If the response is flat, only save a single point  
          if(flat){
            for(int piter=(dial_graphs[sys_iter]->GetN()-1); piter>=0; piter--) dial_graphs[sys_iter]->RemovePoint(piter);
            dial_graphs[sys_iter]->SetPoint(0,0.0,1.0);
          }
        }  
        }
        if(Run==-2){
          double mweight = 1.0;
          if(Enu>1.5) mweight = 0.0; 
          mec_graph->SetName("mec_gr");
          for(int piter=(mec_graph->GetN()-1); piter>=0; piter--) mec_graph->RemovePoint(piter);
          mec_graph->SetPoint(0,-1.0,0.0);
          mec_graph->SetPoint(1,0.0,mweight);
          double rweight = 0.0;
          if(Enu>0.2 && Enu<1.5) rweight = mec_ratio->Eval(Enu,0,"S");
          mec_graph->SetPoint(2,1.0, rweight);
          //for(int piter=(pdd_mec_graph->GetN()-1); piter>=0; piter--) pdd_mec_graph->RemovePoint(piter);
          //pdd_mec_graph->SetPoint(0,0.0,1.0);
          for(unsigned int sys_iter=0; sys_iter<dial_names.size(); sys_iter++){
             dial_graphs[sys_iter]->SetName(Form("%s_gr",dial_names[sys_iter].c_str()));
             for(int piter=(dial_graphs[sys_iter]->GetN()-1); piter>=0; piter--) dial_graphs[sys_iter]->RemovePoint(piter);
             dial_graphs[sys_iter]->SetPoint(0,0.0,1.0);
          }
        } else {
          mec_graph->SetName("mec_gr");
          for(int piter=(mec_graph->GetN()-1); piter>=0; piter--) mec_graph->RemovePoint(piter);
          mec_graph->SetPoint(0,0.0,1.0);
        }
      

        //std::cout << "Finished splines" << std::endl;

     } // vertex container
     if( !vtx_match ){
       std::cout << "Could not match NRooTrackerVtx" << std::endl;
       HaveTruth = false;
     } 

     //Fill summary tree
     ssum_tree.Fill();

   } // numner of events

  TFile fin(file_list[fiter].c_str());
  //TTree *nrtvtx = (TTree*)fin.Get("NRooTrackerVtx");
  TTree *def = (TTree*)fin.Get("default");
  TTree *hdr = (TTree*)fin.Get("header");
 
  fout.cd();

  //TFile fout(outfile.c_str(),"RECREATE");
  //TTree *nrtvtx1 = nrtvtx->CloneTree();
  TTree *def1 = def->CloneTree();
  TTree *hdr1 = hdr->CloneTree();
  ssum_tree.Write("sample_sum");
  //nrtvtx1->Write("NRooTrackerVtx");
  def1->Write("default");
  hdr1->Write("header");
  fout.Close();

  fin.Close();
  //ssum_tree->Reset();
   
 
 } // file loop
 

#endif
 //fsum.cd();
 //ssum_tree->Write("sample_sum");
 //fsum.Close();

 //storer->SaveToFile(); // save the weights to a file
 //delete storer; 

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
    else if(string(argv[i]) == "-d") fId = std::atoi(argv[i+1]);
    else if(string(argv[i]) == "-e") fNEvts = std::atoi(argv[i+1]);
    else if(string(argv[i]) == "-r") fRunPeriod = argv[i+1];
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
