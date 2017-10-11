//This app reads in a BANFF input file (i.e. that contains the summary tree)
//and a flux tuning file, and changes the information in the summary tree to
//the information specified in the flux tuning file, writing the output to the
//supplied output file name.
//This now uses the flux files included with psyche, and allows for selection of Production 5 or
//Production 6 on the command line.

//Usage: ChangeFluxTuning.exe inputFile runperiod outputFile productionNumber

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include <iostream>
#include <stdlib.h>
#include "SampleId.hxx"

int main(int argc, char** argv){

    if(argc < 5){
        std::cout << "Command line options are incorrect." << std::endl;
        std::cout << "Usage: ChangeFluxTuning.exe inputFile runperiod outputFile productionNumber" << std::endl;
        return 1;
    }

    //Open the input file.
    TFile* inputFile = new TFile(argv[1]);

    //Take in the run period to perform the tuning to.  If it is not
    //recognized, exit.
    Int_t Run = -1;
    std::string runPeriod(argv[2]);

    if (runPeriod == "run1"){

        Run = 0;
    }

    else if(runPeriod == "run2"){

        Run = 1;
    }

    else if(runPeriod == "run3b"){

        Run = 2;

    }

    else if(runPeriod == "run3c"){

        Run = 3;

    }

    else if(runPeriod == "run4"){

        Run = 4;

    }

    else if (runPeriod == "run5c"){

        Run = 5;
    
    }

    else{

        std::cout << "Run period not recognized.  Exiting." << std::endl;
        exit(0);

    }

    //Use the production to determine the tuning to use.
    std::string tuning = "tuned11bv3.2";

    if(atoi(argv[4]) == 6){

        tuning = "tuned13av1.1";

    }

    //The flux tunings are stored in psycheUtils.
    std::string psycheUtilsPath(getenv("PSYCHEUTILSROOT"));

    //Read in the flux file information (copy and pasted from the T2KReWeight
    //app.
    char flavor_name[4][20] = {"numu","numub","nue","nueb"};

    //TFile *flux_file[5];
    
    TFile** flux_file = NULL;
    int nPeriods = 0;
    //Production 5
    if(atoi(argv[4]) == 5){
        
        nPeriods = 5;
        flux_file = new TFile*[5];
    
        flux_file[0] = new TFile((psycheUtilsPath + "/data/tuned11bv3.2/run1/nd5_tuned11bv3.2_11anom_run1_fine.root").c_str());
        flux_file[1] = new TFile((psycheUtilsPath + "/data/tuned11bv3.2/run2/nd5_tuned11bv3.2_11anom_run2_fine.root").c_str());
        flux_file[2] = new TFile((psycheUtilsPath + "/data/tuned11bv3.2/run3b/nd5_tuned11bv3.2_11anom_run3b_fine.root").c_str());
        flux_file[3] = new TFile((psycheUtilsPath + "/data/tuned11bv3.2/run3c/nd5_tuned11bv3.2_11anom_run3c_fine.root").c_str());
        flux_file[4] = new TFile((psycheUtilsPath + "/data/tuned11bv3.2/run4/nd5_tuned11bv3.2_11anom_run4_fine.root").c_str());
    }

    //Production 6.
    if(atoi(argv[4]) == 6){

        nPeriods = 6;
        flux_file = new TFile*[6];
 
        flux_file[0] = new TFile((psycheUtilsPath + "/data/tuned13av1.1/run1/nd5_tuned13av1.1_13anom_run1_fine.root").c_str());
        flux_file[1] = new TFile((psycheUtilsPath + "/data/tuned13av1.1/run2/nd5_tuned13av1.1_13anom_run2_fine.root").c_str());
        flux_file[2] = new TFile((psycheUtilsPath + "/data/tuned13av1.1/run3b/nd5_tuned13av1.1_13anom_run3b_fine.root").c_str());
        flux_file[3] = new TFile((psycheUtilsPath + "/data/tuned13av1.1/run3c/nd5_tuned13av1.1_13anom_run3c_fine.root").c_str());
        flux_file[4] = new TFile((psycheUtilsPath + "/data/tuned13av1.1/run4/nd5_tuned13av1.1_13anom_run4_fine.root").c_str());
        flux_file[5] = new TFile((psycheUtilsPath + "/data/tuned13av1.1/run5c/nd5_tuned13av1.1_13anom_run5c_antinumode_fine.root").c_str());

    }

    TH1D *flux_hist[6][4];

    for(int i = 0; i < nPeriods; i++){
        for(int j = 0; j < 4; j++){
            
            //If the file is NULL, it couldn't be found, so print an error and
            //exit.
            if(flux_file[i] == NULL){
                std::cout << "Flux tuning file not found." << std::endl;
                exit(0);
            }

            if(atoi(argv[4]) == 5){
                flux_hist[i][j] = (TH1D*)flux_file[i]->Get(Form("enu_nd5_tuned11b_%s_ratio",flavor_name[j]));
            }
            if(atoi(argv[4]) == 6){
                flux_hist[i][j] = (TH1D*)flux_file[i]->Get(Form("enu_nd5_tuned13a_%s_ratio",flavor_name[j]));
            }
        }
    }


    //Get the input trees.
    TTree* inputSum = (TTree*)(inputFile->Get("sample_sum"));
    TTree* inputFlat = (TTree*)(inputFile->Get("flattree"));
    TTree* inputHeader = (TTree*)(inputFile->Get("header"));


    //Get the information from the summary tree that we need to set the flux
    //tuning.
    //The flux weight we'll be modifying.
    Int_t NSamples = 0;
    Double_t* FluxWeight = new Double_t[SampleId::kNSamples];
    Double_t* Enu = new Double_t[SampleId::kNSamples];
    Int_t* NeutrinoCode = new Int_t[SampleId::kNSamples];
    inputSum->SetBranchAddress("NSamples",&NSamples);
    inputSum->SetBranchAddress("FluxWeight",FluxWeight);
    inputSum->SetBranchAddress("Enu", Enu);
    inputSum->SetBranchAddress("NeutrinoCode",NeutrinoCode);

    //Create the new file, which contains a full clone of both other trees, and
    //an empty clone of the summary tree.
    TFile* outputFile = new TFile(argv[3],"RECREATE");
    TTree* outputSum = inputSum->CloneTree(0);
    TTree* outputFlat = inputFlat->CloneTree();
    TTree* outputHeader = inputHeader->CloneTree();

    for(int i = 0; i < inputSum->GetEntries(); i++){

        inputSum->GetEntry(i);

        //Loop over all entries of the arrays.
        for(int j = 0; j < NSamples; j++){ 

            //Convert the NeutrinoCode to a flavor index.
            Int_t flavor = 0;
            if(NeutrinoCode[j] == 14){
                flavor = 0;
            }
            else if(NeutrinoCode[j] == -14){

                flavor = 1;
            }
            else if(NeutrinoCode[j] == 12){

                flavor = 2;

            }
            else if(NeutrinoCode[j] == -12){

                flavor = 3;
            }


            //If Enu is not set, it defaults to -999.  There will never be a
            //negative value though, so might as well do it this way and not have
            //to worry about floating point error.
            if(Enu[j] < 0){

                FluxWeight[j] = 1.0;

            }

            //Otherwise we have a valid neutrino energy, so should put a real value
            //in/
            else{
                FluxWeight[j] = flux_hist[Run][flavor]->GetBinContent(flux_hist[Run][flavor]->FindBin(Enu[j]));
            }
        }

        outputSum->Fill();
    }


    //Write out the output trees now.
    outputHeader->Write();
    outputFlat->Write();
    outputSum->Write();

    //With all events looped over, close the output file.
    outputFile->Close();

}
