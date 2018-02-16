#include "AnalysisManager.hxx"
#include "ND280AnalysisUtils.hxx"
#include <sys/time.h>
#include <unistd.h>

#include "ToyMakerExample.hxx"
#include <Parameters.hxx>
#include <TCanvas.h>
#include <TH1.h>
#include <TH1F.h>
#include "TRandom3.h"
#include "MultiThread.hxx"
#include "TSystem.h"

std::string InFileName;
std::string OutFileName;

class ToyMakerSigma: public ToyMaker{
  
public:
  
    /// Create the Toy experiment 
    ToyMakerSigma(){
        CurrentSyst=NULL;
        Sigma=-999;
    };
  
    /// Everyone should have a destructor.
    /// Do NOT delete the CurrentSyst here otherwise you'll probably get segfault (and shame)
    virtual ~ToyMakerSigma() {};
  
    void SetCurrentSystematic(SystematicBase* syst) { CurrentSyst = syst; };
    SystematicBase* GetCurrentSystematic() { return CurrentSyst; };
    void SetSigma(double s) { Sigma = s; };
    double GetSigma() { return Sigma; };
  
private:
    SystematicBase* CurrentSyst;
    double Sigma;

    /// Fills the Toy Experiment with a given index
    void FillToyExperiment(ToyExperiment& toy){
        
        if(Sigma == -999){
            std::cerr << "Set Sigma first!!" << std::endl;
            throw;
        }
        if(CurrentSyst == NULL){
            std::cerr << "Set CurrentSyst first!!" << std::endl;
            throw;
        }
        Float_t weight = 1.;
        for (UInt_t isyst = 0; isyst < NMAXSYSTEMATICS; isyst++){
            SystematicBase* syst = _systematics[isyst];
            if (!syst) continue;
            // Create the proper structure for the ToyExperiment adding each of the systematics
            toy.AddToyVariation(syst->GetIndex(), syst->GetNParameters());
            // Loop over parameters for this systematic
            for (UInt_t ipar = 0; ipar < syst->GetNParameters(); ipar++){
                Float_t var = Sigma;
                if (CurrentSyst == syst)
                    toy.SetToyVariation(syst->GetIndex(), ipar, var, weight);
                else
                    toy.SetToyVariation(syst->GetIndex(), ipar, 0., weight);
            }
        }
    };

};


