//
// NIWG-oscillation analysis dials for spline generation.
// See TN*** for details
//
// Jim Dobson       - 28/03/2011
// Patrick de Perio - 15/04/2011
// Kendall Mahn - 29/08/2011
// Kendall Mahn - 9/03/2012
// Kendall Mahn - 9/10/2012
// Mark Scott - 06/01/2015
// Jacob Morrison - 09/19/2016
// 

#include <stdlib.h>
#include <cstdlib>

#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TString.h"

#include "T2KReWeight.h"
#include "T2KSyst.h"

#include "T2KGenieReWeight.h" 
#include "T2KNeutReWeight.h"
#include "T2KJNuBeamReWeight.h"
#include "T2KNIWGReWeight.h"
#include "T2KNIWGUtils.h"

#include "AnalysisManager.hxx"

// For weight storer class
#include "T2KWeightsStorer.h"


using std::cout;
using std::cerr;
using std::endl;

using namespace t2krew;

class SelectedVertex{

    public:

        SelectedVertex(int sampleIDVal, ND::NRooTrackerVtx* rtvPointer, double mom){
            sampleID = sampleIDVal;
            rtv = new ND::NRooTrackerVtx(*rtvPointer);
            Momentum = mom/1000.0;
        }
        virtual ~SelectedVertex(){};

        int sampleID;

        double Momentum;

        ND::NRooTrackerVtx* rtv;

};

int fNEvts = -1;
int fSelectAll = 0;
TString fFileName;
std::string fOutfile;

void Usage();
int ParseArgs(int argc, char *argv[]);


