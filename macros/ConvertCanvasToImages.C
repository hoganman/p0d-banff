#include <vector>
#include <iostream>

#include "TFile.h"
#include "TCanvas.h"
#include "TString.h"

using namespace std;

/// Convert a ROOT file with a canvas to various output formats
void ConvertCanvasToImages(const TString& inputFileName, const TString& canvasName, const TString& outputNamePrefix, const TString& formats = "eps,png,pdf", const char delimiter = ',');
/// Split a TString into its components by a separator
std::vector<TString> SplitString(const TString& theOpt, const char separator);


void ConvertCanvasToImages(const TString& inputFileName, const TString& canvasName, const TString& outputNamePrefix, const TString& formats, const char delimiter)
{

  TFile* const inputFile = TFile::Open(inputFileName.Data());
  const TCanvas* const canvas = static_cast<TCanvas*>(inputFile->Get(canvasName.Data()));
  if(!canvas){
    cout << "ERROR: No TCanvas with name \""<< canvasName.Data() <<"\" exists in " << inputFileName.Data() << endl;
    return;
  }
  std::vector<TString> fileOutputs = SplitString(formats,delimiter);
  for(UInt_t outputI = 0;outputI<fileOutputs.size();++outputI){
    char buffer[1000];
    const TString outputFormat = fileOutputs.at(outputI);
    sprintf(buffer,"%s.%s",outputNamePrefix.Data(),outputFormat.Data());
    canvas->SaveAs(buffer); 
  }

}

std::vector<TString> SplitString(const TString& theOpt, const char separator)
{
   // splits the option string at 'separator' and fills the list
   // 'splitV' with the primitive strings
   std::vector<TString> splitV;
   TString splitOpt(theOpt);
   splitOpt.ReplaceAll("\n"," ");
   splitOpt = splitOpt.Strip(TString::kBoth,separator);
   while (splitOpt.Length()>0) {
      if ( !splitOpt.Contains(separator) ) {
         splitV.push_back(splitOpt);
         break;
      }
      else {
         TString toSave = splitOpt(0,splitOpt.First(separator));
         splitV.push_back(toSave);
         splitOpt = splitOpt(splitOpt.First(separator),splitOpt.Length());
      }
      splitOpt = splitOpt.Strip(TString::kLeading,separator);
   }
   return splitV;
}