void MakeSigmaVars(){

    std::string paramFile = "";
    int nmax = 100000000;
    std::string inputFileType = "kHighlandTree";
		
    ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);

    // Make sure no parameters have been accessed yet
    ND::params().SetReadParamOverrideFilePointPassed();

    // Parameters to control the systematics
    bool applyVariationSystematics  = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyVariationSystematics");
    bool applyWeightSystematics     = (bool)ND::params().GetParameterI("psycheSteering.Systematics.ApplyWeightSystematics");
    if(!applyVariationSystematics && !applyWeightSystematics) { 
        std::cout<<" no systematics is set to be applied "<<std::endl;
        std::cout << applyVariationSystematics << std::endl;
        std::cout << applyWeightSystematics    << std::endl;
        throw;
    }
	
    AnalysisManager _man;

    // Make and fill the EventSummary even when the selection is not passed.
    if(ND::params().GetParameterI("psycheSteering.Selections.ForceFillEventSummary"))
        _man.sel().SetForceFillEventSummary(true);
    int Run         = -999;
    int SubRun      = -999;
    int EventNumber = -999;
    int Systematic  = -999;
  
    int nVar = 5;
    double Sigma     [5] = {-999,-999,-999,-999,-999};
    int    Selection [5] = {-999,-999,-999,-999,-999};
    double TrueEnu   [5] = {-999,-999,-999,-999,-999};
    int    TrueNuPDG [5] = {-999,-999,-999,-999,-999};
    double LeptonMom [5] = {-999,-999,-999,-999,-999};
    double LeptonCos [5] = {-999,-999,-999,-999,-999};
    double DetWeight [5] = {-999,-999,-999,-999,-999};
    double FluxWeight[5] = {-999,-999,-999,-999,-999};
  
    TFile *OutFile = new TFile(OutFileName.c_str(),"RECREATE");
    OutFile->cd();
    TTree *OutTree = new TTree("Validation", "Validation");
    OutTree->Branch("Run",         &Run,         "Run/I"                );
    OutTree->Branch("SubRun",      &SubRun,      "SubRun/I"             );
    OutTree->Branch("EventNumber", &EventNumber, "EventNumber/I"        );
    OutTree->Branch("Systematic",  &Systematic,  "Systematic/I"         );
    OutTree->Branch("nVar",        &nVar,        "nVar/I"               );
    OutTree->Branch("Sigma",        Sigma,       "Sigma[nVar]/D"        );
    OutTree->Branch("Selection",    Selection,   "Selection[nVar]/I"    );
    OutTree->Branch("TrueEnu",      TrueEnu,     "TrueEnu[nVar]/D"      );
    OutTree->Branch("TrueNuPDG",    TrueNuPDG,   "TrueNuPDG[nVar]/I"    );
    OutTree->Branch("LeptonMom",    LeptonMom,   "LeptonMom[nVar]/D"    );
    OutTree->Branch("LeptonCos",    LeptonCos,   "LeptonCos[nVar]/D"    );
    OutTree->Branch("DetWeight",    DetWeight,   "WeightNom[nVar]/D"    );
    OutTree->Branch("FluxWeight",   FluxWeight,  "FluxWeightNom[nVar]/D");
   
	
    // Initialize the InputManager by specifying the input type and the input file
    if (!_man.input().Initialize(InFileName,inputFileType, false)) throw;
    int nMax = (int)_man.input().GetEntries();
    
    std::vector<SystematicBase*> allSystWithNULL = _man.syst().GetSystematics();
    _man.sel().CreateToyBoxArray(nMax);
    std::cout << "Will loop over " << nMax << " entries" << std::endl;
    
  
    // Create a ToyMaker to configure the toy experiment. Initialize it with a random seed

    Weight_h totalweight;
    Weight_h fluxweight;
    Long64_t entry = 0;
    
    while (entry < nMax) {
      
        if(entry%1000 == 0) std::cout << "Progress " << 100.*entry/nMax << "%" << std::endl;
        // 1. ====================================================================
          // Fill the event structure
        AnaEventB* event;
        
        event = static_cast<AnaEventB*>((_man.LoadSuperEvent(entry))->Event);
        if(!event) continue;
        // Fill the EventBox
        _man.sel().InitializeEvent(*event);
        if (_man.evar().HasEventVariations()){
            // Create the SystBox array (only the first time it is called for each EventVariation)
            _man.evar().Initialize(nMax);
        
            // Initialize The SystBox for EventVariations
            _man.evar().InitializeEvent(_man.sel(),*event);
        }
        
        if (_man.eweight().HasEventWeights()){
            // Create the SystBox array (only the first time it is called for each EventWeight)
            _man.eweight().Initialize(_man.sel(),nMax);
        
            // Initialize The SystBox for variation systematics
            _man.eweight().InitializeEvent(_man.sel(),*event);
        }

        Run =-999;
        SubRun = -999;
        EventNumber =-999;
        // Initialize The SystBox for variation systematics
        for(size_t Syst = 0; Syst < allSystWithNULL.size(); ++Syst){
            if(!allSystWithNULL[Syst])continue;
            if(!allSystWithNULL[Syst]->IsEnabled()) continue;
            Systematic = allSystWithNULL[Syst]->GetIndex();
            for (int iVar = 0; iVar < nVar; ++iVar) {
                Sigma     [iVar] = -2. + iVar;
                Selection [iVar] = -999;
                TrueEnu   [iVar] = -999;
                TrueNuPDG [iVar] = -999;
                LeptonMom [iVar] = -999;
                LeptonCos [iVar] = -999;
                DetWeight [iVar] = -999;
                FluxWeight[iVar] = -999;
                // Run the  nominal selection
                // Create the SystBox array (only the first time it is called for each systematic)
                //_man.syst().Initialize(_man.sel(),nmax);
                
                
                ToyMakerSigma* TheToyMaker = new ToyMakerSigma();
                TheToyMaker->SetSigma(Sigma[iVar]);
                TheToyMaker->SetCurrentSystematic(allSystWithNULL[Syst]);
                TheToyMaker->CreateToyExperiments(1, _man.syst().GetSystematics());
                ToyExperiment* TheToy = TheToyMaker->GetToyExperiment(0);

                bool passed = _man.ProcessEvent(*TheToy, *event, totalweight, fluxweight);
                if(passed){
                  
                    AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(event->Summary);
                
                    if(summary && summary->EventSample){
                        LeptonMom [iVar] = (Double_t)(static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->Momentum);
                        LeptonCos [iVar] = (Double_t)(static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample])->DirectionStart[2]);
                        Selection [iVar] = (Int_t)   summary->EventSample;
                        FluxWeight[iVar] = (Double_t)fluxweight .Correction;
                        DetWeight [iVar] = (Double_t)totalweight.Systematic;
                        EventNumber      = (Int_t)event->EventInfo.Event;
                        Run              = (Int_t)event->EventInfo.Run;
                        SubRun           = (Int_t)event->EventInfo.SubRun;
                        if(summary->TrueVertex[summary->EventSample]){
                            TrueEnu  [iVar] = (Double_t)(summary->TrueVertex[summary->EventSample]->NuEnergy);
                            TrueNuPDG[iVar] = (Int_t)   (summary->TrueVertex[summary->EventSample]->NuPDG   );
                        }
                    }
                }
                
                 /// if you don't do that, GetToyExperiment(0) will be wrong.
                 delete TheToyMaker;
     
            }

            /// Fill only if the tree has soming interesting in it
            if(Selection[0]>0||
               Selection[1]>0||
               Selection[2]>0||
               Selection[3]>0||
               Selection[4]>0){
              OutTree->Fill();
            }
        
        }
        
        // Delete the SystBox for the current event
           if (event->GetIsMC() && _man.evar().GetNEnabledEventVariations()>0)
               _man.evar().FinalizeEvent(*event);
        
           // Delete the SystBox for the current event
           if (event->GetIsMC() && _man.eweight().GetNEnabledEventWeights()>0)
               _man.eweight().FinalizeEvent(*event);
        
           _man.sel().FinalizeEvent(*event);
       }

    OutFile->cd();
    OutTree->Write();
    OutFile->Close();
}



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
            }
        }
    }
}

int main(int argc, char *argv[]){
    ParseArg(argc, argv);
    MakeSigmaVars();
    return 1;
}