int main(int argc, char *argv[])
{

    fOutfile = "NIWG_Outfile.root";

    // process the arguments
    int args = ParseArgs(argc, argv);
    if(args != 0){
        std::cerr << "Usage " << std::endl;
        return 0;
    }

    std::string stringFileName(fFileName.Data());

    double Enu = 0;
    double Q2 = 0;
    int Mode = 0;
    int Target = 0;
    int NuPdg = 0;

    //New Eb dial and PDD dial values
    double Plep = 0.;
    double q0 = 0.;
    double q3 = 0.;
    double ediff = 0.;

    double true_angle = 0.;
    double true_momentum = 0.;
    double Emu = 0.;
    double eneut = 0.;
    double cosang = 0.;
    double erec = 0.;

    TFile* outfile = new TFile(fOutfile.c_str(),"RECREATE");
    TTree* outtree = new TTree("SelectedEventTruth","SelectedEventTruth");
    TTree* ediffouttree = new TTree("EdiffSelectedEventTruth","EdiffSelectedEventTruth");
    outtree->Branch("Enu",&Enu);
    outtree->Branch("Q2",&Q2);
    outtree->Branch("Mode",&Mode);
    outtree->Branch("Target",&Target);
    outtree->Branch("NuPdg",&NuPdg);
    outtree->Branch("Plep",&Plep);
    outtree->Branch("q0",&q0);
    outtree->Branch("q3",&q3);
    ediffouttree->Branch("erec",&erec);
    ediffouttree->Branch("ediff",&ediff);
    ediffouttree->Branch("true_angle",&true_angle);
    ediffouttree->Branch("true_momentum",&true_momentum);
    ediffouttree->Branch("Emu",&Emu);

    TH1F* ccIncMom = new TH1F("CCIncMomNomainl","CCIncMomNominal",1000,0,10);
    TH1F* cc0PiMom = new TH1F("CC0PiMomNominal","CC0PiMomNominal",1000,0,10);
    TH1F* cc1PiMom = new TH1F("CC1PiMomNominal","CC1PiMomNominal",1000,0,10);
    TH1F* ccNPiMom = new TH1F("CCNpiMomNominal","CCNPiMomNominal",1000,0,10);

    TH1F* ccIncMomTuned = new TH1F("CCIncMomTuned","CCIncMomTuned",1000,0,10);
    TH1F* cc0PiMomTuned = new TH1F("CC0PiMomTuned","CC0PiMomTuned",1000,0,10);
    TH1F* cc1PiMomTuned = new TH1F("CC1PiMomTuned","CC1PiMomTuned",1000,0,10);
    TH1F* ccNPiMomTuned = new TH1F("CCNpiMomTuned","CCNPiMomTuned",1000,0,10);

    TH1F* ccIncEnu = new TH1F("CCIncEnuNomainl","CCIncEnuNominal",1000,0,10);
    TH1F* cc0PiEnu = new TH1F("CC0PiEnuNominal","CC0PiEnuNominal",1000,0,10);
    TH1F* cc1PiEnu = new TH1F("CC1PiEnuNominal","CC1PiEnuNominal",1000,0,10);
    TH1F* ccNPiEnu = new TH1F("CCNpiEnuNominal","CCNPiEnuNominal",1000,0,10);

    TH1F* ccIncEnuTuned = new TH1F("CCIncEnuTuned","CCIncEnuTuned",1000,0,10);
    TH1F* cc0PiEnuTuned = new TH1F("CC0PiEnuTuned","CC0PiEnuTuned",1000,0,10);
    TH1F* cc1PiEnuTuned = new TH1F("CC1PiEnuTuned","CC1PiEnuTuned",1000,0,10);
    TH1F* ccNPiEnuTuned = new TH1F("CCNpiEnuTuned","CCNPiEnuTuned",1000,0,10);

    std::string weightName = "_Weights";
    fOutfile.insert(fOutfile.end() - 5, weightName.begin(), weightName.end());

    T2KWeightsStorer *storer = new T2KWeightsStorer(fOutfile.c_str()); //forweightstorer

    cout << "Starting to reweight NIWG events from psyche file: " << fFileName << endl;

    // Load in the NRooTracker vertex tree.
    TFile * infile = new TFile(fFileName, "OPEN");
    if(!infile){
        cerr << "Cannot open input file!" << endl;
        exit(1);
    }

    //Set up an AnalysisManager.
    AnalysisManager _man;

    //Preload the number of events we want.
    int nEvents = fNEvts;
    _man.ReadEvents(stringFileName, nEvents);
    if(fNEvts == -1){
        nEvents = _man.input().GetEntries(); 
    }


    TTree* RTV = (TTree*)infile->Get("NRooTrackerVtx");
    TClonesArray *nRooVtxs = new TClonesArray("ND::NRooTrackerVtx",100);
    int NRooVtx = 0;
    RTV->SetBranchAddress("Vtx",&nRooVtxs);
    RTV->SetBranchAddress("NVtx",&NRooVtx);


    //We only need to process the events once with psyche to know what's
    //worthwhile to pick out in terms of NRooTrackerVertex objects.  So, do the
    //processing now and put them out into an array.
    //Create an array to hold the NRooTrackerVertex objects.
    Int_t nvtxs = 0;
    SelectedVertex** vtxs = new SelectedVertex*[nEvents*50];

    //Set up a nominal toy.
    ToyExperiment *nominalToy = _man.syst().CreateToyExperiment();

    // Fixed 0 sigma for all systematics in the toy.
    for (UInt_t isyst = 0; isyst < nominalToy->GetNSystematics(); isyst++){
        for (UInt_t ipar = 0;ipar<nominalToy->GetNParameters(isyst);ipar++){
            nominalToy->SetVariation(isyst,ipar,0.);
        }
    }

    //Start at the 0th RooTrackerVtx tree entry.
    Int_t rtvi = 0;

    int nfound = 0;

    if(fSelectAll){
        if(nEvents > RTV->GetEntries()) nEvents = RTV->GetEntries();
        for(; rtvi < nEvents; rtvi++){
            ND::NRooTrackerVtx * vtx = NULL;
            RTV->GetEntry(rtvi);

            for(int i = 0; i < NRooVtx; ++i){
                vtx = (ND::NRooTrackerVtx*)(*nRooVtxs)[i];
                nfound++;

                //If the vertex is not null, add it to the array of vertices at
                //position  nvtxs, then increment nvtxs.
                if(vtx != NULL){
                    int sampleID = -1;
                    double mom = 99;
                    vtxs[nvtxs] = new SelectedVertex(sampleID, vtx, mom);
                    nvtxs++;
                }
            }
        }
    }
    else{
        int rtv_entries = RTV->GetEntries();
        for(Long64_t entry=0; entry < nEvents; entry++){
            //Get the event to be modified.
            AnaEventB* event = _man.GetEvent(entry);

            Float_t totalWeightSyst;
            Float_t fluxWeightSyst;

            //Process the event.
            _man.ProcessEvent(*nominalToy,*event,totalWeightSyst,fluxWeightSyst);

            int evt_smpl = event->Summary->EventSample;

            //Only interested if the event passed selection.
            if(evt_smpl != nd280Samples::kUnassigned){
                if(event->Summary->TrueVertex[evt_smpl] != NULL){

                    //What we need to get the RooTrackerVtx is truth vertex identifier
                    //which is given by AnaTrueVertexB::TrueVertexID
                    Int_t TrueVertexID = event->Summary->TrueVertex[evt_smpl]->TruthVertexID;

                    //Use the TruthVertexID to get the RooTrackerVertex corresponding to this
                    //event.
                    ND::NRooTrackerVtx * vtx = NULL;

                    int rtv_curr = rtvi;
                    while(vtx==NULL){
                        //Pull out the correct RooTrackerVtx tree entry.
                        RTV->GetEntry(rtvi);
                        for(int i = 0; i < NRooVtx; ++i){
                            vtx = (ND::NRooTrackerVtx*)(*nRooVtxs)[i];
                            if(vtx->TruthVertexID == TrueVertexID){
                                nfound++;
                                break;
                            }
                            vtx = NULL;
                        }
                        if(vtx==NULL) rtvi++;
                        if(rtvi==rtv_entries){
                            rtvi = rtv_curr;
                            break;
                        }
                    }

                    // Calculate ediff in GeV
                    true_angle    = event->Summary->LeptonCandidate[evt_smpl]->TrueTrack->Direction[2];
                    true_momentum = event->Summary->LeptonCandidate[evt_smpl]->TrueTrack->Momentum/1000; //GeV
                    Emu           = sqrt(true_momentum*true_momentum + .10566*.10566); //GeV
                    eneut         = event->Summary->TrueVertex[evt_smpl]->NuEnergy/1000; //GeV
                    cosang        =  event->Summary->LeptonCandidate[evt_smpl]->TrueTrack->Direction[1] * 0.019197442 + true_angle * 0.999815712;
                    erec          = (.93957*Emu - .10566*.10566/2)/(.93957 - Emu + true_momentum*cosang);
                    ediff         = erec - eneut;
                    ediffouttree->Fill();


                    //If the vertex is not null, add it to the array of vertices at
                    //position  nvtxs, then increment nvtxs.
                    //Want 0 in my plot array to be nominal.
                    if(vtx != NULL){

                        int sampleID = -1;
                        if(evt_smpl == nd280Samples::kFGD1NuMuCC0Pi){
                            sampleID = 0;
                        }
                        else if(evt_smpl == nd280Samples::kFGD1NuMuCC1Pi){
                            sampleID = 1;
                        }
                        else if(evt_smpl == nd280Samples::kFGD1NuMuCCOther){
                            sampleID = 2;
                        }
                        else if(evt_smpl == nd280Samples::kFGD1AntiNuMuCC){
                            sampleID = 3;
                        }
                        else{
                            sampleID = evt_smpl+3;// std::cout << "If you are reading this, this event somehow passed selection but is apparently not in any of the samples." << std::endl;
                        }
                        double mom = 99;//event->Summary->LeptonCandidate[evt_smpl]->Momentum;
                        vtxs[nvtxs] = new SelectedVertex(sampleID, vtx, mom);
                        nvtxs++;
                    }
                }//End true vertex exists check.
            }//End passed check.
        } //end loop over psyche events.
    }
    //So at this point we now have an array of NRooTrackerVertex objects which
    //we can now use for weight calculation.
    std::cout << "Number of vertices to consider: " << nvtxs << std::endl;

    const int ndraws = 9 + (7*21) + 1; // -3 sigma ... +3 sigma (7 variations x 21 dials + 9 tunings + proton FSI fix)
    //const int ndraws = 7*17+9+1; // -3 sigma ... +3 sigma (7 variations, 17 dials + 9 tunings + proton FSI fix)

    //Now make the arrays of plots that we want.
    //One per sample per draw.
    TH1D* pmu[4][ndraws];
    TH1D* enu[4][ndraws];
    char* sample[4];
    sample[0] = "CC0pi";
    sample[1] = "CC1pi";
    sample[2] = "CCOth";
    sample[3] = "AnuCC";

    //Initialize the histograms.
    for(int si = 0; si < 4; si++){
        for(int di = 0; di < ndraws; di++){
            pmu[si][di] = new TH1D(Form("%s_pmu_%d",sample[si],di),Form("%s_pmu_%d",sample[si],di),50, 0.0, 5000.0);
            enu[si][di] = new TH1D(Form("%s_enu_%d",sample[si],di),Form("%s_enu_%d",sample[si],di),50, 0.0, 5000.0);
        }
    }

#ifdef __T2KRW_NIWG_ENABLED__
#ifdef __T2KRW_NEUT_ENABLED__
    // Make a t2kreweighting object and add a NIWG and NEUT weighting engine. 
    t2krew::T2KReWeight rw; 
    rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
    rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());

    //-- WARNING 
    //--    The old Eb dials (kNIWG2014a_Eb_C12/O16) are now
    //--    obsolete and will not be used in future analyses
    //--    and will be removed from the code at a future time.
    //--    In a similar manner, the SCCA and SCCV dials need more
    //--    validation and will not be used in future analyses
    //--    until more validation can be done. They will be 
    //--    removed from the code in the future, if needed.
    //-- WARNING


    // Uncertainties
    // CCQE:
    rw.Systematics().Include(t2krew::kNIWG2014a_pF_C12);
    rw.Systematics().Include(t2krew::kNIWG2014a_pF_O16);
    rw.Systematics().Include(t2krew::kNIWG2014a_Eb_C12);
    rw.Systematics().Include(t2krew::kNIWG2014a_Eb_O16);
    rw.Systematics().Include(t2krew::kNIWGMEC_Norm_C12);
    rw.Systematics().Include(t2krew::kNIWGMEC_Norm_O16);  
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

    rw.Systematics().Include(t2krew::kNIWG_protonFSI_bugfix);

    //-- PDD Weights, New Eb dial
    rw.Systematics().Include(t2krew::kNIWGMEC_PDDWeight_C12);
    rw.Systematics().Include(t2krew::kNIWGMEC_PDDWeight_O16);
    rw.Systematics().Include(t2krew::kNNucl_CCQEBindingEnergy_C12);
    rw.Systematics().Include(t2krew::kNNucl_CCQEBindingEnergy_O16);

    // Absolute tweak dials set the fractional uncertainty, instead of 
    // in units of "sigma", defined in the code.
    // Useful so that you define the uncertainty within the code, as what is
    // hardcoded may not be the same as what is used for analysis. 

    // CCQE:
    rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_C12);
    rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_O16);
    rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_C12);
    rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_Eb_O16);
    rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_Norm_C12);
    rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_Norm_O16);  
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

    rw.Systematics().SetAbsTwk(t2krew::kNIWG_protonFSI_bugfix);

    //-- PDD Weights, New Eb dial
    rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_PDDWeight_C12);
    rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_PDDWeight_O16);
    rw.Systematics().SetAbsTwk(t2krew::kNNucl_CCQEBindingEnergy_C12);
    rw.Systematics().SetAbsTwk(t2krew::kNNucl_CCQEBindingEnergy_O16);

    // loop over total variations, changing each tweak dial accordingly
    for(int dial=0; dial<ndraws; dial++){

        // NIWG 2015
        // Tuning for CCQE

        Double_t pfcdial = 0.;
        Double_t pfodial = 0.;
        Double_t ebcdial = 0.;
        Double_t ebodial = 0.;
        Double_t meccdial = 0.;
        Double_t mecodial = 0.;
        Double_t maqedial = 0.;

        Double_t cadial = 0.;
        Double_t manresdial = 0.;
        Double_t bkgdial = 0.;

        Double_t sccvdial = 0.;
        Double_t sccadial = 0.;
        Double_t ccnuedial = 0.;

        Double_t dismpidial = 0.;
        Double_t cccohdial = 0.;
        Double_t nccohdial = 0.;
        Double_t ncothdial = 0.;

        Double_t mecpddcdial = 0.;
        Double_t mecpddodial = 0.;
        Double_t newebcdial = 0.;
        Double_t newebodial = 0.;
        Double_t protonfsibugfixdial = 1;
        Double_t sf_rfg_dial = 0;
        Double_t vecffccqedial = 402;
        Double_t rpanormdial = 0;
        Double_t rpashapedial = 0;

        switch (dial) {
        case   0:
            sf_rfg_dial = 1;
            vecffccqedial = 2;
            break;

        case   1:
            // Non Rel RPA correction, default tuning
            rpanormdial = 1;
            rpashapedial = -1;
            break;

        case   2:
            maqedial = 0.0; //Nominal is 1.2. Fit result is 1.02+/-0.08. Using 10\% spacing, revisit this.
            //maqedial = -0.15; //Nominal is 1.2. Fit result is 1.02+/-0.08. Using 10\% spacing, revisit this.
            break;

        case   3:
            meccdial = 0.0; //Nominal is 1. Fit result is 0.58+/-0.25. Use 0.25 increments, revisit this.
            //meccdial = -0.42; //Nominal is 1. Fit result is 0.58+/-0.25. Use 0.25 increments, revisit this.
            break;

        case   4:
            pfcdial = 0.0; // Nominal is 217 MeV/c for C12.  Fit result was 239 +/- 16 MeV/c. Apply fractional change of 16/217 = 0.073733.
            //pfcdial = 0.101382488; // Nominal is 217 MeV/c for C12.  Fit result was 239 +/- 16 MeV/c. Apply fractional change of 16/217 = 0.073733.
            break;

        case   5:
            pfcdial = 0.0; // Nominal is 217 MeV/c for C12.  Fit result was 239 +/- 16 MeV/c. Apply fractional change of 16/217 = 0.073733.
            maqedial = 0.0; //Nominal is 1.2. Fit result is 1.02+/-0.08. Using 10\% spacing, revisit this.
            meccdial = 0.0; //Nominal is 1. Fit result is 0.58+/-0.25. Use 0.25 increments, revisit this.
            //pfcdial = 0.101382488; // Nominal is 217 MeV/c for C12.  Fit result was 239 +/- 16 MeV/c. Apply fractional change of 16/217 = 0.073733.
            //maqedial = -0.15; //Nominal is 1.2. Fit result is 1.02+/-0.08. Using 10\% spacing, revisit this.
            //meccdial = -0.42; //Nominal is 1. Fit result is 0.58+/-0.25. Use 0.25 increments, revisit this.
            break;

        case   6:
            sf_rfg_dial = 1;
            vecffccqedial = 2;

            // Non Rel RPA correction, default tuning
            rpanormdial = 1;
            rpashapedial = -1;

            pfcdial = 0.0; // Nominal is 217 MeV/c for C12.  Fit result was 239 +/- 16 MeV/c. Apply fractional change of 16/217 = 0.073733.
            maqedial = 0.0; //Nominal is 1.2. Fit result is 1.02+/-0.08. Using 10\% spacing, revisit this.
            meccdial = 0.0; //Nominal is 1. Fit result is 0.58+/-0.25. Use 0.25 increments, revisit this.
            //pfcdial = 0.101382488; // Nominal is 217 MeV/c for C12.  Fit result was 239 +/- 16 MeV/c. Apply fractional change of 16/217 = 0.073733.
            //maqedial = -0.15; //Nominal is 1.2. Fit result is 1.02+/-0.08. Using 10\% spacing, revisit this.
            //meccdial = -0.42; //Nominal is 1. Fit result is 0.58+/-0.25. Use 0.25 increments, revisit this.
            break;

        case   8:
            // Rel RPA correction
            rpanormdial = 1;
            rpashapedial = 0;
            break;

        // MAQE dial -3 to +3 sigma variations
        case   9:
        case  10:
        case  11:
        case  12: //< 'nominal'
        case  13:
        case  14:
        case  15:
            maqedial = 0.0 + (dial - 12)*(0.08/1.2); // 0.08 uncertainty, but do dial for 10%
            //maqedial = -0.15 + (dial - 12)*(0.08/1.2); // 0.08 uncertainty, but do dial for 10%
            break;

        // Carbon CCQE dials (MEC)
        case  16:
        case  17:
        case  18:
        case  19: //< 'nominal'
        case  20:
        case  21:
        case  22:
            meccdial = 0.0 + (dial - 19)*(0.25/1.0);
            //meccdial = -0.42 + (dial - 19)*(0.25/1.0);
            break;

        // Carbon CCQE dials (pF)
        case  23:
        case  24:
        case  25:
        case  26: //< 'nominal'
        case  27:
        case  28:
        case  29:
            pfcdial = 0.0 + (dial - 26)*(16./239.);
            //pfcdial = 0.101382488 + (dial - 26)*(16./239.);
            break;

        // Carbon CCQE dials (Eb)
        case  30:
        case  31:
        case  32:
        case  33: //< 'nominal'
        case  34:
        case  35:
        case  36:
            ebcdial = 0.0 + (dial - 33)*(9./25.);
            break;

        // Oxygen CCQE dials (MEC)
        case  37:
        case  38:
        case  39:
        case  40: //< 'nominal'
        case  41:
        case  42:
        case  43:
            mecodial = 0.0 + (dial - 40)*(0.25/1.0);
            //mecodial = -0.42 + (dial - 40)*(0.25/1.0);
            break;

        // Oxygen CCQE dials (pF)
        case  44:
        case  45:
        case  46:
        case  47: //< 'nominal'
        case  48:
        case  49:
        case  50:
            pfodial = 0.0 + (dial - 47)*(30./225.);
            break;

        // Oxygen CCQE dials (Eb)
        case  51:
        case  52:
        case  53:
        case  54: //< 'nominal'
        case  55:
        case  56:
        case  57:
            ebodial = 0.0 + (dial - 54)*(9./27.);
            break;

        // CC1Pi dials (CA5)
        case  58:
        case  59:
        case  60:
        case  61: //< 'nominal'
        case  62:
        case  63:
        case  64:
            cadial = 0.0 + (dial - 61)*(0.12/1.01);
            break;

        // CC1Pi dials (manres)
        case  65:
        case  66:
        case  67:
        case  68: //< 'nominal'
        case  69:
        case  70:
        case  71:
            manresdial = 0.0 + (dial - 68)*(0.15/0.95);
            break;

        // CC1Pi dials (bkg)
        case  72:
        case  73:
        case  74:
        case  75: //< 'nominal'
        case  76:
        case  77:
        case  78:
            bkgdial = 0.0 + (dial - 75)*(0.2/1.3);
            break;

        // SSC dials (vector)
        case  79:
        case  80:
        case  81:
        case  82: //< 'nominal'
        case  83:
        case  84:
        case  85:
            sccvdial = 0.0 + (dial - 82)*(4.4);
            break;

        // SSC dials (axial)
        case  86:
        case  87:
        case  88:
        case  89: //< 'nominal'
        case  90:
        case  91:
        case  92:
            sccadial = 0.0 + (dial - 89)*(0.15);
            break;

        // numu/nue dials
        case  93:
        case  94:
        case  95:
        case  96: //< 'nominal'
        case  97:
        case  98:
        case  99:
            ccnuedial = 0.0 + (dial - 96)*(0.02/1.);
            break;

        // DIS dial
        case 100:
        case 101:
        case 102:
        case 103: //< 'nominal'
        case 104:
        case 105:
        case 106:
            dismpidial = 0.0 + (dial - 103)*(0.4/1.);
            break;

        // CC Coherent dial
        case 107:
        case 108:
        case 109:
        case 110: //< 'nominal'
        case 111:
        case 112:
        case 113:
            cccohdial = 0.0 + (dial - 110);//(1/1);
            break;

        // NC Coherent dial
        case 114:
        case 115:
        case 116:
        case 117: //< 'nominal'
        case 118:
        case 119:
        case 120:
            nccohdial = 0.0 + (dial - 117)*(0.3/1.);
            break;

        // NC Other dial
        case 121:
        case 122:
        case 123:
        case 124: //< 'nominal'
        case 125:
        case 126:
        case 127:
            ncothdial = 0.0 + (dial - 124)*(0.3/1.);
            break;

        // New Eb dial on Carbon
        case 128:
        case 129:
        case 130:
        case 131: //< 'nominal'
        case 132:
        case 133:
        case 134:
            newebcdial = 0.0 + (dial - 131)*(9./25.);
            // Also apply SF-RFG reweighting
            sf_rfg_dial = 1; vecffccqedial = 2;
            break;

        // New Eb dial on Oxygen
        case 135:
        case 136:
        case 137:
        case 138: //< 'nominal'
        case 139:
        case 140:
        case 141:
            newebodial = 0.0 + (dial - 138)*(9./27.);
            // Also apply SF-RFG reweighting
            sf_rfg_dial = 1; vecffccqedial = 2;
            break;

        // New MEC PPD dial on Carbon
        case 142:
        case 143:
        case 144:
        case 145: //< 'nominal'
        case 146:
        case 147:
        case 148:
            mecpddcdial = 0.0 + (dial - 145)/3.; //-1, ..., 0, ..., 1
            break;

        // New MEC PPD dial on Oxygen
        case 149:
        case 150:
        case 151:
        case 152: //< 'nominal'
        case 153:
        case 154:
        case 155:
            mecpddodial = 0.0 + (dial - 152)/3.; //-1,0,1
            break;

        case 156:
            protonfsibugfixdial = 1;
            break;

        default:
            break;

        };



        rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,sf_rfg_dial);
        rw.Systematics().SetTwkDial(t2krew::kNXSec_VecFFCCQE, vecffccqedial);
        rw.Systematics().SetTwkDial(t2krew::kNIWG_protonFSI_bugfix, protonfsibugfixdial);

        rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_pF_C12, pfcdial);
        rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_pF_O16, pfodial);
        rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_Eb_C12, ebcdial);
        rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_Eb_O16, ebodial);
        rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_Norm_C12, meccdial);
        rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_Norm_O16, mecodial);
        rw.Systematics().SetTwkDial(t2krew::kNXSec_MaCCQE, maqedial);

        rw.Systematics().SetTwkDial(t2krew::kNXSec_CA5RES, cadial);
        rw.Systematics().SetTwkDial(t2krew::kNXSec_MaNFFRES, manresdial);
        rw.Systematics().SetTwkDial(t2krew::kNXSec_BgSclRES, bkgdial);

        rw.Systematics().SetTwkDial(t2krew::kNXSec_SCCVecQE, sccvdial);
        rw.Systematics().SetTwkDial(t2krew::kNXSec_SCCAxlQE, sccadial);
        rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_ccnueE0, ccnuedial);

        rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_dismpishp, dismpidial);
        rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_cccohE0, cccohdial);
        rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_nccohE0, nccohdial);
        rw.Systematics().SetTwkDial(t2krew::kNIWG2012a_ncotherE0, ncothdial);

        rw.Systematics().SetTwkDial(t2krew::kNNucl_CCQEBindingEnergy_C12, newebcdial);
        rw.Systematics().SetTwkDial(t2krew::kNNucl_CCQEBindingEnergy_O16, newebodial);
        rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_PDDWeight_C12, mecpddcdial);
        rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_PDDWeight_O16, mecpddodial);

        // Relative tweak dials used for the tuning only set for tuning cases.

        rw.Reconfigure();
        storer->NewSystSet(rw.Systematics()); // save the current twk dial values to the weight storer class

        //Loop over the stored NRooTrackerVertex pointers and calculate
        //weights.
        for(int i = 0; i < nvtxs; i++){
            Double_t weight = 1.0;
            if (i % 1000 == 0) {
                const int tenths_complete = static_cast<int>(static_cast<double>(i) / nvtxs * 10);
                std::string progress = "[";
                for (int tenth = 0; tenth != 10; ++tenth) {
                    if (tenth < tenths_complete) { progress += "="; }
                    else                         { progress += " "; }
                }
                progress += "]";
                std::cout << Form("    %s: processing event %6d / %6d for dial %3d\n", progress.c_str(), i, nvtxs, dial);
            }
            try { weight = rw.CalcWeight(vtxs[i]->rtv); }
            catch (const std::runtime_error& e) {
                std::cerr << Form("[%s:L%d] - Error processing event %6d for dial %3d\n", __FILE__, __LINE__, i, dial)
                    << e.what() << "\n";
                throw e;
            }

            if(dial == 6){
                if(vtxs[i]->sampleID == 3){
                    ccIncMom->Fill(vtxs[i]->Momentum);
                    ccIncEnu->Fill(vtxs[i]->rtv->StdHepP4[0][3]);
                    ccIncMomTuned->Fill(vtxs[i]->Momentum, weight);
                    ccIncEnuTuned->Fill(vtxs[i]->rtv->StdHepP4[0][3], weight);
                }
                if(vtxs[i]->sampleID == 0){
                    cc0PiMom->Fill(vtxs[i]->Momentum);
                    cc0PiEnu->Fill(vtxs[i]->rtv->StdHepP4[0][3]);
                    cc0PiMomTuned->Fill(vtxs[i]->Momentum, weight);
                    cc0PiEnuTuned->Fill(vtxs[i]->rtv->StdHepP4[0][3], weight);
                }
                else if(vtxs[i]->sampleID == 1){
                    cc1PiMom->Fill(vtxs[i]->Momentum);
                    cc1PiEnu->Fill(vtxs[i]->rtv->StdHepP4[0][3]);
                    cc1PiMomTuned->Fill(vtxs[i]->Momentum, weight);
                    cc1PiEnuTuned->Fill(vtxs[i]->rtv->StdHepP4[0][3], weight);
                }
                else if(vtxs[i]->sampleID == 2){
                    ccNPiMom->Fill(vtxs[i]->Momentum);
                    ccNPiEnu->Fill(vtxs[i]->rtv->StdHepP4[0][3]);
                    ccNPiMomTuned->Fill(vtxs[i]->Momentum, weight);
                    ccNPiEnuTuned->Fill(vtxs[i]->rtv->StdHepP4[0][3], weight);
                }
            }
            else if(dial == 7){
                Enu    = vtxs[i]->rtv->StdHepP4[0][3];
                Mode   = atoi(vtxs[i]->rtv->EvtCode->GetString());
                Target = vtxs[i]->rtv->StdHepPdg[1];
                NuPdg  = vtxs[i]->rtv->StdHepPdg[0];
                Q2     = T2KNIWGUtils::Q2(vtxs[i]->rtv);
                Plep    = T2KNIWGUtils::Plep(vtxs[i]->rtv);
                q0     = T2KNIWGUtils::q0(vtxs[i]->rtv);
                q3     = T2KNIWGUtils::q3(vtxs[i]->rtv);
                outtree->Fill();
            }
            storer->AddWeight(weight);
        }

        const int tenths_complete = static_cast<int>(static_cast<double>(dial) / ndraws * 10);
        std::string progress = "[";
        for (int tenth = 0; tenth != 10; ++tenth) {
            if (tenth < tenths_complete) { progress += "X"; }
            else                         { progress += " "; }
        }
        progress += "]";
        std::cout << Form("%s: dial %3d complete.\n\n", progress.c_str(), dial);
    } // index of tweak dial changes

    cout << "Outputfile " << fOutfile << " has weight storer tree"  << endl;

    storer->SaveToFile(); // save the weights to a file
