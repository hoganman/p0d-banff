//An app to take the the ND280 detector covariance file format and convert it to
//the file format needed by the BANFF code.
#include "TFile.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include "TKey.h"
#include "TAxis.h"
#include "TDirectory.h"

#include <stdlib.h>
#include <iostream>

//Usage: ConvertCovFileToObsNorm.exe covFile.root outputFile.root
int main(int argc, char** argv){

  if(argc != 3){
    std::cerr << "Warning: Unexpected number of arguments." << std::endl;
    std::cerr << "USAGE: ConvertCovFileToObsNorm.exe CovFile.root OutputFile.root" << std::endl;
    std::cerr << "       Where CovFile.root has all the axis in it" << std::endl;
    throw;
  }

  TFile* InputFile  = new TFile(argv[1], "READ");
  TFile* OutputFile = new TFile(argv[2], "RECREATE");

  //Get the covariance and mean values from the covInputFile.
  TMatrixDSym* cov     = (TMatrixDSym*)(InputFile->Get("Covariance_Matrix"));
  TVectorD*    weights = (TVectorD*)   (InputFile->Get("Mean_Value"));

  //Write these to the output file with the expected names.
  OutputFile->cd();
  cov    ->Write("obsNorm_cov");
  weights->Write("obsNorm_weights");

  //Now, loop through the axes input file, picking out all the TAxis objects
  //and writing them to the obsNorm output file,  just as is done in
  //BinnedToysToObsNormFile.cxx.
  TDirectory* AxisCovDir = (TDirectory*)InputFile->Get("AxisForCov");
  TList* keys  = AxisCovDir->GetListOfKeys();
  Int_t  nKeys = AxisCovDir->GetNkeys();

  for(int i = 0; i < nKeys; i++){

    TKey* tmpKey = (TKey*)(keys->At(i));

    //The object is accessed via the key's name.
    std::string keyName(tmpKey->GetName());

    //If "_axis" is in the key name, retrieve it from the input file and then
    //save it to the output file.
    if(keyName.find("_axis") != std::string::npos){

      TAxis* tmpAxis = (TAxis*)AxisCovDir->Get(keyName.c_str());
      OutputFile->cd();
      tmpAxis->Write(keyName.c_str());
    }
  }
  
  //Now that we're done with the output file, close it.
  OutputFile->Close();

}
