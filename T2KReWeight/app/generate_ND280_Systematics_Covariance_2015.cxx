//
// 2015 NIWG-oscillation analysis dials for spline generation.
// See TN192 for details
//
// Jim Dobson       - 28/03/2011
// Patrick de Perio - 15/04/2011
// Kendall Mahn - 29/08/2011
// Kendall Mahn - 9/03/2012
// Kendall Mahn - 9/10/2012
// Mark Scott - 06/01/2015
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
int fNThrows = 1;
TString fFileName;
std::string fOutfile;

void Usage();
int ParseArgs(int argc, char *argv[]);


int main(int argc, char *argv[]){

    fOutfile = "ND280_Covariance_2015_Outfile.root";

    // process the arguments
    int args = ParseArgs(argc, argv);
    if(args != 0){
        std::cerr << "Usage " << std::endl;
        return 0;
    }

    std::string stringFileName(fFileName.Data());

    TFile* outfile = new TFile(fOutfile.c_str(),"RECREATE");
    TTree* outtree = new TTree("SelectedEventTruth","SelectedEventTruth");

    int nThrows = fNThrows + 2;
    float* PMu = new float[nThrows];
    float* ThetaMu = new float[nThrows];
    float* DetWeight = new float[nThrows];
    float* PileUpWeight = new float[nThrows];
    float* FluxWeight = new float[nThrows];
    float* NIWGWeight = new float[nThrows];
    int* Selection = new int[nThrows];

    for( int i = 0; i < nThrows; ++i){
        PileUpWeight[i] = 1;
        FluxWeight[i] = 1;
        DetWeight[i] = 1;
        NIWGWeight[i] = 1;
        PMu[i] = -999;
        ThetaMu[i] = -999;
        Selection[i] = 0;
    }

    outtree->Branch("nThrows", &nThrows, "nThrows/I");
    outtree->Branch("PMu",PMu, "PMu[nThrows]/F");
    outtree->Branch("ThetaMu",ThetaMu,"ThetaMu[nThrows]/F");
    outtree->Branch("DetWeight",DetWeight,"DetWeight[nThrows]/F");
    outtree->Branch("PileUpWeight",PileUpWeight,"PileUpWeight[nThrows]/F");
    outtree->Branch("FluxWeight",FluxWeight,"FluxWeight[nThrows]/F");
    outtree->Branch("NIWGWeight",NIWGWeight,"NIWGWeight[nThrows]/F");
    outtree->Branch("Selection",Selection,"Selection[nThrows]/I");

    std::cout << "Starting to reweight events from psyche file: " << fFileName << std::endl;

    // Load in the NRooTracker vertex tree.
    TFile * infile = new TFile(fFileName, "OPEN");
    if(!infile){
        std::cerr << "Cannot open input file!" << std::endl;
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


    bool applySyst[17]={false};                    
    applySyst[0]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableBFieldDist");
    applySyst[1]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableMomScale");
    applySyst[2]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableMomResol");
    applySyst[3]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableTpcPid");
    applySyst[4]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableFgdPid");
    applySyst[5]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableChargeConf");
    applySyst[6]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableTpcClusterEff");
    applySyst[7]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableTpcTrackEff");
    applySyst[8]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableTpcFgdMatchEff");
    applySyst[9]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableFgdTrackEff");
    applySyst[10]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableFgdHybridTrackEff");
    applySyst[11]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableMichelEleEff");
    applySyst[12]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnablePileUp");
    applySyst[13]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableFgdMass");
    applySyst[14]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableOOFV");
    applySyst[15]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableSIPion");
    applySyst[16]= (bool)ND::params().GetParameterI("psycheSteering.Systematics.EnableSandMuons");

    int n;
    SystematicVariationBase** varSyst = _man.syst().GetVariationSystematics(n);
    SystematicWeightBase** weightSyst = _man.syst().GetWeightSystematics(n);
    UInt_t nw=_man.syst().GetNWeightSystematics();
    UInt_t nv=_man.syst().GetNVariationSystematics();
    for(unsigned int i = 0; i < nw; i++){
        SystematicWeightBase* tmp = weightSyst[i];
        tmp->SetPDF(SystematicBase::kGaussian);
    }
    for(unsigned int i = 0; i < nv; i++){
        SystematicVariationBase* tmp = varSyst[i];
        if(tmp->Name()=="kBFieldDist"){
            tmp->SetPDF(SystematicBase::kUniform);
        }
        else{
            tmp->SetPDF(SystematicBase::kGaussian);
        }
    }

    //Set up a nominal toy.
    std::vector<double> systpull[nThrows][nw+nv];
    ToyExperiment toy = (*_man.syst().CreateToyExperiment());
    TRandom3 random(0);

    /// Define the throws and generate them 
    for (Int_t itoy= 0; itoy<nThrows;itoy++){
        for(unsigned int isyst = 0; isyst < nv; isyst++){
            SystematicVariationBase* tmp = varSyst[isyst];
            for (UInt_t ipar = 0;ipar<toy.GetNParameters(isyst);ipar++){
                Float_t var;
                if(tmp->PDF()==SystematicBase::kUniform){
                    var=random.Uniform(0.,1.);
                }
                else{
                    var=random.Gaus(0.,1.);
                }
                if(itoy<2) var = 0;
                systpull[itoy][isyst].push_back(var);
            }
        }
        for(unsigned int isyst = 0; isyst < nw; isyst++){
            SystematicWeightBase* tmp = weightSyst[isyst];
            for (UInt_t ipar = 0;ipar<toy.GetNParameters(isyst+nv);ipar++){
                Float_t var;
                if(tmp->PDF()==SystematicBase::kUniform){
                    var=random.Uniform(0.,1.);
                }
                else{
                    var=random.Gaus(0.,1.);
                }
                if(itoy<2) var = 0;
                systpull[itoy][isyst+nv].push_back(var);
            }
        }
    }

#ifdef __T2KRW_NIWG_ENABLED__
#ifdef __T2KRW_NEUT_ENABLED__
    // Make a t2kreweighting object and add a NIWG and NEUT weighting engine. 
    t2krew::T2KReWeight rw; 
    rw.AdoptWghtEngine("neut_rw", new t2krew::T2KNeutReWeight());
    rw.AdoptWghtEngine("niwg_rw", new t2krew::T2KNIWGReWeight());

    // Uncertainties
    // CCQE:
    rw.Systematics().Include(t2krew::kNIWG2014a_pF_C12);
    rw.Systematics().Include(t2krew::kNIWGMEC_Norm_C12);
    rw.Systematics().Include(t2krew::kNIWGMEC_Norm_O16);  
    rw.Systematics().Include(t2krew::kNXSec_MaCCQE);
    rw.Systematics().Include(t2krew::kNXSec_VecFFCCQE); // used to set MAQE to act according to for RFG MC (2) or SF MC (402)
    rw.Systematics().Include(t2krew::kNIWG2014a_SF_RFG);
    rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_norm);
    rw.Systematics().Include(t2krew::kNIWG_rpaCCQE_shape);
    rw.Systematics().Include(t2krew::kNIWG_protonFSI_bugfix);
    // Absolute tweak dials set the fractional uncertainty, instead of 
    // in units of "sigma", defined in the code.
    // Useful so that you define the uncertainty within the code, as what is
    // hardcoded may not be the same as what is used for analysis. 

    // CCQE:
    rw.Systematics().SetAbsTwk(t2krew::kNIWG2014a_pF_C12);
    rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_Norm_C12);
    rw.Systematics().SetAbsTwk(t2krew::kNIWGMEC_Norm_O16);  
    rw.Systematics().SetAbsTwk(t2krew::kNXSec_MaCCQE);
    rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_norm);
    rw.Systematics().SetAbsTwk(t2krew::kNIWG_rpaCCQE_shape);
    rw.Systematics().SetAbsTwk(t2krew::kNIWG_protonFSI_bugfix);

    rw.Systematics().SetTwkDial(t2krew::kNIWG_protonFSI_bugfix, 0);
    rw.Systematics().SetTwkDial(t2krew::kNXSec_VecFFCCQE, 2); // Note that when we set MAQE, we need to set NXSec_VecFFCCQE to 2 for SF->RFG MC
    rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_SF_RFG,1); // SF->RFG
    rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_norm,1); // RPA correction
    rw.Systematics().SetTwkDial(t2krew::kNIWG_rpaCCQE_shape,0); // Relativistic (0), Non-relativistic (-1)