#endif // __T2KRW_NIWG_ENABLED__
#endif // __T2KRW_NEUT_ENABLED__
    delete storer; 

    outfile->cd();
    outtree->Write();
    ediffouttree->Write();
    ccIncMom->Write();
    ccIncEnu->Write();
    cc0PiMom->Write();
    cc0PiEnu->Write();
    cc1PiMom->Write();
    cc1PiEnu->Write();
    ccNPiMom->Write();
    ccNPiEnu->Write();

    ccIncMomTuned->Write();
    ccIncEnuTuned->Write();
    cc0PiMomTuned->Write();
    cc0PiEnuTuned->Write();
    cc1PiMomTuned->Write();
    cc1PiEnuTuned->Write();
    ccNPiMomTuned->Write();
    ccNPiEnuTuned->Write();

    outfile->Close();
    return 0;
}

// Print the cmd line syntax
void Usage(){
    cout << "Cmd line syntax should be:" << endl;
    cout << "generateWeightsFromNRooTracker_ND280_NIWG_Validation.exe -s nd280_inputfile -a process_all_vertices -o outputfile.root [-n nevents]" << endl;
}

int ParseArgs(int argc, char **argv){

    for (;;) {
        int c = getopt(argc, argv, "n:s:a:o:");
        if (c < 0)
            break;
        switch (c) {
            case 'n': {
                          std::istringstream tmp(optarg);
                          tmp >> fNEvts;
                          break;
                      }
            case 's': {
                          fFileName = optarg;
                          break;
                      }
            case 'a': {
                          std::istringstream tmp(optarg);
                          tmp >> fSelectAll;
                          break;
                      }
            case 'o': {
                          fOutfile = optarg;
                          break;
                      }
            default:  {
                          std::cerr << "Option " << c << " is not recognized." << std::endl;
                          Usage();
                          exit(1);
                      }
        }
    }
    return 0;
}
