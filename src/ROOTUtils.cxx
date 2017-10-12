#define ROOTUtils_cxx

#include"ROOTUtils.hxx"

void ROOTRandomSleep(Double_t scale, Int_t seed)
{
  if(scale <= 0){
    return;
  }
  if(seed==0){
    const TDatime time;
    seed = time.Get();
  }
  TRandom3 ran(seed);
  const UInt_t sleepTime = TMath::Nint(ran.Integer(scale+1)*1e3);//seconds
  std::cout << "Sleeping for " << (sleepTime*1e-3) << " seconds " << std::endl;
  gSystem->Sleep(sleepTime);
}

Bool_t ROOTCheckFile(TString fileName)
{
  Bool_t status = false;
  const Char_t* const str = fileName.Data();
  if(!gSystem->FindFile(str,fileName)){
    status = false;
  }
  else{
    TFile* const f = TFile::Open(str);
    if(!f)
      status = false;
    else if(!f->IsOpen())
      status = false;
    else if(f->IsRaw())
      status  = false;
    else if(!f->IsBinary())
      status =  false;
    else if(!f->GetListOfKeys()->GetSize())
      status =  false;
    else if(f->IsZombie())
      status =  false;
    else
      status = true;
    f->Close();
  }
  return status;
}

void ROOTSaveCanvasAs(const TString& inputFileName, const TString& canvasName,
    const TString& outputNamePrefix, const TString& formats,
    const Char_t delimiter)
{
  if(!ROOTCheckFile(inputFileName)){
    std::cerr << "ERROR: Invalid input file: " << inputFileName.Data()
      << std::endl;
    return;
  }

  TFile* const inputFile = TFile::Open(inputFileName.Data());
  TCanvas* const canvas =
    static_cast<TCanvas*>(inputFile->Get(canvasName.Data()));
  if(!canvas){
    std::cerr << "ERROR: No TCanvas with name \""<< canvasName.Data() <<
      "\" exists in " << inputFileName.Data() << std::endl;
    return;
  }
  std::vector<TString> fileOutputs = ROOTSplitString(formats,delimiter);
  for(UInt_t outputI = 0;outputI<fileOutputs.size();++outputI){
    Char_t buffer[1000];
    const TString outputFormat = fileOutputs.at(outputI);
    sprintf(buffer,"%s.%s",outputNamePrefix.Data(),outputFormat.Data());
    canvas->SaveAs(buffer);
  }
  inputFile->Close();

}

std::vector<TString> ROOTSplitString(const TString& theOpt,
    const Char_t separator)
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
