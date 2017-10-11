#define CheckFileROOT_C_seen
#include"TFile.h"
#include"TString.h"
#include"TSystem.h"
#include<cstring>

int CheckFileROOT(TString fileName = ""){

  int status = -1;
  const char* const str = fileName.Data(); 
  if(!gSystem->FindFile(str,fileName)){
    status = 0;
  }
  else{
    TFile * const f = TFile::Open(str);
    if(!f)
      status = 0;
    else if(!f->IsOpen())
      status = 0;
    else if(f->IsRaw())
      status  = 0;
    else if(!f->IsBinary())
      status =  0;
    else if(!f->GetListOfKeys()->GetSize())
      status =  0;
    else if(f->IsZombie())
      status =  0;
    else 
      status = 1;
    f->Close();
  }
  return status;
}
