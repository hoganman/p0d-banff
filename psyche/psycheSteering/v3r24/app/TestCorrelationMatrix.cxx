#include "ToyMakerCorrelatedSyst.hxx"
#include "AnalysisManager.hxx"
#include "TRandom.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TMath.h"





int main(int argc, char *argv[]){

  ND::params().LoadParametersFiles(anaUtils::GetPackageHierarchy(), true);
  ND::params().SetReadParamOverrideFilePointPassed();

  if(argc != 3){
    std::cerr << "USAGE: ./TestCorrelationMatrix input.xml output.root" << std::endl;
    throw;
  }
  std::string XMLFileName = argv[1];
  std::string OutputName = argv[2];
  
  AnalysisManager* m = new AnalysisManager();
  ToyMakerCorrelatedSyst* tm = new ToyMakerCorrelatedSyst(123,m->syst());
  tm->SetXMLFile(XMLFileName);
  tm->ParseInputXMLFileAndCreateCorrelation();
  tm->InvertMatrix();
  tm->SaveCorrelationMatrixTH2D(OutputName);
  tm->CreateToyExperiments(10, m->syst().GetSystematics());

  ToyExperiment* toy = tm->GetToyExperiment(0);
  toy->Dump();
  return 0;
}