//    rw.Systematics().SetTwkDial(t2krew::kNIWG2014a_pF_C12, (223.-217.)/217.);
//    rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_Norm_C12, (0.27-1.0)/1.0);
//    rw.Systematics().SetTwkDial(t2krew::kNIWGMEC_Norm_O16, (0.27-1.0)/1.0);  
//    rw.Systematics().SetTwkDial(t2krew::kNXSec_MaCCQE, (1.15-1.2)/1.2);

    rw.Reconfigure();

    int prevTruthID = -1;
    float prevNIWGWeight = -1.;
    int prevRTV = -1;

    for(Long64_t entry=0; entry < nEvents; entry++){

        int nSelected = 0;
        //Start at the 0th RooTrackerVtx tree entry.
        Int_t rtvi = 0;

        for (Int_t itoy= 0; itoy<nThrows;itoy++){
            if(itoy == 0){
                for(int i = 5; i < 17; ++i){
                    _man.syst().DisableSystematic(i);
                }
            }
            if(itoy==1){
                for(UInt_t jsyst = 5; jsyst<toy.GetNVariationSystematics()+toy.GetNWeightSystematics();jsyst++){
                    if(applySyst[jsyst]){
                        _man.syst().EnableSystematic(jsyst);
                    }
                }
            }
            for (UInt_t jsyst = 0; jsyst<toy.GetNVariationSystematics()+toy.GetNWeightSystematics();jsyst++){
                for (UInt_t ipar = 0;ipar<toy.GetNParameters(jsyst);ipar++){
                    toy.SetVariation(jsyst,ipar,systpull[itoy][jsyst][ipar]);
                }
            }

            PileUpWeight[itoy] = 1;
            FluxWeight[itoy] = 1;
            DetWeight[itoy] = 1;
            NIWGWeight[itoy] = 1;
            PMu[itoy] = -999;
            ThetaMu[itoy] = -999;

            //Get the event to be modified.
            AnaEventB* event = _man.GetEvent(entry);

            Float_t totalWeightSyst;
            Float_t fluxWeightSyst;

            //Process the event.
            _man.ProcessEvent(toy,*event,totalWeightSyst,fluxWeightSyst);

            Selection[itoy] = event->Summary->EventSample;

            //Only interested if the event passed selection.
            if(event->Summary->EventSample != nd280Samples::kUnassigned){
                if(event->Summary->TrueVertex[event->Summary->EventSample] != NULL){

                    PMu[itoy] = event->Summary->LeptonCandidate[event->Summary->EventSample]->Momentum;
                    ThetaMu[itoy] = event->Summary->LeptonCandidate[event->Summary->EventSample]->DirectionStart[2];
                    PileUpWeight[itoy] = event->Weight;
                    FluxWeight[itoy] = fluxWeightSyst;
                    DetWeight[itoy] = totalWeightSyst;

                    //What we need to get the RooTrackerVtx is truth vertex identifier
                    //which is given by AnaTrueVertexB::TrueVertexID
                    Int_t TrueVertexID = event->Summary->TrueVertex[event->Summary->EventSample]->TruthVertexID;

                    if(TrueVertexID == prevTruthID){
                        NIWGWeight[itoy] = prevNIWGWeight;
                        nSelected++;
                        continue;
                    }

                    if(event->EventInfo.IsSand){
                        NIWGWeight[itoy] = 1;
                        nSelected++;
                        continue;
                    }

                    //Use the TruthVertexID to get the RooTrackerVertex corresponding to this
                    //event.
                    ND::NRooTrackerVtx * vtx = NULL;
                    prevTruthID = TrueVertexID;
                    prevRTV = rtvi;
                    while(vtx==NULL){
                        //Pull out the correct RooTrackerVtx tree entry.
                        RTV->GetEntry(rtvi);
                        for(int i = 0; i < NRooVtx; ++i){
                            vtx = (ND::NRooTrackerVtx*)(*nRooVtxs)[i];
                            if(vtx->TruthVertexID == TrueVertexID){
                                break;
                            }
                            vtx = NULL;
                        }
                        if(vtx==NULL) rtvi++;
                        if(rtvi == RTV->GetEntries()){
                            rtvi = prevRTV;
                            break;
                        }
                    }
                    if(vtx != NULL){
                        NIWGWeight[itoy] = rw.CalcWeight(vtx);
                    }
                    prevNIWGWeight = NIWGWeight[itoy];
                }//End true vertex exists check.
                nSelected++;
            }//End passed check.
        } //end loop over psyche events.
        if(nSelected>nThrows/200){
            outtree->Fill();
        }

    }
#endif
#endif

    outfile->cd();
    outtree->Write();
    outfile->Close();
    return 0;
}

// Print the cmd line syntax
void Usage(){
    std::cout << "Cmd line syntax should be:" << std::endl;
    std::cout << "generate_ND280_Systematics_Covariance_2015.cxx -s nd280_inputfile -t nThrows -o outputfile.root [-n nevents]" << std::endl;
}

int ParseArgs(int argc, char **argv){

    for (;;) {
        int c = getopt(argc, argv, "n:s:t:o:");
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
            case 't': {
                          std::istringstream tmp(optarg);
                          tmp >> fNThrows;
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
