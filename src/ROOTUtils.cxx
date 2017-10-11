#define ROOTUtils_cxx

#include"ROOTUtils.hxx"

void ROOTRandomSleep(Double_t scale, Int_t seed){
  using namespace std;
  if(scale <= 0){
    return; 
  }
  if(seed==0){
    const TDatime time;
    seed = time.Get();
  }
  TRandom3 ran(seed);
  const UInt_t sleepTime = TMath::Nint(ran.Integer(scale+1)*1e3);//seconds
  cout << "Sleeping for " << (sleepTime*1e-3) << " seconds " << endl;
  gSystem->Sleep(sleepTime);
}
