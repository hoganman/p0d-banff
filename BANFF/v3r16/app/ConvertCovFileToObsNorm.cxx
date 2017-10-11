//An app to take the the ND280 detector covariance file format and convert it to
//the file format needed by the BANFF code.
#include "TFile.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include <iostream>
#include "TKey.h"
#include "TAxis.h"
#include <stdlib.h>

//Usage: ConvertCovFileToObsNorm.exe covFile fileWithSampleAxes outputFile
int main(int argc, char** argv){

    if(argc != 4){

        std::cout << "Warning: Unexpected number of arguments." << std::endl;
        exit(0);
    }

    TFile* covInputFile = new TFile(argv[1]);
    TFile* axesInputFile = new TFile(argv[2]);
    TFile* outputFile = new TFile(argv[3], "RECREATE");

    //Get the covariance and mean values from the covInputFile.
    TMatrixDSym* cov = (TMatrixDSym*)(covInputFile->Get("Covariance_Matrix"));
    TVectorD* weights = (TVectorD*)(covInputFile->Get("Mean_Value"));

    //Write these to the output file with the expected names.
    outputFile->cd();
    cov->Write("obsNorm_cov");
    weights->Write("obsNorm_weights");

    //Now, loop through the axes input file, picking out all the TAxis objects
    //and writing them to the obsNorm output file,  just as is done in
    //BinnedToysToObsNormFile.cxx.
    TList* keys = axesInputFile->GetListOfKeys();
    Int_t nKeys = axesInputFile->GetNkeys();

    for(int i = 0; i < nKeys; i++){

        TKey* tmpKey = (TKey*)(keys->At(i));

        //The object is accessed via the key's name.
        std::string keyName(tmpKey->GetName());

        //If "_axis" is in the key name, retrieve it from the input file and then
        //save it to the output file.
        if(keyName.find("_axis") != std::string::npos){

            TAxis* tmpAxis = (TAxis*)axesInputFile->Get(keyName.c_str());
            outputFile->cd();
            tmpAxis->Write(keyName.c_str());
        }
    }


    //Now that we're done with the output file, close it.
    outputFile->Close();

}
